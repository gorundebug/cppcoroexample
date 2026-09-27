#include <gtest/gtest.h>
#include "orderservice/internal/functions/endpoint/order_processed_endpoint_sink.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/use_future.hpp>

#include <chrono>
#include <future>
#include <optional>
#include <thread>

namespace example::order_service::functions {

class KafkaCoroutineSinkTest : public testing::Test {
 protected:
  KafkaCoroutineSinkTest() {
    servicelib::detail::ParallelExecutorRegistry::Set(io.get_executor());
    servicelib::detail::BlockingExecutorRegistry::Set(blocking.get_executor());
  }
  ~KafkaCoroutineSinkTest() override {
    blocking.join();
    servicelib::detail::BlockingExecutorRegistry::Clear();
    servicelib::detail::ParallelExecutorRegistry::Clear();
  }

  void run(boost::asio::awaitable<void> operation) {
    auto done = boost::asio::co_spawn(io, std::move(operation), boost::asio::use_future);
    io.run();
    done.get();
  }

  boost::asio::io_context io;
  boost::asio::thread_pool blocking{1};
};

TEST_F(KafkaCoroutineSinkTest, PublishesOrderWithoutBlockingGraphWorkerAndDrainsResult) {
  using namespace std::chrono_literals;
  servicelib::detail::AsyncOperations deliveries;
  std::string observedKey;
  std::string observedValue;
  bool workerProgressedDuringSend = false;
  bool delivered = false;
  const auto graphThread = std::this_thread::get_id();
  bool separateThread = false;
  servicelib::datasink::kafka::SinkMessage<std::monostate> message{
      "orders", {},
      [&](servicelib::MessageContext, std::monostate) -> boost::asio::awaitable<void> {
        co_await boost::asio::post(boost::asio::use_awaitable);
        delivered = true;
      },
      [] { return std::optional<std::uint32_t>{}; },
      [&](std::string key, std::string value, std::optional<std::uint32_t>) {
        separateThread = std::this_thread::get_id() != graphThread;
        auto progressed = std::make_shared<std::promise<void>>();
        auto ready = progressed->get_future();
        boost::asio::post(io, [progressed] { progressed->set_value(); });
        workerProgressedDuringSend = ready.wait_for(2s) == std::future_status::ready;
        observedKey = std::move(key);
        observedValue = std::move(value);
        return servicelib::datasink::kafka::DeliveryResult{0, 17, {}};
      }, deliveries};
  OrderProcessedEndpointSink handler;
  example::model::types::OrderProcessed value{};
  value.order_id = "order-42";
  auto operation = [&]() -> boost::asio::awaitable<void> {
    std::monostate stream;
    EXPECT_EQ(co_await handler.getStreamId({}, value), value.order_id);
    auto begin = co_await handler.beginRequest({}, stream);
    co_await handler.consumeMessage(begin.context, stream, begin.state, value, message);
    co_await handler.endRequest(begin.context, stream, {}, begin.state);
    co_await deliveries.stopAndWait();
  };
  run(operation());
  EXPECT_TRUE(separateThread);
  EXPECT_TRUE(workerProgressedDuringSend);
  EXPECT_TRUE(delivered);
  EXPECT_EQ(observedKey, value.order_id);
  EXPECT_EQ(observedValue, boost::json::serialize(boost::json::value_from(value)));
}

TEST_F(KafkaCoroutineSinkTest, SendSyncAwaitsDeliveryAndOutAwaitsCollector) {
  servicelib::detail::AsyncOperations deliveries;
  int result = 0;
  servicelib::datasink::kafka::SinkMessage<int> message{
      "orders", {},
      [&](servicelib::MessageContext, int value) -> boost::asio::awaitable<void> {
        co_await boost::asio::post(boost::asio::use_awaitable);
        result = value;
      },
      [] { return std::optional<std::uint32_t>{2}; },
      [](std::string, std::string, std::optional<std::uint32_t> partition) {
        return servicelib::datasink::kafka::DeliveryResult{partition, 17, {}};
      }, deliveries};
  auto operation = [&]() -> boost::asio::awaitable<void> {
    const auto delivery = co_await message.sendSync();
    EXPECT_FALSE(delivery.error);
    EXPECT_EQ(delivery.partition, 2);
    EXPECT_EQ(delivery.offset, 17);
    co_await message.out(17);
    EXPECT_EQ(result, 17);
  };
  run(operation());
}

TEST_F(KafkaCoroutineSinkTest, AsyncFailureReachesAwaitedDeliveryCallback) {
  servicelib::detail::AsyncOperations deliveries;
  int result = 0;
  bool failed = false;
  servicelib::datasink::kafka::SinkMessage<int> message{
      "orders", {},
      [&](servicelib::MessageContext, int value) -> boost::asio::awaitable<void> {
        co_await boost::asio::post(boost::asio::use_awaitable);
        result = value;
      },
      [] { return std::optional<std::uint32_t>{}; },
      [](std::string, std::string, std::optional<std::uint32_t>)
          -> servicelib::datasink::kafka::DeliveryResult {
        throw std::runtime_error("broker unavailable");
      }, deliveries};
  auto operation = [&]() -> boost::asio::awaitable<void> {
    message.send([&](const auto& delivery) -> boost::asio::awaitable<int> {
      failed = static_cast<bool>(delivery.error);
      co_await boost::asio::post(boost::asio::use_awaitable);
      co_return 41;
    });
    co_await deliveries.stopAndWait();
  };
  run(operation());
  EXPECT_TRUE(failed);
  EXPECT_EQ(result, 41);
}

}  // namespace example::order_service::functions

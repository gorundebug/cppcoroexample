#include "analyticsservice/internal/app/bindings.generated.hpp"
#include "analyticsservice/internal/app/substreams.generated.hpp"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/use_future.hpp>
#include <servicelib/runtime/detail/sync.hpp>
#include <gtest/gtest.h>

#include <chrono>
#include <future>
#include <memory>

namespace example::analytics_service::app {
namespace {

struct CallState {
  servicelib::detail::SingleUseEvent resume;
  bool entered{};
  int result{};
};

class SuspendedSubStream final : public servicelib::ISubStream<int, int> {
 public:
  explicit SuspendedSubStream(CallState& state) : state_(state) {}

  boost::asio::awaitable<void> consume(
      servicelib::MessageContext, servicelib::Payload<int> value,
      std::shared_ptr<servicelib::SubStreamCollector<int>>) override {
    state_.entered = true;
    co_await state_.resume.AsyncWait();
    state_.result = value.get();
  }

 private:
  CallState& state_;
};

TEST(CoroutineBindings, SubStreamKeepsNodeAliveUntilAwaitedCallFinishes) {
  boost::asio::io_context io;
  CallState state;
  auto stream = std::make_shared<SuspendedSubStream>(state);
  std::weak_ptr<SuspendedSubStream> lifetime = stream;
  SubStreamHandle<int, int> handle;
  handle.bind(stream);
  auto completed = boost::asio::co_spawn(
      io, handle.consume({}, servicelib::Payload<int>::make(42), nullptr),
      boost::asio::use_future);
  io.poll();
  EXPECT_TRUE(state.entered);
  EXPECT_EQ(completed.wait_for(std::chrono::seconds{0}),
            std::future_status::timeout);
  stream.reset();
  EXPECT_FALSE(lifetime.expired());
  state.resume.Send();
  io.restart();
  io.run();
  EXPECT_NO_THROW(completed.get());
  EXPECT_EQ(state.result, 42);
  EXPECT_TRUE(lifetime.expired());
}

TEST(CoroutineBindings, UnboundSubStreamReportsFailureToAwaiter) {
  boost::asio::io_context io;
  SubStreamHandle<int, int> handle;
  auto completed = boost::asio::co_spawn(
      io, handle.consume({}, servicelib::Payload<int>::make(42), nullptr),
      boost::asio::use_future);
  io.run();
  EXPECT_THROW(completed.get(), std::logic_error);
}

TEST(CoroutineBindings, SinkBindingWaitsForEndpointCompletion) {
  boost::asio::io_context io;
  CallState state;
  WriteCycleAnalyticsSinkBinding binding;
  binding.consume = [&state](servicelib::MessageContext,
                            const types::AnalyticsEvent&)
      -> boost::asio::awaitable<void> {
    state.entered = true;
    co_await state.resume.AsyncWait();
    state.result = 1;
  };
  WriteCycleAnalyticsSinkBinding::Function function{&binding};
  types::AnalyticsEvent value{};
  auto completed = boost::asio::co_spawn(
      io, function({}, value), boost::asio::use_future);
  io.poll();
  EXPECT_TRUE(state.entered);
  EXPECT_EQ(state.result, 0);
  EXPECT_EQ(completed.wait_for(std::chrono::seconds{0}),
            std::future_status::timeout);
  state.resume.Send();
  io.restart();
  io.run();
  EXPECT_NO_THROW(completed.get());
  EXPECT_EQ(state.result, 1);
}

}  // namespace
}  // namespace example::analytics_service::app

#include <concepts>

#include <gtest/gtest.h>

#include "analyticsservice/internal/functions/endpoint/analytics_orders_source.hpp"
#include "analyticsservice/internal/functions/endpoint/analytics_payments_source.hpp"
#include "analyticsservice/internal/functions/endpoint/analytics_shipments_source.hpp"
#include "analyticsservice/internal/functions/endpoint/cycle_analytics_input_source.hpp"
#include "analyticsservice/internal/functions/endpoint/substream_analytics_input_source.hpp"
#include "analyticsservice/internal/functions/test_stream.hpp"

namespace example::analytics_service::functions {

namespace {
using Event = types::AnalyticsEvent;

template <typename Source>
void CheckProducer(const std::vector<Event>& expected) {
  Source source;
  std::vector<Event> actual;
  std::size_t active = 0;
  auto receive = [&](servicelib::MessageContext, servicelib::Payload<Event> value)
      -> boost::asio::awaitable<void> {
    ++active;
    EXPECT_EQ(active, 1U);
    co_await boost::asio::post(boost::asio::use_awaitable);
    actual.push_back(value.get());
    --active;
  };
  test::Run(source.start({}, receive));
  EXPECT_EQ(active, 0U);
  ASSERT_EQ(actual.size(), expected.size());
  for (std::size_t i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual[i].key, expected[i].key);
    EXPECT_EQ(actual[i].value, expected[i].value);
    EXPECT_EQ(actual[i].kind, expected[i].kind);
  }
  test::Run(source.stop({}));
}

struct Results {
  bool* completed;
  void done() { *completed = true; }
};

struct Stream {
  bool* completed;
  std::vector<Event> values;
  boost::asio::awaitable<void> collect(servicelib::MessageContext, const Event& value) {
    EXPECT_FALSE(*completed);
    co_await boost::asio::post(boost::asio::use_awaitable);
    EXPECT_FALSE(*completed);
    values.push_back(value);
  }
};
}  // namespace

TEST(AnalyticsOrdersSource, ProducersAwaitDeliveryAndKeepTheirData) {
  CheckProducer<AnalyticsOrdersSource>({{"high-value", 10, "order"}, {"standard", 1, "order"}});
  CheckProducer<AnalyticsPaymentsSource>({{"high-value", 20, "payment"}, {"standard", 2, "payment"}});
  CheckProducer<AnalyticsShipmentsSource>({{"high-value", 30, "shipment"}, {"standard", 3, "shipment"}});
  CheckProducer<CycleAnalyticsInputSource>({{"cycle", 0, "cycle"}});
  CheckProducer<SubstreamAnalyticsInputSource>({{"substream", 7, "input"}});
}

TEST(AnalyticsOrdersSource, SignalsDoneOnlyAfterDownstreamReturns) {
  AnalyticsOrdersSource source;
  bool completed = false;
  Stream stream{&completed, {}};
  auto begin = test::Run(source.beginRequest({}, stream));
  const Event input{"order-1", 42, "order"};
  test::Run(source.consumeMessage(begin.context, stream, begin.state, input, Results{&completed}));
  EXPECT_TRUE(completed);
  ASSERT_EQ(stream.values.size(), 1U);
  EXPECT_EQ(stream.values.front().key, input.key);
  EXPECT_EQ(stream.values.front().value, input.value);
  test::Run(source.endRequest(begin.context, stream, {}, begin.state));
}

}  // namespace example::analytics_service::functions

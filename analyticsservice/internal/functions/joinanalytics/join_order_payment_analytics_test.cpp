#include <concepts>

#include <gtest/gtest.h>

#include "analyticsservice/internal/functions/joinanalytics/join_order_payment_analytics.hpp"
#include "analyticsservice/internal/functions/test_stream.hpp"

namespace example::analytics_service::functions {

TEST(JoinOrderPaymentAnalytics, WaitsForBothSidesAndAwaitsCollector) {
  using Event = types::AnalyticsEvent;
  JoinOrderPaymentAnalytics function;
  test::Stream stream;
  test::Collector<types::AnalyticsResult> out;
  std::string key = "order-1";
  std::pair<std::vector<Event>, std::vector<Event>> values;
  values.first.push_back({key, 10, "order"});
  EXPECT_FALSE(test::Run(function({}, stream, key, values, out)));
  EXPECT_TRUE(out.values.empty());
  values.second.push_back({key, 20, "payment"});
  EXPECT_TRUE(test::Run(function({}, stream, key, values, out)));
  ASSERT_EQ(out.values.size(), 1U);
  EXPECT_EQ(out.values.front().key, key);
  EXPECT_EQ(out.values.front().total, 30);
  EXPECT_EQ(out.values.front().kind, "join");
  EXPECT_EQ(values.first.size(), 1U);
  EXPECT_EQ(values.second.size(), 1U);
}

}  // namespace example::analytics_service::functions

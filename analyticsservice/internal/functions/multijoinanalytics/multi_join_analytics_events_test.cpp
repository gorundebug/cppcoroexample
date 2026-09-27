#include <concepts>

#include <gtest/gtest.h>

#include "analyticsservice/internal/functions/multijoinanalytics/multi_join_analytics_events.hpp"
#include "analyticsservice/internal/functions/test_stream.hpp"

namespace example::analytics_service::functions {

TEST(MultiJoinAnalyticsEvents, WaitsForAllThreeSidesAndAwaitsCollector) {
  using Event = types::AnalyticsEvent;
  MultiJoinAnalyticsEvents function;
  test::Stream stream;
  test::Collector<types::AnalyticsResult> out;
  std::string key = "order-1";
  std::tuple<std::vector<Event>, std::vector<Event>, std::vector<Event>> values;
  std::get<0>(values).push_back({key, 10, "order"});
  EXPECT_FALSE(test::Run(function({}, stream, key, values, out)));
  std::get<1>(values).push_back({key, 20, "payment"});
  EXPECT_FALSE(test::Run(function({}, stream, key, values, out)));
  EXPECT_TRUE(out.values.empty());
  std::get<2>(values).push_back({key, 30, "shipment"});
  EXPECT_TRUE(test::Run(function({}, stream, key, values, out)));
  ASSERT_EQ(out.values.size(), 1U);
  EXPECT_EQ(out.values.front().key, key);
  EXPECT_EQ(out.values.front().total, 60);
  EXPECT_EQ(out.values.front().kind, "multi");
}

}  // namespace example::analytics_service::functions

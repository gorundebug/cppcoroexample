#include <concepts>

#include <gtest/gtest.h>

#include "analyticsservice/internal/functions/substreamanalytics/build_substream_analytics_result.hpp"
#include "analyticsservice/internal/functions/test_stream.hpp"

namespace example::analytics_service::functions {

TEST(BuildSubstreamAnalyticsResult, AwaitsOutputAndPreservesInput) {
  BuildSubstreamAnalyticsResult function;
  test::Stream stream;
  test::Collector<types::AnalyticsResult> out;
  const types::AnalyticsEvent input{"order-1", 21, "input"};
  test::Run(function({}, stream, input, out));
  ASSERT_EQ(out.values.size(), 1U);
  EXPECT_EQ(out.values.front().key, input.key);
  EXPECT_EQ(out.values.front().total, 42);
  EXPECT_EQ(out.values.front().kind, "substream");
  EXPECT_EQ(input.value, 21);
}

}  // namespace example::analytics_service::functions

#include <concepts>

#include <gtest/gtest.h>

#include "analyticsservice/internal/functions/endpoint/high_value_analytics_sink.hpp"
#include "analyticsservice/internal/functions/endpoint/standard_analytics_sink.hpp"
#include "analyticsservice/internal/functions/endpoint/joined_analytics_sink.hpp"
#include "analyticsservice/internal/functions/endpoint/cycle_analytics_result_sink.hpp"
#include "analyticsservice/internal/functions/endpoint/substream_analytics_result_sink.hpp"
#include "analyticsservice/internal/functions/test_stream.hpp"

namespace example::analytics_service::functions {

namespace {
template <typename Sink, typename Value>
void CheckSink(const Value& good, const Value& bad) {
  Sink sink;
  test::Stream stream;
  auto begin = test::Run(sink.beginRequest({}, stream));
  EXPECT_TRUE(test::Run(sink.getStreamId({}, good)).empty());
  EXPECT_NO_THROW(test::Run(sink.consumeMessage(begin.context, stream, begin.state, good)));
  EXPECT_THROW(test::Run(sink.consumeMessage(begin.context, stream, begin.state, bad)), std::runtime_error);
  test::Run(sink.endRequest(begin.context, stream, {}, begin.state));
}
}  // namespace

TEST(HighValueAnalyticsSink, CoroutineHandlersPreserveResultValidation) {
  using Result = types::AnalyticsResult;
  CheckSink<HighValueAnalyticsSink>(Result{"high-value", 60, "multi"}, Result{"high-value", 59, "multi"});
  CheckSink<StandardAnalyticsSink>(Result{"standard", 6, "multi"}, Result{"standard", 7, "multi"});
  CheckSink<JoinedAnalyticsSink>(Result{"high-value", 30, "join"}, Result{"high-value", 31, "join"});
  CheckSink<JoinedAnalyticsSink>(Result{"standard", 3, "join"}, Result{"standard", 4, "join"});
  CheckSink<SubstreamAnalyticsResultSink>(Result{"substream", 14, "substream"}, Result{"substream", 13, "substream"});
  CheckSink<CycleAnalyticsResultSink>(types::AnalyticsEvent{"cycle", 3, "cycle"}, types::AnalyticsEvent{"cycle", 2, "cycle"});
}

}  // namespace example::analytics_service::functions

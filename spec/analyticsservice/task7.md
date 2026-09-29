# Task 7/26: `JoinOrderPaymentAnalytics`

> Rules: [`spec/rules.md`](../rules.md)

| Field | Value |
|-------|-------|
| Language | `C++/Coro` |
| Kind | `join` |
| File | `analyticsservice/internal/functions/joinanalytics/join_order_payment_analytics.hpp` |
| Test | `analyticsservice/internal/functions/joinanalytics/join_order_payment_analytics_test.cpp` |
| Service | `Analytics Service` |


## Behaviour

Join matching order and payment analytics events and emit their combined total.




## Stream types
- Input: `AnalyticsEvent` — `analyticsservice/internal/types/analytics_event.hpp`
- Output: `AnalyticsResult` — `analyticsservice/internal/types/analytics_result.hpp`
- Key: `AnalyticsKey` — `analyticsservice/internal/types/analytics_key.hpp`

## Checklist

- [ ] Read [`spec/rules.md`](../rules.md), especially the `C++/Coro` section
- [ ] Open `analyticsservice/internal/functions/joinanalytics/join_order_payment_analytics.hpp` and preserve its generated contract
- [ ] Inspect input type `AnalyticsEvent` in `analyticsservice/internal/types/analytics_event.hpp`
- [ ] Inspect output type `AnalyticsResult` in `analyticsservice/internal/types/analytics_result.hpp`
- [ ] Implement the C++ coroutine function object without retaining borrowed payload/context references
- [ ] Await collector, sender and result operations according to their generated contracts
- [ ] Run `./scripts/test.generated.sh`
- [ ] Implement meaningful assertions in `analyticsservice/internal/functions/joinanalytics/join_order_payment_analytics_test.cpp`
- [ ] Re-read this checklist
- [ ] Append to `spec/progress.md`: `- [x] analyticsservice/task7.md — JoinOrderPaymentAnalytics — C++/Coro — done`
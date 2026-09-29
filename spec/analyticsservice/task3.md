# Task 3/26: `CompleteCycleAnalytics`

> Rules: [`spec/rules.md`](../rules.md)

| Field | Value |
|-------|-------|
| Language | `C++/Coro` |
| Kind | `filter` |
| File | `analyticsservice/internal/functions/cycleanalytics/complete_cycle_analytics.hpp` |
| Test | `analyticsservice/internal/functions/cycleanalytics/complete_cycle_analytics_test.cpp` |
| Service | `Analytics Service` |


## Behaviour

Keep the terminal analytics event once its cycle counter reaches three.




## Stream types
- Input: `AnalyticsEvent` — `analyticsservice/internal/types/analytics_event.hpp`
- Output: `AnalyticsEvent` — `analyticsservice/internal/types/analytics_event.hpp`

## Checklist

- [ ] Read [`spec/rules.md`](../rules.md), especially the `C++/Coro` section
- [ ] Open `analyticsservice/internal/functions/cycleanalytics/complete_cycle_analytics.hpp` and preserve its generated contract
- [ ] Inspect input type `AnalyticsEvent` in `analyticsservice/internal/types/analytics_event.hpp`
- [ ] Inspect output type `AnalyticsEvent` in `analyticsservice/internal/types/analytics_event.hpp`
- [ ] Implement the C++ coroutine function object without retaining borrowed payload/context references
- [ ] Await collector, sender and result operations according to their generated contracts
- [ ] Run `./scripts/test.generated.sh`
- [ ] Implement meaningful assertions in `analyticsservice/internal/functions/cycleanalytics/complete_cycle_analytics_test.cpp`
- [ ] Re-read this checklist
- [ ] Append to `spec/progress.md`: `- [x] analyticsservice/task3.md — CompleteCycleAnalytics — C++/Coro — done`
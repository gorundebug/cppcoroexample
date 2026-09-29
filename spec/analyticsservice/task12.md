# Task 12/26: `KeyShipmentsForMultiJoin`

> Rules: [`spec/rules.md`](../rules.md)

| Field | Value |
|-------|-------|
| Language | `C++/Coro` |
| Kind | `keyBy` |
| File | `analyticsservice/internal/functions/multijoinanalytics/key_shipments_for_multi_join.hpp` |
| Test | `analyticsservice/internal/functions/multijoinanalytics/key_shipments_for_multi_join_test.cpp` |
| Service | `Analytics Service` |


## Behaviour

Key the shipment analytics event for the multi-way join.




## Stream types
- Input: `AnalyticsEvent` — `analyticsservice/internal/types/analytics_event.hpp`
- Output: `AnalyticsEvent` — `analyticsservice/internal/types/analytics_event.hpp`

## Checklist

- [ ] Read [`spec/rules.md`](../rules.md), especially the `C++/Coro` section
- [ ] Open `analyticsservice/internal/functions/multijoinanalytics/key_shipments_for_multi_join.hpp` and preserve its generated contract
- [ ] Inspect input type `AnalyticsEvent` in `analyticsservice/internal/types/analytics_event.hpp`
- [ ] Inspect output type `AnalyticsEvent` in `analyticsservice/internal/types/analytics_event.hpp`
- [ ] Implement the C++ coroutine function object without retaining borrowed payload/context references
- [ ] Await collector, sender and result operations according to their generated contracts
- [ ] Run `./scripts/test.generated.sh`
- [ ] Implement meaningful assertions in `analyticsservice/internal/functions/multijoinanalytics/key_shipments_for_multi_join_test.cpp`
- [ ] Re-read this checklist
- [ ] Append to `spec/progress.md`: `- [x] analyticsservice/task12.md — KeyShipmentsForMultiJoin — C++/Coro — done`
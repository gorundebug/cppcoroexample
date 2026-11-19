# Task 17/26: `AnalyticsOrdersSource`

> Rules: [`spec/rules.md`](../rules.md)

Paths and commands below are relative to the generated project root.

| Field | Value |
|-------|-------|
| Language | `C++/Coro` |
| Kind | `custom-source` |
| File | `analyticsservice/internal/functions/endpoint/analytics_orders_source.hpp` |
| Test | `analyticsservice/internal/functions/endpoint/analytics_orders_source_test.cpp` |
| Service | `Analytics Service` |


## Behaviour

Produce a deterministic order analytics event for the canonical join examples.




## Stream types
- Emits into graph: `AnalyticsEvent` — `analyticsservice/internal/types/analytics_event.hpp`

## Checklist

- [ ] Read [`spec/rules.md`](../rules.md), especially the `C++/Coro` section
- [ ] Open `analyticsservice/internal/functions/endpoint/analytics_orders_source.hpp` and preserve its generated contract
- [ ] Inspect emitted message type `AnalyticsEvent` in `analyticsservice/internal/types/analytics_event.hpp`
- [ ] Implement the C++ coroutine function object without retaining borrowed payload/context references
- [ ] Await collector, sender and result operations according to their generated contracts
- [ ] Run `./scripts/test.generated.sh`
- [ ] Implement meaningful assertions in `analyticsservice/internal/functions/endpoint/analytics_orders_source_test.cpp`
- [ ] Re-read this checklist
- [ ] Append to `spec/progress.md`: `- [x] analyticsservice/task17.md — AnalyticsOrdersSource — C++/Coro — done`
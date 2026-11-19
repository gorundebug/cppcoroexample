# Task 15/26: `AnalyticsScheduleSource`

> Rules: [`spec/rules.md`](../rules.md)

Paths and commands below are relative to the generated project root.

| Field | Value |
|-------|-------|
| Language | `C++/Coro` |
| Kind | `schedule-source` |
| File | `analyticsservice/internal/functions/cron/analytics_schedule_source.hpp` |
| Test | `analyticsservice/internal/functions/cron/analytics_schedule_source_test.cpp` |
| Service | `Analytics Service` |


## Behaviour

Create an analytics job message identifying the local scheduled firing.





## Stream types
- Emits into graph: `AnalyticsKey` — `analyticsservice/internal/types/analytics_key.hpp`

## Checklist

- [ ] Read [`spec/rules.md`](../rules.md), especially the `C++/Coro` section
- [ ] Open `analyticsservice/internal/functions/cron/analytics_schedule_source.hpp` and preserve its generated contract
- [ ] Inspect emitted message type `AnalyticsKey` in `analyticsservice/internal/types/analytics_key.hpp`
- [ ] Implement the C++ coroutine function object without retaining borrowed payload/context references
- [ ] Await collector, sender and result operations according to their generated contracts
- [ ] Run `./scripts/test.generated.sh`
- [ ] Implement meaningful assertions in `analyticsservice/internal/functions/cron/analytics_schedule_source_test.cpp`
- [ ] Re-read this checklist
- [ ] Append to `spec/progress.md`: `- [x] analyticsservice/task15.md — AnalyticsScheduleSource — C++/Coro — done`
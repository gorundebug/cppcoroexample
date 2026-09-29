# Task 5/26: `CountOrderProcessed`

> Rules: [`spec/rules.md`](../rules.md)

| Field | Value |
|-------|-------|
| Language | `C++/Coro` |
| Kind | `process` |
| File | `analyticsservice/internal/functions/analytics/count_order_processed.hpp` |
| Test | `analyticsservice/internal/functions/analytics/count_order_processed_test.cpp` |
| Service | `Analytics Service` |


## Behaviour

Count successful and unsuccessful orders independently, then return the event unchanged.





## Stream types
- Input: `OrderProcessed` — `model_cppcoro/include/example/model/types/order_processed.hpp`
- Output: `OrderProcessed` — `model_cppcoro/include/example/model/types/order_processed.hpp`

## Checklist

- [ ] Read [`spec/rules.md`](../rules.md), especially the `C++/Coro` section
- [ ] Open `analyticsservice/internal/functions/analytics/count_order_processed.hpp` and preserve its generated contract
- [ ] Inspect input type `OrderProcessed` in `model_cppcoro/include/example/model/types/order_processed.hpp`
- [ ] Inspect output type `OrderProcessed` in `model_cppcoro/include/example/model/types/order_processed.hpp`
- [ ] Implement the C++ coroutine function object without retaining borrowed payload/context references
- [ ] Await collector, sender and result operations according to their generated contracts
- [ ] Run `./scripts/test.generated.sh`
- [ ] Implement meaningful assertions in `analyticsservice/internal/functions/analytics/count_order_processed_test.cpp`
- [ ] Re-read this checklist
- [ ] Append to `spec/progress.md`: `- [x] analyticsservice/task5.md — CountOrderProcessed — C++/Coro — done`
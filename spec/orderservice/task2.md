# Task 2/8: `MapToOrderProcessed`

> Rules: [`spec/rules.md`](../rules.md)

| Field | Value |
|-------|-------|
| Language | `C++/Coro` |
| Kind | `map` |
| File | `orderservice/internal/functions/order/map_to_order_processed.hpp` |
| Test | `orderservice/internal/functions/order/map_to_order_processed_test.cpp` |
| Service | `Order Service` |


## Behaviour

Create an OrderProcessed event from the final order state.
Preserve the order ID, status, and processing time. Count all item results and reserved items; for unsuccessful orders use the final status as the failure reason.





## Stream types
- Input: `OrderState` — `orderservice/internal/types/order_state.hpp`
- Output: `OrderProcessed` — `model_cppcoro/include/example/model/types/order_processed.hpp`

## Checklist

- [ ] Read [`spec/rules.md`](../rules.md), especially the `C++/Coro` section
- [ ] Open `orderservice/internal/functions/order/map_to_order_processed.hpp` and preserve its generated contract
- [ ] Inspect input type `OrderState` in `orderservice/internal/types/order_state.hpp`
- [ ] Inspect output type `OrderProcessed` in `model_cppcoro/include/example/model/types/order_processed.hpp`
- [ ] Implement the C++ coroutine function object without retaining borrowed payload/context references
- [ ] Await collector, sender and result operations according to their generated contracts
- [ ] Run `./scripts/test.generated.sh`
- [ ] Implement meaningful assertions in `orderservice/internal/functions/order/map_to_order_processed_test.cpp`
- [ ] Re-read this checklist
- [ ] Append to `spec/progress.md`: `- [x] orderservice/task2.md — MapToOrderProcessed — C++/Coro — done`
# Task 16/26: `OrderProcessedEndpointSource`

> Rules: [`spec/rules.md`](../rules.md)

Paths and commands below are relative to the generated project root.

| Field | Value |
|-------|-------|
| Language | `C++/Coro` |
| Kind | `kafka-source` |
| File | `analyticsservice/internal/functions/endpoint/order_processed_endpoint_source.hpp` |
| Test | `analyticsservice/internal/functions/endpoint/order_processed_endpoint_source_test.cpp` |
| Service | `Analytics Service` |


## Behaviour

Exchange OrderProcessed events keyed by order ID.
Producers include the final status, processing time, total and confirmed item counts, and a failure reason for unsuccessful orders.
Consumers decode the event and mark its Kafka message processed only after the pipeline handles it successfully.





## Stream types
- Emits into graph: `OrderProcessed` — `model_cppcoro/include/example/model/types/order_processed.hpp`
- Receives graph result: `OrderProcessed` — `model_cppcoro/include/example/model/types/order_processed.hpp`

## Checklist

- [ ] Read [`spec/rules.md`](../rules.md), especially the `C++/Coro` section
- [ ] Open `analyticsservice/internal/functions/endpoint/order_processed_endpoint_source.hpp` and preserve its generated contract
- [ ] Inspect emitted message type `OrderProcessed` in `model_cppcoro/include/example/model/types/order_processed.hpp`
- [ ] Inspect graph result type `OrderProcessed` in `model_cppcoro/include/example/model/types/order_processed.hpp`
- [ ] Implement the C++ coroutine function object without retaining borrowed payload/context references
- [ ] Await collector, sender and result operations according to their generated contracts
- [ ] Run `./scripts/test.generated.sh`

- [ ] Implement meaningful assertions in `analyticsservice/internal/functions/endpoint/order_processed_endpoint_source_test.cpp`
- [ ] Re-read this checklist
- [ ] Append to `spec/progress.md`: `- [x] analyticsservice/task16.md — OrderProcessedEndpointSource — C++/Coro — done`
# Task 8/8: `OrderProcessedEndpointSink`

> Rules: [`spec/rules.md`](../rules.md)

| Field | Value |
|-------|-------|
| Language | `C++/Coro` |
| Kind | `kafka-sink` |
| File | `orderservice/internal/functions/endpoint/order_processed_endpoint_sink.hpp` |
| Test | `orderservice/internal/functions/endpoint/order_processed_endpoint_sink_test.cpp` |
| Service | `Order Service` |


## Behaviour

Exchange OrderProcessed events keyed by order ID.
Producers include the final status, processing time, total and confirmed item counts, and a failure reason for unsuccessful orders.
Consumers decode the event and mark its Kafka message processed only after the pipeline handles it successfully.





## Stream types
- Input: `OrderProcessed` — `model_cppcoro/include/example/model/types/order_processed.hpp`

## Checklist

- [ ] Read [`spec/rules.md`](../rules.md), especially the `C++/Coro` section
- [ ] Open `orderservice/internal/functions/endpoint/order_processed_endpoint_sink.hpp` and preserve its generated contract
- [ ] Inspect input type `OrderProcessed` in `model_cppcoro/include/example/model/types/order_processed.hpp`
- [ ] Implement the C++ coroutine function object without retaining borrowed payload/context references
- [ ] Await collector, sender and result operations according to their generated contracts
- [ ] Run `./scripts/test.generated.sh`
- [ ] Implement meaningful assertions in `orderservice/internal/functions/endpoint/order_processed_endpoint_sink_test.cpp`
- [ ] Re-read this checklist
- [ ] Append to `spec/progress.md`: `- [x] orderservice/task8.md — OrderProcessedEndpointSink — C++/Coro — done`
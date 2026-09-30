#pragma once

#include <memory>
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

#include <boost/asio/awaitable.hpp>
#include <boost/json.hpp>

#include <servicelib/runtime/common.hpp>
#include <servicelib/runtime/config/endpoint_types.hpp>
#include <servicelib/runtime/environment/environment.hpp>
#include <servicelib/datasink/kafka/librdkafka.hpp>

#include <example/model/types/order_processed.hpp>


namespace example::order_service::functions {

struct OrderProcessedEndpointSink final {
  using State = std::monostate;

  boost::asio::awaitable<std::string> getStreamId(
      servicelib::MessageContext,
      const example::model::types::OrderProcessed& value) const {
    co_return value.order_id;
  }

  boost::asio::awaitable<servicelib::BeginResult<State>> beginRequest(
      servicelib::MessageContext context, auto&) const {
    co_return servicelib::BeginResult<State>{std::move(context), {}};
  }

  boost::asio::awaitable<void> consumeMessage(
      servicelib::MessageContext, auto&, State&,
      const example::model::types::OrderProcessed& value,
      servicelib::datasink::kafka::SinkMessage<std::monostate>& message) const {
    message.key = value.order_id;
    message.value = boost::json::serialize(boost::json::value_from(value));
    message.send([](const auto&) -> boost::asio::awaitable<std::monostate> {
      co_return std::monostate{};
    });
    co_return;
  }

  boost::asio::awaitable<void> endRequest(
      servicelib::MessageContext, auto&, std::exception_ptr,
      State&) const { co_return; }
};

inline boost::asio::awaitable<std::unique_ptr<OrderProcessedEndpointSink>> MakeOrderProcessedEndpointSink(
    servicelib::Context context, servicelib::IServiceEnvironment& environment,
    const auto& config) {
  (void)config;
  (void)context; (void)environment;
  co_return std::make_unique<OrderProcessedEndpointSink>();
}

}  // namespace example::order_service::functions

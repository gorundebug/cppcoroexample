#pragma once

#include <memory>

#include <boost/asio/awaitable.hpp>

#include <exception>
#include <boost/json.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

#include <servicelib/runtime/common.hpp>
#include <servicelib/runtime/config/endpoint_types.hpp>
#include <servicelib/runtime/environment/environment.hpp>
#include <servicelib/datasource/kafka/librdkafka.hpp>
#include <example/model/types/order_processed.hpp>


namespace example::analytics_service::functions {

struct OrderProcessedEndpointSource final {
  using State = std::monostate;

  int concurrency(auto&) const noexcept { return 0; }

  boost::asio::awaitable<servicelib::BeginResult<State>> beginRequest(
      servicelib::MessageContext context, auto&) const {
    co_return servicelib::BeginResult<State>{std::move(context), {}};
  }

  boost::asio::awaitable<void> consumeMessage(
      servicelib::MessageContext context, auto& stream, State&,
      const servicelib::datasource::kafka::ConsumerMessage& message,
      auto result) const {
    auto value = boost::json::value_to<
        example::model::types::OrderProcessed>(
        boost::json::parse(message.value()));
    const auto message_id = value.order_id;
    result.setResultCallback(
        message_id,
        [message, result](servicelib::MessageContext, auto&, State&,
                          const auto&) mutable -> boost::asio::awaitable<bool> {
          message.markMessage("processed");
          result.done();
          co_return true;
        });
    co_await stream.collect(std::move(context), std::move(value));
  }

  boost::asio::awaitable<std::string> getMessageId(
      servicelib::MessageContext, auto&, State&,
      const example::model::types::OrderProcessed& value) const {
    co_return value.order_id;
  }

  boost::asio::awaitable<void> endRequest(
      servicelib::MessageContext, auto&, std::exception_ptr,
      State&) const { co_return; }
};

inline boost::asio::awaitable<std::unique_ptr<OrderProcessedEndpointSource>> MakeOrderProcessedEndpointSource(
    servicelib::Context context, servicelib::IServiceEnvironment& environment,
    const auto& config) {
  (void)config;
  (void)context; (void)environment;
  co_return std::make_unique<OrderProcessedEndpointSource>();
}

}  // namespace example::analytics_service::functions

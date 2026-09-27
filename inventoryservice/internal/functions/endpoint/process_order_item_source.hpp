#pragma once

#include <memory>

#include <boost/asio/awaitable.hpp>

#include <exception>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

#include <servicelib/runtime/common.hpp>
#include <servicelib/runtime/config/endpoint_types.hpp>
#include <servicelib/runtime/environment/environment.hpp>
#include <example/model/types/order_item.hpp>
#include <example/model/types/order_item_result.hpp>
#include <proto/inventoryserviceapi/processorderitem/processorderitem.pb.h>


namespace example::inventory_service::functions {

// User-owned transport mapping. The generated endpoint owns request
// correlation, cancellation, metrics and graceful shutdown.
struct ProcessOrderItemSource final {
  using State = std::monostate;

  boost::asio::awaitable<servicelib::BeginResult<State>> beginRequest(
      servicelib::MessageContext context, auto&) const {
    co_return servicelib::BeginResult<State>{std::move(context), {}};
  }

  boost::asio::awaitable<void> consumeMessage(
      servicelib::MessageContext context, auto& stream_context, State&,
      const inventoryserviceapi::processorderitem::ProcessOrderItemRequest& request, auto result_context,
      auto& sender) const {
    (void)sender;
    result_context.setResultCallback(
        request.item_id(),
        [](
            servicelib::MessageContext callback_context,
            auto& callback_stream_context, State&,
            const example::model::types::OrderItemResult& result,
            auto& callback_sender) -> boost::asio::awaitable<bool> {
          (void)callback_context;
          (void)callback_stream_context;
          inventoryserviceapi::processorderitem::ProcessOrderItemResponse response;
          response.set_available_qty(result.available_qty);
          response.set_reserved(result.reserved);
          response.set_status(result.status);
          co_await callback_sender.send(std::move(response));
          co_return true;
        });

    co_await stream_context.collect(
        std::move(context),
        example::model::types::OrderItem{
            request.order_id(),
            request.item_id(),
            request.sku(),
            request.quantity(),
            0.0,
        });
  }

  boost::asio::awaitable<std::string> getMessageId(
      servicelib::MessageContext context, auto& stream_context, State&,
      const example::model::types::OrderItemResult& result) const {
    (void)context;
    (void)stream_context;
    co_return result.item_id;
  }

  boost::asio::awaitable<void> eof(servicelib::MessageContext, auto&, State&) const { co_return; }

  boost::asio::awaitable<void> endRequest(servicelib::MessageContext, auto&, std::exception_ptr,
                  State&) const { co_return; }
};

inline boost::asio::awaitable<std::unique_ptr<ProcessOrderItemSource>> MakeProcessOrderItemSource(
    servicelib::Context context, servicelib::IServiceEnvironment& environment,
    const auto& config) {
  (void)config;
  (void)context; (void)environment;
  co_return std::make_unique<ProcessOrderItemSource>();
}

}  // namespace example::inventory_service::functions

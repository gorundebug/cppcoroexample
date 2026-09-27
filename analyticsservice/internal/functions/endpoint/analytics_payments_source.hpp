#pragma once

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

#include <boost/asio/awaitable.hpp>

#include <servicelib/runtime/common.hpp>
#include <servicelib/runtime/config/endpoint_types.hpp>
#include <servicelib/runtime/environment/environment.hpp>
#include <servicelib/datasource/localsource/custom.hpp>
#include <analyticsservice/internal/types/analytics_event.hpp>


namespace example::analytics_service::functions {

struct AnalyticsPaymentsSource final
    : public servicelib::datasource::localsource::DataProducer<
          example::analytics_service::types::AnalyticsEvent> {
  using State = std::monostate;

  boost::asio::awaitable<void> start(
      servicelib::Context,
      typename servicelib::datasource::localsource::DataProducer<
          example::analytics_service::types::AnalyticsEvent>::Consumer consumer) override {
    using Event = example::analytics_service::types::AnalyticsEvent;
    co_await consumer(servicelib::MessageContext{},
             servicelib::Payload<Event>::make(Event{"high-value", 20, "payment"}));
    co_await consumer(servicelib::MessageContext{},
             servicelib::Payload<Event>::make(Event{"standard", 2, "payment"}));
  }

  boost::asio::awaitable<void> stop(servicelib::Context) override { co_return; }

  int concurrency(auto&) const noexcept { return 0; }

  boost::asio::awaitable<servicelib::BeginResult<State>> beginRequest(
      servicelib::MessageContext context, auto&) const {
    co_return servicelib::BeginResult<State>{std::move(context), {}};
  }

  boost::asio::awaitable<void> consumeMessage(
      servicelib::MessageContext context, auto& stream, State&,
      const example::analytics_service::types::AnalyticsEvent& value,
      auto result) const {
    co_await stream.collect(std::move(context), value);
    result.done();
  }

  boost::asio::awaitable<std::string> getMessageId(
      servicelib::MessageContext, auto&, State&,
      const std::monostate&) const {
    co_return std::string{};
  }

  boost::asio::awaitable<void> endRequest(
      servicelib::MessageContext, auto&, std::exception_ptr,
      State&) const { co_return; }
};

inline boost::asio::awaitable<std::unique_ptr<AnalyticsPaymentsSource>> MakeAnalyticsPaymentsSource(
    servicelib::Context context, servicelib::IServiceEnvironment& environment,
    const auto& config) {
  (void)config;
  (void)context;

  (void)environment;
  co_return std::make_unique<AnalyticsPaymentsSource>();
}

}  // namespace example::analytics_service::functions

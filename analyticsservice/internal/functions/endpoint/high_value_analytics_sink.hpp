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
#include <servicelib/datasink/localsink/custom.hpp>
#include <analyticsservice/internal/types/analytics_result.hpp>


namespace example::analytics_service::functions {

struct HighValueAnalyticsSink final
 {
  using State = std::monostate;

  boost::asio::awaitable<std::string> getStreamId(
      servicelib::MessageContext, const example::analytics_service::types::AnalyticsResult&) const {
    co_return std::string{};
  }

  boost::asio::awaitable<servicelib::BeginResult<State>> beginRequest(
      servicelib::MessageContext context, auto&) const {
    co_return servicelib::BeginResult<State>{std::move(context), {}};
  }

  boost::asio::awaitable<void> consumeMessage(
      servicelib::MessageContext, auto&, State&,
      const example::analytics_service::types::AnalyticsResult& value) const {
    if (value.key != "high-value" || value.total != 60 ||
        value.kind != "multi") {
      throw std::runtime_error("unexpected high-value analytics result");
    }
    co_return;
  }

  boost::asio::awaitable<void> endRequest(
      servicelib::MessageContext, auto&, std::exception_ptr,
      State&) const { co_return; }
};

inline boost::asio::awaitable<std::unique_ptr<HighValueAnalyticsSink>> MakeHighValueAnalyticsSink(
    servicelib::Context context, servicelib::IServiceEnvironment& environment,
    const auto& config) {
  (void)config;
  (void)context;

  (void)environment;
  co_return std::make_unique<HighValueAnalyticsSink>();
}

}  // namespace example::analytics_service::functions

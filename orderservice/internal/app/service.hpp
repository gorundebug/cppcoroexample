#pragma once

#include "orderservice/internal/app/service.generated.hpp"

namespace example::order_service::app {

// User-owned extension point. The generator never overwrites this file.
class Service final : public ServiceGenerated {
 public:
  using ServiceGenerated::ServiceGenerated;
  ~Service() override = default;

 protected:
  boost::asio::awaitable<void> customMakersInit(servicelib::Context context) override;
  boost::asio::awaitable<void> customFunctionsInit(servicelib::Context context) override;
  boost::asio::awaitable<void> serviceInit(servicelib::Context context) override;
  boost::asio::awaitable<void> serviceStarted(servicelib::Context context) override;
  boost::asio::awaitable<void> serviceStopping(servicelib::Context context) override;
};

}  // namespace example::order_service::app

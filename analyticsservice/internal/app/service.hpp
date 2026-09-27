#pragma once

#include "analyticsservice/internal/app/service.generated.hpp"

namespace example::analytics_service::app {

// User-owned extension point. The generator never overwrites this file.
class Service final : public ServiceGenerated {
 public:
  using ServiceGenerated::ServiceGenerated;
  ~Service() override = default;

 protected:
  boost::asio::awaitable<void> customMakersInit(servicelib::Context context) override;
  boost::asio::awaitable<void> customFunctionsInit(servicelib::Context context) override;
  boost::asio::awaitable<void> serviceInit() override;
  boost::asio::awaitable<void> serviceStarted() override;
  boost::asio::awaitable<void> serviceStopping() override;
};

}  // namespace example::analytics_service::app
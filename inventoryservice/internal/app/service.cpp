#include "inventoryservice/internal/app/service.hpp"

namespace example::inventory_service::app {

boost::asio::awaitable<void> Service::customMakersInit(servicelib::Context context) {
  (void)context;
  // Add only explicit user overrides here. Generated defaults stay in the
  // generated service and may change freely when the graph is regenerated.
  co_return;
}

boost::asio::awaitable<void> Service::customFunctionsInit(servicelib::Context context) {
  (void)context;
  // Add only explicit post-construction customization here.
  co_return;
}
boost::asio::awaitable<void> Service::serviceInit(servicelib::Context) { co_return; }
boost::asio::awaitable<void> Service::serviceStarted(servicelib::Context) { co_return; }
boost::asio::awaitable<void> Service::serviceStopping(servicelib::Context) { co_return; }

}  // namespace example::inventory_service::app

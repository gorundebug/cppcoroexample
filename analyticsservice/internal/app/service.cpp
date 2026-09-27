#include "analyticsservice/internal/app/service.hpp"

namespace example::analytics_service::app {

boost::asio::awaitable<void> Service::customMakersInit(servicelib::Context context) {
  (void)context;
  makers_.invoke_analytics_substream = [substream =
      getAnalyzeAnalyticsSubstreamSubStream()](
      servicelib::Context, servicelib::IServiceEnvironment&)
      -> boost::asio::awaitable<
          std::unique_ptr<functions::InvokeAnalyticsSubstream>> {
    co_return std::make_unique<functions::InvokeAnalyticsSubstream>(substream);
  };
  co_return;
}

boost::asio::awaitable<void> Service::customFunctionsInit(servicelib::Context context) {
  (void)context;
  // Add only explicit post-construction customization here.
  co_return;
}
boost::asio::awaitable<void> Service::serviceInit() { co_return; }
boost::asio::awaitable<void> Service::serviceStarted() { co_return; }
boost::asio::awaitable<void> Service::serviceStopping() { co_return; }

}  // namespace example::analytics_service::app

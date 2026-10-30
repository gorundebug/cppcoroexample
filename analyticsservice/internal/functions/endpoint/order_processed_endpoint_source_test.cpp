#include <concepts>

#include <gtest/gtest.h>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/use_future.hpp>

#include "analyticsservice/internal/functions/endpoint/order_processed_endpoint_source.hpp"

namespace example::analytics_service::functions {

struct TransportErrorStream {
  std::exception_ptr error;
  boost::asio::awaitable<void> collectError(servicelib::MessageContext, std::exception_ptr value) {
    error = std::move(value);
    co_return;
  }
};

TEST(OrderProcessedEndpointSource, Contract) {
  static_assert(std::default_initializable<OrderProcessedEndpointSource>);
  boost::asio::io_context executor;
  TransportErrorStream stream;
  const auto error = std::make_exception_ptr(std::runtime_error("broker unavailable"));
  OrderProcessedEndpointSource handler;
  auto done = boost::asio::co_spawn(executor,
      handler.onError(servicelib::MessageContext{}, stream, error), boost::asio::use_future);
  executor.run();
  done.get();
  EXPECT_EQ(stream.error, error);
}

}  // namespace example::analytics_service::functions

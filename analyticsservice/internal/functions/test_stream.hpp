#pragma once

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/use_future.hpp>
#include <servicelib/runtime/base.hpp>
#include <servicelib/runtime/context.hpp>
#include <utility>
#include <vector>

namespace example::analytics_service::functions::test {

template <typename T>
T Run(boost::asio::awaitable<T> operation) {
  boost::asio::io_context io;
  auto result = boost::asio::co_spawn(io, std::move(operation), boost::asio::use_future);
  io.run();
  return result.get();
}

class Stream final : public servicelib::StreamBase {
 private:
  size_t buildTopology(servicelib::StreamBuilderContext&, size_t id,
                       std::vector<size_t>*, bool) override { return id; }
  void verifyTopology(servicelib::StreamVerifyContext&) const override {}
  void printTopology(servicelib::TopologyPrinter&,
                     std::unordered_set<size_t>&) const override {}
};

template <typename T>
struct Collector final {
  std::vector<T> values;
  boost::asio::awaitable<void> out(servicelib::MessageContext, T value) {
    co_await boost::asio::post(boost::asio::use_awaitable);
    values.push_back(std::move(value));
  }
};

}  // namespace example::analytics_service::functions::test

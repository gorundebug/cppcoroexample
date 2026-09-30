#pragma once

#include <utility>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/use_future.hpp>

#include <servicelib/runtime/base.hpp>

namespace example::order_service::functions::test {

// Run only from the test thread, never from an application worker.
template <typename T>
T Run(boost::asio::awaitable<T> operation) {
  boost::asio::io_context io;
  auto result = boost::asio::co_spawn(io, std::move(operation), boost::asio::use_future);
  io.run();
  return result.get();
}

class Stream final : public servicelib::StreamBase {
 public:
  ~Stream() override = default;

 private:
  size_t buildTopology(servicelib::StreamBuilderContext&, size_t id,
                       std::vector<size_t>*, bool) override {
    return id;
  }

  void verifyTopology(servicelib::StreamVerifyContext&) const override {}

  void printTopology(servicelib::TopologyPrinter&,
                     std::unordered_set<size_t>&) const override {}
};

}  // namespace example::order_service::functions::test

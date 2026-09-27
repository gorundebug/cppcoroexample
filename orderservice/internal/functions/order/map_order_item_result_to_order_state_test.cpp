#include <optional>
#include <utility>

#include <gtest/gtest.h>

#include "orderservice/internal/functions/order/map_order_item_result_to_order_state.hpp"
#include "orderservice/internal/functions/test_stream.hpp"

namespace example::order_service::functions {

namespace {
struct StateCollector final {
  std::optional<example::order_service::types::OrderState> value;
  boost::asio::awaitable<void> out(servicelib::MessageContext,
           example::order_service::types::OrderState state) {
    co_await boost::asio::post(boost::asio::use_awaitable);
    value = std::move(state);
    co_return;
  }
};
}  // namespace

TEST(MapOrderItemResultToOrderState, MapsReservationResult) {
  MapOrderItemResultToOrderState function;
  StateCollector out;
  test::Stream stream;
  test::Run(function(
      servicelib::MessageContext{},
      stream,
      example::model::types::OrderItemResult{
          "order-1", "item-1", "SKU-001", 2, 2, true, "CONFIRMED", 7.0, {}},
      out));

  ASSERT_TRUE(out.value.has_value());
  EXPECT_EQ(out.value->order_id, "order-1");
  EXPECT_EQ(out.value->status, "CONFIRMED");
  ASSERT_EQ(out.value->confirmed_items.size(), 1);
  EXPECT_EQ(out.value->confirmed_items.front().item_id, "item-1");
}

}  // namespace example::order_service::functions

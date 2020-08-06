#include "../orderbook.hpp"

#include "catch.hpp"

TEST_CASE("Orderbook", "[orderbook]") {
	SECTION("price gap") {
		orderbook ob;
		REQUIRE_THROWS_AS(ob.price_gap(), std::out_of_range);

		ob.add_bid(10.0, order("", 2.0));
		ob.add_ask(11.0, order("", 2.0));
		REQUIRE(ob.price_gap() == Approx(1.0));
	}
	
	SECTION("bids above") {
		orderbook ob;
		REQUIRE(ob.bids_above(0.0) == Approx(0.0));
		ob.add_bid(1.0, order("", 2.0));
		REQUIRE(ob.bids_above(0.0) == Approx(2.0));
		ob.add_bid(3.0, order("", 3.0));
		REQUIRE(ob.bids_above(0.0) == Approx(5.0));
		REQUIRE(ob.bids_above(2.0) == Approx(3.0));
		REQUIRE(ob.bids_above(3.0) == Approx(0.0));
	}
	
	SECTION("asks above") {
		orderbook ob;
		REQUIRE(ob.asks_below(100.0) == Approx(0.0));
		ob.add_ask(1.0, order("", 2.0));
		REQUIRE(ob.asks_below(2.0) == Approx(2.0));
		ob.add_ask(3.0, order("", 3.0));
		REQUIRE(ob.asks_below(0.0) == Approx(0.0));
		REQUIRE(ob.asks_below(2.0) == Approx(2.0));
		REQUIRE(ob.asks_below(3.0) == Approx(2.0));
		REQUIRE(ob.asks_below(4.0) == Approx(5.0));
	}
	
	SECTION("price to buy") {
		orderbook ob;
		REQUIRE_THROWS_AS(ob.price_to_buy(-1.0), std::invalid_argument);
		REQUIRE_THROWS_AS(ob.price_to_buy(1.0), std::invalid_argument);
		ob.add_ask(3.0, order("", 4.0));
		REQUIRE_THROWS_AS(ob.price_to_buy(5.0), std::invalid_argument);
		REQUIRE(ob.price_to_buy(2.0) == Approx(6.0));
		ob.add_ask(2.0, order("", 4.0));
		REQUIRE(ob.price_to_buy(5.0) == Approx(11.0));
	}
	
	SECTION("price to sell") {
		orderbook ob;
		REQUIRE_THROWS_AS(ob.price_to_sell(-1.0), std::invalid_argument);
		REQUIRE_THROWS_AS(ob.price_to_sell(1.0), std::invalid_argument);
		ob.add_bid(3.0, order("", 4.0));
		REQUIRE_THROWS_AS(ob.price_to_sell(5.0), std::invalid_argument);
		REQUIRE(ob.price_to_sell(2.0) == Approx(6.0));
		ob.add_bid(2.0, order("", 4.0));
		REQUIRE(ob.price_to_sell(5.0) == Approx(14.0));
	}
	
	SECTION("reduce offer") {
		orderbook ob;
		REQUIRE_THROWS_AS(ob.reduce_offer("aaa", 1.0), std::invalid_argument);
		ob.add_bid(2.0, order("aaa", 4.0));
		REQUIRE_THROWS_AS(ob.reduce_offer("bbb", 1.0), std::invalid_argument);
		REQUIRE(ob.reduce_offer("aaa", 1.0) == Side::Bid);
		REQUIRE(ob.total_bids() == Approx(3.0));
		ob.add_ask(3.0, order("bbb", 4.0));
		REQUIRE(ob.reduce_offer("bbb", 1.0) == Side::Ask);
	}
}

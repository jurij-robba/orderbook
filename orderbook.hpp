#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

/*----------------------------------------------------------------------------*\

							orderbook.hpp

						Copyright 2020 Jurij Robba

							All rights reserved

\*----------------------------------------------------------------------------*/


#include <map>
#include <vector>
#include <stdexcept>
#include <limits>

// Offer side
enum class Side {
	Invalid,
	Bid,
	Ask,
};

// Order stored in book
struct order {
	order(const std::string& id_val, const double& amount_val) : id(id_val),
		amount(amount_val) {}
	std::string id;
	double amount;
};

// Market orderbook for single instrument
// Quantities are stored into double to also enable markets
// Where you can buy parts of a unit (like forex)
class orderbook {

public:

	// Typedef for clarity
	using book_t = std::map<double, std::vector<order>>;

private:

	// Map of all bids
	book_t _bids;
	
	// Map of all tasks
	book_t _asks;
	
	// Reduce order in general orderbook
	// (helper function to reduce code replication) 
	static bool reduce(book_t& book, const std::string& id,
						const double amount);

public:

	// default constructor
	explicit orderbook() = default;

	// Accessors
	[[nodiscard]]
	inline const book_t& bids() const noexcept {
		return _bids;
	}
	[[nodiscard]]
	inline const book_t& asks() const noexcept {
		return _asks;
	}

	// best bid
	[[nodiscard]]
	inline std::pair<double, std::vector<order>> best_bid() const {
		return *_bids.rbegin();
	}

	// best ask
	[[nodiscard]]
	inline std::pair<double, std::vector<order>> best_ask() const {
		return *_asks.begin();
	}

	// price gap
	[[nodiscard]]
	inline double price_gap() const {
		if(_asks.empty() || _bids.empty()) {
			throw std::out_of_range("Bids or Asks empty");
		}
		return _asks.begin()->first - _bids.rbegin()->first;
	}

	// amount above price
	[[nodiscard]]
	double bids_above(double const& price) const;

	// amount below price
	[[nodiscard]]
	double asks_below(const double& price) const;

	// total bids
	[[nodiscard]]
	inline double total_bids() const {
		return bids_above(-1.0);
	}

	// total asks
	[[nodiscard]]
	inline double total_asks() const {
		return asks_below(std::numeric_limits<double>::max());
	}
	
	// Total price of buying [amount] of asset
	// throws invalid argument if there isn't enough orders to get price
	// or if amount is less than 0 (invalid)
	[[nodiscard]]
	double price_to_buy(double amount);
	
	// Total recieved by selling [amount] of asset
	// throws invalid argument if there isn't enough orders to get price
	// or if amount is less than 0 (invalid)
	[[nodiscard]]
	double price_to_sell(double amount);

	// add new ask
	// relies on order.id being unique, but doesn't check it
	// also relies on price to be bigger than largest bid, but doesn't check
	inline void add_ask(const double& price, const order& new_order) {
		_asks[price].emplace_back(new_order);
	}

	// add new bid
	// relies on order.id being unique, but doesn't check it
	// also relies on price to be smaller than smallest ask, but doesn't check
	inline void add_bid(const double& price, const order& new_order) {
		_bids[price].emplace_back(new_order);
	}

	// reduce offer by id
	// throws invalid_argument when id isn't found or if amout is < 0
	Side reduce_offer(const std::string& id, const double amount);
};

#endif

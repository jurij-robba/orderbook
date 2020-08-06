#include "orderbook.hpp"

#include <iostream>

bool orderbook::reduce(book_t& book, const std::string& id, const double amount)
{
	for(auto book_it = book.begin(); book_it != book.end(); ++book_it) {
		auto order_it = book_it->second.begin();
		for(;order_it != book_it->second.end(); ++order_it) {
			// reduce offer
			if(order_it->id == id) {
				order_it->amount -= amount;
				// remove if it is now empty
				if (order_it->amount < 0.0) {
					book_it->second.erase(order_it);
				}
				break;
			}
		}
		// order was found
		if(order_it != book_it->second.end()) {
			// remove price level if there is no offers left
			if(book_it->second.empty()) {
				book.erase(book_it);
			}
			return true;
		}
	}
	return false;
}

// bids above
double orderbook::bids_above(const double& price) const {
	double sum = 0.0;
	for(auto it = _bids.rbegin(); it != _bids.rend(); ++it) {
		if(it->first <= price) {
			return sum;
		}
		for(const auto& order : it->second) {
			sum += order.amount;
		}
	}
	return sum;
}

// asks below
double orderbook::asks_below(const double& price) const {
	double sum = 0.0;
	for(auto it = _asks.begin(); it != _asks.end(); ++it) {
		if(it->first >= price) {
			return sum;
		}
		for(const auto& order : it->second) {
			sum += order.amount;
		}
	}
	return sum;
}

// Total price of buying [amount] of asset
double orderbook::price_to_buy(double amount) {
	if(amount < 0.0) {
		throw std::invalid_argument("amount");
	}
	double sum = 0.0;
	for(auto it = _asks.begin(); it != _asks.end(); ++it) {
		for(const order& order : it->second) {
			if(order.amount >= amount) {
				sum += amount * it->first;
				return sum;
			}
			else {
				sum += order.amount * it->first;
				amount -= order.amount;
			}
		}
	}
	throw std::invalid_argument("amount");
}

// Total recieved by selling [amount] of asset
double orderbook::price_to_sell(double amount) {
	if(amount < 0.0) {
		throw std::invalid_argument("amount");
	}
	double sum = 0.0;
	for(auto it = _bids.rbegin(); it != _bids.rend(); ++it) {
		for(const order& order : it->second) {
			if(order.amount >= amount) {
				sum += amount * it->first;
				return sum;
			}
			else {
				sum += order.amount * it->first;
				amount -= order.amount;
			}
		}
	}
	throw std::invalid_argument("amount");
}

Side orderbook::reduce_offer(const std::string& id, const double amount) {
	if(amount < 0.0) {
		throw std::invalid_argument("amount");
	}
	if(reduce(_bids, id, amount)) {
		return Side::Bid;
	}
	if(reduce(_asks, id, amount)) {
		return Side::Ask;
	}
	throw std::invalid_argument("id");
}

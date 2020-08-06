#include "pricer.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <optional>

Side pricer::parse_add(const std::vector<std::string>& parts) {
	// verify add
	if(parts[1] != "A") {
		std::cerr << "Error parsing add message: " << parts[1] << " != A"
			<< std::endl;
		return Side::Invalid;
	}
	
	size_t parsed = 0;
	
	// parse price
	double price = std::stod(parts[4], &parsed);
	if(parsed != parts[4].size()) {
		std::cerr << "Error parsing price from "
			<< parts[4] << std::endl;
		return Side::Invalid;
	}
	
	// parse amount
	int amount = std::stoi(parts[5], &parsed);
	if(parsed != parts[5].size()) {
		std::cerr << "Error parsing amount from "
			<< parts[5] << std::endl;
		return Side::Invalid;
	}
	
	// Verify side and add to orderbook
	if(parts[3] == "B") {
		_book.add_bid(price, order(parts[2], amount));
		return Side::Bid;
	}
	else if(parts[3] == "S") {
		_book.add_ask(price, order(parts[2], amount));
		return Side::Ask;
	}
	else {
		std::cerr << "Error parsing offer side from "
			<< parts[3] << std::endl;
		return Side::Invalid;
	}
}
Side pricer::parse_reduce(const std::vector<std::string>& parts) {

	// verify reduce
	if(parts[1] != "R") {
		std::cerr << "Error parsing message: " << parts[1]
			<< " != R" << std::endl;
		return Side::Invalid;
	}
	
	size_t parsed = 0;
	
	// parse amount
	int amount = std::stoi(parts[3], &parsed);
	if(amount < 0 || parsed != strlen(parts[3].c_str())) {
		std::cerr << "Error parsing amount from: " << parts[3]
			<< std::endl;
		return Side::Invalid;
	}
	
	// Try to reduce order
	try {
		return _book.reduce_offer(parts[2], static_cast<double>(amount));
	}
	catch(std::invalid_argument&) {
		std::cerr << "Invalid message id: " << parts[2] << std::endl;
		return Side::Invalid;
	}
}

Side pricer::parse_line(const std::vector<std::string>& parts) {

    // add order
    if(parts.size() == 6) {
    	return parse_add(parts);
    } 
    // Reduce order
    else if (parts.size() == 4) {
    	return parse_reduce(parts);
    }
    // There was an error
    else {
    	
    	return Side::Invalid;
    }
}

// Const/home/neo/Projects/orderbook/pricer.hppructor
pricer::pricer(const size_t& target_size_val) :
	_target_size(static_cast<double>(target_size_val)) { }

// Event loop
void pricer::run() {

	// save last prices
	std::optional<double> last_price_to_buy;
	std::optional<double> last_price_to_sell;
	
	// set output format
	std::cout << std::fixed << std::setprecision(2);
	
	// start event loop
	std::string line;
	while(getline(std::cin, line) && !std::cin.eof() && !line.empty()) {
	
		// split string
		std::istringstream iss(line);
		std::vector<std::string> data(std::istream_iterator<std::string>{iss},
		                         std::istream_iterator<std::string>());
                             
		// parse line
		Side side = parse_line(data);
		
		// print to standard output
        if(side == Side::Bid) {
        	try {
        		double price_to_sell = _book.price_to_sell(_target_size);
        		if(price_to_sell != last_price_to_sell) {
        			std::cout << data[0] << " S "
        				<< price_to_sell << std::endl;
        			last_price_to_sell = std::make_optional(price_to_sell);
        		}
        	}
        	catch(std::invalid_argument&) {
        		if(last_price_to_sell.has_value()) {
        			last_price_to_sell.reset();
        			std::cout << data[0] << " S NA" << std::endl;
        		}
        	}
        }
        else if(side == Side::Ask) {
        	try {
        		double price_to_buy = _book.price_to_buy(_target_size);
        		if(price_to_buy != last_price_to_buy) {
		    		std::cout << data[0] << " B "
		    			<< price_to_buy
		    			<< std::endl;
        			last_price_to_buy = std::make_optional(price_to_buy);
        		}
        	}
        	catch(std::invalid_argument&) {
        		if(last_price_to_buy.has_value()) {
        			last_price_to_buy.reset();
        			std::cout << data[0] << " B NA" << std::endl;
        		}
        	}
        }
        else {
        	std::cerr << "Error parsing message: " << line << std::endl;
        }
	}
}

#ifndef PRICER_HPP
#define PRICER_HPP

#include "orderbook.hpp"

// Application core
class pricer {

private:

	// orderbook uses double for quantities
	double _target_size;
	
	// orderbook
	orderbook _book;
	
	// parsers for types of message. Will print any errors in cerr
	// and return the side of message. On Side::invalid orderbook wasn't
	// changed
	Side parse_add(const std::vector<std::string>& parts);
	Side parse_reduce(const std::vector<std::string>& parts);
	
	
	// parses input line, storing data in orderbook and printing
	// messages to cerr
	// Returns Side that was updated. If side is invalid orderbook
	// wasn't changed
	Side parse_line(const std::vector<std::string>& parts);
	
public:
	
	// prevent unitentional initialisations of new core
	pricer() = delete;
	pricer(const pricer&) = delete;
	pricer(pricer&&) = delete;
	
	// initialize pricer for given size;
	pricer(const size_t& target_size_val);
	
	// run event loop
	void run();	
};

#endif

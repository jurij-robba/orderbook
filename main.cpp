#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>

#include "pricer.hpp"

// version constant
static constexpr const char* VERSION = "0.1.0";

// prints help message on std out
void print_help() {
	std::cout << "Pricer v" << VERSION << std::endl;
	std::cout << "Usage: pricer [arg]" << std::endl;
	std::cout << "Avaliable arguments:" << std::endl;
	std::cout << "\t* -v\t--version\tDisplays version " << std::endl;
	std::cout << "\t* -h\t--help\t\tDisplays this help message " << std::endl;
	std::cout << "\t* target-size\t\texecutes pricer for number of shares"
		<< std::endl;
}

// Prints version to std out (given argument)
[[nodiscard]]
bool handle_version(char* cstr) {
	if(strcmp(cstr, "-v") == 0 || strcmp(cstr, "--version") == 0) {
		std::cout << VERSION << std::endl;
		return true;
	}
	return false;
}

// Prints help to std out (given argument)
[[nodiscard]]
bool handle_help(char* cstr) {
	if(strcmp(cstr, "-h") == 0 || strcmp(cstr, "--help") == 0) {
		print_help();
		return true;
	}
	return false;
}

// Parse arguments and act on them
// Will return target size if it is valid argument
// Quits otherwise
[[nodiscard]]
size_t parse_args(int argc, char* argv[]) {
	// wrong number of arguments
	if(argc != 2) {
		std::cout << "Wrong number of arguments" << std::endl;
		print_help();
		std::exit(0);
	}
	// utility parsers
	else if(handle_version(argv[1]) || handle_help(argv[1])) {
		std::exit(0);
	}
	// verify and parse target size
	else {
		size_t parsed = 0;
		int target_size = std::stoi(argv[1], &parsed, 10);
		if(target_size > 0 && parsed == strlen(argv[1])) {
			return static_cast<size_t>(target_size);
		}
		std::cout << "Argument must be valid positive number" << std::endl;
		std::exit(0);
	}
}

//==================================MAIN======================================//

int main(int argc, char* argv[]) {

	// Parse arguments
	size_t target_size = parse_args(argc, argv);
	
	// initialize core with target size and start execution
	pricer core(target_size);
	core.run();
}

# compiler
CC = g++

# compiler flags
CFLAGS = -std=c++2a -O3 -Wall -Wextra -Wfloat-equal -Wundef -Wshadow \
	-Wpointer-arith -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual \
	-Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code \
	-Winit-self -march=native -Wold-style-cast -Woverloaded-virtual \
	-pedantic-errors -Wuninitialized -Werror -fdiagnostics-show-option \
	-fno-elide-constructors -Wstrict-aliasing -Wcast-align -Wcast-qual \
	-Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Wlogical-op \
	-Wmissing-include-dirs -Wnoexcept -Wredundant-decls -Wshadow \
	-Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef \
	-Wno-unused -Wno-variadic-macros -Wno-parentheses

#------------------------------------------------------------------------------#

default: build

build:
	$(CC) $(CFLAGS) *.cpp -o ./pricer
	
run: build
	./pricer 200

clean:
	rm -f ./pricer
	
#------------------------------------------------------------------------------#

test: clean_test
	$(CC) $(CFLAGS) ./test/orderbook_test.cpp ./test/tests.cpp orderbook.cpp -o ./test/test
	./test/test

update_test:
	rm -f ./test/catch.hpp
	wget https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp -P ./test/
clean_test:
	rm -f ./test/test

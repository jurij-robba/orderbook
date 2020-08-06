# Pricer

An application that parses orderbook messages and displays updates for the price
to buy and sell certain amount of shares.

Application is tested on Pop OS 20.04 with gcc 9.3 and make 4.2.1.

## Usage

Pricer parses standard input, displays results in standard output and any error
messages into standard error stream.

```
	pricer <number of shares>

```

will start application for given amount of shares.

Other arguments:
 * `-v` displays version and quits
 * `-h` displays help and quits
 

## Building

`make` builds application.
`make run` build and runs application with default of 200 shares.
`make clean` cleans current build

## Testing

Orderbook class is ment to be reusable library grade code and is tested
using [Catch2](https://github.com/catchorg/Catch2) framework.

To download / update latest version run

```
make update_test
```

To test orderbook implementation run `make test`
To clean test run `make clean_test`



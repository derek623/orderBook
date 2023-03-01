Simple order book

------------------------------
DEVELOPMENT PLATFORM
------------------------------

- WSL Ubuntu 20.04.5 LTS
- gcc version 9.4.0
- C++ 20 (minimum)
- cmake 3.16.3

------------------------------
BUILD INSTRUCTION
------------------------------

Unzip the folder, then go to the orderBook folder. 
For release build, type
1) "cmake . -DCMAKE_BUILD_TYPE=Release"
2) make
3) The generated binary "VivCourt" is in bin/Release 

For debug build, type
1) "cmake . -DCMAKE_BUILD_TYPE=Debug"
2) make
3) The generated binary "VivCourt" is in bin/Debug. 
4) A unit test binary is also generated in test/VivCourt_test. This project uses gtest and cmake will download the gtest library from https://github.com/google. 
    The dev server needs to be able to access the link.
5) ctest to run the unit tests (actual test binary is generated in the orderBook/test folder). The test binary (test/VivCourt_test) can
    also be run on it's own.

Both the debug and release build can be run like below:
cat input1.stream | ./VivCourt 5

------------------------------
DESIGN
------------------------------

There are 4 main components
1) Parser: it parses the stream for different message types
2) Exchange: it stores all orders and the orderBook for each symbol
3) OrderBook: it stores the buy and sell price depth
4) DisplayHandle: it handles whether output is generated based on the affected price level
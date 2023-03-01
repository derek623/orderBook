Simple order book

------------------------------
DEVELOPMENT PLATFORM
------------------------------

- WSL Ubuntu 20.04.5 LTS
- gcc version 9.4.0
- C++ 20
- cmake 3.16.3

------------------------------
BUILD INSTRUCTION
------------------------------

Unzip the folder, then go to the orderBook folder. 
For release build, type
1) "cmake . -DCMAKE_BUILD_TYPE=Release"
2) make

For debug build, type
1) "cmake . -DCMAKE_BUILD_TYPE=Release"
2) make
3) ctest to run the unit tests (actual test binary is generated in the orderBook/test folder)

------------------------------
DESIGN
------------------------------

There are 4 main components
1) Parser: it parses the stream for different message types
2) Exchange: it stores all orders and the orderBook for each symbol
3) OrderBook: it stores the buy and sell price depth
4) DisplayHandle: it handles whether output is generated based on the affected price level
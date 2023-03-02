#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "displayHandler.hpp"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
      std::cout << "Please provide the display level as the first argument" << std::endl;
      return 0;;
    }
    
    try
    {
      uint64_t l = strtoul(argv[1], nullptr, 10);
      DisplayHandler dh(l);
      Exchange<DisplayHandler> exchange(dh);
      Parser p(cin, exchange);
      p.parse();
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
    
    return 0;
    
}
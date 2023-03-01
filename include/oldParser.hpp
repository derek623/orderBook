#ifndef OLDPARSER
#define OLDPARSER

#include <iostream>
#include <fstream>
#include <memory>
#include "exchange.hpp"

using namespace std;

class OldParser {
    public:
        OldParser(istream& stream, Exchange& exchange) : _s{stream}, _exchange{exchange}{}
        void oldParse()
        {
            while(_s)
            {
                    uint32_t seqno = ReadU32(_s);
                    uint32_t size = ReadU32(_s);
                    cout << " seqno is " << seqno << " size is " << size << endl;
                    if(seqno == 0 || size == 0) // something wrong with the data
                        continue;
                    char type;
                    _s.read( (char*)&type, 1 );
                    cout << "Order type: " <<  type << endl;                    
                    switch(type)
                    {
                        case 'A':
                            addOrder(seqno, size - 1);
                            break;
                        case 'U':
                            updateOrder(seqno, size - 1);
                            break;
                        case 'D':
                            deleteOrder(seqno, size - 1);
                            break;
                        case 'E':
                            executeOrder(seqno, size - 1);
                            break;
                        default:
                            cout << " Got invalid type: " << type << endl;
                    }
            }
            _exchange.printBook();
        }
        void addOrder(uint32_t seqno, uint32_t size)
        {
            cout << " New order" << endl;
            //Get the symbol
            auto sym = ReadAlpha(_s, 3);
            size -= 3;
            string symbol{sym.get()};
            //Get the order id
            uint64_t orderId = ReadU64(_s);
            size -= 8;
            //Get the side
            char side;
            _s.read( (char*)&side, 1 );
            size -= 1;

            //Read the pad                      
            auto pad = ReadAlpha(_s, 3);

            //Get quantity
            uint64_t quantity = ReadU64(_s);
            size -= 8;

            //Get price
            int32_t price = Read32(_s);
            size -= 4;

            //Read the pad
            auto pad2 = ReadAlpha(_s, 4);
            size -= 4;

            cout << " Symbol: " << symbol << " Order id: " << orderId << " Side: " << side << " Quantity: " << quantity << " Price: " << price << 
                " size: " << size << endl;

            _exchange.addOrder(seqno, symbol, orderId, side, quantity, price);
            
        }
        void updateOrder(uint32_t seqno, uint32_t size)
        {
            cout << " Update order" << endl;
            //Get the symbol
            auto sym = ReadAlpha(_s, 3);
            size -= 3;
            string symbol{sym.get()};
            //Get the order id
            uint64_t orderId = ReadU64(_s);
            size -= 8;
            //Get the side
            char side;
            _s.read( (char*)&side, 1 );
            size -= 1;

            //Read the pad                     
            auto pad = ReadAlpha(_s, 3);
            size -= 3;

            //Get quantity
            uint64_t quantity = ReadU64(_s);
            size -= 8;

            //Get price
            int32_t price = Read32(_s);
            size -= 4;

            //Read the pad
            auto pad2 = ReadAlpha(_s, 4);
            size -= 4;

            cout << " Symbol: " << symbol << " Order id: " << orderId << " Side: " << side << " Quantity: " << quantity << " Price: " << price << 
                " size: " << size << endl;

            _exchange.updateOrder(seqno, symbol, orderId, quantity, price);
        }

        void deleteOrder(uint32_t seqno, uint32_t size)
        {
            cout << " Delete order" << endl;
            //Get the symbol
            auto sym = ReadAlpha(_s, 3);
            size -= 3;
            string symbol{sym.get()};
            //Get the order id
            uint64_t orderId = ReadU64(_s);
            size -= 8;
            //Get the side
            char side;
            _s.read( (char*)&side, 1 );
            size -= 1;

            //Read the pad                      
            auto pad = ReadAlpha(_s, 3);
            size -= 3;

            cout << " Symbol: " << symbol << " Order id: " << orderId << " Side: " << side << endl;
            //deleteOrder(const uint32_t& seqno, const std::string& symbol, const uint64_t& orderId, const char& side)
            _exchange.deleteOrder(seqno, symbol, orderId);
        }

        void executeOrder(uint32_t seqno, uint32_t size)
        {
            cout << "Execute order " << endl;
            //Get the symbol
            //char sym[4] = {0};
            //_s.read( (char*)sym, 3 );
            auto sym = ReadAlpha(_s, 3);
            size -= 3;
            string symbol{sym.get()};
            //Get the order id
            uint64_t orderId = ReadU64(_s);
            size -= 8;
            //Get the side
            char side;
            _s.read( (char*)&side, 1 );
            size -= 1;

            //Read the pad                      
            auto pad = ReadAlpha(_s, 3);
            size -= 3;
            //Get the traded quantity
            uint64_t tradedQuantity = ReadU64(_s);
            size -= 8;
            cout << " Symbol: " << symbol << " Order id: " << orderId << " Side: " << side << " tradedQuantity: " << tradedQuantity <<
                " size: " << size << endl;
            _exchange.executeOrder(seqno, symbol, orderId, tradedQuantity);
        }
    private:
        shared_ptr<char[]> ReadAlpha(istream& file, uint32_t size)
        {
            shared_ptr<char[]> tmp(new char[size + 1]{0});
            _s.read( tmp.get(), size );
            return tmp;
        }
        uint32_t ReadU32(istream& file)
        {
            uint32_t val;
            unsigned bytes[4] = {0};

            file.read( (char*)bytes, 4 );  // read 4 bytes from the file
            val = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);  // construct the 32-bit value from those bytes
            //val = bytes[0]  // construct the 32-bit value from those bytes

            return val;
        }

        int32_t Read32(istream& file)
        {
            int32_t val;
            unsigned bytes[4] = {0};

            file.read( (char*)bytes, 4 );  // read 4 bytes from the file
            val = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);  // construct the 32-bit value from those bytes
            //val = bytes[0]  // construct the 32-bit value from those bytes

            return val;
        }

        unsigned long long ReadU64(istream& file)
        {
            unsigned long long val = 0;
            unsigned long long val2 = 0;
            unsigned long long finalVal = 0;
            unsigned bytes1[4] = {0};
            unsigned bytes2[4] = {0};

            file.read( (char*)bytes1, 4 );  // read 4 bytes from the file
            file.read( (char*)bytes2, 4 );  // read 4 bytes from the file
            //val = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24) | (bytes[4] << 32) | (bytes[5] << 40) 
            //  | (bytes[6] << 48) | (bytes[7] << 56);  // construct the 32-bit value from those bytes
            val = bytes1[0] | (bytes1[1] << 8) | (bytes1[2] << 16) | (bytes1[3] << 24);
            val2 = bytes2[0] | (bytes2[1] << 8) | (bytes2[2] << 16) | (bytes2[3] << 24);
            finalVal = val | val2 << 32;
            //cout << " Val1: " << hex << val << endl;
            //cout << " Val2: " << hex << val2 << endl;
            //cout << " finalVal: " << hex << finalVal << endl;
            //cout << " finalVal(dec): " << finalVal << endl;
            //val = bytes[0]  // construct the 32-bit value from those bytes

            return finalVal;
        }
        istream& _s;
        Exchange& _exchange;
};

#endif
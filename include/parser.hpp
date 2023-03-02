#ifndef PARSER
#define PARSER

#include <iostream>
#include <fstream>
#include <memory>
#include "exchange.hpp"

using namespace std;

template <typename Handler>
class Parser {
    public:
        Parser(istream& stream, Exchange<Handler>& exchange) : _s{stream}, _exchange{exchange}{}
        void parse()
        {
            while(_s)
            {
                char buf[4];
                _s.read(buf, 4);
                uint32_t seqno = ReadU32(buf);
                memset(buf, 0, 4);
                _s.read(buf, 4);
                uint32_t size = ReadU32(buf);
                
                if(seqno == 0 || size == 0) // something wrong with the data
                    return;
                
                shared_ptr<char[]> tmp(new char[size]{0}); //prepare a buffer for storing each message
                _s.read( tmp.get(), size );

                parse(seqno, size, tmp.get());
            }
        }
        void parse(uint32_t seqno, uint32_t size, const char* buf)
        {   
                                          
            char type;
            type = buf[0];
            
            switch(type)
            {
                case 'A':
                    addOrder(buf + 1, seqno, size - 1);
                    break;
                case 'U':
                    updateOrder(buf + 1, seqno, size - 1);
                    break;
                case 'D':
                    deleteOrder(buf + 1, seqno, size - 1);
                    break;
                case 'E':
                    executeOrder(buf + 1, seqno, size - 1);
                    break;
                default:
                    cout << " Got invalid type: " << type << endl;
            }
            
        }
        void addOrder(const char* buf, uint32_t seqno, uint32_t size)
        {
            const char *curBuf = buf;
            
            //Get the symbol            
            string_view symbol{buf, 3};
            curBuf += 3;
            
            //Get the order id
            uint64_t orderId = ReadU64(curBuf);
            curBuf += 8;
            
            //Get the side
            char side = (curBuf)[0];
            curBuf += 1;

            //Skip the pad                      
            curBuf += 3;

            //Get quantity
            uint64_t quantity = ReadU64(curBuf);
            curBuf += 8;

            //Get price
            int32_t price = Read32(curBuf);
            curBuf += 4;

            //Skip the pad
            curBuf += 4;
            //Soomthing wrong with this message, ignore it
            if(curBuf - buf != size)
            {
                cout << "Ignore mesage" << endl;
                return;
            }        

            _exchange.addOrder(seqno, symbol, orderId, side, quantity, price);
            
        }
        void updateOrder(const char* buf, uint32_t seqno, uint32_t size)
        {
            const char *curBuf = buf;
            //Get the symbol
            string_view symbol{buf, 3};
            curBuf += 3;
            
            //Get the order id
            uint64_t orderId = ReadU64(curBuf);
            curBuf += 8;
            //Get the side
            char side = curBuf[0];
            curBuf += 1;

            //Skip the pad                     
            curBuf += 3;

            //Get quantity
            uint64_t quantity = ReadU64(curBuf);
            curBuf += 8;

            //Get price
            int32_t price = Read32(curBuf);
            curBuf += 4;

            //Read the pad
            curBuf += 4;

            //Soomthing wrong with this message, ignore it
            if(curBuf - buf != size)
            {
                cout << "Ignore mesage" << endl;
                return;
            }

            _exchange.updateOrder(seqno, symbol, orderId, quantity, price);
        }

        void deleteOrder(const char* buf, uint32_t seqno, uint32_t size)
        {
            const char *curBuf = buf;
            //Get the symbol        
            string_view symbol{buf, 3};
            curBuf += 3;
            
            //Get the order id
            uint64_t orderId = ReadU64(curBuf);
            curBuf += 8;
            //Get the side
            char side = curBuf[0];
            curBuf += 1;

            //Read the pad                      
            curBuf += 3;

            //Soomthing wrong with this message, ignore it
            if(curBuf - buf != size)
            {
                cout << "Ignore mesage" << endl;
                return;
            }

            _exchange.deleteOrder(seqno, symbol, orderId);
        }

        void executeOrder(const char* buf, uint32_t seqno, uint32_t size)
        {
            const char *curBuf = buf;
            //Get the symbol
            string_view symbol{buf, 3};
            curBuf += 3;
            
            //Get the order id
            uint64_t orderId = ReadU64(curBuf);
            curBuf += 8;
            //Get the side
            char side = curBuf[0];
            curBuf += 1;

            //Read the pad                      
            curBuf += 3;
            //Get the traded quantity
            uint64_t tradedQuantity = ReadU64(curBuf);
            curBuf += 8;

            if(curBuf - buf != size)
            {
                cout << "Ignore mesage" << endl;
                return;
            }

            _exchange.executeOrder(seqno, symbol, orderId, tradedQuantity);
        }
    private:
        uint32_t ReadU32(const void* buf)
        {
            if constexpr (std::endian::native == std::endian::big)
            {
                // Big-endian system
                return __builtin_bswap64(*static_cast<const uint32_t *>(buf));
            }
            else if constexpr (std::endian::native == std::endian::little)
            {
                // Little-endian system
                return *static_cast<const uint32_t *>(buf);
            }
            else
            {
                // Something else
                return std::numeric_limits<uint32_t>::max();
            }            
        }

        int32_t Read32(const void* buf)
        {
            
            if constexpr (std::endian::native == std::endian::big)
            {
                return __builtin_bswap64(*static_cast<const int32_t *>(buf));
            }
            else if constexpr (std::endian::native == std::endian::little)
            {
                // Little-endian system
                return *static_cast<const int32_t *>(buf);
            }
            else
            {
                // Something else
                return std::numeric_limits<int32_t>::max();
            }
        }

        uint64_t ReadU64(const void* buf)
        {
            if constexpr (std::endian::native == std::endian::big)
            {
                return __builtin_bswap64(*static_cast<const uint64_t *>(buf));
            }
            else if constexpr (std::endian::native == std::endian::little)
            {
                return *static_cast<const uint64_t *>(buf);
            }
            else
            {
                return std::numeric_limits<uint64_t>::max();
            }
        }
        istream& _s;
        Exchange<Handler>& _exchange;
};

#endif
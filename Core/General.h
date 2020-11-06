#pragma once
#include <cstdlib>
#include <string>
#include <memory>
#include <iostream>

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __COUNTER__)


#define MAX_INST_LEN 16    // maximal length of inst name
#define MAX_ACCOUNT_LEN 16 //maximal length of account field

#define MAX_INST_ID 65536 //maximal amoutn of instruments


// Elementary logging to the std error output
#define LOG_RAW_ERROR(msg)  std::cerr <<"ERROR: " << msg << endl;
// Simple try catch wrapper for uniform handling
#define TRY try{
#define CATCH_ERR    }catch(std::exception& e) { LOG_RAW_ERROR(e.what()); }
#define FATAL_CATCH  }catch(std::exception& e) \
                         { LOG_RAW_ERROR(e.what() << "\nTerminating"); std::exit(1); }


#define EXCEPTION_ASSERT(cond, msg) if(!(cond)) throw std::runtime_error(msg)

#define INVALID_PRICE 1e12 // value for a missing price


namespace SC
{

typedef int64_t IntTime;

typedef double Price;


typedef char InstrumentName[MAX_INST_LEN];  //used for exchange facing or fixed length places
typedef int InstrumentId;


enum eBuySell {
    Buy=0,
    Sell,
    SideCount //also used for unknown
};

template<eBuySell>
struct BuySellTraits {}; 

template<>
struct BuySellTraits<Buy> 
{
    static const int idx = static_cast<int>(Buy);
    static const eBuySell side = Buy;
    static const int inc = -1; // iterator to lower levels
    static constexpr Price worst_price = -1e12;
    template<typename U, typename V> static bool better(U a, V b) { return a>b; }
    template<typename U, typename V> static bool worse(U a, V b) { return a<b; }
}; 

template<>
struct BuySellTraits<Sell> 
{
    static const int idx = static_cast<int>(Sell);
    static const eBuySell side = Sell;
    static const int inc = 1; // iterator to lower levels
    static constexpr Price worst_price = 1e12;
    
    template<typename U, typename V> static bool better(U a, V b) { return a<b; }
    template<typename U, typename V> static bool worse(U a, V b) { return a>b; }
}; 

// return -1 for sell and +1 for buy
inline int sideSign(eBuySell side)
{
    return 1-(side<<1); 
}


inline eBuySell strToSide(const std::string& s)
{
    if(s=="BUY") return Buy;
    if(s=="SELL") return Sell;
    if(s=="UNK") return SideCount;
    throw std::runtime_error("Invalid side " + s);
    
}

}
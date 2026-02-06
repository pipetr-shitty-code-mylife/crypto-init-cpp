#ifndef SIM_HPP
#define SIM_HPP


#include <string>
#include "mainInclude.hpp"
#include <map>


namespace Sim{
 
void initAll();
void startReadingMainConfig();
void ClassInit();
void CheckOldCoinIndex();
class Coin{
    public:
        int64_t lastUp;
        void Init(coin *C);
        void RateUpdate();
        void CheckOfferS();
        void CheckOfferB();
        long double ShowRate();
    
    private:
        rateBody cM{};
        int64_t  Soffer = 0;
        int64_t  Boffer  = 0;
        float sens = 0;
        std::string name;
};

}
extern std::map<std::string, Sim::Coin> CoinAll;

#endif
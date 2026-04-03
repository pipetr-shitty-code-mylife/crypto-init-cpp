#ifndef MAIN_INCLUDE_HPP
#define MAIN_INCLUDE_HPP

#include<iostream>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/ini_parser.hpp>
#include<tgbot/tgbot.h>
#include<cstdio>
#include<ctime>
#include<chrono>
#include<random>
#include<cstring>
#include<cstdlib>
#include<iomanip>
#include<vector>
#include<sstream>
#include<termcolor/termcolor.hpp>
#include <stdint.h>
#include <map>

#define MAX_COINS 100

typedef struct{
    int userId;
    int creditCount;
    bool partyMember;
    int bonusCount;
    int lastPlayed;
}social;

typedef struct {
    std::string name;
    long double rate;
    float sens;
    std::string uniqueId;
}coin;

typedef struct {
   int userId;
   int scPrice;
   int count;
   int coinId;
}offer;


typedef struct{
    int id_coin;
    double count;
    long lastMine;
    int numUp;
    int userId;
}userCoins;

typedef struct{
    char coinId[16];
    double Rate;
}rateBody;

void debug(std::string type, std::string text);

int loadCoinInfo(std::string name);
std::vector<std::string> splitArgs(const std::string& text);


namespace File{
      void del(offer& o, char* type);
      void save(social& s);
      social loadSocial(int userId);
      void resave(offer& o, char* type);
      void soffer(offer& o, int type /*0 - buy, 1 - sell*/);
      void scoin(coin& c);
      void load(offer *o, char* type);
      int init(bool	silent);
      int coinInit(bool silent);
}
namespace Crypto{
        void listStrGen(std::string *str);
        void buy(offer& o);
        void sell(offer& o);
        void sum(std::string type, long *sum);
        int saveCoinUser(userCoins& uc);
        userCoins loadData(int userId, int num);
        void addCoin(coin c);
        int findOffer(offer &o, offer *retoffer,int type);
        void giveUniqueId(coin *C);
}
namespace Game{
        double mine(userCoins& coins);
        int socialCredit(int user, social& s);
}

namespace Tg{
        void soc(TgBot::Bot& bot);
        void min(TgBot::Bot& bot);
        void buySold(TgBot::Bot& bot);    
        void menu(TgBot::Bot& bot);
}



extern bool DNOT;
extern std::vector<coin> CDATA_ALL;
extern std::string DFILE;
extern std::string OSFILE;
extern std::string OBFILE;
extern std::string RFILE;
extern std::string AFILE;
extern std::string CFILE;
extern std::string MUSER;
extern int radmins[MAX_COINS];
extern int TIME;
extern float SCALED;

typedef struct {
	social Social;
	std::map<std::string, userCoins> coinInf;
} userData;

extern std::map<int, userData> usr;
#endif

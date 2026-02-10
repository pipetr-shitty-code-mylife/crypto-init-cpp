#include "mainInclude.hpp"


int Crypto::saveCoinUser(userCoins& uc){
    usr[uc.userId].coinInf[CDATA_ALL[uc.id_coin].name] = uc;
    FILE* file = fopen(MUSER.c_str(), "r+b");

    if (!file) {
        file = fopen(MUSER.c_str(), "wb");
        if (!file) {

            debug("err", "error create coin file");
            return 1;

        }
    }

    userCoins temp{};

    bool found = false;

    long pos = 0;

    while (fread(&temp, sizeof(userCoins), 1, file) == 1) {
        if (temp.userId == uc.userId && temp.id_coin == uc.id_coin) {
            found = true;
            break;
        }
        pos++;
    }

    if (found) 
    {
        debug("inf", "saved mlb for: " + std::to_string(uc.userId));
        fseek(file, pos * sizeof(userCoins), SEEK_SET);
    } 
    else
    {

        fseek(file, 0, SEEK_END);

    }
    userCoins uctemp{};
    uctemp = uc;
    fwrite(&uctemp, sizeof(userCoins), 1, file);
    
    fclose(file);
    return 0;
}

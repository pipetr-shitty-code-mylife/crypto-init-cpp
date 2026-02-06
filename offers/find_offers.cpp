#include "mainInclude.hpp"

int Crypto::findOffer(offer &o, offer *retoffer, int type){
    offer trash{};
    std::string file = (type == 1) ? OSFILE : OBFILE;
    FILE* f = fopen(file.c_str(), "rb");
    if(!f){
        debug("err", "cant open " + file);
        fclose(f); 
        return 1;
    }
    while(fread(retoffer, sizeof(offer), 1, f) == 1)
    {
        if(retoffer->scPrice == o.scPrice && retoffer->userId != o.userId && retoffer->count >= o.count){
            debug("inf", "find offer" "(" + file + ")");
            fclose(f);
            return 0;
        }
    }
    debug("inf", "not found f:" + file);
    fclose(f);
    return 1;
}
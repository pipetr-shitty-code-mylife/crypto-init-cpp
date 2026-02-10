#include "mainInclude.hpp"

namespace File{
social loadSocial(int userId){

    if(usr.contains(userId))
    { 
	debug("inf", "social loaded from memory");
    	return usr[userId].Social;
    }
   	
    social sTrash{};
    FILE* file = fopen(DFILE.c_str(), "rb");
    if(!file){
        std::cerr << termcolor::blue << "[X] " << termcolor::reset << "- can't open data file" << '\n';
        return sTrash;
    }
    social sTemp{};
    while (fread(&sTemp, sizeof(social), 1, file) == 1)
    {
        if(sTemp.userId == userId){
            fclose(file);
            std::cout << termcolor::green << "[V]" << termcolor::reset << " - find data for id: " << termcolor::bold << std::to_string(userId) + " (" + std::to_string(sTemp.lastPlayed) + ", " + std::to_string(sTemp.creditCount) + ")"<< termcolor::reset <<"\n";
            return sTemp;
        }
    }
    
    fclose(file);
    return sTrash;

}

void save(social& s){
    usr[s.userId].Social = s;
    FILE* file = fopen(DFILE.c_str(), "r+b");
    if (!file) {
        file = fopen(DFILE.c_str(), "wb");
        if (!file) {
            std::cerr << termcolor::blue << "[X] " << termcolor::reset << "- can't create data file" << '\n';
            return;
        }
    }

    social temp;
    bool found = false;
    long pos = 0;

    while (fread(&temp, sizeof(social), 1, file) == 1) {
        if (temp.userId == s.userId) {
            found = true;
            break;
        }
        pos++;
    }

    if (found) {
        std::cout << termcolor::green << "[V]" << termcolor::reset << " - saved data for id: " << termcolor::bold << std::to_string(s.userId) << termcolor::reset <<"\n";
        fseek(file, pos * sizeof(social), SEEK_SET);
    } else {
        fseek(file, 0, SEEK_END);
    }

    fwrite(&s, sizeof(social), 1, file);
    
    fclose(file);
    return;
}
}

userCoins Crypto::loadData(int userId , int num){
    debug("wrn", "1");
    debug("inf", usr.contains(userId) ? "true" : "false");
    if(usr.contains(userId)){
	    if(usr[userId].coinInf.contains(CDATA_ALL[num].name))
		  return usr[userId].coinInf[CDATA_ALL[num].name];
    
    }
    userCoins trash{};
    FILE* file = fopen(MUSER.c_str(), "rb");
    if(!file){
        debug("err", "coin data load error");
        return trash;
    }
    userCoins temp{};
    while(fread(&temp, sizeof(temp), 1, file) == 1){
        if(temp.userId == userId && num == temp.id_coin){
            debug("inf", "find");
            fclose(file);
            return temp;
        }
    }
    fclose(file);
    return trash;
}

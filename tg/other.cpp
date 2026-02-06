#include "mainInclude.hpp"
#include "adm.hpp"

void debug(std::string type, std::string text){
    if(type == "err")
    {
        std::cerr << termcolor::blue << "[X]" << termcolor::reset << " - " + text << '\n';
    }else if(type == "inf"){
        std::cout << termcolor::green << "[V]" << termcolor::reset << " - " + text << "\n";
    }else if(type == "wrn"){
        std::cout << termcolor::yellow << "[W]" << termcolor::reset << " - " + text << "\n";   
    }else if(type == "cmn"){
        std::cout << termcolor::bright_green << "[C]" << termcolor::reset << " - " + text << "\n";
    }
}
namespace Crypto{
void addCoin(coin c){
    try{
    boost::property_tree::ptree crypto;
    boost::property_tree::ini_parser::read_ini(CFILE, crypto);
    std::string number = std::to_string(crypto.size() + 1);

    crypto.put(number + ".name", c.name);
    crypto.put(number + ".DRATE", c.rate);
    crypto.put(number + ".sens", c.sens);

    boost::property_tree::write_ini(CFILE, crypto);
    std::string str = c.name;
    debug("cmn", "coin " + str + " added");
    }
    catch(std::exception& e){
        debug("err", e.what());
    }

}
}
bool isAdmin(Rights *r){
    int pos = 0;
    if(AdminList.contains(r->tgId)){
        Rights *a = &AdminList[r->tgId];
        if (a->CRYPT_DATA_EDITING == r->CRYPT_DATA_EDITING || a->CRYPT_DATA_EDITING){
            if(a->ADMIN_LIST_EDITING == r->ADMIN_LIST_EDITING || a->ADMIN_LIST_EDITING){
                if(a->CRYPT_EDITING == r->CRYPT_EDITING || a->CRYPT_EDITING){
                    if(a->DATA_EDITING == r->DATA_EDITING || a->CRYPT_EDITING){
                        if(a->SIMULATION == r->SIMULATION || a->SIMULATION){
                            if(a->GLOBAL == r->GLOBAL || a->GLOBAL){
                                if(a->chatId == r->chatId || a->chatId == 0)
                                    return true;
                            }
                        }
                    }
                }
            }
        }
     }
    debug("wrn", std::to_string(r->tgId) + " try use admin command");
    return false;
    
}

int loadCoinInfo(std::string name){
    short pos = 0;
    while(CDATA_ALL[pos].sens != 0){
        if(CDATA_ALL[pos].name == name){
            debug("cmn", "coin search success");
            return pos;
        }
        pos++;
    }
    return -1;
}

void Crypto::giveUniqueId(coin *C){
    std::string id = "";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disrtrib(33, 126);

    for(int8_t i = 0; i < 15;  i++ ){
        char rand = toascii(disrtrib(gen));
        id += rand;
    }
    C->uniqueId = id;
    return;
}
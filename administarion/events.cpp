#include "adm.hpp"
#include "events.hpp"

void Crypto::listStrGen(std::string *str){
         *str += "------монеты------\n";
        int pos = 0;
        while(CDATA_ALL.size() > pos){
            *str += "монета: " + CDATA_ALL[pos].name + "\n";
            *str += "курс: " + std::to_string(CDATA_ALL[pos].rate) + "\n";
            *str += "\n";
            pos++;
        }

        return;
}
int test_event(TgBot::Bot* bot, TgBot::Message::Ptr m){
    std::string str = "";
    Crypto::listStrGen(&str);
    bot->getApi().sendMessage(m->chat->id, str);
    return 0;
}

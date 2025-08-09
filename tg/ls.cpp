#include "MLB_SIM.hpp"
#include "main_data.hpp"

void load_list( TgBot::Bot& bot){
    bot.getEvents().onCommand("ls_sold", [&bot](TgBot::Message::Ptr m){
        FILE* file = fopen(OFSF, "rb");
        if (!file ){
            perror("sold ate shit");
            return;
        }
        ofSold os{};
        while(fread(&os,sizeof(ofSold), 1, file) == 1){
            bot.getApi().sendMessage(m->chat->id, "sc price:" + std::to_string(os.sc_price) + "\n" + "count: " + std::to_string(os.count) );
        }
        fclose (file);
        return;
    });

    bot.getEvents().onCommand("ls_buy", [&bot](TgBot::Message::Ptr m){
        FILE* file = fopen(FSB, "rb");
        if (!file ){
            perror("buy ate shit");
            return;
        }
        ofBuy os{};
        while(fread(&os,sizeof(ofBuy), 1, file) == 1){
            bot.getApi().sendMessage(m->chat->id, "sc price:" + std::to_string(os.sc_price) + "\n" + "count: " + std::to_string(os.count) );
        }
        fclose (file);
        return;
    });

    bot.getEvents().onCommand("ls_mlb", [&bot](TgBot::Message::Ptr m){
        FILE* file = fopen("mlb_data.sosibitara", "rb");
        if (!file ){
            perror("mlb ate shit");
            return;
        }
        MLB_data os{};
        while(fread(&os,sizeof(MLB_data), 1, file) == 1){
            bot.getApi().sendMessage(m->chat->id, "count:" + std::to_string(os.cout_MLB) + "\n" + "id: " + std::to_string(os.user_id) );
        }
        fclose (file);
        return;
    });
}
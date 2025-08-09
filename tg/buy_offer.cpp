#include "MLB_SIM.hpp"
#include "main_data.hpp"

std::vector<std::string> splitArgs(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> result;
    std::string word;
    while (iss >> word) {
        result.push_back(word);
    }
    return result;
}

void buy_offer (TgBot::Bot& bot){

	bot.getEvents().onCommand("buy",[&bot](TgBot::Message::Ptr message){
        try{
		std::vector<std::string>  parts = splitArgs(message->text);
        if (parts.size() < 2) {
            bot.getApi().sendMessage(message->chat->id, "fuck yourself nigger");
            return;
        }

        if(parts.size() == 2){
        try {
            unsigned int b = std::stoi(parts[1]);
            chatData  data{};
            data = load_data(message->from->id, data, message->chat->id);
            if (data.us.user_id == 0){
                bot.getApi().sendMessage(message->chat->id, "ты ище не работать на партия(sc = 0)");
                return;
            }else if (data.us.credit_count < b){
                bot.getApi().sendMessage(message->chat->id, "недостаточно средств");
                return;
            }
            data.us.credit_count -= b;
            save_file(data);
			ofBuy bu{};
			MLB mmm{};
			mmm = loadRate();
			bu.from_id = message->from->id;
			bu.sc_price = b;
			bu.count = b / mmm.rate;
			bu.rate = mmm.rate;
            ofSold sold_ident{};
            sold_ident = load_sold(bu);
            if(sold_ident.from_id == 0){
			int  popa = saveOB(bu);}
            else {
                if(sold_ident.count > bu.count)
                        {
                            ofSold os_save = sold_ident;
                            os_save.count = sold_ident.count -  bu.count;
                            os_save.sc_price = os_save.count * mmm.rate;
                            resave_sold(sold_ident, os_save);
                            printf("resaved\n");
                        }
                        else{
                            printf("deleted\n");
                          delete_sold(sold_ident);
                        }
                MLB_data mData{};
                mData = load_MLB(bu.from_id, mData);
                mData.cout_MLB += bu.count;
                mData.user_id = message->from->id;
                save_mlb(mData);
                chatData cSolder{};
                cSolder = load_data(sold_ident.from_id, cSolder, 0);
                cSolder.us.credit_count += bu.sc_price;
                save_file(cSolder);
            }
            bot.getApi().sendMessage(message->chat->id,  std::to_string(bu.count));
        } catch (std::exception& e) {
            bot.getApi().sendMessage(message->chat->id, "pizdec");
        }
        }else if(parts.size() == 3){
            try{
            int b = std::stoi(parts[1]);
            int c = std::stoi(parts[2]);
            chatData cd{};
            cd = load_data(message->from->id, cd, message->chat->id);
            if(cd.us.user_id == 0){
                bot.getApi().sendMessage(message->chat->id, "ты ищё не работать на партия");
            }else if (cd.us.credit_count < b || b <= 0 || c <= 0){
                bot.getApi().sendMessage(message->chat->id, "недостаточно средств");
            }
            else{
                
                ofBuy bu{};//penis
                bu.from_id = message->from->id;
                bu.count = c;
                bu.sc_price = b;
                ofSold os{};
                os = load_sold(bu);
                if(os.from_id != 0){
                    cd.us.credit_count -= b;
                    save_file(cd);
                    MLB_data bDATA{};
                    bDATA = load_MLB(bu.from_id, bDATA);
                    bDATA.cout_MLB += bu.count;
                    bDATA.user_id = bu.from_id;
                    save_mlb(bDATA);
                    chatData sChat{};
                    sChat = load_data(os.from_id, sChat, 0);
                    sChat.us.credit_count += bu.sc_price;
                    save_file(sChat);
                    delete_sold(os);
                }
                else{
                    bot.getApi().sendMessage(message->chat->id,std::to_string(os.from_id) + "не получилось найти предложение");
                }
            }
        
        }catch(std::exception& e){
            bot.getApi().sendMessage(message->chat->id, 
            message->from->firstName + "\\, ты неправильно использвал команду\\.\nПример правильного использвания\\:\n```example\n/buy <цена в sc> <количество монет>\n```",
             nullptr,nullptr,nullptr, "MarkdownV2");
        }
        }
        }catch(std::exception&e){
            std::cerr << "error: " << e.what() << std::endl;
        }
			});

}

#include "main_data.hpp"

int main(){
    std::string token;
    std::ifstream token_file("token.txt");
    if(!token_file.is_open()){
        std::cerr << "can't find token file" << std::endl;
        return 1;
    }
    std::getline(token_file, token);
    token_file.close();

    TgBot::Bot bot(token);

    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query) {
        std::cout << "button taped by " + query->from->firstName << std::endl;
        if (query->data == "callback_buy") {
            bot.getApi().deleteMessage(query->message->chat->id, query->message->messageId);
            chatData cd{};
            cd = load_data(query->from->id, cd, query->message->chat->id);
            MLB_data mlb{};
            mlb = load_MLB(query->from->id, mlb);
        switch (mlb.num_upgrade)
        {
        case 0:{
            if(cd.us.credit_count < 70){
                bot.getApi().sendMessage(query->message->chat->id, query->from->firstName + ", тебе не хватает кредитов");
            }
            else{
           
            mlb.num_upgrade = 1;
            mlb.user_id = query->from->id;
            cd.us.credit_count -= 70;
            save_file(cd);
            save_mlb(mlb);
            }
            break;
        }
        case 1:{
             if(cd.us.credit_count < 220){
                    bot.getApi().sendMessage(query->message->chat->id, query->from->firstName + ", на 2 уровень нужно 220 кредитов");
                } else {
                    cd.us.credit_count -= 220;
                    mlb.num_upgrade = 2;
                    save_file(cd);
                    save_mlb(mlb);
                    bot.getApi().sendMessage(query->message->chat->id, query ->from->firstName + ", ты купил ферму 2 уровня!");
                }
                break;
        }
        default:
            break;
        }
    }
        bot.getApi().answerCallbackQuery(query->id);
    });
    
    bot.getEvents().onCommand("info", [&bot](TgBot::Message::Ptr message){
		    std::cout << message->text << std::endl;
        if(message->replyToMessage){
        if(message->from->id == 2043848333){
        chatData cd{};
        cd = load_data(message->replyToMessage->from->id, cd, message->chat->id);
        MLB_data mData{};
        mData = load_MLB(message->replyToMessage->from->id, mData);
        bot.getApi().sendMessage(
            message->chat->id,
            std::to_string(cd.us.user_id) + "\n" + std::to_string(cd.us.credit_count) + "\n" + std::to_string(cd.us.last_played) + "\nMLB:" + std::to_string(mData.cout_MLB) + "\nUP:" + std::to_string(mData.num_upgrade),
            nullptr,
            nullptr,
            nullptr,
            "HTML"

        );
        }
    }
     });
    
    
    std::cout << "Size of User: " << sizeof(userData) << "\n";
    int popa = social_credit(bot);
    mining(bot);
    buy_offer(bot);
    sell_offer(bot);
    load_list(bot);
    std::cout << popa << std::endl;
    while(true)
    {
    try {
        std::cout << "bot starting..." << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
    }
    return 0;
}

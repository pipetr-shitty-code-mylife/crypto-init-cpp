#include "main_data.hpp"

int/*int*/ social_credit(TgBot::Bot& bot){
    bot.getEvents().onCommand("pe", [&bot](TgBot::Message::Ptr message){
        if(message->from->id == 5261207811){
        printf("ignat detected\n");
        }   
        if(message->chat->type == TgBot::Chat::Type::Private){
                bot.getApi().sendMessage(message->chat->id, "иди нахуй я только в групах работаю");
        }
        else{
            chatData cd{};
            cd = load_data(message->from->id, cd, message->chat->id);
            printf("%ld\n", cd.us.last_played);
            if(cd.us.last_played - std::time(nullptr) < -300 || cd.us.last_played == 0){
            auto replyOptions = std::make_shared<TgBot::ReplyParameters>();
            replyOptions->messageId = message->messageId;
            replyOptions->chatId = message->chat->id;
            std::string text = "<a href=\"tg://user?id=" + std::to_string(message->from->id) + "\">" + message->from->firstName + "</a>";
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(-50, 100);
            int sum = distrib(gen);
            cd.chatId = message->chat->id;
            cd.us.last_played = std::time(nullptr);
            cd.us.credit_count = cd.us.credit_count + sum;
            cd.us.user_id = message->from->id;
            if(sum > 0){
                bot.getApi().sendMessage(
                    message->chat->id, 
                    text + ", партия гордится тобой!\nВеликий стержень Хи дал тебе " + std::to_string(sum ) + " социальных кредитов!\nТеперь у тебя " + std::to_string(cd.us.credit_count) + " кредитов!",nullptr,
                    replyOptions,
                    nullptr, 
                    "HTML");
                }
            if(sum < 0){
                bot.getApi().sendMessage(
                    message->chat->id, 
                    text + ", ну и ну ты разочировать партия!\nВеликий стержень Хи забрал у тебя " + std::to_string(sum * -1) + " социальных кредитов!\nТеперь у тебя " + std::to_string(cd.us.credit_count) + " кредитов!\nТы отправлятся к уйгур в трудовой лагерь на неопределённый срок!",
                    nullptr,
                    replyOptions,
                    nullptr, 
                    "HTML");
                } 
            if(sum == 0){
                bot.getApi().sendMessage(
                    message->chat->id, 
                    text + ", ой-ой ты разочировать партия!\nНо Великий Хи сегодня добрый поэтому он у тебя ничего не забрал!\n",
                    nullptr,
                    replyOptions,
                    nullptr, 
                    "HTML");
                }
            std::cout << cd.us.user_id << std::endl;
            std::cout << cd.us.credit_count << std::endl;
            std::cout << cd.us.last_played << std::endl;
            save_file(cd);  
            } 
        }
    });
    return 0;/*return 0;*/
}
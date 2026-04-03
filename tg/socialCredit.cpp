#include "mainInclude.hpp"

int gen_semen(int max, int min){
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<> distrib(min, max);
     return distrib(gen);
}

int Game::socialCredit(int user, social& s){
        int min = -10;
        int max = 60;
        int num = gen_semen(max, min);
        s.creditCount += num;
        s.userId = user;
        s.lastPlayed = std::time(nullptr);
        File::save(s);
        return num;
}


namespace Tg{

void soc(TgBot::Bot& bot){

    bot.getEvents().onCommand("credit", [&bot](TgBot::Message::Ptr message){

        if(message->from->id == 5261207811)
            debug("wrn", "ignat detected");

        debug("inf", message->from->firstName + " activated /credit command");

        int returns;

        social s{};
        s = File::loadSocial(message->from->id);

        int rand = gen_semen(1000, 0);
        debug("wrn", "semen: " + std::to_string(rand));

        auto replyOptions = std::make_shared<TgBot::ReplyParameters>();
        replyOptions->messageId = message->messageId;
        replyOptions->chatId = message->chat->id;
        std::string text = "<a href=\"tg://user?id=" + std::to_string(message->from->id) + "\">" + message->from->firstName + "</a>";
        
    if(s.lastPlayed - std::time(nullptr) < TIME * -1 || s.lastPlayed == 0){
        s.bonusCount++;
        switch(rand){
            case 167:
            case 168:
            case 169:
                {
                    bot.getApi().sendMessage(message->chat->id, "ИГНАТ ЗАБРАЛ У ТЕБЯ 1000 КРЕДИТОВ ИДИ НАХУЙ🫵😸",
                    nullptr,
                    replyOptions,
                    nullptr,
                    "HTML");
                    
                    social ss = s;
                    ss.creditCount -= 1000;
                    ss.lastPlayed = std::time(nullptr);
                    ss.userId = message->from->id;
                    File::save(ss);
                    break;
                }

            case 678:
            case 679:
            case 690:
            case 402:
            case 333:
            case 245:
            case 111:
            case 336: {
                s.creditCount += 100;
                s.lastPlayed = std::time(nullptr);
                s.userId = message->from->id;
                s.bonusCount++;
                File::save(s);
                bot.getApi().sendMessage(message->chat->id,
                    text + "! Ого ты понравился ИГНАТУ!\nПолучи cвои 100 кредитов" +
                    "\nТеперь у тебя " + std::to_string(s.creditCount) + " кредитов!",
                    nullptr, 
                    replyOptions, 
                    nullptr, 
                    "HTML");
            }
        

        default:
        {
            rand = 0;
            returns = Game::socialCredit(message->from->id, s);
            if (!DNOT){     
                if(returns > 0){
                    bot.getApi().sendMessage(
                    message->chat->id,
                    text + ", админы гордится тобой!\nВеликий ИГНАТ дал тебе " + std::to_string(returns) + " кредитов!\nТеперь у тебя " + std::to_string(s.creditCount) + " кредитов!",nullptr,
                    replyOptions,
                    nullptr,
                    "HTML");
                }
                else if(returns < 0){
                    bot.getApi().sendMessage(
                    message->chat->id,
                    text + ", ну и ну ты разочировать админов этого чата!\nВеликий ИГНАТ забрал у тебя " + std::to_string(returns * -1) + " кредитов!\nТеперь у тебя " + std::to_string(s.creditCount) + " кредитов!\nТы отправлятся в бан к шлюхоботам на неопределённый срок!",nullptr,
                    replyOptions,
                    nullptr,
                    "HTML");
                }
                else if(returns == 0){
                    bot.getApi().sendMessage(
                    message->chat->id,
                    text + ", ой-ой ты разочировать ИГНАТ!\nНо Великий ИГНАТ сегодня добрый поэтому он у тебя ничего не забрал!\n",
                    nullptr,
                    replyOptions,
                    nullptr,
                    "HTML");
                }
            }
        }
    } 
        }
        else {
            bot.getApi().sendMessage(message->chat->id, "ище рано");
        }
    });
}
}
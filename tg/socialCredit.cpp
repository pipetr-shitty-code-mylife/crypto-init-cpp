#include "mainInclude.hpp"

int gen_semen(int max, int min){
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<> distrib(min, max);
     return distrib(gen);
}
namespace Game{
int socialCredit(int user, social& s){
    if(s.lastPlayed - std::time(nullptr) < TIME * -1 || s.lastPlayed == 0){
        int min = -10;
        int max = 60;
        int num = gen_semen(max, min);
        s.creditCount += num;
        s.userId = user;
        s.lastPlayed = std::time(nullptr);
        File::save(s);
        return num;
    }
    else{
        return 15000;
    }
}
}

namespace Tg{
void soc(TgBot::Bot& bot){
    bot.getEvents().onCommand("credit", [&bot](TgBot::Message::Ptr message){
        if(message->from->id == 5261207811)
            debug("wrn", "ignat detected");
        debug("inf", message->from->firstName + " suck party");
        int returns;
        social s{};
        s = File::loadSocial(message->from->id);
        int rand = gen_semen(1000, 0);
        debug("wrn", "semen: " + std::to_string(rand));
        auto replyOptions = std::make_shared<TgBot::ReplyParameters>();
        replyOptions->messageId = message->messageId;
        replyOptions->chatId = message->chat->id;
        std::string text = "<a href=\"tg://user?id=" + std::to_string(message->from->id) + "\">" + message->from->firstName + "</a>";
        
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

        default:
        {
            rand = 0;
            returns = Game::socialCredit(message->from->id, s);
            if (!DNOT){
            if (returns == 15000){
            bot.getApi().sendMessage(message->chat->id, "ище рано");
        }
        else if(returns > 0){
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
        }}
    }
    }
    });
}
}
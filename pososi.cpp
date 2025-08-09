#include "main_data.hpp"

int otsosi(TgBot::Bot& bot){
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message){
        std::string  text = message->text;
        if(!text.empty() && message->replyToMessage){
            std::string firstWord = text.substr(0, text.find(' '));
            if(firstWord == "соси" || firstWord == "отсоси" || firstWord == "Соси" || firstWord == "Отсоси" || firstWord == "СОСИ" || firstWord == "ОТСОСИ"){
                std::string to = "<a href=\"tg://user?id=" + std::to_string(message->from->id) + "\">" + message->from->firstName + "</a>";
                std::string from = "<a href=\"tg://user?id=" + std::to_string(message->replyToMessage->from->id) + "\">" + message->replyToMessage->from->firstName + "</a>";
                if(message->from->id != message->replyToMessage->from->id){
                bot.getApi().sendMessage(
                    message->chat->id,
                    from + " сделал(а) отличный минет " + to + ".\nХорошая работа.",
                    nullptr,
                    nullptr,
                    nullptr,
                    "HTML"
                );
                } else {
                    bot.getApi().sendMessage(
                    message->chat->id,
                    to + " ты сделал сам себе минет.\nМолодец!",
                    nullptr,
                    nullptr,
                    nullptr,
                    "HTML"
                );
                }
            }
        }
    });
    return 0;
}
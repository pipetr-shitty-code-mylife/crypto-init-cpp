#include "MLB_SIM.hpp"
#include "main_data.hpp"

void mining(TgBot::Bot& bot){
    bot.getEvents().onCommand("mine", [&bot](TgBot::Message::Ptr m){
        MLB_data mlb{};
        mlb = load_MLB(m->from->id, mlb);
        if(mlb.num_upgrade == 0){
            TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

            TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
            button->text = "Купить ферму";
            button->callbackData = "callback_buy";
            
            keyboard->inlineKeyboard.push_back({button});

            bot.getApi().sendMessage(m->chat->id, "У тебя ещё нет фермы.\nЕё можно купить с помощью инлайн кнопки или команды /buy_farm за 70 кредитов!", 0, 0, keyboard );
            return;
               }
        if(mlb.num_upgrade == 1){
            if(mlb.last_played == 0)
                mlb.last_played = std::time(nullptr) + 1;
            auto replyOptions = std::make_shared<TgBot::ReplyParameters>();
            replyOptions->messageId = m->messageId;
            replyOptions->chatId = m->chat->id;
            std::cout << std::to_string(m->from->id) << std::endl;
            std::string text = "<a href=\"tg://user?id=" + std::to_string(m->from->id) + "\">" + m->from->firstName + "</a>";
            double cost =  220.0f;
            long double last = std::time(nullptr) - mlb.last_played;
            last = last/60;
            last /= cost;
            mlb.cout_MLB += last;
            mlb.last_played = std::time(nullptr);
            save_mlb(mlb);
            TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

            TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
            button->text = "Прокачать ферму";
            button->callbackData = "callback_buy";
            
            keyboard->inlineKeyboard.push_back({button});

            bot.getApi().sendMessage(m->chat->id, text + ", майнинг успшен!\nТы намайнил " + std::to_string(last) + " MLB.\nТеперь у тебя " + std::to_string(mlb.cout_MLB) + " MLB!",nullptr,replyOptions, keyboard, "HTML");
        }
    });

    bot.getEvents().onCommand("buy_farm", [&bot](TgBot::Message::Ptr message){
        MLB_data mlb{};
        mlb = load_MLB(message->from->id, mlb);
        chatData cd{};
        cd = load_data(message->from->id, cd, message->chat->id);
        switch(mlb.num_upgrade)
        {
        case 0:{
                if(cd.us.credit_count < 70){
                    bot.getApi().sendMessage(message->chat->id, message->from->firstName + ", тебе не хватает кредитов");
                } else {
                    cd.us.credit_count -= 70;
                    mlb.num_upgrade = 1;
                    save_file(cd);
                    save_mlb(mlb);
                    bot.getApi().sendMessage(message->chat->id, message->from->firstName + ", ты купил ферму 1 уровня!");
                }
                break;
        }
        case 1:{
                if(cd.us.credit_count < 220){
                    bot.getApi().sendMessage(message->chat->id, message->from->firstName + ", на 2 уровень нужно 220 кредитов");
                } else {
                    cd.us.credit_count -= 220;
                    mlb.num_upgrade = 2;
                    save_file(cd);
                    save_mlb(mlb);
                    bot.getApi().sendMessage(message->chat->id, message->from->firstName + ", ты купил ферму 2 уровня!");
                }
                break;
        }

        
        default:
            break;
        }
    });

}
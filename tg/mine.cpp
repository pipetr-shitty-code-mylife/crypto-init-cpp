#include "mainInclude.hpp"

std::vector<std::string> splitArgs(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> result;
    std::string word;
    while (iss >> word) {
        result.push_back(word);
    }
    return result;
}

double Game::mine(userCoins& coins){
        debug("inf", "mining elixir.....");
        double sum = 0;
        switch (coins.numUp)
        {
        case 0:{
            return -23;
        }
        default:{
            
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(-13 + (coins.numUp * 2), 4 * coins.numUp);
            int num = distrib(gen);
            float num2 = num / 12.0 + 1;
            debug("inf", "random: " + std::to_string(num2));

            if(coins.lastMine == 0)
                coins.lastMine = std::time(nullptr) - 200;
            debug("inf", "last mine: " + std::to_string(coins.lastMine));
            double t = std::time(nullptr) - coins.lastMine;
            debug("inf", "t: " + std::to_string(t));
            double scaled = std::log1p(t) / std::log1p(SCALED); 
            debug("inf", "scaled: " + std::to_string(scaled));
            double diff = 400.0 + CDATA_ALL[coins.id_coin].rate * 10;
            debug("inf", "diff: " + std::to_string(diff));
            sum = (scaled * num2) / diff * 2; 
            
            return sum;
            break;
        }
        }
        return sum;     
    
}


void Tg::min(TgBot::Bot& bot){
    bot.getEvents().onCommand("mine",[&bot](TgBot::Message::Ptr message){
        debug("inf", message->from->firstName + " suck mine");
        double ret;
        std::vector<std::string> parse = splitArgs(message->text);
        if(parse.size() < 2 || parse.size() > 2){
            bot.getApi().sendMessage(message->chat->id, "непрвильный синтаксис команды");
            debug("wrn", "user autist: " + std::to_string(sizeof(parse)));
        }else{
        int number = loadCoinInfo(parse[1]);
        userCoins cData{};
        if(number == -1){
            debug("inf", "NULL COIN");
            auto replyOptions = std::make_shared<TgBot::ReplyParameters>();
            replyOptions->messageId = message->messageId;
            replyOptions->chatId = message->chat->id; 
            
            bot.getApi().sendMessage(message->chat->id, "ты ввёл не существующую монету.\nиди нахуй", nullptr, replyOptions );

            return;
        }
        cData = Crypto::loadData(message->from->id, number); 
        if(cData.numUp == 0){
            TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

            TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
            button->text = "Купить ферму";
            
            button->callbackData = "farm_buy " + std::to_string(number);
            debug("inf" , button->callbackData);
            keyboard->inlineKeyboard.push_back({button});

            bot.getApi().sendMessage(message->chat->id, "У тебя ещё нет фермы для этой монеты.\nЕё можно купить с помощью инлайн кнопки или команды /buy_farm <имя монеты> за 70 кредитов!", 0, 0, keyboard );
            return;


        }
        
            ret = Game::mine(cData);
            cData.id_coin = number;
        

        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
        TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
        button->text = "Пойти нахуй";
        button->callbackData = "callback_buy";
        keyboard->inlineKeyboard.push_back({button});

        std::string text = "<a href=\"tg://user?id=" + std::to_string(message->from->id) + "\">" + message->from->firstName + "</a>";

        auto replyOptions = std::make_shared<TgBot::ReplyParameters>();
        replyOptions->messageId = message->messageId;
        replyOptions->chatId = message->chat->id; 

        bot.getApi().sendMessage(message->chat->id, text + ", майнинг успшен!\nТы намайнил " + std::to_string(ret) + " " +  CDATA_ALL[cData.id_coin].name +".\nТеперь у тебя " + std::to_string(cData.count + ret) + " " + CDATA_ALL[cData.id_coin].name + "!",nullptr,replyOptions, keyboard, "HTML");
        
        
        cData.count += ret;
        cData.lastMine = std::time(nullptr);
        cData.userId = message->from->id;
        Crypto::saveCoinUser(cData);
    }
    });

    
    bot.getEvents().onCommand("buy_farm", [&bot](TgBot::Message::Ptr message){

        std::vector<std::string> parse = splitArgs(message->text);
        if (parse.size() < 2 || parse.size() > 2){
            bot.getApi().sendMessage(message->chat->id, "неправильный синтаксис команды");
            debug("wrn", "user invalid");
        }else{
            short numCoin = loadCoinInfo(parse[1]); 
            if(numCoin != -1){

                userCoins uCoins{};
                uCoins = Crypto::loadData(message->from->id, numCoin);
                if(uCoins.numUp != 1){
                social sc{};
                sc = File::loadSocial(message->from->id);
                if(sc.creditCount >= 70){
                    sc.creditCount -= 70;
                    uCoins.userId = message->from->id;
                    uCoins.id_coin = numCoin;
                    uCoins.numUp = 1;
                    File::save(sc);
                    Crypto::saveCoinUser(uCoins);
                    debug("inf", message->from->firstName + " buy 1lvl farm");
                }else
                    bot.getApi().sendMessage(message->chat->id, "недостаточно средств");
                
                }
            }else{
                bot.getApi().sendMessage(message->chat->id, "неправильное имя монеты");
            }
        }
    });

    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query){


            std::vector<std::string> parse =  splitArgs(query->data);
            if(parse[0] == "farm_buy"){
                debug("inf", "query callback buy");
                try{
                    int number = 0;
                    try{number = std::stoi(parse[1]); 
                        }
                    catch (std::exception &e){
                        return;
                    }
                    social sc{};
                    sc = File::loadSocial(query->from->id);
                    userCoins uc{};
                    uc = Crypto::loadData(query->from->id, number);

                    switch (uc.numUp)
                    {
                    case 0:
                        if(sc.creditCount >= 70){
                            sc.creditCount -= 70;
                            sc.userId = query->from->id;
                            uc.userId = sc.userId;
                            uc.numUp = 1;
                            uc.id_coin = number;
                            File::save(sc);
                            Crypto::saveCoinUser(uc);
                            bot.getApi().deleteMessage(query->message->chat->id, query->message->messageId);
                        }
                        break;
                    
                    default:
                        break;
                    }
                    
                }
                catch(std::exception &e){
                    debug("err", e.what());
                }
            }
        });

    bot.getEvents().onCommand("list_coins", [&bot](TgBot::Message::Ptr message){
        std::string str;
        Crypto::listStrGen(&str);
        debug("inf", message->from->firstName + " call list");
        bot.getApi().sendMessage(message->chat->id, str);
    });
}

#include "mainInclude.hpp"


void Tg::buySold(TgBot::Bot &bot){
 
    bot.getEvents().onCommand("offer_buy", [&bot](TgBot::Message::Ptr message){

                auto send = [&](const std::string text) {
                    bot.getApi().sendMessage(message->chat->id, text);
                };

                social ss{};
                ss = File::loadSocial(message->from->id);
                std::vector<std::string> parse;
                parse = splitArgs(message->text);
                if(parse.size() != 4)
                {
                    send("неправильный синтаксис");
                    return;
                }
                    int coinId = loadCoinInfo(parse[1]);

                    if (coinId == -1)
                    {
                         send("не найдено такой монеты");
                         return;
                    }
                        int creditCount, mlbCount;
                    try{
                        creditCount = stoi(parse[2]);
                        mlbCount = stoi(parse[3]);
                    }
                    catch (std::out_of_range &e)
                    {
                        debug("err", e.what());
                        send("вы ввели слишком большое число");
                        return;
                    } 
                    catch (std::invalid_argument &e){
                        debug("err", e.what());
                        send("неправильный синтаксис");
                        return;
                    } 
                    
                    if(creditCount <= 0 || mlbCount <= 0){
                        send("ты не можешь купить отрицтельное количество");
                        return;
                    }
                        if(creditCount <= ss.creditCount){
                        ss.creditCount -= creditCount;
                        ss.userId = message->from->id;
                        File::save(ss);
                        offer of;
                        of.coinId = coinId;
                        of.count = mlbCount;
                        of.scPrice = creditCount;
                        of.userId = ss.userId;
                        File::soffer(of, 0);
                        debug("cmn", std::to_string(ss.userId) + " buy " + parse[3] + " " + parse[1]);
                        send(message->from->firstName + ",\nВы оставили заявку на покупку " + std::to_string(mlbCount) + " " + CDATA_ALL[coinId].name + "\nСредства прийдут вам как только найдётся подходящще предложение!");
                        }else
                        {
                            send("нехватает средств");
                        }    
    });

    bot.getEvents().onCommand("offer_sell", [&bot](TgBot::Message::Ptr message){
        
        auto send = [&](const std::string mes){
            bot.getApi().sendMessage(message->chat->id, mes);
        };

        social soc{};
        soc = File::loadSocial(message->from->id);
        
        std::vector<std::string> parse = splitArgs(message->text);
        if(parse.size() != 4){
            send("неправильный синтаксис");
            debug("wrn", "user autist: " + message->from->firstName);
            return;
        }
        int coinId = loadCoinInfo(parse[1]);
        if (coinId == -1){
            send("нету такой монеты или она была удалена");
            return;
        }
        int mlbCount, scprice = 0;
        try 
        {
            mlbCount = std::stoi(parse[3]);
            scprice = std::stoi(parse[2]);
        }
        catch(std::out_of_range &e){
            debug("err", e.what());
            send("слишком большое число");
            return;
        }
        catch(std::invalid_argument &e){
            debug("err", e.what());
            send("неправильный синтаксис");
            return;
        }

        userCoins uc{};
        uc = Crypto::loadData(message->from->id, coinId);
        if  (uc.count < mlbCount){
            send("нехватает средств");
            return;
        }

        if (mlbCount <= 0 || scprice <= 0){
            send("ты не можешь продать отрицательное количество монет");
            return;
        }

        uc.count -= mlbCount;
        if (Crypto::saveCoinUser(uc)){
            send("Не удалось обработать вашу заявку\n" "Обратитесь к администрации за помощью");
            return;
        }
        offer o{};
        o.coinId = coinId;
        o.count = mlbCount;
        o.scPrice = scprice;
        o.userId = message->from->id;

        File::soffer(o, 1);

        send(message->from->firstName + ",\n" "Ваш запрос обработан успешно.\nВаши средства прийдут вам как только появится подходящее предожение.");
    
    });

}



void File::soffer(offer& o, int type){
        FILE* f = fopen((type == 1)? OSFILE.c_str() : OBFILE.c_str() , "ab");
        if(!f){
            debug("inf", "creating " + (type == 1)? "OSFILE...." : "OBFILE....");
            f = fopen((type == 1)? OSFILE.c_str() : OBFILE.c_str(), "wb");
            if(!f){
                debug("err", "error creating " +  (type == 1)? "OSFILE" : "OBFILE");
                return;
            }
        }
        fwrite(&o, sizeof(offer), 1, f);
        fclose(f);
        return;
}


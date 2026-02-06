#include "mainInclude.hpp"
#include "adm.hpp"

std::string DFILE;
std::string OSFILE;
std::string OBFILE;
std::string RFILE;
std::string AFILE;
std::string CFILE;
std::string MUSER;
bool DNOT = true;
coin CDATA_ALL[MAX_COINS] = {};

int TIME;
float SCALED;

int File::init(bool silent){
    boost::property_tree::ptree config;
    boost::property_tree::ini_parser::read_ini("config.ini", config);
    try{
        DFILE = config.get<std::string>("main.DFILE");
        RFILE = config.get<std::string>("main.RFILE");
        OSFILE = config.get<std::string>("main.OSFILE");
        OBFILE = config.get<std::string>("main.OBFILE");
        AFILE = config.get<std::string>("main.AFILE");
        CFILE = config.get<std::string>("main.CFILE");
        MUSER = config.get<std::string>("main.MUSER");
        TIME = config.get<int>("other.TIME");
        SCALED = config.get<float>("other.SCALED");
        
        
        boost::property_tree::ptree admins;
        boost::property_tree::read_ini(AFILE, admins);
        for(const auto &section: admins){
              debug("wrn", "SECTION: " + section.first);
              
                for (const auto &kv : section.second ){
                    debug("wrn", kv.first);
                    Rights r{};

                     if (section.first == "GLOBAL_ADMINS"){
                        r.GLOBAL = true;
                        r.chatId = 0;
                     } else {
                       try{ r.chatId = std::stoi(section.first); }
                       catch (std::exception &e )
                       {
                            debug("err", e.what());
                            exit(1);
                       }
                     }

                    if((kv.first).find('{') != std::string::npos){
                        std::stringstream ss(kv.first);
                        std::string arg;
                        while(std::getline(ss, arg , ',')){
                            arg.erase(std::remove(arg.begin(), arg.end(), ' '), arg.end());
                            if(arg.find('{') != std::string::npos)
                                arg.erase(0, 1);
                            if(arg.find('}') != std::string::npos){
                                auto pos = arg.find('}');
                                if(pos != std::string::npos)
                                    arg.erase(pos, 1);
                            }
                            debug("wrn", arg);
                            if(arg == "SIMULATION"){
                                r.SIMULATION = true;
                            }
                            else if(arg == "ECONOMICS"){
                                r.CRYPT_DATA_EDITING == true;
                            }
                            else if(arg == "CRYPT"){
                                r.CRYPT_EDITING = true;
                            }
                            else if(arg == "DATA"){
                                r.DATA_EDITING = true;
                            }
                            else if(arg == "ADMINLIST") {
                                r.ADMIN_LIST_EDITING = true; 
                            }
                        }
                    }

                    rightModes rm = (kv.first == "root" || kv.first == "ROOT" ) ? rightModes::ROOT : 
                        (kv.first == "mod" || kv.first == "MOD") ? rightModes::MOD :
                        (kv.first == "crypt" || kv.first == "CRYPT") ? rightModes::CRYPT :
                        (kv.first == "modcrypt" || kv.first == "MODCRYPT") ? rightModes::MODCRYPT : rightModes::NONE;

                    
                    Adm::SetEnvViaMod(&rm, &r);
                
                    try{
                     r.tgId = std::stoi(kv.second.get_value<std::string>());
                    } catch (std::exception &e) {
                        debug("err", e.what());
                        exit(1);
                    }
                    
                    std::cout << ("CRYPT_DATA_EDITING: " + std::string ((r.CRYPT_DATA_EDITING) ? "\033[32mtrue\033[0m\n" : "\033[31mfalse\033[0m\n"));
                    std::cout <<  ("ADMIN_LIST_EDITING: " + std::string ((r.ADMIN_LIST_EDITING) ? "\033[32mtrue\033[0m\n" : "\033[31mfalse\033[0m\n" ));
                    std::cout << ( "CRYPT_EDITING: " + std::string ( (r.CRYPT_EDITING) ? "\033[32mtrue\033[0m\n" : "\033[31mfalse\033[0m\n" ));
                    std::cout << ("DATA_EDITING: " + std::string ( (r.DATA_EDITING) ? "\033[32mtrue\033[0m\n" : "\033[31mfalse\033[0m\n" ));
                    std::cout << ("SIMULAION: " + std::string ((r.SIMULATION) ? "\033[32mtrue\033[0m\n" : "\033[31mfalse\033[0m\n" ));
                    std::cout << ("GLOBAL: " + std::string( (r.GLOBAL) ? "\033[32mtrue\033[0m\n" : "\033[31mfalse\033[0m\n" ));
                    std::cout << ("CHAT_ID: " + std::to_string(r.chatId) + '\n');
                    std::cout << ("TG_ID: " + std::to_string(r.tgId) + '\n');
                    Adm::SaveAdmin(&r);
                   
                }
              
              
        }
        if(!silent){
		 std::cout << termcolor::green << "[V]" << termcolor::reset << " - init complete" << "\n";
		 debug("wrn", "user coins array: " + std::to_string(sizeof(userCoins)));
		 debug("inf", "admins loaded");
      	}
        return 0;
    }catch(std::exception& e){
         std::cout << termcolor::blue << "[X]" << termcolor::reset << " - " << e.what() << "\n";
         return 1;
    }

}
int File::coinInit(bool silent){
    try{
    boost::property_tree::ptree coin;
    boost::property_tree::ini_parser::read_ini(CFILE, coin);
    short pos = 1;
    while(coin.size() >= pos){
        CDATA_ALL[pos - 1].name = coin.get<std::string>(std::to_string(pos) + ".name");
        CDATA_ALL[pos - 1].rate = coin.get<double>(std::to_string(pos) + ".DRATE");
        CDATA_ALL[pos - 1].sens = coin.get<float>(std::to_string(pos) + ".sens");
       std::string temp =  CDATA_ALL[pos - 1].uniqueId  = coin.get<std::string>(std::to_string(pos) + ".uid");
       debug("wrn", "uid: " + temp);
       if (temp == "null"){
            Crypto::giveUniqueId(&CDATA_ALL[pos-1]);
            coin.put(std::to_string(pos) + ".uid", CDATA_ALL[pos-1].uniqueId);
            debug ("wrn", "new uid for " + CDATA_ALL[pos-1].name + ": " + CDATA_ALL[pos-1].uniqueId);
       }
        if(!silent)
        	std::cout << termcolor::bright_green << "[C]" << termcolor::reset << " - detected " + CDATA_ALL[pos - 1].name << "\n";
        pos++;
    }
	if(!silent)
    {
    std::cout << termcolor::green << "[V]" << termcolor::reset << " - init coin complete" << "\n";
    debug("wrn", "size of array coin: " + std::to_string(pos - 1));
    }
    boost::property_tree::write_ini(CFILE, coin);
    debug("cmn", "new ver of " + CFILE  + " added" );
    return 0;
    }catch(std::exception& e){
         std::cout << termcolor::blue << "[X]" << termcolor::reset << " - coin init error: " << e.what() << "\n";
         return 1;
    }
}

int main(){
    if(File::init(false)){
        return 1;
    }
     if(File::coinInit(false)){
        return 1;
    }
    boost::property_tree::ptree config;
    boost::property_tree::ini_parser::read_ini("config.ini", config);
    std::string token;
   try{
    token = config.get<std::string>("main.token");
    }catch(std::exception& e){
        std::cout << termcolor::blue << "[X]" << termcolor::reset << " - " << e.what() << "\n";
        return 1;
    }

    TgBot::Bot bot(token);

    Tg::buySold(bot);
    Tg::soc(bot);
    Tg::min(bot);

    bot.getEvents().onCommand("add", [&bot](TgBot::Message::Ptr message){
        Rights r{};
        r.CRYPT_EDITING = true;
        r.tgId = message->from->id;
        if(isAdmin(&r)){
            std::vector<std::string> parse;
            parse = splitArgs(message->text);
            if(parse.size() == 4){
            coin cc;
            std::string str = parse[1];
            try{
            cc.name = str.c_str();
            cc.rate = std::stod(parse[2]);
            cc.sens = std::stod(parse[3]);
            Crypto::addCoin(cc);
            }catch (std::exception &e){
                debug("err", e.what());
            }
        }else{
            bot.getApi().sendMessage(message->chat->id, "непрвильный синтакс");
        }
        }
    });
    
    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query){
        if(query->data.find("farm_buy") == std::string::npos){
            Menu::callBackCheck(query, &bot);
        }
    });

    Tg::menu(bot);
    bot.getEvents().onCommand("reinit", [&bot](TgBot::Message::Ptr message){
        Rights r{};
        r.SIMULATION = true;
        r.GLOBAL = true;
        r.tgId = message->from->id;
        if(isAdmin(&r)){
        	std::vector<std::string> parse;
        	parse = splitArgs(message->text);
        	bool silent = false;
			if(parse.size() > 1){
        		silent = (parse[1] == "silent") ? true : false;
				debug ("inf", "silent: " + std::to_string(silent));
        		}
        		 
            File::init(silent);
            File::coinInit(silent);
        }
    });

    bot.getEvents().onCommand("scap", [&bot](TgBot::Message::Ptr message){
        Rights r{};
        r.DATA_EDITING, r.CRYPT_EDITING, r.ADMIN_LIST_EDITING, r.CRYPT_DATA_EDITING, r.SIMULATION , r.GLOBAL = true;
        r.tgId = message->from->id;
        if(isAdmin(&r)){
            std::vector<std::string> parse;
            parse = splitArgs(message->text);
            if(parse.size() > 1){
                if(parse[1] == "mode"){
                    DNOT = (parse[2] == "silent") ? true : (parse[2] == "loud") ? false : DNOT;
                    debug("wrn", (DNOT)? "silent mode activated" : "loud mode activated"); 
                }
                else if(parse[1] == "info"){

                    int64_t id = message->replyToMessage->from->id;

                    social s{};
                    s = File::loadSocial(message->replyToMessage->from->id);

                    std::string creditPallet =  "```credit\n"  "bonus count: " + std::to_string(s.bonusCount) + 
                    "\ncount: " + std::to_string(s.creditCount) + 
                    "\nlast played: " + std::to_string(s.lastPlayed) +
                    "\nparty member: " + ((s.partyMember) ? "true" : "false") + "\n```\n";


                    short count = 0;
                    
                    while(CDATA_ALL[count].sens != 0){
                        count++;
                    }
                     
                    std::string pallet = "";

                    for(int i = 0; i < count; i++){
                        userCoins coins{};
                        
                        coins = Crypto::loadData(id, i);

                        pallet +=  "```" + CDATA_ALL[i].name + 
                        "\ncount: " + std::to_string(coins.count) + "\n" + 
                        "last mine: " + std::to_string(coins.lastMine) + "\n" +
                        "num up: " + std::to_string(coins.numUp) + "\n```\n" ;

                    }

                    auto replyOptions = std::make_shared<TgBot::ReplyParameters>();
                    replyOptions->messageId = message->messageId;
                    replyOptions->chatId = message->chat->id;

                    bot.getApi().sendMessage(message->chat->id, creditPallet + pallet, nullptr, replyOptions, nullptr, "MarkdownV2");
                    debug("wrn" , message->from->firstName + " used scap info");
                }
            }
        }
    });

    while(true){
        try
        {
            debug("inf", "bot start.");
            TgBot::TgLongPoll lPoll(bot);
            while(true){
                lPoll.start();
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << termcolor::blue << "[X]" << termcolor::reset << " - " << e.what() << '\n';
        }
        
    }

    return 0;
}


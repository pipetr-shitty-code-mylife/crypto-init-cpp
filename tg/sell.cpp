#include "MLB_SIM.hpp"
#include "main_data.hpp"



std::vector<std::string> splitArg(const std::string& text){
	std::istringstream iss(text);
	std::vector<std::string> result;
	std::string word;
	while (iss >> word){
		result.push_back(word);
	}
	return result;
}

void sell_offer(TgBot::Bot& bot){
	
	bot.getEvents().onCommand("sell", [&bot](TgBot::Message::Ptr message){
		std::vector<std::string> parts = splitArg(message->text);
		if(parts.size() < 2){
			bot.getApi().sendMessage(message->chat->id, "fuck yourself nigger");
			return;
		}

		try{
			int b = std::stoi(parts[1]);
			ofSold su{};
			ofBuy obob{};
			MLB mmm{};
			MLB_data mmd{};
			mmd = load_MLB(message->from->id, mmd);
			if(mmd.cout_MLB < b || b < 0){
				bot.getApi().sendMessage(message->chat->id, "у тебя недостаточно монет");
				return;
			}
			mmm = loadRate();
			su.from_id = message->from->id;
			su.sc_price = b * mmm.rate;
			su.count = b;
			su.rate = mmm.rate;
			obob = load_buy(su);
			MLB_data mData{};
			mData = load_MLB(su.from_id, mData);
			mData.cout_MLB -= su.count;
			save_mlb(mData);
			if(obob.from_id != 0){
				if(obob.count == su.count)
					delete_buy(obob);
				else {
					ofBuy ob_save = obob;
					ob_save.count = obob.count - su.count;
					ob_save.sc_price = ob_save.count * mmm.rate;
					resave_buy(obob, ob_save);
				}
				chatData cData{};
				cData = load_data(su.from_id, cData, message->chat->id);
				cData.us.credit_count += su.sc_price;
				MLB_data mBuy{};
				mBuy = load_MLB(obob.from_id , mBuy);
				mBuy.cout_MLB += su.count;
				save_mlb(mBuy);
				
			}
			else{
				int popa = saveOS(su);	
			}
			bot.getApi().sendMessage(message->chat->id, std::to_string(su.count));
			
		} catch (std::exception& e){
			bot.getApi().sendMessage(message->chat->id, "pizdec_s" );
		}

			});
	return;
}

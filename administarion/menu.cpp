#include  "mainInclude.hpp"
#include "adm.hpp"
#include "events.hpp"

Adm::Submenu mainMenu;

std::map<int64_t, MenuIds> currentIdMessage;
std::map<std::string, Adm::Submenu> allSubmenus;



void MenuInit(){
        mainMenu.menuText = "main menu";
        mainMenu.menuId = "menu0";
        mainMenu.buttonText = "Go to main menu";
        mainMenu.mapp["0"] = {};
        mainMenu.mapp["0"].buttonText = "ya pokakal";
        mainMenu.mapp["0"].menuId = "menu2";
        mainMenu.menuText = "shit";
        allSubmenus[mainMenu.menuId] = mainMenu;
        Adm::Submenu popa{};
        popa.buttonText  = "ya pokakal";
        popa.mapp["2"] = {};
        popa.mapp["2"].buttonText = "test";
        popa.mapp["2"].menuId = "menu0";
        popa.mapp["3"] = {};
        popa.mapp["3"].buttonText = "test";
        popa.mapp["3"].menuId = "menu0";
        popa.mapp["4"] = {};
        popa.mapp["4"].buttonText = "test_event";
        popa.mapp["4"].menuId = "menu2";
        popa.mapp["4"].event = test_event;
        popa.mapp["4"].eevent = true;
        popa.menuText = "another shit";
        popa.menuId = "menu2";
        allSubmenus[popa.menuId] = popa;
}

void Tg::menu(TgBot::Bot& bot){
    MenuInit();
    bot.getEvents().onCommand("menu", [&bot] (TgBot::Message::Ptr message)
    {
        Adm::Submenu menu = mainMenu;
        Rights r {};
        rightModes m = rightModes::ROOT;
        Adm::SetEnvViaMod(&m, &r);
        r.tgId = message->from->id;
        if(isAdmin(&r))
        {
       
        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

        std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;
        for (auto& [key, value] : menu.mapp){
            TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
            button->text =  value.buttonText;
            button->callbackData = key;
            buttons.push_back(button);
	    if(buttons.size() == 2) {
	    	keyboard->inlineKeyboard.push_back(buttons);
		buttons.clear();
        	}
	}
	   if(buttons.size() == 1)
	   	keyboard->inlineKeyboard.push_back(buttons);
       TgBot::Message::Ptr msg = bot.getApi().sendMessage(message->chat->id, "test                                                                          POPS\ntest\n\test", 0 , 0 , keyboard);
        currentIdMessage[msg->messageId].mId = menu.menuId;
    }      
    });
    
}

TgBot::InlineKeyboardMarkup::Ptr GenKeyboard(Adm::Submenu* menu, TgBot::InlineKeyboardMarkup::Ptr keyboardret){
        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

        std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;
        for (auto& [key, value] : menu->mapp){
            TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
            button->text =  value.buttonText;
            button->callbackData = key;
            buttons.push_back(button);
	    if(buttons.size() == 2) {
	    	keyboard->inlineKeyboard.push_back(buttons);
		buttons.clear();
        	}
             keyboardret = keyboard;
	    }
	if(buttons.size() == 1)
	keyboard->inlineKeyboard.push_back(buttons);
    return keyboard;
}

void Menu::callBackCheck(TgBot::CallbackQuery::Ptr query, TgBot::Bot *bot){
    std::string data = query->data;
    int16_t messageId = query->message->messageId;
    debug("inf", "callback for " + std::to_string(query->message->from->id));
    std::string menuID = "menu0";

    if(currentIdMessage.contains(messageId)){
        menuID = currentIdMessage[messageId].mId;
        debug("inf", "current have " + std::to_string(messageId) + " " + menuID);
    }
    Adm::Submenu* menu = &allSubmenus[menuID];
    if(!menu->mapp[data].menuId.empty()){

    if(menu->mapp[data].eevent){
        int return_code = menu->mapp[data].event(bot, query->message);
        debug((return_code >= 1)? "err" : "inf" , "event(" + menu->mapp[data].buttonText + ") returned " + std::to_string(return_code));
    }
    
    if(menuID != menu->mapp[data].menuId){
   
    debug("inf", "query data " + data);
    if(menu->mapp.contains(data)){
        debug("inf", "changing....");
        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
        keyboard = GenKeyboard(&allSubmenus[menu->mapp[data].menuId], keyboard);
        debug("wrn", allSubmenus[menu->mapp[data].menuId].menuText);
        debug("wrn", data);
        debug("wrn", menu->mapp[data].menuId);
        debug("wrn", "message id: " + std::to_string(messageId));
        bot->getApi().editMessageText(allSubmenus[menu->mapp[data].menuId].menuText, query->message->chat->id, query->message->messageId, "", "", nullptr, keyboard);
        MenuIds id = {menu->mapp[data].menuId, query->message->from->id};
        currentIdMessage[messageId] = id;
        }   
    }
}  
    return;

}

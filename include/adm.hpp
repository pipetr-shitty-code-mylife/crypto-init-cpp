#ifndef ADM_HPP
#define ADM_HPP

#include <map>
#include <ctype.h>
#include <tgbot/tgbot.h>

enum class rightModes{
    ROOT,
    MOD,
    CRYPT,
    MODCRYPT,
    NONE
};

struct Button{
    std::string buttonText;
    std::string menuId;
    bool eevent;
    int (*event)(TgBot::Bot* bot, TgBot::Message::Ptr m);
};

struct Rights{
        int tgId;
        int chatId;
        bool GLOBAL = false;
        bool SIMULATION = false;
        bool DATA_EDITING = false;
        bool CRYPT_DATA_EDITING = false;
        bool CRYPT_EDITING = false;
        bool ADMIN_LIST_EDITING = false;
};

namespace Adm{
    void SetEnvViaMod(rightModes *m, Rights *r);
    void SaveAllAdminLists();
    void SaveAdmin(Rights* r);
    void GetEnvsViaId(int tgId, Rights* r);

    class Submenu {
        public:
            std::map<std::string, Button> mapp ;
            std::string menuText;
            std::string buttonText;
            std::string menuId;
            
    };
};

struct MenuIds{
    std::string mId;
    int64_t uId;
};

namespace Menu{
    void callBackCheck(TgBot::CallbackQuery::Ptr query, TgBot::Bot* bot);
};

extern std::map<int64_t, MenuIds> currentIdMessage;
extern std::map<std::string, Adm::Submenu> allSubmenus;

bool isAdmin(Rights *r);
extern std::map<int, Rights> AdminList;

#endif
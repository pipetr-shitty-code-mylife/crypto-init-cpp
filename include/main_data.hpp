#ifndef MAIN_DATA_HPP
#define MAIN_DATA_HPP
#include <tgbot/tgbot.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <sstream>

#define OFSF "offers_s.sosibitara"
#define FSB "offers_b.sosibitara"

typedef struct{
    int user_id;
    int credit_count;
    bool party_member;
    int bonus_count;
    long last_played;
}userData;

typedef struct{
    int user_id;
    double cout_MLB;
    int num_upgrade;
    long last_played;
}MLB_data;

typedef struct{
    int chatId;
    userData us;
}chatData;

int social_credit(TgBot::Bot& bot);
int save_file(const chatData& data);
chatData load_data(int id, chatData& user, int c_id);
void buy_offer (TgBot::Bot& bot);
void sell_offer(TgBot::Bot& bot);
void load_list( TgBot::Bot& bot);
MLB_data load_MLB(int id, MLB_data& user);
void save_mlb(const MLB_data& data);
void mining(TgBot::Bot& bot);
#endif
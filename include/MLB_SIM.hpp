#pragma once


#ifndef MLB_SIM_HPP
#define MLB_SIM_HPP

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

#define RATE "rate.sosibitara"



typedef struct{
    long double rate;
}MLB;

typedef struct{
    int from_id;
    int count;
    long double rate;
    int sc_price;
}ofSold;

typedef struct{
    int from_id;
    int count;
    long double rate;
    int sc_price;
}ofBuy;

std::vector<std::string> splitArg(const std::string& text);
ofBuy load_buy(ofSold& os);
MLB loadRate();
int saveOS(const ofSold& os);
long sumo_buy();
long sumo_sold();
int saveOB(const ofBuy& ob);
void save_rate(const MLB& m);
void delete_sold(ofSold& os);
void delete_buy(ofBuy& ob);
ofSold load_sold(ofBuy& ob);
void resave_sold(const ofSold& data , const ofSold& data_save);
void resave_buy(const ofBuy& data , const ofBuy& data_save);

#endif

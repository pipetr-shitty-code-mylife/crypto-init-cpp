#include "MLB_SIM.hpp"
#include <unistd.h>


long double rate_new(long double rate, long double ob, long double os)
{
    float v = 0.02;
    long double tmp_rate;
    tmp_rate = rate * (1.0 + ((ob - os)/(ob+ os) * v));
    if(os == 0 && ob == 0){
	    return 0.23;}
    return tmp_rate;
}

int main() {
	long int sumB = 0;
	long int sumS = 0;
    MLB m{};
    m = loadRate();
    std::cout << m.rate << std::endl;
    long double rate;
    if(m.rate == 0){
	    rate = 23.0;}
    else{
        rate = m.rate;
    }

 	long int last  = std::time(nullptr);
	long int last_sum = std::time(nullptr);
while(true){
	if(last_sum + 2 < std::time(nullptr)){
	sumB = sumo_buy();
	sumS = sumo_sold();
	printf("\rsb:%ld , ss:%ld , r:%Lf",sumB, sumS, rate);
    fflush(stdout);
	last_sum = std::time(nullptr);
	}
    if (last + 60 < std::time(nullptr)){
    long double tmp_rate;
    tmp_rate = rate_new(rate, sumB, sumS);
    rate = tmp_rate;
    last = std::time(nullptr);
    m.rate = rate;
    save_rate(m);
    }
}
    return 0;
}


#include "MLB_SIM.hpp"

long   sumo_buy(){
    long  sum_buy = 0;
    FILE* file = fopen("offers_b.sosibitara", "rb");
    if(!file){
        printf("error reading ofb\n");
        return  0 ;
    } 
    
    ofBuy b{};
    while (fread(&b, sizeof(ofBuy), 1, file)  ==  1)
    {
      sum_buy += b.count;
    }
    return  sum_buy;

}

long sumo_sold(){
    long  sum_sold = 0;
    FILE* file = fopen("offers_s.sosibitara", "rb");
    if(!file){
        printf("error reading ofs\n");
        return  0 ;
    } 
    
    ofSold b{};
    while (fread(&b, sizeof(ofSold), 1, file) == 1)
    {
      sum_sold += b.count;
    }

  
    return  sum_sold;
}

#include "sim.hpp"
#include "cstring"


std::map<std::string, Sim::Coin> CoinAll;
std::string RFILE;
std::string OBFILE;
std::string OSFILE;
std::string CFILE;
std::string MUSER;
coin CDATA_ALL[100]{};
int32_t Time = 10;

long double Sim::Coin::ShowRate(){
      return cM.Rate;
}

void Sim::Coin::RateUpdate(){

            CheckOfferB();
            std::cout << "[" + name + "]: BO count: " + std::to_string(Boffer) + "\n";
            CheckOfferS();
            std::cout << "[" + name + "]: SO count: " + std::to_string(Soffer) + "\n";

            long double tmp_rate = 0.0f;
            if (Soffer != 0 && Boffer != 0)
                  tmp_rate = cM.Rate * (1.0f + ( (Boffer - Soffer)/(Boffer + Soffer) + sens));
            std::cout <<  "[" + name +"]: rate" + std::to_string(tmp_rate)+ "\n";
            cM.Rate = tmp_rate;
            FILE* file = fopen(RFILE.c_str(), "r+b");

            if(!file) { std::cout << "[" + RFILE + "]: error while opening. Trying another way....\n";      
            file = fopen(RFILE.c_str(), "wb");
            if(!file){
                  std::cout << "[" + RFILE + "]: error while opening.\n FATAL \n";
                  exit(1);
            }
            }

            rateBody rTemp{};

            int32_t pos = 0;
            bool found = false;

            while ( ( fread(&rTemp, sizeof(rateBody), 1, file) ) == 1  ){
                  if( strcmp(rTemp.coinId, cM.coinId) ==  0 ){
                        found = true;
                        break;
                  }
                  pos++;
            }

            if(found){
                  fseek(file, pos * sizeof(rateBody), SEEK_SET);
            }
            else {
                  fseek (file, 0, SEEK_END);
            }
            
            fwrite(&cM, sizeof(rateBody), 1, file);
            
            lastUp = std::time(nullptr);
            fclose(file);
}

void Sim::Coin::Init(coin *C){
      cM.Rate = C->rate;
      strcpy(cM.coinId, (C->uniqueId).c_str());
      printf("[debug]: %s\n", cM.coinId);
      name = C->name;
      sens = C->sens;
}

void Sim::Coin::CheckOfferB(){
      FILE* file = fopen(OBFILE.c_str(), "rb");
      
      int32_t pos;
      while (CDATA_ALL[pos].sens != 0 ){
            if(CDATA_ALL[pos].name == name){
                  break;
            }
            pos++;
      }

      if(!file) {
            std::cout << "[" + OBFILE + "]: " + "error while opening";
      }else{
      
      offer oT{};

      while((fread(&oT, sizeof(offer), 1, file))  ==  1){
            if ( pos == oT.coinId){
                  Boffer += oT.count;
            }
      }

      fclose(file);
}
}
void Sim::Coin::CheckOfferS(){
      FILE* file = fopen(OSFILE.c_str(), "rb");
      
      int32_t pos;
      while (CDATA_ALL[pos].sens != 0 ){
            if(CDATA_ALL[pos].name == name){
                  break;
            }
            pos++;
      }

      if(!file) {
            std::cout << "[" + OSFILE + "]: " + "error while opening";
      }else{
      
      offer oT{};

      while((fread(&oT, sizeof(offer), 1, file))  ==  1){
            if ( pos == oT.coinId){
                  Soffer += oT.count;
            }
      }

      fclose(file);
}
}
void Sim::ClassInit(){
      for (int16_t pos; CDATA_ALL[pos].sens != 0; pos++ ){
            Sim::Coin temp{};
            temp.Init(&CDATA_ALL[pos]); 
            CoinAll[CDATA_ALL[pos].name] = temp;
            std::cout << "[classes]: " + CDATA_ALL[pos].name + '(' + CDATA_ALL[pos].uniqueId + ')' + " class created\n";
      }
}
void Sim::startReadingMainConfig(){
      boost::property_tree::ptree config;
      boost::property_tree::ini_parser::read_ini("config.ini", config);
      
      try {
            RFILE = config.get<std::string>("main.RFILE");
            OSFILE = config.get<std::string>("main.OSFILE");
            OBFILE = config.get<std::string>("main.OBFILE");
            CFILE = config.get<std::string>("main.CFILE");
            Time = config.get<int>("other.RTIME");
            MUSER = config.get<std::string>("main.MUSER");
            boost::property_tree::ptree crypto;
            boost::property_tree::ini_parser::read_ini(CFILE, crypto);

            try {
                  for (int16_t i = 1; i <= crypto.size(); i++){
                        std::string pos = std::to_string(i);
                        CDATA_ALL[i-1].name = crypto.get<std::string>(pos + ".name");
                        CDATA_ALL[i-1].rate = crypto.get<double>(pos + ".DRATE");
                        CDATA_ALL[i-1].sens = crypto.get<float>(pos + ".sens");
                        CDATA_ALL[i-1].uniqueId = crypto.get<std::string>(pos + ".uid");
                  
                  FILE* f = fopen(RFILE.c_str(), "rb");
                  if(!f){
                    std::cout << '[' + RFILE + ']' + ": erro while opening\n" ;
                  }else {
                  rateBody body{};
                  while(fread(&body, sizeof(rateBody), 1 ,f) == 1){
                        if(strcmp(body.coinId,(CDATA_ALL[i-1].uniqueId).c_str()) == 0){
                                  std::cout << "rate founded: " + std::to_string(body.Rate)  + "\n";
                                    if(body.Rate != 0)
                                    CDATA_ALL[i-1].rate = body.Rate;
                                  break;
                        }
                  }
                  fclose(f);
                  }
                }
            }
            catch (std::exception &e ){
                  throw std::runtime_error(e.what()); 
            }
      }
      catch (std::exception &e){
        std::cout << e.what() << "\n";
        std::cout << "fatal\n";
        exit(1);
      }
}

void Sim::initAll(){
    std::cout << "[main config]: reading\n";
    Sim::startReadingMainConfig();
}
int main(int argc, char* argv[]){
    Sim::initAll();
    Sim::ClassInit();


    while(true){ 
            for (auto&  [uid, value] : CoinAll){
                  if(std::time(nullptr) - value.lastUp >= Time ) {
                              value.RateUpdate();
                               std::cout << "[" + uid +   "]: rate updated " + std::to_string(value.ShowRate()) + "\n" ;
                  }
            }
    }
}
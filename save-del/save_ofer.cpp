#include "MLB_SIM.hpp"
#include <cstdio>
//INCLUDE
#define OFSF "offers_s.sosibitara"
#define FSB "offers_b.sosibitara"

int saveOS(const ofSold& os){
	FILE* file = fopen(OFSF, "ab");
	if(!file){
		file = fopen(OFSF, "wb");
	 	if(!file){
		perror("pizdec");
		return 1;
		}
	}

	fwrite(&os, sizeof(ofSold), 1,file);
	fclose(file);
	return 0;
}


int saveOB(const ofBuy& ob){
    FILE* file = fopen(FSB, "ab");
	if(!file){
		file = fopen(FSB, "wb");
	 	if(!file){
		perror("pizdec_B");
		return 1;
		}
	}

	fwrite(&ob, sizeof(ofBuy), 1,file);
	fclose(file);
	return 0;
}

void save_rate(const MLB& m){
	FILE* file = fopen(RATE, "wb");
	if(!file){
		perror("pizdec_R/n");
		return;
	}
	fwrite(&m, sizeof(MLB), 1, file);
	fclose(file);
	return;
}

void resave_sold(const ofSold& data , const ofSold& data_save){
    FILE* file = fopen(OFSF, "r + b");
    if (!file) {
           perror("error reading to resave file");
           return;
        
    }

    ofSold temp;
    bool found = false;
    long pos = 0;

    while (fread(&temp, sizeof(ofSold), 1, file) == 1) {
        if (temp.from_id == data.from_id && temp.sc_price == data.sc_price && temp.rate == data.rate && temp.count == data.count) {
            found = true;
            break;
        }
        pos++;
    }

    if (found) {
        fseek(file, pos * sizeof(ofSold), SEEK_SET);
    } else {
        fseek(file, 0, SEEK_END);
    }

    fwrite(&data_save, sizeof(ofSold), 1, file);
    fclose(file);
    return;
}

void resave_buy(const ofBuy& data , const ofBuy& data_save){
    FILE* file = fopen(FSB, "r + b");
    if (!file) {
           perror("error reading to resave file b");
           return;
        
    }

    ofBuy temp;
    bool found = false;
    long pos = 0;

    while (fread(&temp, sizeof(ofBuy), 1, file) == 1) {
        if (temp.from_id == data.from_id && temp.sc_price == data.sc_price && temp.rate == data.rate && temp.count == data.count) {
            found = true;
			std::cout << "found" << std::endl;
            break;
        }
        pos++;
    }

    if (found) {
        fseek(file, pos * sizeof(ofBuy), SEEK_SET);
    } else {
        fseek(file, 0, SEEK_END);
    }

    fwrite(&data_save, sizeof(ofBuy), 1, file);
    fclose(file);
    return;
}


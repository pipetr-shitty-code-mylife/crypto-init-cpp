#include "MLB_SIM.hpp"
#include <cstdio>

ofSold load_sold(ofBuy& ob){
	ofSold osos{};
 	FILE* file = fopen("offers_s.sosibitara", "rb");
	if(!file){
		perror("error load sold");
		
		return osos;
	}
	ofSold os{};
	if(ob.rate != 0){
	while(fread(&os, sizeof(ofSold), 1, file) == 1){
		if(os.count >= ob.count && os.rate == ob.rate){
			printf("id seller:%d\n", os.from_id);
			fclose(file);
			return os;
			
		}
		
	}
}
	else{
		printf("rate = 0\n");
		while(fread(&os, sizeof(ofSold), 1, file) == 1){
			if(os.count == ob.count && os.sc_price == ob.sc_price){
				printf("id seller:%d\n", os.from_id);
				fclose(file);
				return os;
			
			}
		
	}
}
	fclose(file);
	return osos;
}

ofBuy load_buy(ofSold& os){
	ofBuy obob{};
	FILE* file = fopen("offers_b.sosibitara", "rb");
	if(!file){
		perror("error load buy");
		return obob;
	}
	while(fread(&obob, sizeof(ofBuy), 1, file) == 1){
		if(obob.count >= os.count && os.rate == obob.rate){
			printf("id offer b: %d\n", obob.from_id);
			fclose(file);
			return obob;
		}
	}
	fclose(file);
	obob.from_id = 0;
	return obob;
}

#include "MLB_SIM.hpp"

#define RATE "rate.sosibitara"

MLB  loadRate(){
	FILE* file = fopen(RATE, "rb");
	if(!file){
		perror("pizdec_RR \n");
		MLB mmm{};
		return  mmm;
	}
	MLB m_temp{};
	fread(&m_temp, sizeof(MLB), 1, file);
	return m_temp;
}
#include "MLB_SIM.hpp"
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define OFS "offers_s.sosibitara"
#define OBS "offers_b.sosibitara"

void delete_sold(ofSold& os) {
    FILE* file = fopen(OFS, "rb");
    if (!file) {
        perror("can't open of");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    int count = file_size / sizeof(ofSold);
    if (count == 0) {
        fclose(file);
	printf("dont find offers\n");
        return;
    }

    ofSold* solds = new ofSold[count];
    if (fread(solds, sizeof(ofSold), count, file) != (size_t)count) {
        perror("read error");
        delete[] solds;
        fclose(file);
        return;
    }
    fclose(file);

    int index = -1;
    for (int i = 0; i < count; ++i) {
        if (solds[i].count == os.count && solds[i].from_id == os.from_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        delete[] solds;
	printf("index = -1\n");
        return;
    }

    for (int i = index; i < count - 1; ++i) {
        solds[i] = solds[i + 1];
    }
    count--;

    file = fopen(OFS, "wb");
    if (!file) { 
        perror("can't open for writing");
        delete[] solds;
        return;
    }

    fwrite(solds, sizeof(ofSold), count, file);
    fclose(file);
    delete[] solds;
	printf("delete suck\n");
	return;
}

void delete_buy(ofBuy& ob) {
    FILE* file = fopen(OBS, "rb");
    if (!file) {
        perror("can't open ofb");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    int count = file_size / sizeof(ofBuy);
    if (count == 0) {
        fclose(file);
	printf("dont find offers\n");
        return;
    }

    ofBuy* solds = new ofBuy[count];
    if (fread(solds, sizeof(ofBuy), count, file) != (size_t)count) {
        perror("read error");
        delete[] solds;
        fclose(file);
        return;
    }
    fclose(file);

    int index = -1;
    for (int i = 0; i < count; ++i) {
        if (solds[i].count == ob.count && solds[i].from_id == ob.from_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        delete[] solds;
	printf("index = -1\n");
        return;
    }

    for (int i = index; i < count - 1; ++i) {
        solds[i] = solds[i + 1];
    }
    count--;

    file = fopen(OBS, "wb");
    if (!file) {
        perror("can't open OBS for writing");
        delete[] solds;
        return;
    }

    fwrite(solds, sizeof(ofBuy), count, file);
    fclose(file);
    delete[] solds;
	printf("delete suck b\n");
	return;
}

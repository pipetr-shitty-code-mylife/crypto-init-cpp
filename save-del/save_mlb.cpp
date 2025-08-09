#include "main_data.hpp"

void save_mlb(const MLB_data& data){
    FILE* file = fopen("mlb_data.sosibitara", "r+b");
    if (!file) {
        file = fopen("mlb_data.sosibitara", "wb");
        if (!file) {
            perror("error creating mlb file");
            return;
        }
    }

    MLB_data temp;
    bool found = false;
    long pos = 0;

    while (fread(&temp, sizeof(MLB_data), 1, file) == 1) {
        if (temp.user_id == data.user_id) {
            printf("founded on writing mlb\n");
            found = true;
            break;
        }
        pos++;
    }

    if (found) {
        fseek(file, pos * sizeof(MLB_data), SEEK_SET);
    } else {
        fseek(file, 0, SEEK_END);
        printf("don't founded");
    }

    fwrite(&data, sizeof(MLB_data), 1, file);
    fclose(file);
    return;
}
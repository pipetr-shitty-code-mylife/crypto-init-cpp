#include "main_data.hpp"
#include "MLB_SIM.hpp"
MLB_data load_MLB(int id, MLB_data& user){
    FILE* file = fopen("mlb_data.sosibitara", "rb");
    if(!file){
        perror("error open while reading mlb\n");
        return user;
    }
    MLB_data temp{};
    while (fread(&temp, sizeof(MLB_data), 1, file) == 1) {
        if (temp.user_id == id) {
            fclose(file);
            printf("mlb founded\n");
            return temp;
        }
    }
    printf("don't mlb founded\n");
    fclose(file);
    return user;
}
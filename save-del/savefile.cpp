#include "main_data.hpp"

int save_file(const chatData& data){
    FILE* file = fopen("data.sosibitara", "r+b");
    if (!file) {
        file = fopen("data.sosibitara", "wb");
        if (!file) {
            perror("error creating file");
            return 1;
        }
    }

    chatData temp;
    bool found = false;
    long pos = 0;

    while (fread(&temp, sizeof(chatData), 1, file) == 1) {
        if (temp.chatId == data.chatId && temp.us.user_id == data.us.user_id) {
            found = true;
            break;
        }
        pos++;
    }

    if (found) {
        fseek(file, pos * sizeof(chatData), SEEK_SET);
    } else {
        fseek(file, 0, SEEK_END);
    }

    fwrite(&data, sizeof(chatData), 1, file);
    fclose(file);
    return 0;
}
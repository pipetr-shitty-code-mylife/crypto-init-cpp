#include "main_data.hpp"

chatData load_data(int id, chatData& user, int c_id){
    FILE* file = fopen("data.sosibitara", "rb");
    if(!file){
        perror("error open while reading\n");
        return user;
    }
    chatData temp{};
    if(c_id != 0){
    while (fread(&temp, sizeof(chatData), 1, file) == 1) {
        if (temp.us.user_id == id && temp.chatId == c_id) {
            fclose(file);
            return temp;
        }
        else {
            std::cout << "biba" << std::endl;
        }
    }
    }else {
          while (fread(&temp, sizeof(chatData), 1, file) == 1) {
        if (temp.us.user_id == id) {
            fclose(file);
            return temp;
        }
        else {
            std::cout << "sand" << std::endl;
        }
    }
}
    fclose(file);
    return user;
}
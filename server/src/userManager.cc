#include "userManager.h"

std::map<std::string, std::string> UserManager::_user_data;
UserManager::UserManagerPtr UserManager::_my_instance = nullptr;
std::mutex UserManager::_lock;

UserManager::UserManager(std::string user_data_file) {
    std::ifstream infile(user_data_file);
    char *user_name = new char[255];
    char *psw = new char[255];
    while (infile) {
        bzero(user_name, 255);
        bzero(psw, 255);
        infile.getline(user_name, 255);
        infile.getline(psw, 255);
        _user_data[user_name] = psw;
    }
}
UserManager::~UserManager() {
}
#include <fstream>
#include <map>
#include <mutex>
#include <string.h>
#include <string>

#include <iostream>

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

class UserManager {
    static std::map<std::string, std::string> _user_data;
    static UserManager *_my_instance;
    static std::mutex _lock;
    UserManager(std::string user_data_file);
    ~UserManager();

    public:
    static UserManager *getManager(std::string user_data_file) {
        _lock.lock();
        if (_my_instance == nullptr)
            _my_instance = new UserManager(user_data_file);
        _lock.unlock();
        return _my_instance;
    }

    static bool login(std::string name, std::string psw) {
        if (_user_data.find(name) != _user_data.end())
            return _user_data.find(name)->second == psw;
        return false;
    }
};

#endif
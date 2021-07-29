#include <fstream>
#include <string>
#include <map>
#include <string.h>

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

class UserManager{
    std::map<std::string,std::string> _user_data;
    public:
    UserManager(std::string user_data_file){
        std::ifstream infile(user_data_file);
        char* user_name=new char[255];
        char* psw=new char[255];
        while(infile){
            bzero(user_name,255);
            bzero(psw,255);
            infile.getline(user_name,255);
            infile.getline(psw,255);
            this->_user_data[user_name]=psw;
        }
    }
    ~UserManager(){
    }
    bool login(std::string name,std::string pws){
        return this->_user_data.find(name)->second==pws;
    }
};

#endif
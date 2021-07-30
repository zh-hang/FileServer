#include <cstddef>
#include <iostream>
#include <future>
#include <string>
#include <map>
#include <fstream>
#include <string.h>
#include <thread>

class test{
    public:
    template<class F,class ...Args>
    void print(F&&f,Args&&...args);
};


template<class F,class ...Args>
void test::print(F&&f,Args&&...args){
    std::cout<<f(args...);
}

int foo(int a,int &b,int &c){
    std::cout<<a<<b<<c<<std::endl;
    return a+b+c;
}

auto initUser(std::string user_data_file)->std::map<std::string, std::string>{
    std::ifstream infile(user_data_file);
    std::map<std::string, std::string> user_data;
    char* user_name=new char[256];
    char* psw=new char[256];
    while(infile){
        bzero(user_name,256);
        bzero(psw,256);
        infile.getline(user_name,256);
        infile.getline(psw,256);
        user_data[user_name]=psw;
    }
    return user_data;
}

int main(){
    int a(1),b(2),c(3);
    test t;
    std::thread t1(foo,a,std::ref(b),std::ref(c));
    t1.join();
    // t.print(foo,a,b,c);
    // auto user=initUser("user.txt");
    // for(auto u:user){
    //     std::cout<<u.first<<std::endl;
    // }
    // std::string user_data("%admin%123456");
    // size_t f_pos=user_data.find("%");
    // if(f_pos!=std::string::npos){
    //     size_t s_pos=user_data.find("%",f_pos+1);
    //     if(s_pos!=std::string::npos){
    //         if(user.find(user_data.substr(f_pos,s_pos-f_pos))->second==user_data.substr(s_pos));
    //             std::cout<<"right\n";
    //     }
    // }
    return 0;
}
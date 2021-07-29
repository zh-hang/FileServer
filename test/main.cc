#include <iostream>
#include <future>
#include "../lib/threadpool.h"

class test{
    public:
    template<class F,class ...Args>
    void print(F&&f,Args&&...args);
};


template<class F,class ...Args>
void test::print(F&&f,Args&&...args){
    std::cout<<f(args...);
}

int foo(int a,int b,int c){
    std::cout<<a<<b<<c<<std::endl;
    return a+b+c;
}

int main(){
    int a(1),b(2),c(3);
    test t;
    t.print(foo,a,b,c);
    ThreadPool pool;
    return 0;
}
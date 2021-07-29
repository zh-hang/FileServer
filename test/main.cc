#include <iostream>
#include <future>

class test{
    public:
    template<class F,class ...Args>
    void print(F&&f,Args&&...args);
};


template<class F,class ...Args>
void test::print(F&&f,Args&&...args){
    *f(args...);
}

int foo(int a,int b,int c){
    return a+b+c;
}

int main(){
    int a(1),b(2),c(3);
    test t;
    t(&foo,a,b,c);
    return 0;
}
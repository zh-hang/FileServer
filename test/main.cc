#include <iostream>
#include "../lib/threadpool.cc"

int foo(int a,int b,int c){
    return a+b+c;
}

int main(){
    ThreadPool pool;
    int a(1),b(2),c(3);
    pool.commit(foo, a,b,c);
    while(1){
        ;
    }
    return 0;
}
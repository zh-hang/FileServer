#include <thread>
#include <vector>

#ifndef THREAD_POOL_CLASS
#define THREAD_POOL_CLASS

#define MAX_NUM 32

class ThreadPool{
    std::vector<std::thread>_thred_pool;
public:
    ThreadPool(){}
    ~ThreadPool(){}
};

#endif
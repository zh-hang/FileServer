#include <thread>
#include <vector>
#include <functional>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <memory>
#include <future>

#ifndef MY_THREAD_POOL_H
#define MY_THREAD_POOL_H

#define MAX_NUM 32

using Task=std::function<void()>;

class ThreadPool{

    std::vector<std::thread>_thread_pool;
    std::queue<Task>_tasks;
    std::mutex _lock;
    std::condition_variable _task_cv;
    std::atomic<bool> _run;
    std::atomic<int> _free_thread_num;

public:
    ThreadPool(){
        this->_run=true;
        this->_free_thread_num=0;
        this->addThread(MAX_NUM);
    }
    ~ThreadPool(){
        this->_run=false;
        this->_task_cv.notify_all();
        for(auto& t:this->_thread_pool){
            if(t.joinable())
                t.join();
        }
    }

    template<class F,class...Args>
    void commit(F &&f,Args &&...args);
    
    int freeThreadCount(){
        return this->_free_thread_num;
    }
    int threadCount(){
        return this->_thread_pool.size();
    }

#ifndef GPOWABLE
private:
#endif
    void addThread(int num);

};


template<class F,class...Args>
void print(F f,Args...args){
    std::cout<<"fd:"<<f<<std::endl;
}

template<class F,class...Args>
void ThreadPool::commit(F &&f,Args&&...args){
    if(!this->_run){
        std::cout<<"thread pool is stopped.\n";
        exit(0);
    }
    print(args...);
    using RetType=decltype(f(args...));
    auto task=std::make_shared<std::packaged_task<RetType()>>(
        std::bind(std::forward<F>(f),std::forward<Args>(args)...)
    );
    {
        std::lock_guard<std::mutex> lock{ this->_lock };
        this->_tasks.emplace([task](){
           (*task)();
        });
    }
    this->_task_cv.notify_one();
}

#endif
#include "threadpool.h"



void ThreadPool::addThread(int num){
    for (; this->_thread_pool.size() < MAX_NUM && num > 0; --num){
        this->_thread_pool.emplace_back( [this]{
            while (_run)
            {
                Task task;
                {
                    std::unique_lock<std::mutex> lock{ _lock };
                    _task_cv.wait(lock, [this]{
                            return !_run || !_tasks.empty();
                    });
                    if (!_run && _tasks.empty())
                        return;
                    task = move(_tasks.front());
                    _tasks.pop();
                }
                this->_free_thread_num--;
                task();
                this->_free_thread_num++;
            }
        });
        this->_free_thread_num++;
    }    
}
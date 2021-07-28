#include "threadpool.h"

template<class F,class...Args>
auto ThreadPool::commit(F&&f,Args&&...args) ->std::future<decltype(f(args...))>{
    if(!this->_run){
        std::cout<<"thread pool is stopped.\n";
        exit(0);
    }
    using RetType=decltype(f(args...));
    auto task=std::make_shared<std::packaged_task<RetType()>>(
        std::bind(std::forward<F>(f),std::forward<Args>(args)...)
    );
    std::future<RetType> future = task->get_future();
    {
        std::lock_guard<std::mutex> lock{ this->_lock };
        this->_tasks.emplace([task](){
           (*task)();
        });
    }
    this->_task_cv.notify_one();
    return future;
}

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
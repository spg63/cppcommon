//
//  ThreadPool.hpp
//  cpplib
//
//  Created by Sean Grimes on 5/11/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto push(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> mWorkers;
    // the task queue
    std::queue< std::function<void()>> mTasks;
    
    // synchronization
    std::mutex mQueueMutex;
    std::condition_variable mCondVar;
    bool mStop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
    :   mStop(false)
{
    for(size_t i = 0;i<threads;++i){
        mWorkers.emplace_back(
            [this]{
                while(true){
                    std::function<void()> task;
                                     
                    {
                        std::unique_lock<std::mutex> lock(this->mQueueMutex);
                        this->mCondVar.wait(lock,
                                            [this]{ return this->mStop || !this->mTasks.empty(); });
                        if(this->mStop && this->mTasks.empty())
                            return;
                        task = std::move(this->mTasks.front());
                        this->mTasks.pop();
                    }
                    task();
                }
        });
    }
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::push(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>{
    using return_type = typename std::result_of<F(Args...)>::type;
    
    auto task = std::make_shared< std::packaged_task<return_type()>>(
                                                                      std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                                                                      );
    
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        
        // don't allow enqueueing after stopping the pool
        if(mStop)
            throw std::runtime_error("push on stopped ThreadPool");
        
        mTasks.emplace([task](){ (*task)(); });
    }
    mCondVar.notify_one();
    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        mStop = true;
    }
    mCondVar.notify_all();
    for(std::thread &worker: mWorkers)
        worker.join();
}

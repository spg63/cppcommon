//
//  TSQueue.hpp
//  cppcommon
//
//  Created by Sean Grimes on 12/27/15.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

/**
 \brief Thread safe queue for c++
 \details supports move c'tor and copy c'tor in a thread-safe manner
 \author Sean Grimes, spg63@cs.drexel.edu
 \date 12-27-15
 */
template <class T>
class TSQueue{
public:
    using MutexType = std::mutex;
    using ReadLock = std::unique_lock<MutexType>;
    using WriteLock = std::unique_lock<MutexType>;
    
    /**
        \brief Default c'tor
    */
    TSQueue(){}
    
    /**
        \brief Move c'tor
    */
    TSQueue(TSQueue &&q){
        WriteLock rhs_lk(q.mutex_);
        queue_ = std::move(q.queue_);
    }
    
    /**
        \brief Move assignment
    */
    TSQueue &operator=(TSQueue &&q){
        if(this != &q){
            WriteLock lhs_lk(mutex_, std::defer_lock);
            WriteLock rhs_lk(q.mutex_, std::defer_lock);
            std::lock(lhs_lk, rhs_lk);
            swap(*this, q);
        }
        return *this;
    }
    
    /**
        \brief Copy c'tor
    */
    TSQueue(const TSQueue &q){
        ReadLock rhs_lk(q.mutex_);
        queue_ = q.queue_;
    }
    
    /**
        \brief Copy assignment
    */
    TSQueue &operator=(TSQueue q){
        if(this != &q){
            WriteLock lhs_lk(mutex_, std::defer_lock);
            WriteLock rhs_lk(q.mutex_, std::defer_lock);
            std::lock(lhs_lk, rhs_lk);
            swap(*this, q);
        }
        return *this;
    }
    
    /**
        \brief Swap
    */
    void swap(TSQueue &left, TSQueue &right){
        using std::swap;
        swap(left.queue_, right.queue_);
    }
    
    /**
        \details Single writer thread access
        @param element Item to be pushed onto the queue
    */
    void push(const T &element){
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.push(element);
        }
        condVar_.notify_one();
    }
    
    // This will wait indefinitely if the queue remains empty, not intended for short-running
    // applications
    /**
        \brief Waits while queue is empty
        \details Thread will wait indefinitely while queue is empty, use with caution
        \return Element at the front of the queue
    */
    T wait_and_pop(){
        // Lock released when unique_lock goes out of scope
        std::unique_lock<std::mutex> mlock(mutex_);
        
        // Prevent spurious wakes from ruining everything
        // then release lock and wait
        while(queue_.empty())
            condVar_.wait(mlock);
        
        // Same queue operations as normal
        auto element = std::move(queue_.front());
        queue_.pop();
        return element;
    }
    
    // This can be used for while(true) try_and_pop(T &item), however cpu use could be an issue
    /**
        \brief Will not wait if queue is empty
        \details Will call queue.front(), if no item is available will immediately return. If item is available it will std::move it to the item passed by reference
        @param item Will be populated with item at front of the queue if available, otherwise untouched
        @return True if item is available, false if no item available
    */
    bool try_and_pop(T &item){
        std::unique_lock<std::mutex> mlock(mutex_);
        if(queue_.empty())
            return false;
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    // Should be more CPU efficient, tell the queue how long we're willing to wait
    /**
        \brief Waits for 'timeout' if queue is empty
        \details More cpu efficient alternative to while(true) try_and_pop. Will wait for 'timeout' amount of time while the queue is empty before returning.
        @param item Will be populated with item at front of the queue if available, otherwise untouched
        @param timeout Amount of time to wait on empty queue before returning (std::chrono::milliseconds)
        @return True if item is available, false if no item available
    */
    bool try_and_pop(T &item, std::chrono::milliseconds timeout){
        std::unique_lock<std::mutex> mlock(mutex_);
        if(!condVar_.wait_for(mlock, timeout, [this]{return !queue_.empty();}))
            return false;
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }
    
    /**
        \brief Get copy of underlying std::queue
        @return The underlying queue used in this class
    */
    std::queue<T> backing_queue(){
        std::unique_lock<std::mutex> mlock(mutex_);
        return queue_;
    }
    
    /**
        \brief Check is queue is empty
        @return True if empty, false otherwise
    */
    bool empty(){
        std::unique_lock<std::mutex> mlock(mutex_);
        return queue_.empty();
    }
    
    /**
        \brief Get number of items in queue
        @return Number of items in the queue
    */
    size_t size(){
        std::unique_lock<std::mutex> mlock(mutex_);
        return queue_.size();
    }
    
private:
    std::queue<T> queue_{};
    mutable MutexType mutex_;
    std::condition_variable condVar_;
};

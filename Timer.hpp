//
//  Timer.hpp
//  cppcommon
//
//  Created by Sean Grimes on 11/30/15.
//  Copyright © 2015 Sean Grimes. All rights reserved.
//

#pragma once

#include <chrono>
#include <iostream>

using time_device = std::chrono::high_resolution_clock;
using time_p = std::chrono::high_resolution_clock::time_point;

/**
 \brief A simple code exection timer
 \details exetimer allows one to start and stop timer, and get the time between start and stop calls in seconds, milliseconds, microseconds, or nanoseconds
 \author Sean Grimes, spg63@cs.drexel.edu
 \date 11-30-15
*/

class Timer{
public:
    /**
        \brief start the timer
    */
    void startTimer() { start_ = time_device::now(); }

    /**
        \brief stop the timer
    */
    void stopTimer() { stop_ = time_device::now(); }
    
    /**
        \brief pause the timer
    */
    void pauseTimer() { pause_start_ = time_device::now(); }
    
    /**
        \brief resumse a paused timer
    */
    void resumeTimer() {
        pause_stop_ = time_device::now();
        auto this_pause_duration = pause_stop_ - pause_start_;
        total_paused_time_ += std::chrono::duration_cast<std::chrono::nanoseconds>(this_pause_duration).count();
    }
    
    /**
        \brief return current pause time
    */
    double totalPausedNano() { return total_paused_time_; }
    
    /**
        \brief get time in seconds
        \details returns the time between start_timer and stop_timer in seconds
        @return number of seconds
    */
    double seconds() {
        auto duration = stop_ - start_;
        exe_time_sec_ = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        double paused_seconds = total_paused_time_ / 1000000000.0;
        if(paused_seconds < 1) paused_seconds = 0;
        return exe_time_sec_ - paused_seconds;
    }

    /**
        \brief get time in milliseconds
        \details returns the time between start_timer and stop_timer in milliseconds
        @return number of milliseconds
    */
    double milliseconds() {
        auto duration = stop_ - start_;
        exe_time_ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        double paused_milliseconds = total_paused_time_ / 1000000.0;
        if(paused_milliseconds < 1) paused_milliseconds = 0;
        return exe_time_ms_ - paused_milliseconds;
    }
    
    /**
        \brief get time in microseconds
        \details returns the time between start_timer and stop_timer in microseconds
        @return number of microseconds
    */
    double microseconds() {
        auto duration = stop_ - start_;
        exe_time_micro_ = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        double paused_microseconds = total_paused_time_ / 1000.0;
        if(paused_microseconds < 1) paused_microseconds = 0;
        return exe_time_micro_ - paused_microseconds;
    }
    
    /**
        \brief get time in nanoseconds
        \details returns the time between start_timer and stop_timer in nanoseconds
        @return number of nanoseconds
    */
    double nanoseconds() {
        auto duration = stop_ - start_;
        exe_time_ns_ = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        return exe_time_ns_ - total_paused_time_;
    }
   
private:
    time_p start_;
    time_p stop_;
    time_p pause_start_;
    time_p pause_stop_;
    double total_paused_time_{0.0};
    double exe_time_sec_{};
    double exe_time_ms_{};
    double exe_time_micro_{};
    double exe_time_ns_{};
};

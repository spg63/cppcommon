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
    void start_timer() {start_ = time_device::now();}

    /**
        \brief stop the timer
    */
    void stop_timer() {stop_ = time_device::now();}
    
    /**
        \brief get time in seconds
        \details returns the time between start_timer and stop_timer in seconds
        @return number of seconds
    */
    double seconds() {
        auto duration = stop_ - start_;
        exe_time_sec_ = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        return exe_time_sec_;
    }

    /**
        \brief get time in milliseconds
        \details returns the time between start_timer and stop_timer in milliseconds
        @return number of milliseconds
    */
    double milliseconds() {
        auto duration = stop_ - start_;
        exe_time_ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        return exe_time_ms_;
    }
    
    /**
        \brief get time in microseconds
        \details returns the time between start_timer and stop_timer in microseconds
        @return number of microseconds
    */
    double microseconds() {
        auto duration = stop_ - start_;
        exe_time_micro_ = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        return exe_time_micro_;
    }
    
    /**
        \brief get time in nanoseconds
        \details returns the time between start_timer and stop_timer in nanoseconds
        @return number of nanoseconds
    */
    double nanoseconds() {
        auto duration = stop_ - start_;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        exe_time_ns_ = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        return exe_time_ns_;
    }
   
private:
    time_p start_;
    time_p stop_;
    double exe_time_sec_{};
    double exe_time_ms_{};
    double exe_time_micro_{};
    double exe_time_ns_{};
};

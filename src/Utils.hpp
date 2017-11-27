//
// Utils.hpp
// cppcommon
//
// Created by Seam Grimes on 02/08/17.
// Copyrioght @ 2017 Sean Grimes. All rights reserved.
//

#pragma once

#include <type_traits>
#include <functional>
#include <csignal>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace{
    inline void default_sig_handler_(int signum) { exit(signum); }
}

/**
 \brief Utility functions that don't fall into any other category
 \author Sean Grimes, spg63@cs.drexel.edu
 \date 02-08-16
*/
namespace Utils{
    template <class T, class U>
    inline bool isSameType(T first, U second);
    inline std::string timeStamp();
    
    void install_SIGINT(void (*f)(int) = default_sig_handler_);
    void install_SIGABRT(void (*f)(int) = default_sig_handler_);
    void install_SIGFPE(void (*f)(int) = default_sig_handler_);
    void install_SIGILL(void (*f)(int) = default_sig_handler_);
    void install_SIGSEGV(void (*f)(int) = default_sig_handler_);
    void install_SIGTERM(void (*f)(int) = default_sig_handler_);
}

/**
    \brief Determine if two variables are of the same type
    @param first The first variable 
    @param second The second variable
    @return Returns true if both variables are of the same type, false otherwise
*/
template <class T, class U>
bool Utils::isSameType(T first, U second){
    return std::is_same<T, U>::value;
}

/**
 \brief Get current (localtime) time stamp using chrono
 \details time stamp format: YEAR-MONTH-DAY HOUR:MINUTE:SECOND.MILLISECOND
 @return time stamp as a string
 */
std::string Utils::timeStamp() {
    using clock_t = std::chrono::system_clock;
    auto now = clock_t::now();
    auto count = clock_t::to_time_t(now);
    auto secs = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto partial = now - secs;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(partial);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&count), "%Y-%m-%d %X");
    oss << "." << ms.count();
    return oss.str();
}

/**
 * \brief Set function for SIGINT handler, defaults to exit(signum)
 * @param f
 */
void Utils::install_SIGINT(void (*f)(int))  { std::signal(SIGINT, f);   }

/**
 * \brief Set function for SIGABRT handler, defaults to exit(signum)
 * @param f
 */
void Utils::install_SIGABRT(void (*f)(int)) { std::signal(SIGABRT, f);  }

/**
 * \brief Set function for SIGFPR handler, defaults to exit(signum)
 * @param f
 */
void Utils::install_SIGFPE(void (*f)(int))  { std::signal(SIGFPE, f);   }

/**
 * \brief Set function for SIGILL handler, defaults to exit(signum)
 * @param f
 */
void Utils::install_SIGILL(void (*f)(int))  { std::signal(SIGILL, f);   }

/**
 * \brief Set function for SIGSEGV handler, defaults to exit(signum)
 * @param f
 */
void Utils::install_SIGSEGV(void (*f)(int)) { std::signal(SIGSEGV, f);  }

/**
 * \brief Set function for SIGTERM handler, defaults to exit(signum)
 * @param f
 */
void Utils::install_SIGTERM(void (*f)(int)) { std::signal(SIGTERM, f);  }

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


/**
 \brief Utility functions that don't fall into any other category
 \author Sean Grimes, spg63@cs.drexel.edu
 \date 02-08-16
*/
namespace Utils{
    template <class T, class U>
    inline bool isSameType(T first, U second);
    
    void default_sig_handler_(int signum);
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

void Utils::default_sig_handler_(int signum){
    exit(signum);
}

void Utils::install_SIGINT(void (*f)(int)){
    std::signal(SIGINT, f);
}

/*
 
 SIGABRT
 SIGFPE
 SIGILL
 SIGINT
 SIGSEGV
 SIGTERM
 
 
#include <iostream>
#include <csignal>

using namespace std;

void signalHandler( int signum ) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    
    // cleanup and close up stuff here
    // terminate program
    
    exit(signum);
    
}

int main () {
    int i = 0;
    // register signal SIGINT and signal handler
    signal(SIGINT, signalHandler);
    
    while(++i){
        cout << "Going to sleep...." << endl;
        
        if( i == 3 ){
            raise( SIGINT);
        }
        
        sleep(1);
    }
    
    return 0;
}
*/

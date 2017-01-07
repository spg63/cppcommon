//
//  TSLogger.hpp
//  cppcommon
//
//  Created by Sean Grimes on 12/28/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#pragma once

/** 
    \brief Use FUNC to pass function name
    \details FUNC is defined as __PRETTY_FUNCTION__ and will pass the function name to the logger
    when the logger is called. This is always an optional variable on logging fucntions
*/
#define FUNC __PRETTY_FUNCTION__

#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include "TSQueue.hpp"

/**
    \brief Log message type used by the logger
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 12-27-16
*/
struct logmessage_t{
    logmessage_t() = default;
    logmessage_t(const std::string& message_to_be_logged, const std::string &function_name,
               const std::string &type)
        : message_to_be_logged_(message_to_be_logged)
        , function_name_(function_name)
        , log_message_type_(type)
        {}
    
    std::string message_to_be_logged_;
    std::string function_name_;
    std::string log_message_type_;
};

/**
    \brief Thread safe async logger for c++
    \details Uses a single background thread, pulling from an internal queue, to write messages to 
    log file. It will automatically add timestamps to the messages. Writing to the log file is not
    buffered (outside of the queue), messages are written one at a time in an attempt to have 
    complete logs if the event of an unhandled exception. Buffering may be implmented in the future.
    \n
    FUNC is a defined macro that can be (optionally) passed to all logging functions to display the
    calling function name in the log message.
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 12-28-16
*/
class TSLogger{
public:
    /**
        \brief default c'tor
        \details By default logs are written to "log.txt" in the CWD, and the backing queue using a
        timeout of 10 milliseconds
    */
    TSLogger() : TSLogger("log.txt", std::chrono::milliseconds(10)) {}
    
    /**
        \brief custom log file c'tor
        \details Write to logfile of your choice, backing queue timeout is 10 milliseconds
        @param logFile The log file
    */
    TSLogger(std::string logFile) : TSLogger(logFile, std::chrono::milliseconds(10)) {}
    
    /**
        \brief Your choice c'tor
        \details Set log file and backing queue timeout
        @param logFile The log file
        @param queue_cond_var_time Timeout for backing queue, std::chrono::milliseconds
    */
    TSLogger(std::string logFile, std::chrono::milliseconds queue_cond_var_timeout)
        : logFile_(logFile)
        , timeout_(queue_cond_var_timeout)
        {
            consumer_ = std::thread(&TSLogger::pop_and_write, this);
        }
    
    TSLogger(const TSLogger &) = delete;
    TSLogger(const TSLogger &&) = delete;
    void operator=(const TSLogger &) = delete;
    void operator=(const TSLogger &&) = delete;
    
    ~TSLogger(){
        stop_logging_ = true;
        if(consumer_.joinable())
            consumer_.join();
#ifdef PRINT_LIB_ERRORS
        else
            fprintf(stderr, "Logger thread is not joinable\n");
#endif
    }
    
    /**
        \brief Clear log file of previous messages
    */
    void deletePreviousMessagesInLogFile(){
        std::ofstream out(logFile_, std::ios::out);
        out << "" << std::flush;
    }
    
    /**
        \brief Immediately kill logger
        \details Unwritten log messages will be lost
    */
    void kill(){
        kill_ = true;
    }
    
    /**
        \brief info messages
        @param msg The log message
        @param func_name Optional - name of function where message was logged from, passed to logger
        by using 'FUNC' macro...e.g. logger.info("message", FUNC);
    */
    template <class MSG_T>
    void info(const MSG_T &msg, const std::string &func_name = ""){
        form_and_push(msg, func_name, "INFO");
    }
    
    /**
        \brief debug messages
        @param msg The log message
        @param func_name Optional - name of function where message was logged from, passed to logger
        by using 'FUNC' macro...e.g. logger.info("message", FUNC);
    */
    template <class MSG_T>
    void debug(const MSG_T &msg, const std::string &func_name = ""){
        form_and_push(msg, func_name, "DEBUG");
    }
    
    /**
        \brief warning messages
        @param msg The log message
        @param func_name Optional - name of function where message was logged from, passed to logger
        by using 'FUNC' macro...e.g. logger.info("message", FUNC);
    */
    template <class MSG_T>
    void warn(const MSG_T &msg, const std::string &func_name = ""){
        form_and_push(msg, func_name, "WARNING");
    }
    /**
        \brief error messages
        @param msg The log message
        @param func_name Optional - name of function where message was logged from, passed to logger
        by using 'FUNC' macro...e.g. logger.info("message", FUNC);
    */
    template <class MSG_T>
    void error(const MSG_T &msg, const std::string &func_name = ""){
        form_and_push(msg, func_name, "ERROR");
    }
   
    /**
        \brief fatal messages
        @param msg The log message
        @param func_name Optional - name of function where message was logged from, passed to logger
        by using 'FUNC' macro...e.g. logger.info("message", FUNC);
    */
    template <class MSG_T>
    void fatal(const MSG_T &msg, const std::string &func_name = ""){
        form_and_push(msg, func_name, "FATAL");
    }
    
    
private:
    std::string logFile_;
    TSQueue<logmessage_t> msg_queue_;
    volatile bool stop_logging_{false};
    volatile bool kill_{false};
    std::thread consumer_;
    std::chrono::milliseconds timeout_{10};
    
    std::string timeStamp(){
        auto now = std::chrono::system_clock::now();
        auto count = std::chrono::system_clock::to_time_t(now);
        auto secs = std::chrono::time_point_cast<std::chrono::seconds>(now);
        auto partial = now - secs;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(partial);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&count), "%Y-%m-%d %X");
        ss << "." << ms.count();
        return ss.str();
    }
    
    void pop_and_write(){
        // Main loop for the logger thread, will check queue for messages and write them
        while(true){
            
            
            // Use try_and_pop instead of wait_and_pop so the thread can be immediately killed
            // if necessary; wait_and_pop will block on the thread while the queue is empty
            logmessage_t msg;
            bool did_receive_message{false};
            did_receive_message = msg_queue_.try_and_pop(msg, timeout_);
            
            // Process the received message and write it to the log file
            if(did_receive_message){
                std::string time_str = timeStamp();
                std::string fnamestr = "";
                if(msg.function_name_ != "")
                    fnamestr = msg.function_name_;
                if(fnamestr != "")
                    fnamestr += ": ";
                
                std::string type{msg.log_message_type_};
                
                std::stringstream ss;
                ss << time_str << " " << type << ": " << fnamestr;
                ss << msg.message_to_be_logged_ << '\n';
                std::ofstream out(logFile_, std::ios::out | std::ios::app);
                out << ss.str() << std::flush;
            }
            
            // stop_logging_ set to true in d'tor, keep pulling messages until queue is empty
            if(stop_logging_ && msg_queue_.empty())
                break;
            
            // kill_ allows for immediate thread death regardless of messages already in queue
            if(kill_)
                break;
        }
    }
    
    template <class MSG_T>
    void form_and_push(const MSG_T &msg, const std::string &fname, const std::string &type){
        if(stop_logging_ || kill_){
#ifdef PRINT_LIB_ERRORS
            fprintf(stderr, "Tried to log after destruction or kill order\n");
#endif
            return;
        }
        
        std::stringstream ss;
        if(!(ss << msg)){
#ifdef PRINT_LIB_ERRORS
            fprintf(stderr, "LOGGING ERROR - will attempt to log it\n");
            return error("LOGGING ERROR: ", FUNC);
#endif
            return;
        }
        msg_queue_.push({ss.str(), fname, type});
    }
};

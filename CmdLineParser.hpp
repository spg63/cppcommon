//
//  CmdLineParser.hpp
//  cppcommon
//
//  Created by Sean Grimes on 2/13/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

// Inspired by iain's answer found at the link below
// http://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c

/**
    \brief Easy command line argument parsing for c++
    \details cmdLineArgParser takes int argc, char **argv at construction and stores the flags / variables internally. Users can check if a flag exists, and if it does, get the variable associated with the flag.
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 2-13-2016
*/

class CmdLineParser{
public:

    /**
     @param argc from the commandline 
     @param argv from the commandline
    */
    CmdLineParser(int argc, const char **argv){
        for(auto i = 1; i < argc; ++i){
            // Extra string processing to deal with -wworld type flags where the variable and flag
            // are not separated by a space
            std::string arg{argv[i]};
            if(arg[0] == '-'){
                // Check if a space already exists between flag and variable
                if(arg.length() > 2 && arg[2] != ' '){
                    // Pull the flag out of the string to push onto the vector
                    std::string flag{arg.substr(0,2)};
                    // Pull the variable out of the string to push onto the vector
                    std::string var{arg.substr(2)};
                    
                    // Add the flag and the variable to the vector and continue the loop
                    args_.push_back(flag);
                    args_.push_back(var);
                    continue;
                }
            }
            args_.emplace_back(argv[i]);
        }
    }
    
    /**
     @param opt_ID Flag from the command line to search for
     @return True if flag exists, false otherwise
    */
    bool optExists(const std::string &opt_ID){
        return std::find(std::begin(args_), std::end(args_), opt_ID) != std::end(args_);
    }
    
    /**
     If the flag doesn't exist an empty string will be returned. It makes sense to check
     if a flag exists before requesting the variable associated with the flag.
     @param opt_ID Flag from the command line to search for
     @return Variable associated with flag if flag exists, otherwise ""
    */
    std::string getOptValue(const std::string &opt_ID){
        std::vector<std::string>::const_iterator it;
        it = std::find(std::begin(args_), std::end(args_), opt_ID);
        if(it != std::end(args_) && ++it != std::end(args_))
            return *it;
        return "";
    }
    
private:
    std::vector<std::string> args_;
};

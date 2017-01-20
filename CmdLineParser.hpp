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
#include <unordered_map>
#include <utility>
#include <algorithm>

// Inspired by iain's answer found at the link below
// http://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c

/**
    \brief Easy command line argument parsing for c++
    \details Users can check if a flag exists, and if it does, get the variable associated with the flag. The variable can follow the flag immediately or can be separated by a space. The flag needs to be a single dash, followed by a single character.
    \note Flags are single characters following by a dash, e.g. -d or -F
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
        // Start at 1 to avoid the program name
        for(auto i = 1; i < argc; ++i){
            std::string arg{argv[i]};   // The raw argument
            std::string flag;           // The flag with the leading dash removed
            std::string flag_char;      // The flag with the leading dash attached
            std::string var;            // The variable associated with a flag
            // Extra processing to deal with -wworld type flags where the variable and flag are not
            // separated by whitespace
            if(arg[0] == '-'){
                // Split the flag from the variable
                if(arg.length() > 2){
                    flag = arg.substr(1,1);
                    flag_char = arg.substr(0,2);
                    var = arg.substr(2);
                    
                    // Insert both lookups and the variable into the map and continue the loop
                    args_map_.insert(std::make_pair(flag, var));
                    args_map_.insert(std::make_pair(flag_char, var));
                }
                // Flag and variable are not stuck together, just process the
                else{
                    flag = arg.substr(1,1);
                    // Incremenet counter (i) to grab the next element from the argv char array
                    // Make sure to check that we're still in the proper range of argc
                    if(++i >= argc)
                        break;
                    var = argv[i];
                    // arg already represents flag_char, no need to make a copy
                    args_map_.insert(std::make_pair(flag, var));
                    args_map_.insert(std::make_pair(arg, var));
                }
            }
        }
    }
    
    /**
     \details Flag searched for by "-f" or "f", including the leading dash is optional
     @param opt_ID Flag from the command line to search for
     @return True if flag exists, false otherwise
    */
    bool optExists(const std::string &opt_ID){
        auto found = args_map_.find(opt_ID);
        if(found != args_map_.end())
            return true;
        return false;
    }
    
    /**
     \details Flag searched for by "-f" or "f", including the leading dash is optional
     If the flag doesn't exist an empty string will be returned. It makes sense to check
     if a flag exists before requesting the variable associated with the flag.
     @param opt_ID Flag from the command line to search for
     @return Variable associated with flag if flag exists, otherwise ""
    */
    std::string getOptValue(const std::string &opt_ID){
        auto found = args_map_.find(opt_ID);
        if(found != args_map_.end())
            return found->second;
        return "";
    }
    
private:
    std::unordered_map<std::string, std::string> args_map_;
};

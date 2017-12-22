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
    \details Users can check if a flag exists, and if it does, get the variable associated with the flag.
    The variable can follow the flag immediately or can be separated by a space. The flag needs to be a single dash,
    followed by a single character.
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
        // If argc == 2 then there's just a single flag, no variable associated with it
        // Place the flag in the map and move on
        if(argc == 2){
            std::string arg{argv[1]};
            // It's got the leading dash
            if(arg[0] == '-'){
                args_map_.insert(std::make_pair(arg, ""));
                args_map_.insert(std::make_pair(arg.substr(1,1), ""));
            }
            // No leading dash, add it anyway since it's a single flag-only argment list
            else{
                args_map_.insert(std::make_pair(arg, ""));
                args_map_.insert(std::make_pair("-"+arg, ""));
            }
            // Skip the rest of the function
            return;
        }

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
                // Flag and variable are not stuck together, just process
                else{
                    flag = arg.substr(1,1);
                    // Incremenet counter (i) to grab the next element from the argv char array
                    // Make sure to check that we're still in the proper range of argc
                    // If we're at the end, set var to "" to handle a flag being the last element in argv
                    if(++i < argc) {
                        var = argv[i];
                        // Undo the increment of i if the next element is another flag, let the loop counter increment
                        if(var[0] == '-') --i;
                    }
                    else
                        var = "";

                    // Check if we have another flag or if it's a variable associated with the current flag
                    if(var[0] == '-' || var == ""){
                        args_map_.insert(std::make_pair(flag, ""));
                        args_map_.insert(std::make_pair(arg, ""));
                    }
                    else {
                        // arg already represents flag_char, no need to make a copy and we know we don't have another
                        // flag so add it as a variable
                        args_map_.insert(std::make_pair(flag, var));
                        args_map_.insert(std::make_pair(arg, var));
                    }
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
     * \details Determine if a flag exists. Functionally the same as optExists but uses a better name when searching
     * for a flag that doesn't have an associated option
     * @param flag The flag to search for, "-f" or "f", dash can be included or excluded
     * @return True if the flag exists, false otherwise
     */
    bool flagExists(const std::string &flag){
        return optExists(flag);
    }

    /**
     * \details Determine if a flag exists. Assumes no leading dash will be included, takes a single character
     * @param flag The flag character
     * @return True if the flag exists, false otherwise
     */
    bool flagExists(char flag){
        return optExists(std::string(1, flag));
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

    /**
     * \details Flag searched for by character, 'f', without a leading dash
     * @param flag The flag character to be searched for
     * @return Variable associated with flag character if flag exists, otherwise ""
     */
    std::string getOptValue(char flag){
        return getOptValue(std::to_string(flag));
    }

    /**
     * Return the number of options in the argument list
     * @return the number of options
     */
    size_t numOptions(){
        // Divide by two since each flag is in twice, once with the leading dash and once without the leading dash
        return args_map_.size() / 2;
    }

    /**
     * Are there any options
     * @return true if an option exists, false if there are no options
     */
    bool anyOptions(){
        return !args_map_.empty();
    }
    
private:
    std::unordered_map<std::string, std::string> args_map_;
};

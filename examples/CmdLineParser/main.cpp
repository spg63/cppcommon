//
//  main.cpp
//  cmdLineArgParser
//
//  Created by Sean Grimes on 12/30/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#include <iostream>
#include "../../CmdLineParser.hpp"

int main(int argc, const char **argv) {
    /*
     Flags and arguments that are passed on run: 
        -h Hello
        -wworld
        -e !
        -d
     
     Expected results:
        -h flag gives the variable "Hello"
        -w flag gives the variable "world"
        -e flag gives the variable "!"
        -d flag gives the variable ""
     */
    
    // Init the parser with the cmd line variables
    CmdLineParser cmd(argc, argv);
    
    // We're looking for the -h, -w, -e flags
    std::string h_value;
    std::string w_value;
    std::string e_value;
    
    // Check if the flags exist before trying to read the values
    if(cmd.optExists("-h"))
        h_value = cmd.getOptValue("-h");
    if(cmd.optExists("-w"))
        w_value = cmd.getOptValue("-w");
    if(cmd.optExists("-e"))
        e_value = cmd.getOptValue("-e");
    if(cmd.optExists("-d")){
        std::cout << "output from getting -d, should be blank space: \"" << cmd.getOptValue("-d") << "\"\n";
        if(cmd.getOptValue("-d") != "")
            std::cout << "-d does not equal a blank space\n";
    }
    
    std::cout << h_value << " " << w_value << e_value << std::endl;

    return 0;
}

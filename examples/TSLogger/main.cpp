#include <iostream>
#include "../../src/TSLogger.hpp"

void log_from_func(){
    TSLogger l("example.log");
    l.warn("Warning from function", FUNC);
    l.error("Error from function", FUNC);
    l.fatal("Fatal from function", FUNC);
}


int main(){
    
    TSLogger l("example.log", std::chrono::milliseconds(5));

    l.info("Info message with function name", FUNC);
    l.info("Info message");
    l.debug("Debug with fucntion name", FUNC);
    l.debug("Debug message");

    log_from_func();

    l.warn("Warning from main");
    l.error("Error from main");
    l.fatal("Fatal from main");

    return 0;
}

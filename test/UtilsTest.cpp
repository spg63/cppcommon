//
// Created by Sean Grimes on 11/23/17.
//

#include <iostream>
#include "catch.hpp"
#include "../src/Utils.hpp"

// All caps is killing me
#define require REQUIRE
#define test_case TEST_CASE

static int globalSignum = 0;

void sigHandler(int signum){ globalSignum = signum; }

test_case("Signal Handlers"){
    Utils::install_SIGINT(sigHandler);
    raise(SIGINT);
    require(globalSignum == SIGINT);

    Utils::install_SIGABRT(sigHandler);
    raise(SIGABRT);
    require(globalSignum == SIGABRT);

    Utils::install_SIGFPE(sigHandler);
    raise(SIGFPE);
    require(globalSignum == SIGFPE);

    Utils::install_SIGILL(sigHandler);
    raise(SIGILL);
    require(globalSignum == SIGILL);

    Utils::install_SIGSEGV(sigHandler);
    raise(SIGSEGV);
    require(globalSignum == SIGSEGV);

    Utils::install_SIGTERM(sigHandler);
    raise(SIGTERM);
    require(globalSignum == SIGTERM);
}

test_case("timeStamp"){
    std::string str{"Not Testing This"};
    require(str == "Not Testing This");
}

test_case("isSameType"){
    int a = 0, b = 0;
    require(Utils::isSameType(a, b));

    double d = 0;
    float f = 0.0;
    require(!Utils::isSameType(d, f));

    std::vector<std::string> v;
    std::vector<int> vi;
    require(!Utils::isSameType(v, vi));

    std::vector<std::string> v2;
    require(Utils::isSameType(v, v2));
}

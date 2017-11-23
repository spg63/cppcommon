#include <thread>
#include <chrono>
#include <iostream>
#include "../../src/Timer.hpp"

int main(){
    Timer t;
    
    std::cout << "Going to sleep for ~25 milliseconds\n";

    t.startTimer();
    std::this_thread::sleep_for(std::chrono::milliseconds(25));

    t.pauseTimer();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    t.resumeTimer();

    t.stopTimer();

    std::cout << "Total time (nanoseconds): " << t.nanoseconds() << "ns\n";
    std::cout << "Total time (milliseconds): " << t.milliseconds() << "ms\n";
    std::cout << "Total time (microseconds): " << t.microseconds() << "us\n";
    std::cout << "Total time (seconds): " << t.seconds() << "s\n";
    
    std::cout << std::endl;

    return 0;
}

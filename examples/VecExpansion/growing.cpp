#include <iostream>
#include <vector>
#include "../../src/Timer.hpp"

int main() {
    
    std::vector<int> vec;
    std::cout << "vec.size(): " << vec.size() << " vec.cap(): " << vec.capacity() << '\n';

    for(int i = 0; i < 35; ++i) {
        Timer t;
        t.startTimer(); 
        vec.push_back(i);
        t.stopTimer();
        std::cout << "vec.size(): " << vec.size() << " vec.cap(): " << vec.capacity();
        std::cout << " --> " << t.nanoseconds() << "ns\n";
    }
    
    return 0;
}

#include <thread>
#include <iostream>
#include <vector>
#include "../../src/TSQueue.hpp"

const int NUM_PUSHES{25};
const int NUM_THREADS{4};

void producer(TSQueue<int> &queue);
void consumer(TSQueue<int> &queue);

int main(){
    TSQueue<int> q;
    std::vector<std::thread> producers;
    for(auto i = 0; i < NUM_THREADS; ++i)
        producers.push_back(std::thread(producer, std::ref(q)));

    std::thread con(consumer, std::ref(q));

    for(auto i = 0; i < NUM_THREADS; ++i)
        producers[i].join();
    
    con.join();

    return 0;
}

void producer(TSQueue<int> &queue){
    for(int i = 0; i < NUM_PUSHES; ++i)
        queue.push(i);
}

void consumer(TSQueue<int> &queue){
    while(!queue.empty()){
        int x;
        if(queue.try_and_pop(x))
            std::cout << x << " ";
    }
}

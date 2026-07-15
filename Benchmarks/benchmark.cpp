//
// Created by Dark on 15/07/2026.
//
#include<iostream>
#include <chrono>
#include <cstring>

int memory_allocation_speed(const int count,const int size) {
    auto start = std::chrono::steady_clock::now();          //chrono to measue the time
    for (int i = 0; i < count; i++) {
        char* buffer = new char[size];
        memset(buffer, 0, size);                                           //store something in buffer so compiler doesnt optimize or ignore buffer
        delete [] buffer;
    }
    auto end = std::chrono::steady_clock::now();           //this is where time ends
    auto duration = duration_cast<std::chrono::milliseconds>(end - start);   //use time in milliseconds
    return duration.count();
}

int main() {
    auto total_time = memory_allocation_speed(10000, 1024);
    std::cout << total_time << " ms"<< std::endl;
    return 0;
}
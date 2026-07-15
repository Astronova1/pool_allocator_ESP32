//
// Created by Dark on 15/07/2026.
//
#include<iostream>
#include <chrono>
#include <cstring>
#include <memory>

int memory_allocation_speed_make_unique_over(const int count,const int size) {
    auto start = std::chrono::steady_clock::now();          //chrono to measue the time
    for (int i = 0; i < count; i++) {
        auto buffer = std::make_unique_for_overwrite<char[]>(size);
    }
    auto end = std::chrono::steady_clock::now();           //this is where time ends
    auto duration = duration_cast<std::chrono::milliseconds>(end - start);   //use time in milliseconds
    return duration.count();
}

int memory_allocation_speed_uniq(const int count,const int size) {
    auto start = std::chrono::steady_clock::now();          //chrono to measue the time
    for (int i = 0; i < count; i++) {
       auto buffer = std::make_unique<char[]>(size);//store something in buffer so compiler doesnt optimize or ignore buffer
    }
    auto end = std::chrono::steady_clock::now();           //this is where time ends
    auto duration = duration_cast<std::chrono::milliseconds>(end - start);   //use time in milliseconds
    return duration.count();
}

int memory_allocation_speed_new_del(const int count,const int size) {
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
    auto total_time = memory_allocation_speed_new_del(210000, 1024);
    auto total_time1 = memory_allocation_speed_uniq(210000, 1024);
    auto total_time2 = memory_allocation_speed_make_unique_over(210000, 1024);
    std::cout << "Using new and delete: ";
    std::cout << total_time << " ms"<< std::endl;
    std::cout << "Using make_unique: ";
    std::cout << total_time1 << " ms"<< std::endl;
    std::cout << "Using make_unique_for_overwrite: ";
    std::cout << total_time2 << " ms"<< std::endl;
    return 0;
}
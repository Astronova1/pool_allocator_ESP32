//
// Created by Dark on 15/07/2026.
//
#include<iostream>
#include <chrono>
#include <cstring>

int memory_allocation_speed(int count, int size) {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; i++) {
        char* buffer[size];
        memset(buffer, 0, size);
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(end - start);
    return duration.count();
}

int main() {
    auto total_time = memory_allocation_speed(10000, 1024);
    std::cout << total_time << std::endl;
    return 0;
}
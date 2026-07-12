cmake_minimum_required(VERSION 4.2)
project(pool_allocator_ESP32)

set(CMAKE_CXX_STANDARD 14)

add_executable(pool_allocator_ESP32
        server.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 -static-libgcc -static-libstdc++)

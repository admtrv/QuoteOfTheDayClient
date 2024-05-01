cmake_minimum_required(VERSION 3.27)
project(qotd C)

set(CMAKE_C_STANDARD 11)

add_executable(qotd main.c)

target_link_libraries(qotd ws2_32)
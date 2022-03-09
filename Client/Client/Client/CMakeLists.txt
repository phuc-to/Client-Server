cmake_minimum_required(VERSION 3.10)
project(Client)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads)

add_executable(${PROJECT_NAME} client.cpp client.h)

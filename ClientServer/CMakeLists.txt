cmake_minimum_required(VERSION 3.21)
project(ClientServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Werror -pedantic")

add_executable(ClientServer main.cpp server.cpp client.cpp)

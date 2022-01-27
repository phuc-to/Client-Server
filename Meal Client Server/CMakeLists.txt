cmake_minimum_required(VERSION 3.21)
project(Meal_Client_Server)

set(CMAKE_CXX_STANDARD 20)

add_executable(Meal_Client_Server main.cpp client.cpp server.cpp)

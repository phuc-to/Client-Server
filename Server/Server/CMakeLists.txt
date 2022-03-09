cmake_minimum_required(VERSION 3.10)
project(Server)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads)

add_executable(${PROJECT_NAME} 
	main.cpp
	Auth.cpp
	Meal.cpp
	MealGenerator.cpp
	RPCImpl.cpp
	RPCServer.cpp
)

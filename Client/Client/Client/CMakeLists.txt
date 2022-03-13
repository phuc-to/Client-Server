# CMakeList.txt : CMake project for Server-Project, include source and define project specific logic here.
cmake_minimum_required (VERSION 3.16)

# Set project name. 
project("Client" CXX)

# Set language. 
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files to project executable. 
add_executable(${PROJECT_NAME} "client.cpp")

# Add pthread. 
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries("Client" PRIVATE Threads::Threads)



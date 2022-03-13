# CMakeList.txt : CMake project for Server-Project, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.16)

# Set the project name.
project ("Client-Server")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source to this project's executable.
add_subdirectory ("Server")
add_subdirectory ("Client")


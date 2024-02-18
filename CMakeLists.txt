cmake_minimum_required(VERSION 3.10)

project(705udpserver)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the source file(s)
add_executable(705udpserver main.cpp)

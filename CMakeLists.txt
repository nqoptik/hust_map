cmake_minimum_required(VERSION 3.1)

project(hust_map)

## Compile as C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Compile with the highest warning level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

## Declare a C++ executable
add_executable(hust_map	src/hust_map.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(hust_map glut GL GLU)

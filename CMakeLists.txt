cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(hust_map)

add_executable(hust_map	src/hust_map.cpp)
target_link_libraries(hust_map glut GL GLU)

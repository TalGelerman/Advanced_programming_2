cmake_minimum_required(VERSION 3.20)
project(ass02)

set(CMAKE_CXX_STANDARD 11)

add_executable(ass02 main.cpp Distances.cpp GenericVector.cpp Server.cpp Server.h Client.cpp Client.h)

cmake_minimum_required(VERSION 3.20)
project(ass3)

set(CMAKE_CXX_STANDARD 11)

add_executable(ass3
        main.cpp Distances.cpp GenericVector.cpp Server.cpp Server.h)
add_executable(Client
        Client.cpp Client.h)

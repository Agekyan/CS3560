cmake_minimum_required(VERSION 3.16)
project(CS3560_WSS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(wss
    src/Brain.cpp
    src/Item.cpp
    src/Trader.cpp
    src/Vision.cpp
    src/main.cpp
)

target_include_directories(wss PRIVATE include)

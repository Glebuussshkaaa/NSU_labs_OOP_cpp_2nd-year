cmake_minimum_required(VERSION 3.26)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(GameOfLife
        src/main.cpp
        src/Cell.cpp
        src/ConsoleParser.cpp
        src/Life.cpp
        src/LifeGame.cpp)

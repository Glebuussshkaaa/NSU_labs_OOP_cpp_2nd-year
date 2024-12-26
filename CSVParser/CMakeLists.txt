cmake_minimum_required(VERSION 3.26)
project(CSVParser)

set(CMAKE_CXX_STANDARD 17)

add_executable(CSVParser main.cpp
        CSVParser.h
        TupleUtility.h
        CommandParser.h
        CommandParser.cpp
)

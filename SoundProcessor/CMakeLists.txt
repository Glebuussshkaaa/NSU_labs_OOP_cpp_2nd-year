cmake_minimum_required(VERSION 3.26)
project(SoundProcessor)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(SoundProcessor src/main.cpp
        src/Converters.cpp
        src/ConfigParser.cpp
        src/CommandParser.cpp
        src/SoundProcessor.cpp
        src/WAV.cpp
)

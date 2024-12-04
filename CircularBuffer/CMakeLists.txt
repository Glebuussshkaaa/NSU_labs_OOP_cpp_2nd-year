cmake_minimum_required(VERSION 3.26)
project(circularBuffer)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(CircularBufferTest
        src/CircularBuffer.cpp
        test/CircularBufferTest.cpp)

include_directories(/opt/homebrew/Cellar/googletest/1.15.2/include)

target_link_libraries(CircularBufferTest
        /opt/homebrew/Cellar/googletest/1.15.2/lib/libgtest.a)
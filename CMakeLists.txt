cmake_minimum_required(VERSION 3.10)
project(webserver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(src/include)

file(GLOB_RECURSE SOURCES "src/*.cpp" "test/*.cpp")

add_executable(webserver ${SOURCES})


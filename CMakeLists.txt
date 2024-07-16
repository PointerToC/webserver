cmake_minimum_required(VERSION 3.10)
project(webserver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(src)
# add_subdirectory(test)

# includes
set(WEBSERVER_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
# set(WEBSERVER_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
include_directories(${WEBSERVER_SRC_INCLUDE_DIR} ${WEBSERVER_TEST_INCLUDE_DIR})

# Output the include directories
message(STATUS "Include directories:")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "  dir='${dir}'")
endforeach()

# Add a dummy target to show the compiler include paths
add_executable(dummy EXCLUDE_FROM_ALL dummy.cpp)

# Create a dummy.cpp file if it doesn't exist
file(WRITE ${CMAKE_BINARY_DIR}/dummy.cpp "// Dummy file\n")
message(STATUS "Include Path: ${WEBSERVER_SRC_INCLUDE_DIR}")

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

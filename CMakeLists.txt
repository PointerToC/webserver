cmake_minimum_required(VERSION 3.10)
project(webserver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(src)
add_subdirectory(test)

# includes
set(WEBSERVER_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(WEBSERVER_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)

include_directories(${WEBSERVER_SOURCE_INCLUDE_DIR} ${WEBSERVER_TEST_INCLUDE_DIR})

add_executable(webserver ${SOURCES})


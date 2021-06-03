cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(example1)

set(EXAMPLE_FILE "example1.cpp")

# build the library
add_executable(${PROJECT_NAME} ${EXAMPLE_FILE})

target_include_directories(${PROJECT_NAME} PRIVATE ../src/)

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin/)

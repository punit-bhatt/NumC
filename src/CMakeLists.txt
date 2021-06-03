cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MAIN)

set(NUMC_SOURCE_FILES "main.cpp")

add_executable(MAIN ${NUMC_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE .)

# (TODO)Again this is currently installing in local lib. Needs to add flag to install
# it in /usr/lib as well.
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin/)
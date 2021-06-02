cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(numc)

# include PUBLIC directories
set(NUMC_PUBLIC_INCLUDE_FILES "src/NumC.hpp")

# (TODO)Need to find a better solution for this. Works for now as folder structure is
# simple.
set(NUMC_PRIVATE_CORE_FOLDER "src/NumC/Core")
set(NUMC_PRIVATE_UTILS_FOLDER "src/NumC/Utils")

set(NUMC_SOURCE_FILES "src/main.cpp")

add_library(${PROJECT_NAME} INTERFACE)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${NUMC_PUBLIC_INCLUDE_FILES}")
set_target_properties(${PROJECT_NAME} PROPERTIES PRIVATE_HEADER "${NUMC_PRIVATE_INCLUDE_FILES}")

# (TODO)Again this is currently installing in local lib. Needs to add flag to install
# it in /usr/lib as well.
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${PROJECT_NAME})
install(DIRECTORY ${NUMC_PRIVATE_CORE_FOLDER} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${PROJECT_NAME})
install(DIRECTORY ${NUMC_PRIVATE_UTILS_FOLDER} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${PROJECT_NAME})
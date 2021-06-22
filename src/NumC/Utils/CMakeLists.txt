cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Utils)

# include PRIVATE headers
set(NUMC_UTILS_PRIVATE_INCLUDE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/StringUtils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ContainerUtils.hpp)

add_library(${PROJECT_NAME} INTERFACE)
set_target_properties(${PROJECT_NAME} PROPERTIES PRIVATE_HEADER
    "${NUMC_UTILS_PRIVATE_INCLUDE_FILES}")

# (TODO)Again this is currently installing in local lib. Needs to add flag to install
# it in /usr/local as well.
install(TARGETS ${PROJECT_NAME}
    PRIVATE_HEADER DESTINATION ${NUMC_LIB_PATH}/NumC/Utils)
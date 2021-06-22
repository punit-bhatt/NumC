cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(NumC)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

# include PUBLIC directories
set(NUMC_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(NUMC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(NUMC_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

# include all PUBLIC headers.
set(NUMC_PUBLIC_INCLUDE_FILES ${NUMC_SRC_PATH}/NumC.hpp)

# Adding at elast one cpp file.
set(NUMC_SOURCE_FILES "examples/example1.cpp")

add_library(${PROJECT_NAME} INTERFACE)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER
    "${NUMC_PUBLIC_INCLUDE_FILES}")

# Adding PRIVATE header directories.
add_subdirectory(src/NumC/Core)
add_subdirectory(src/NumC/Utils)

# (TODO)Again this is currently installing in local lib. Needs to add flag to
# install it in /usr/local as well.
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib/
    PUBLIC_HEADER DESTINATION lib/)
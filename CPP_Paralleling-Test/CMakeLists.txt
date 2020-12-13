##
## PROJECT NAME
##
cmake_minimum_required(VERSION 3.5)
project(ParallTest VERSION 1.0.0 LANGUAGES CXX)

##
## CONFIGURATION
##
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(CMAKE_CXX_STANDARD 17)
message("Use Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("Use Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# using Clang
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# using GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
# using Visual Studio C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/O2)
    add_compile_options(/MT)
    add_compile_options(/MP)
endif()

##
## TARGET
## create target and add include path
##
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/thirdParty)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/openMPTest)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/parallelSTLTest)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/app)


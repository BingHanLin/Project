# https://stackoverflow.com/questions/54094731/building-tbb-using-visual-studio-2015-x64-and-cmake
# https://www.selectiveintellect.net/blog/2016/7/29/using-cmake-to-add-third-party-libraries-to-your-project-1
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
# set(CMAKE_CXX_FLAGS " -I D:\\msys32\\mingw64\\include -L D:\\msys32\\mingw64\\lib")

set(CMAKE_CXX_STANDARD 17)
message("Use Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("Use Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
# set(ENV{PATH} "D:\\msys32\\mingw64\\bin")
# message(">>>> PATH $ENV{PATH}")
# message(">>>> CMAKE_MAKE_PROGRAM ${CMAKE_MAKE_PROGRAM}")

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

# link_directories( ${CMAKE_SOURCE_DIR}/tbb_20202/build/windows_intel64_gcc_mingw9.2.0_release)

##
## TARGET
## create target and add include path
##
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/thirdParty)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/openMPTest)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/parallelSTLTest)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/app)


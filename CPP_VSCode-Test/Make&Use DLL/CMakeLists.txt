cmake_minimum_required(VERSION 3.18.0)

project(client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/archive)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/client/main.cpp
)

target_include_directories(${PROJECT_NAME} 
  PUBLIC
    ${PROJECT_SOURCE_DIR}/exampleDLL
)

target_link_libraries(${PROJECT_NAME} 
  PUBLIC
    ${PROJECT_SOURCE_DIR}/exampleDLL/build/archive/libexample.lib
)


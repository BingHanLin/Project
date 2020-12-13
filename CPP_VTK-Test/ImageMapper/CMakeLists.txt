cmake_minimum_required(VERSION 3.0)

PROJECT(ImageMapper)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(${CMAKE_PROJECT_NAME} ImageMapper.cxx)

target_link_libraries(${CMAKE_PROJECT_NAME} ${VTK_LIBRARIES})
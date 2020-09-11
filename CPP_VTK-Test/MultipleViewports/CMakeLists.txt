cmake_minimum_required(VERSION 3.0)

PROJECT(MultipleViewports)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(MultipleViewports MultipleViewports.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(MultipleViewports ${VTK_LIBRARIES})
else()
  target_link_libraries(MultipleViewports vtkHybrid vtkWidgets)
endif()
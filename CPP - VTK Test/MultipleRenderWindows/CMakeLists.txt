cmake_minimum_required(VERSION 3.0)

PROJECT(MultipleRenderWindows)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(MultipleRenderWindows MultipleRenderWindows.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(MultipleRenderWindows ${VTK_LIBRARIES})
else()
  target_link_libraries(MultipleRenderWindows vtkHybrid vtkWidgets)
endif()
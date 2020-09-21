cmake_minimum_required(VERSION 3.18)

project(meshQualityVisualization)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message("Skipping meshQualityVisualization: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(meshQualityVisualization meshQualityVisualization.cxx )
  target_link_libraries(meshQualityVisualization PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(meshQualityVisualization meshQualityVisualization.cxx )
  target_link_libraries(meshQualityVisualization PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS meshQualityVisualization
    MODULES ${VTK_LIBRARIES}
    )
endif ()

# install (FILES ${PROJECT_SOURCE_DIR}/testGrid.vtk
#     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

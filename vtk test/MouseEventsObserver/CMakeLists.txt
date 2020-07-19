cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(MouseEventsObserver)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkFiltersSources
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2 QUIET)
if (NOT VTK_FOUND)
  message("Skipping MouseEventsObserver: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(MouseEventsObserver MACOSX_BUNDLE MouseEventsObserver.cxx )
  target_link_libraries(MouseEventsObserver PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(MouseEventsObserver MACOSX_BUNDLE MouseEventsObserver.cxx )
  target_link_libraries(MouseEventsObserver PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS MouseEventsObserver
    MODULES ${VTK_LIBRARIES}
    )
endif () 
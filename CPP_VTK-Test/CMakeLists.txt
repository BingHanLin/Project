cmake_minimum_required(VERSION 3.18)

project(main)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message("VTK NOT FOUND: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()

message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(${CMAKE_PROJECT_NAME} ${TARGET_DIR}/main.cxx )
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(${CMAKE_PROJECT_NAME} ${TARGET_DIR}/main.cxx )
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS ${CMAKE_PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
    )
endif ()

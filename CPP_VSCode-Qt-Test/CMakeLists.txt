cmake_minimum_required(VERSION 3.0)

project(calForm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS Core Widgets Gui OpenGL Network Concurrent REQUIRED)

add_executable(shonDy
    src/main.cpp
    src/calculatorform.cpp 
    src/calculatorform.h
    src/calculatorform.ui 
)

target_include_directories(
    shonDy
        src
)

target_link_libraries(
shonDy
    Qt5::Widgets 
    Qt5::Network 
    Qt5::OpenGL
)

# execute windeployqt.exe
if(WIN32)
  set(WINDEPLOYQT_EXECUTABLE "${Qt5_BIN_DIR}/windeployqt.exe")
  message(" >> WINDEPLOYQT_EXECUTABLE")
  message(" WINDEPLOYQT_EXECUTABLE　>> ${WINDEPLOYQT_EXECUTABLE}")
  
  add_custom_command(TARGET shonDy
  POST_BUILD
          COMMAND set PATH="${Qt5_BIN_DIR}"
          COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:shonDy>"
  )
endif()

if(WIN32)
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
  include(InstallRequiredSystemLibraries)
  if(CMAKE_BUILD_TYPE MATCHES Debug)
    install(TARGETS shonDy  
      DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
    INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} 
      DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
  elseif(CMAKE_BUILD_TYPE MATCHES Release)
    install(TARGETS shonDy 
      DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
    INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} 
      DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
  endif()
endif()


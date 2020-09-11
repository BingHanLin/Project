project (thirdParty)

# ===========================================================
# openMP
# ===========================================================
find_package(OpenMP REQUIRED)
message(">> Found OpenMP ${OpenMP_FOUND} ${OpenMP_VERSION}")

add_library(${PROJECT_NAME}_openMP INTERFACE)
target_link_libraries(${PROJECT_NAME}_openMP INTERFACE OpenMP::OpenMP_CXX)

# https://stackoverflow.com/questions/12399422/how-to-set-linker-flags-for-openmp-in-cmakes-try-compile-function
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")


# ===========================================================
# tbb
# ===========================================================
# using Clang
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# using GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(">> ${CMAKE_SOURCE_DIR}/tbb_20202/build/windows_intel64_gcc_mingw9.2.0_release")
 
    # include_directories(${CMAKE_SOURCE_DIR}/tbb_2020/include)
    # include(${CMAKE_SOURCE_DIR}/tbb_2020/cmake/TBBBuild.cmake)
    # tbb_build(TBB_ROOT ${CMAKE_SOURCE_DIR}/tbb_2020 CONFIG_DIR TBB_DIR  MAKE_ARGS tbb_build_dir=${CMAKE_SOURCE_DIR}/bin tbb_build_prefix=tbb)
    # find_package(TBB REQUIRED)
    message(">> ${TBB_FOUND} ${TBB_VERSION}")

# using Visual Studio C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # include_directories(${PROJECT_SOURCE_DIR}/tbb_win/tbb/include)
    # set(TBB_DIR ${PROJECT_SOURCE_DIR}/tbb_win/tbb/cmake)
    # find_package(TBB)  
    # message(">> ${TBB_FOUND} ${TBB_VERSION}")
endif()

add_library(${PROJECT_NAME}_tbb INTERFACE )

# using Clang
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# using GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("CMAKE_SOURCE_DIR>>>>> ${CMAKE_SOURCE_DIR}")    
    find_library(TBB_LIBRARY tbb HINT ${CMAKE_SOURCE_DIR}/tbb_20202/build/windows_intel64_gcc_mingw9.2.0_release)
    message("${TBB_LIBRARY}")    
    
    # target_link_libraries(${PROJECT_NAME}_tbb INTERFACE ${TBB_LIBRARY})
    # link_directories(${CMAKE_SOURCE_DIR}/tbb_20202/build/windows_intel64_gcc_mingw9.2.0_release)
    target_link_libraries(${PROJECT_NAME}_tbb INTERFACE ${TBB_LIBRARY} )
    
    # target_include_directories(
    #     ${PROJECT_NAME}_tbb INTERFACE
        # ${CMAKE_SOURCE_DIR}/tbb_20202/include)
    
    # target_link_libraries(${PROJECT_NAME} INTERFACE ${TBB_IMPORTED_TARGETS})
# using Visual Studio C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # target_link_libraries(${PROJECT_NAME} PUBLIC ${TBB_IMPORTED_TARGETS})
endif()

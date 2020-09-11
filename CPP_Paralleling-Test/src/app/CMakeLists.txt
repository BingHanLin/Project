project(MYAPP)

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

aux_source_directory(. DIR_MAIN_SRCS)
add_executable(${PROJECT_NAME} ${DIR_MAIN_SRCS})

target_link_libraries(${PROJECT_NAME}
openMPTest
parallelSTLTest
)


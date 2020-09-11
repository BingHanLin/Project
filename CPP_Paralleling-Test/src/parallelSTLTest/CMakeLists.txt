project (parallelSTLTest)

# Add a library with the above sources
aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})

target_include_directories( ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} thirdParty_tbb)

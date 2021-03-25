project (thirdParty)

# ===========================================================
# openMP
# ===========================================================
find_package(OpenMP REQUIRED)
message(">> Found OpenMP ${OpenMP_FOUND} ${OpenMP_VERSION}")

add_library(${PROJECT_NAME}_openMP INTERFACE)
target_link_libraries(${PROJECT_NAME}_openMP INTERFACE OpenMP::OpenMP_CXX)
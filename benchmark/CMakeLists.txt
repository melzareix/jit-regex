cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(ZRegexStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

#re2
CPMAddPackage(
        NAME re2
        GIT_TAG 2021-11-01
        GITHUB_REPOSITORY google/re2
        OPTIONS "RE2_BUILD_TESTING NO"
)
#micro-benchmark
CPMAddPackage(
        NAME benchmark
        GIT_TAG v1.6.0
        GITHUB_REPOSITORY google/benchmark
        OPTIONS "BENCHMARK_ENABLE_TESTING NO"
)

CPMAddPackage(NAME ZRegex SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "ZRegex_BM")

target_link_libraries(${PROJECT_NAME} benchmark)
target_link_libraries(${PROJECT_NAME} re2)

target_link_libraries(${PROJECT_NAME} ZRegex::ZRegex)

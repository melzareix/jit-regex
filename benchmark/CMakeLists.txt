cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(ZRegexStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----
include(../cmake/deps/findPCRE2.cmake)
message(STATUS "PCRE2_LIBRARIES = ${PCRE2_LIBRARIES}")
message(STATUS "PCRE2_INCLUDE_DIRS = ${PCRE2_INCLUDE_DIRS}")

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

#boost-regex
CPMAddPackage(
  NAME boost
  VERSION 1.78.0
  GITHUB_REPOSITORY "boostorg/regex"
  GIT_TAG "boost-1.78.0"
  OPTIONS "BOOST_REGEX_STANDALONE TRUE"
)

CPMAddPackage(NAME ZRegex SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

INCLUDE(../cmake/sse/FindSSE.cmake)
FindSSE()
IF(SSE3_FOUND)
    IF(SSSE3_FOUND)
            SET(CXX_DFLAGS -msse3 -mssse3)
    ENDIF(SSSE3_FOUND)
ENDIF(SSE3_FOUND)

IF(SSE4_1_FOUND)
        message("found sse4_2")
        SET(CMAKE_C_FLAGS -march=native)
        SET(CXX_DFLAGS -march=native)
ENDIF(SSE4_1_FOUND)
ADD_DEFINITIONS(${CXX_DFLAGS})

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "ZRegex_BM")

target_link_libraries(${PROJECT_NAME} boost_regex)
target_link_libraries(${PROJECT_NAME} benchmark)
target_link_libraries(${PROJECT_NAME} re2)
# target_link_libraries(${PROJECT_NAME} pcre2-8)
# message(STATUS "jpcre2_SOURCE_DIR = ${jpcre2_SOURCE_DIR}")
# message(STATUS "jpcre2_ADDED = ${jpcre2_ADDED}")
# target_link_libraries(${PROJECT_NAME} jpcre2)
# target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE "${jpcre2_SOURCE_DIR}/src")
# target_link_libraries(${PROJECT_NAME} ctre)

target_link_libraries(${PROJECT_NAME} ${PCRE2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PCRE2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ZRegex::ZRegex)

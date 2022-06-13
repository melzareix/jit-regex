cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(ZRegexStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 2.2.1
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(NAME ZRegex SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

INCLUDE(../cmake/sse/FindSSE.cmake)
FindSSE()
IF(SSE3_FOUND)
    IF(SSSE3_FOUND)
            SET(CXX_DFLAGS -msse3 -mssse3 -mavx)
    ENDIF(SSSE3_FOUND)
ENDIF(SSE3_FOUND)

IF(SSE4_2_FOUND)
        message("found sse4_2")
        SET(CMAKE_C_FLAGS -march=native)
        # SET(CXX_DFLAGS -msse4.2 -mpopcnt -mavx -madx)
        SET(CXX_DFLAGS -march=native)
ENDIF(SSE4_2_FOUND)
ADD_DEFINITIONS(${CXX_DFLAGS})

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "ZRegex")
target_link_libraries(${PROJECT_NAME} ZRegex::ZRegex cxxopts)

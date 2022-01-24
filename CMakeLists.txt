cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

# Note: update this to your new project's name and version
project(
        ZRegex
        VERSION 1.0
        LANGUAGES CXX
)

# ---- Include guards ----

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

# Google Re2 as fallback
#CPMAddPackage(
#        NAME re2
#        GIT_TAG 2021-11-01
#        GITHUB_REPOSITORY google/re2
#)

#CPMAddPackage(
#        NAME fmt
#        GIT_TAG 7.1.3
#        GITHUB_REPOSITORY fmtlib/fmt
#        OPTIONS "FMT_INSTALL YES" # create an installable target
#)

# logging
CPMAddPackage(
        NAME spdlog
        GIT_TAG v1.9.2
        GITHUB_REPOSITORY gabime/spdlog
        OPTIONS "SPDLOG_INSTALL YES" # create an installable target
)

# json
#CPMAddPackage("gh:nlohmann/json@3.9.1")

#micro-benchmark
#CPMAddPackage(
#        NAME benchmark
#        GIT_TAG v1.6.0
#        GITHUB_REPOSITORY google/benchmark
#        OPTIONS "BENCHMARK_ENABLE_TESTING NO"
#)

set(CXX_STANDARD 17)
#antlr4 parser
add_definitions(-DANTLR4CPP_STATIC)
# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(cmake/antlr/ExternalAntlr4Cpp.cmake)
message("ANTLR_ROOT ${ANTLR4_ROOT}")
message("ANTLR4_INCLUDE_DIRS ${ANTLR4_INCLUDE_DIRS}")
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})
message("include_dirss ${ANTLR4_OUTPUT_DIR}")
set(ANTLR_EXECUTABLE /usr/local/Cellar/antlr/4.9.3/antlr-4.9.3-complete.jar)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/antlr/")
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
message("prr ${PROJECT_BINARY_DIR}")
set(GRAMMAR_SRC ${PROJECT_SOURCE_DIR}/include/parser/grammar)
antlr_target(RegexGrammarParser ${PROJECT_SOURCE_DIR}/include/parser/grammar/regex.g4 VISITOR OUTPUT_DIRECTORY ${GRAMMAR_SRC})
include_directories(${PROJECT_SOURCE_DIR} ${GRAMMAR_SRC})
message("RegexGrammarParser ${ANTLR_RegexGrammarParser_CXX_OUTPUTS}")
# llvm
if (APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/llvm@12/lib/cmake")
endif (APPLE)

find_package(LLVM 12 REQUIRED CONFIG)
message(STATUS "LLVM_INCLUDE_DIRS = ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_INSTALL_PREFIX = ${LLVM_INSTALL_PREFIX}")

add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(LLVM_LIBS core support mcjit x86codegen OrcJIT)
message(STATUS "LLVMLIBS: ${LLVM_LIBS}")

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME} ${headers} ${sources} ${ANTLR_RegexGrammarParser_CXX_OUTPUTS})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Link dependencies
#target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
#target_link_libraries(${PROJECT_NAME} PUBLIC re2)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog)
#target_link_libraries(${PROJECT_NAME} PUBLIC benchmark)
#target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LLVM_LIBS})
target_link_libraries(${PROJECT_NAME} PUBLIC antlr4_static)

message("ANTLR4_STATIC_LIBRARIES ${ANTLR4_STATIC_LIBRARIES}")

target_include_directories(
        ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
target_include_directories(
        ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${ANTLR4_INCLUDE_DIRS}>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
target_include_directories(${PROJECT_NAME} PUBLIC ${LLVM_INCLUDE_DIRS})
# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
        NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        NAMESPACE ${PROJECT_NAME}
        BINARY_DIR ${PROJECT_BINARY_DIR}
        INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
        INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
        VERSION_HEADER "${VERSION_HEADER_LOCATION}"
        COMPATIBILITY SameMajorVersion
        DEPENDENCIES "fmt 7.1.3"
)

cmake_minimum_required (VERSION 2.8.11)

project (poorMansDi)

option (BUILD_TESTS "Build tests" ON)
option (BUILD_EXAMPLE "Build example" ON)
option (OPTIMISE_COMPILATION "Enable O3 optimisation instead O0" OFF)

if(OPTIMISE_COMPILATION)
    set(OPTIMISATION -O3)
else()
    set(OPTIMISATION -O0)
endif()

if(BUILD_TESTS)
    add_definitions(-DBUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})
message("Look for modules in ${CMAKE_MODULE_PATH}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(-D__STRICT_ANSI__)
    set(CMAKE_CXX_FLAGS "${CwMAKE_CXX_FLAGS} -Qunused-arguments")
endif()

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -g ${OPTIMISATION}")

message(STATUS "Compiling with ${CMAKE_CXX_FLAGS}")

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    add_definitions (-fcolor-diagnostics -fansi-escape-codes)
endif()


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)


add_subdirectory (src)

if(BUILD_TESTS)
    add_subdirectory (tests)
endif()
if(BUILD_EXAMPLE)
    add_subdirectory (example)
endif()

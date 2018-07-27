cmake_minimum_required(VERSION 3.10)
project(mustard)

set(CMAKE_CXX_STANDARD 11)

include_directories(lib/pistache/include)
include_directories(lib/poorMansDi/src)
add_subdirectory(lib/pistache)

set (SOURCES
        src/main/system/CommandRunner.cpp src/main/system/CommandRunner.h)

add_executable(mustard lib/poorMansDi/src/DependencyContainer.cpp ${SOURCES} src/main/main.cpp)
target_link_libraries(mustard pistache)

set (TEST_SOURCES
        ${SOURCES}
)

add_executable(mustard_tests lib/poorMansDi/src/DependencyContainer.cpp ${TEST_SOURCES} src/test/main.cpp)
target_link_libraries(mustard_tests pistache gtest)


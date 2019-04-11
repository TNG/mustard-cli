include_directories(
        ${CMAKE_INCLUDE_PATH}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/external/gmock-1.7.0/
        ${CMAKE_SOURCE_DIR}/external/gtest-1.7.0/
)

file(GLOB TEST_SOURCES
        "../src/*.cpp"
        "Test*.cpp"
        "InitTests.cpp"
        )
if(CODE_COVERAGE)
    file(GLOB EXTENDED_SOURCES
	"../src/*.h"
        "../src/*.cpp"
        "Test*.cpp"
        "InitTests.cpp"
            )
endif()
list(APPEND TEST_SOURCES "AllTests.cpp")
add_executable (poorMansDi-tests ${TEST_SOURCES} ${MOCS})
target_link_libraries(poorMansDi-tests ${GTEST_BOTH_LIBRARIES} pthread)

IF (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(poorMansDi-tests -lrt ${GTEST_BOTH_LIBRARIES} pthread)
ENDIF (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")


file(GLOB TEST_SOURCES
        "../src/*.cpp"
        "Test*.cpp"
        "InitTests.cpp"
        )
list(APPEND TEST_SOURCES "UnitTests.cpp")
add_executable (poorMansDi-unittests ${TEST_SOURCES} ${MOCS})
target_link_libraries(poorMansDi-unittests ${GTEST_BOTH_LIBRARIES} pthread )

IF (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(poorMansDi-unittests -lrt ${GTEST_BOTH_LIBRARIES} pthread )
ENDIF (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
file(GLOB TEST_SOURCES
        "../src/*.cpp"
        "Test*.cpp"
        "InitTests.cpp"
        )
list(APPEND TEST_SOURCES "IntegrationTests.cpp")
add_executable (poorMansDi-integrationtests ${TEST_SOURCES} ${MOCS})
target_link_libraries(poorMansDi-integrationtests ${GTEST_BOTH_LIBRARIES} )
IF (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(poorMansDi-integrationtests -lrt ${GTEST_BOTH_LIBRARIES} pthread)
ENDIF (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")

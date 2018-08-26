include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../gtest-1.7.0/
)

add_library (gmock gmock-all.cc)
target_link_libraries (gmock)

set(target math)
add_library(${target}
	binary.h
	binary_phase.h
	graph.h
	graph.cpp
	matrix.h
)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})


add_unit_test(${target}_unit_tests
	SOURCES 
		tests/graph_tests.cpp
		tests/matrix_tests.cpp
	DEPENDENCIES
		${target}
)

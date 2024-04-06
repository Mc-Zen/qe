set(target math)
add_library(${target}
	binary.h
	binary_phase.h
	graph.h
	graph.cpp
	matrix.h
)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(${target} PROPERTIES FOLDER ${QE_LIBRARY_IDE_FOLDER})


add_unit_test(${target}_unit_tests
	SOURCES 
		tests/graph_tests.cpp
		tests/matrix_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)

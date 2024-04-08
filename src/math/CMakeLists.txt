set(target math)

add_qe_library(${target}
	binary.h
	binary_phase.h
	graph.h
	graph.cpp
	matrix.h
)


add_unit_test(${target}_unit_tests
	SOURCES 
		tests/graph_tests.cpp
		tests/matrix_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)

set(target math)

add_qe_library(${target}
	binary.h
	binary_phase.h
	graph.h
	graph.cpp
	matrix.h
	format_binary.h
	format_binary_phase.h
	format_matrix.h
	format_math.h
)

target_link_libraries(${target} PUBLIC fmt)

add_unit_test(${target}_unit_tests
	SOURCES 
		tests/binary_tests.cpp
		tests/binary_phase_tests.cpp
		tests/graph_tests.cpp
		tests/matrix_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)

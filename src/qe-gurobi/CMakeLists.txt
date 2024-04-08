set(target qe-gurobi)

add_qe_library(${target}
	solve.h
	solve.cpp
)
target_link_libraries(${target} PUBLIC gurobi_cpp)

add_unit_test(${target}_unit_tests
	SOURCES 
		tests/solve_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)

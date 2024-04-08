set(target base)

add_qe_library(${target}
	circuit.h
	pauli.h
	circuit.cpp
)


add_unit_test(${target}_unit_tests
	SOURCES 
		tests/circuit_tests.cpp
		tests/pauli_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)

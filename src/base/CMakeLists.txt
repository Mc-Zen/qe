set(target base)
add_library(${target}
	circuit.h
	pauli.h
	circuit.cpp
)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_unit_test(${target}_unit_tests
	SOURCES 
		tests/circuit_tests.cpp
		tests/pauli_tests.cpp
	DEPENDENCIES
		${target}
)

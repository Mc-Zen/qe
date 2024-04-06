set(target base)
add_library(${target}
	circuit.h
	pauli.h
	circuit.cpp
)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(${target} PROPERTIES FOLDER ${QE_LIBRARY_IDE_FOLDER})


add_unit_test(${target}_unit_tests
	SOURCES 
		tests/circuit_tests.cpp
		tests/pauli_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)

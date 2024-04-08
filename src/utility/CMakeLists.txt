
set(target utility)

add_qe_library(${target}
	string_utility.h
	string_utility.cpp
)


add_unit_test(${target}_unit_tests
	SOURCES 
		tests/string_utility_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)

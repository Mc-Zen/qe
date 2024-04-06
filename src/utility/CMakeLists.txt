
set(target utility)
add_library(${target}
	string_utility.h
	string_utility.cpp
)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${target} PROPERTIES FOLDER ${QE_LIBRARY_IDE_FOLDER})


add_unit_test(${target}_unit_tests
	SOURCES 
		tests/string_utility_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)

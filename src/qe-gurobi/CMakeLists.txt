set(target qe-gurobi)
add_library(${target}
	solve.h
	solve.cpp

)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(${target} PROPERTIES FOLDER ${QE_LIBRARY_IDE_FOLDER})
target_link_libraries(${target} PUBLIC gurobi_cpp)

add_unit_test(${target}_unit_tests
	SOURCES 
		tests/solve_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)

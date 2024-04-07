add_subdirectory(math)
add_subdirectory(base)
add_subdirectory(utility)

if (QE_ENABLE_GUROBI) 
	add_subdirectory(qe-gurobi)
endif()

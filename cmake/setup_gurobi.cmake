
find_package(GUROBI REQUIRED)
set(target gurobi_cpp)
add_library(${target} INTERFACE)
target_link_libraries(${target} INTERFACE optimized ${GUROBI_CXX_LIBRARY} debug ${GUROBI_CXX_DEBUG_LIBRARY})
target_link_libraries(${target} INTERFACE ${GUROBI_LIBRARY})
target_include_directories(${target} INTERFACE ${GUROBI_INCLUDE_DIRS})
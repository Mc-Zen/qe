cmake_minimum_required(VERSION 3.19)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


project(qe)


# Setup gurobi

# set(GUROBI_PATH "C:/gurobi1000/win64")
# set(target gurobi_c++)
# add_library(${target} INTERFACE)
# target_link_libraries(${target} INTERFACE debug "${GUROBI_PATH}/lib/gurobi_c++mdd2017.lib" optimized "${GUROBI_PATH}/lib/gurobi_c++md2017.lib")
# target_link_libraries(${target} INTERFACE "${GUROBI_PATH}/lib/gurobi100.lib")
# target_include_directories(${target} INTERFACE "${GUROBI_PATH}/include")


include(cmake/add_unit_test.cmake)

set(QE_LIBRARY_IDE_FOLDER "QE")
set(QE_LIBRARY_UNIT_TESTS_IDE_FOLDER "${QE_LIBRARY_IDE_FOLDER}/Unit Tests")


if(PROJECT_IS_TOP_LEVEL)
	option(UNIT_TESTING "Enable unit tests for this project" OFF)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

if (UNIT_TESTING)
	find_package(Catch2 REQUIRED)
	enable_testing()
	include(Catch)
endif()

add_subdirectory(src)

set(target qe)
add_library(${target} INTERFACE)
target_link_libraries(${target} INTERFACE math base utility)
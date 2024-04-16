cmake_minimum_required(VERSION 3.19)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)





project(qe)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(cmake/add_unit_test.cmake)
include(cmake/add_qe_library.cmake)


set(QE_LIBRARY_IDE_FOLDER "QE")
set(QE_LIBRARY_UNIT_TESTS_IDE_FOLDER "${QE_LIBRARY_IDE_FOLDER}/Unit Tests")

if(PROJECT_IS_TOP_LEVEL)
	option(UNIT_TESTING "Enable unit tests for this project" OFF)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()


option(QE_ENABLE_GUROBI "Add and build libraries that depend on Gurobi" OFF)
if (QE_ENABLE_GUROBI)
	include(cmake/setup_gurobi.cmake)
endif()


if (UNIT_TESTING)
	find_package(Catch2 REQUIRED)
	enable_testing()
	include(Catch)
endif()

add_subdirectory(libs)
add_subdirectory(src)

set(target qe)
add_library(${target} INTERFACE)
target_link_libraries(${target} INTERFACE math base utility)
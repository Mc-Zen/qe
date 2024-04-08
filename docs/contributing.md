# Contributing to QE

We recommend posting an issue before starting significant work on a pull request, so we can discuss the changes and avoid large pull requests that cannot be merged. 



## Unit tests

Every library feature should be tested with one or more unit tests. This makes it especially possible to ensure 
- compatibility across different compilers and operating systems and
- that future changes don't break the functionailty. 

Unit tests are instrumented with [Catch2](https://github.com/catchorg/Catch2), a testing framework for C++. In order to add unit tests to a sub project, add test `.cpp` files into a `tests` folder within the library's directory and add the following to the projects `CMakeLists.txt`:
```cmake
add_unit_test(${target}_unit_tests
	SOURCES 
		tests/my_tests.cpp
	DEPENDENCIES
		${target}
	FOLDER
		${QE_LIBRARY_UNIT_TESTS_IDE_FOLDER}
)
```
Prepending the library's name with `${target}` is a good idea to match libraries and their unit tests. Test files should be roughly per `.cpp` source file and match their name suffixed with `"_tests"`. In general, each source file should have a test file (except where similar files can be tested more easily in one unit).


## Adding a new library

The individual **QE** libraries live in the `/src` directory. They are comprised of a `CMakeLists.txt` file, a number of C++ header and source files as well as a test directory. You can take a look at the project in [src/math/](/src/math/) for an example on how to set up the `CMakeLists.txt` file. When adding a new library, it is also necessary to add a corresponding `add_subdirectory(..)` entry to the `CMakeLists.txt` file living in the `/src` directory. 

Projects that use Gurobi may link to the project `gurobi_cpp`, i.e.,
```cmake
target_link_libraries(${target} PUBLIC gurobi_cpp)
```
Also see [src/qe-gurobi/](/src/qe-gurobi/) for an example. This will add the necessary dependencies and include directories to the target. 
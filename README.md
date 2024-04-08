# QE - Quantum Computing in C++
[![CI](https://github.com/Mc-Zen/qe/actions/workflows/ci.yml/badge.svg)](https://github.com/Mc-Zen/qe/actions/workflows/ci.yml)

You need it fast? Welcome. 

Here we start gathering some useful classes and algorithms for computing quantum-info related things. 

- **Quantum Circuit**. Generation, iteration, depth-computation. 
- **Matrices**. Dense, fixed-size and dynamic matrices, manipulation, matrix views. 
- **Graphs**. Graph templates, local complementation, transformations. 
- **Binary**. $\mathbb{F}_2$ numbers, binary phases (mod 4). 
- **Pauli**. 


## Installation/Setup

The project uses CMake and can be built without any dependencies or integration into other projects. 
However, it can also be integrated as a submodule into another CMake-based project. 
This will prevent this library's `CMakeLists.txt` file to search for Catch2 (see below), to enable `USE_FOLDERS` and to remove the option for unit testing which will then need to be given by the top-level project. 

### Requirements
- CMake (for setup)
- C++20 or newer
- (optional) Gurobi 10.0.0 or newer. Necessary for some projects. Note that Gurobi is proprietary software. 

### Supported Platforms
Since this repository uses C++20 language features, a fairly new C++ compiler is necessary. 

|Platform | Compiler   | Minimum version |
|---------|------------|-----------------|
|Windows  | MSVC       | 14.30           |
|Linux    | GCC        | 13              |
|         | Clang      | 15              |
|MacOS    | AppleClang | 15              |


### Setting up Gurobi (optional)
Gurobi is a numerical solver that is used in some of the projects of **QE**. In order to enable these projects, set `QE_ENABLE_GUROBI` to `ON` in the CMake GUI or run CMAKE wih the flag `-DQE_ENABLE_GUROBI=ON` when building the project. Ensure to have Gurobi installed and licensed. When installed in the default place, the CMake script should be able to find Gurobi automatically. If not, contact us (for now). 


## Contributing

Contributions to the codebase are welcome. Please look into the [Contributing Guide](/docs/contributing.md) and feel free to open a pull request. 

## Tests

This library features unit tests with [Catch2](https://github.com/catchorg/Catch2). 
The unit tests can be built by setting `UNIT_TESTING=ON` when running CMake or running CMake with the flag `-DUNIT_TESTING=ON`. 
Ensure that you have Catch2 installed and that it can be found in CMake by `find_package(Catch2 3 REQUIRED)`. 
If folders in the IDE are enabled, the tests are put into a folder named `Unit Tests`. 

See [here](docs/contributing.md#unit-tests) for more information on how to add unit tests. 

## License

This repository is licensed under the [MIT License][license]

[repository]: https://github.com/Mc-Zen/qe
[license]: https://github.com/Mc-Zen/qe/blob/main/LICENSE.txt
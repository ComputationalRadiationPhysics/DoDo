DoDo - Specification of HPC hardware and program components to enable further optimized mappings
================================================================================================

Introduction
------------


Installation/Compilation
------------------------

### Dependencies
 - C++14 compatible compiler
   - `gcc` >= 4.9
   - `clang` >= 3.5
 - `boost` >= 1.58
   - compile time headers
   - `boost::filesystem`
   - `boost::program_options` (optional, for some examples)
 - `CMake` >= 3.3
 - `git` >= 1.7.9.5 (optional)
 - OpenMPI >= 1.8 (optional, for some examples)

### Installation
This is an example how to compile DoDo to test the example code snippets and shows to install DoDo in a custom directory.
If you want a system-wide installation, you can change the `CMAKE_INSTALL_PREFIX` to something like `/usr`, which will install DoDo in `/usr/include/dodo`

1. **Download the source code:**
 -  `git clone https://github.com/ComputationalRadiationPhysics/DoDo.git`
2. **Setup directories:**
 - `mkdir -p build`
 - `cd build`
 - `cmake ../DoDo -DCMAKE_INSTALL_PREFIX=$HOME/libs`
3. **Build Examples (optional)**
 - `make`
4. **Install**
 - `make install`


Linking to your Project
-----------------------

To use DoDo in your project, you must include the header `dodo.hpp` and
add the correct include path.

The easiest way for CMake-based projects is to use the following lines in your `CMakeLists.txt`:
```cmake
# find mallocMC installation
find_package(Dodo REQUIRED)

# where to find headers (-I includes for compiler)
include_directories(SYSTEM ${Dodo_INCLUDE_DIRS})
```

************************************************************************

Documentation
-------------

Documentation is available at **TODO: Github Pages**

You can also build your own documentation through the `make doc` target.


Literature
----------

- Diploma thesis [![DOI](https://zenodo.org/badge/doi/10.5281/zenodo.163329.svg)](http://dx.doi.org/10.5281/zenodo.163329) by Carlchristian Eckert (2016)


Branches
--------

Integration testing done with gcc-4.9

| *branch*    | *state* | *description*           |
| ----------- | ------- | ----------------------- |
| **master**  | [![Build Status Master](https://travis-ci.org/ComputationalRadiationPhysics/DoDo.png?branch=master)](https://travis-ci.org/ComputationalRadiationPhysics/DoDo "master") | our latest stable release |
| **dev**     | [![Build Status Development](https://travis-ci.org/ComputationalRadiationPhysics/DoDo.png?branch=dev)](https://travis-ci.org/ComputationalRadiationPhysics/DoDo "dev") | our development branch - start and merge new branches here |


Software License
----------------

DoDo is licensed under the **LGPLv3+**.
For more information, please refer to the [LICENSE](utility/autoHeaderLicense.txt) file.

************************************************************************

[![I make tools for managing job-hunting sites for people who make tools for managing job-hunting sites for people who make tools for ...](http://imgs.xkcd.com/comics/tools.png "I make tools for managing job-hunting sites for people who make tools for managing job-hunting sites for people who make tools for ...")](http://xkcd.com/1629/)

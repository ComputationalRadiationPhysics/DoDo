Dodo
====

Introduction
------------


Installation/Compilation
------------------------

### Dependencies
 - C++14 compatible compiler
   - `gcc` >= 5.3
   - `clang` >= 3.6
 - `boost` >= 1.61
   - compile time headers
   - `boost::program_options`
 - `CMake` >= 3.3
 - `git` >= 1.7.9.5

### Examples
This is an example how to compile DoDo and test the example code snippets

1. **Setup directories:**
 - `mkdir -p build`
2. **Download the source code:**
 -  `git clone https://github.com/ComputationalRadiationPhysics/DoDo.git`
3. **Build**
 - `cd build`
 - `cmake ../DoDo/examples`
 - `make`
4. **Run the examples**


### Installation
This is an example how to install DoDo in your home directory

1. **Setup directories:**
 - `mkdir -p build`
2. **Download the source code:**
 -  `git clone https://github.com/ComputationalRadiationPhysics/DoDo.git`
3. **Build**
 - `cd build`
 - `cmake ../DoDo -DCMAKE_INSTALL_PREFIX=$HOME/libs`
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


Literature
----------

- Diploma thesis [![DOI](https://zenodo.org/badge/doi/10.5281/zenodo.163329.svg)](http://dx.doi.org/10.5281/zenodo.163329) by Carlchristian Eckert (2016)


Branches
--------

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

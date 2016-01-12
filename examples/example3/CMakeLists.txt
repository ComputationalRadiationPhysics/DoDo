################################################################################
# Required CMake version.
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)


################################################################################
# Project.
################################################################################

PROJECT("example3")


#-------------------------------------------------------------------------------
# Find Dodo.
#-------------------------------------------------------------------------------

SET(DODO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../" CACHE STRING  "The location of the Dodo library")
LIST(APPEND CMAKE_PREFIX_PATH "${DODO_ROOT}")

FIND_PACKAGE("Dodo")
ADD_DEFINITIONS(${Dodo_DEFINITIONS})
INCLUDE_DIRECTORIES(${Dodo_INCLUDE_DIRS})


#-------------------------------------------------------------------------------
# Add library.
#-------------------------------------------------------------------------------

file(GLOB_RECURSE _SOURCES "src" "*.cpp")

INCLUDE_DIRECTORIES("include/")

# Always add all files to the target executable build call to add them to the build project.
ADD_EXECUTABLE("example3" ${_SOURCES})

# Set the link libraries for this library (adds libs, include directories, defines and compile options).
#TARGET_LINK_LIBRARIES("example1" PUBLIC "Dodo")

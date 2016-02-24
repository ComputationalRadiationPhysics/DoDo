CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

#-------------------------------------------------------------------------------
# Find Dodo (for correct includes)
#-------------------------------------------------------------------------------
LIST(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}")
FIND_PACKAGE(Dodo REQUIRED)
INCLUDE_DIRECTORIES(${Dodo_INCLUDE_DIRS})

#-------------------------------------------------------------------------------
# Add Dodo files (so the IDE considers the files part of the project)
#-------------------------------------------------------------------------------
file(GLOB_RECURSE _SOURCES_PRE "*")
foreach(P ${_SOURCES_PRE})
    # Remove dot-directories and the build directory
    if(NOT ${P} MATCHES  "${CMAKE_CURRENT_LIST_DIR}/([.].*/.*|build/.*)")
        set(_SOURCES ${_SOURCES} ${P})
    endif()
endforeach()
ADD_LIBRARY(CLION_DUMMY_TARGET EXCLUDE_FROM_ALL ${_SOURCES})


################################################################################
# Add examples and tests
###################################################################k#############
ADD_SUBDIRECTORY("examples/")
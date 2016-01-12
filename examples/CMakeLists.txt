################################################################################
# Required CMake version.
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

set(CMAKE_VERBOSE_MAKEFILE OFF)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

PROJECT("Dodo_examples")

################################################################################
# Compiler Flags
################################################################################
# GNU
#if(CMAKE_COMPILER_IS_GNUCXX)
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wall")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wextra")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wpedantic")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wnarrowing")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Waddress")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Warray-bounds")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wchar-subscripts")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wcomment")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wformat")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wmain")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wmissing-braces")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wparentheses")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wreturn-type")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wsequence-point")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wsign-compare")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wstrict-aliasing")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wstrict-overflow=1")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wswitch")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wtrigraphs")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wuninitialized")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wunused-function")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wunused-label")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wunused-value")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wunused-variable")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wvolatile-register-var")
    set(MY_WARN_FLAGS "${MY_WARN_FLAGS} -Wunused-local-typedefs")

    set(MY_COMMON_FLAGS "${MY_COMMON_FLAGS} ${MY_WARN_FLAGS}")
    set(MY_COMMON_FLAGS "${MY_COMMON_FLAGS} -std=c++1z")

    unset(MY_WARN_FLAGS)
    #endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${MY_COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MY_COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror")

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CMAKE_CXX_FLAGS_RELEASE}")

unset(MY_COMMON_FLAGS)



################################################################################
# Add subdirectories.
################################################################################
ADD_SUBDIRECTORY("example1/")
ADD_SUBDIRECTORY("example2/")
ADD_SUBDIRECTORY("example3/")

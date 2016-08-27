################################################################################
# Required CMake version.
################################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
PROJECT("Dodo_examples")

#-------------------------------------------------------------------------------
# Find Dodo.
#-------------------------------------------------------------------------------
SET(DODO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../" CACHE STRING  "The location of the Dodo library")
LIST(APPEND CMAKE_PREFIX_PATH "${DODO_ROOT}")

FIND_PACKAGE(Dodo REQUIRED)
ADD_COMPILE_OPTIONS(${Dodo_COMPILE_OPTIONS})
INCLUDE_DIRECTORIES(${Dodo_INCLUDE_DIRS})
LINK_LIBRARIES(${Dodo_LIBRARIES})


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

################################################################################
# Compiler Flags
################################################################################
if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror")

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")


################################################################################
# Add subdirectories.
###################################################################k#############
file(GLOB EXAMPLES "." "*")
foreach(EXAMPLE ${EXAMPLES})
    if(IS_DIRECTORY ${EXAMPLE})
        ADD_SUBDIRECTORY(${EXAMPLE})
    endif()
endforeach()

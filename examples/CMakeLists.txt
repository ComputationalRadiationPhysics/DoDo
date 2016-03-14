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


################################################################################
# Compiler Flags
################################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror")

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")

################################################################################
# Add subdirectories.
###################################################################k#############
file(GLOB EXAMPLES "." "*")
foreach(EXAMPLE ${EXAMPLES})
    if(IS_DIRECTORY ${EXAMPLE})
        ADD_SUBDIRECTORY(${EXAMPLE})
    endif()
endforeach()

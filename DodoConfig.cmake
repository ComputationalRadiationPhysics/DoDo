# - Config file for the Dodo package
cmake_minimum_required(VERSION 3.0.2)
project(Dodo)


#-------------------------------------------------------------------------------
# Directory of this file.
#-------------------------------------------------------------------------------
set(_DODO_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Normalize the path (e.g. remove ../)
GET_FILENAME_COMPONENT(_DODO_ROOT_DIR "${_DODO_ROOT_DIR}" ABSOLUTE)

#-------------------------------------------------------------------------------
# Set found to true initially and set it on false if a required dependency is missing.
#-------------------------------------------------------------------------------
set(_DODO_FOUND TRUE)

#-------------------------------------------------------------------------------
# Common.
#-------------------------------------------------------------------------------
# Add common functions.
set(_DODO_COMMON_FILE "${_DODO_ROOT_DIR}/cmake/common.cmake")
INCLUDE("${_DODO_COMMON_FILE}")


#-------------------------------------------------------------------------------
# Options.
#-------------------------------------------------------------------------------
#OPTION(DODO_ACC_CPU_B_SEQ_T_SEQ_ENABLE "Enable the serial CPU accelerator" ON)

# Drop-down combo box in cmake-gui.
set(DODO_DEBUG "0" CACHE STRING "Debug level")
SET_PROPERTY(CACHE DODO_DEBUG PROPERTY STRINGS "0;1;2")

#-------------------------------------------------------------------------------
# Find Boost.
#-------------------------------------------------------------------------------
#set(_DODO_BOOST_MIN_VER "1.58.0") # minimum version for basic features
#if(${DODO_DEBUG} GREATER 1)
#    set(Boost_DEBUG ON)
#    set(Boost_DETAILED_FAILURE_MSG ON)
#endif()
#FIND_PACKAGE(Boost ${_DODO_BOOST_MIN_VER} QUIET)
#
#if(${DODO_DEBUG} GREATER 1)
#    MESSAGE(STATUS "Boost in:")
#    MESSAGE(STATUS "BOOST_ROOT : ${BOOST_ROOT}")
#    MESSAGE(STATUS "BOOSTROOT : ${BOOSTROOT}")
#    MESSAGE(STATUS "BOOST_INCLUDEDIR: ${BOOST_INCLUDEDIR}")
#    MESSAGE(STATUS "BOOST_LIBRARYDIR: ${BOOST_LIBRARYDIR}")
#    MESSAGE(STATUS "Boost_NO_SYSTEM_PATHS: ${Boost_NO_SYSTEM_PATHS}")
#    MESSAGE(STATUS "Boost_ADDITIONAL_VERSIONS: ${Boost_ADDITIONAL_VERSIONS}")
#    MESSAGE(STATUS "Boost_USE_MULTITHREADED: ${Boost_USE_MULTITHREADED}")
#    MESSAGE(STATUS "Boost_USE_STATIC_LIBS: ${Boost_USE_STATIC_LIBS}")
#    MESSAGE(STATUS "Boost_USE_STATIC_RUNTIME: ${Boost_USE_STATIC_RUNTIME}")
#    MESSAGE(STATUS "Boost_USE_DEBUG_RUNTIME: ${Boost_USE_DEBUG_RUNTIME}")
#    MESSAGE(STATUS "Boost_USE_DEBUG_PYTHON: ${Boost_USE_DEBUG_PYTHON}")
#    MESSAGE(STATUS "Boost_USE_STLPORT: ${Boost_USE_STLPORT}")
#    MESSAGE(STATUS "Boost_USE_STLPORT_DEPRECATED_NATIVE_IOSTREAMS: ${Boost_USE_STLPORT_DEPRECATED_NATIVE_IOSTREAMS}")
#    MESSAGE(STATUS "Boost_COMPILER: ${Boost_COMPILER}")
#    MESSAGE(STATUS "Boost_THREADAPI: ${Boost_THREADAPI}")
#    MESSAGE(STATUS "Boost_NAMESPACE: ${Boost_NAMESPACE}")
#    MESSAGE(STATUS "Boost_DEBUG: ${Boost_DEBUG}")
#    MESSAGE(STATUS "Boost_DETAILED_FAILURE_MSG: ${Boost_DETAILED_FAILURE_MSG}")
#    MESSAGE(STATUS "Boost_REALPATH: ${Boost_REALPATH}")
#    MESSAGE(STATUS "Boost_NO_BOOST_CMAKE: ${Boost_NO_BOOST_CMAKE}")
#    MESSAGE(STATUS "Boost out:")
#    MESSAGE(STATUS "Boost_FOUND: ${Boost_FOUND}")
#    MESSAGE(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#    MESSAGE(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
#    MESSAGE(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
#    MESSAGE(STATUS "Boost_FIBER_FOUND: ${Boost_FIBER_FOUND}")
#    MESSAGE(STATUS "Boost_FIBER_LIBRARY: ${Boost_FIBER_LIBRARY}")
#    MESSAGE(STATUS "Boost_CONTEXT_FOUND: ${Boost_CONTEXT_FOUND}")
#    MESSAGE(STATUS "Boost_CONTEXT_LIBRARY: ${Boost_CONTEXT_LIBRARY}")
#    MESSAGE(STATUS "Boost_SYSTEM_FOUND: ${Boost_SYSTEM_FOUND}")
#    MESSAGE(STATUS "Boost_SYSTEM_LIBRARY: ${Boost_SYSTEM_LIBRARY}")
#    MESSAGE(STATUS "Boost_THREAD_FOUND: ${Boost_THREAD_FOUND}")
#    MESSAGE(STATUS "Boost_THREAD_LIBRARY: ${Boost_THREAD_LIBRARY}")
#    MESSAGE(STATUS "Boost_ATOMIC_FOUND: ${Boost_ATOMIC_FOUND}")
#    MESSAGE(STATUS "Boost_ATOMIC_LIBRARY: ${Boost_ATOMIC_LIBRARY}")
#    MESSAGE(STATUS "Boost_CHRONO_FOUND: ${Boost_CHRONO_FOUND}")
#    MESSAGE(STATUS "Boost_CHRONO_LIBRARY: ${Boost_CHRONO_LIBRARY}")
#    MESSAGE(STATUS "Boost_DATE_TIME_FOUND: ${Boost_DATE_TIME_FOUND}")
#    MESSAGE(STATUS "Boost_DATE_TIME_LIBRARY: ${Boost_DATE_TIME_LIBRARY}")
#    MESSAGE(STATUS "Boost_VERSION: ${Boost_VERSION}")
#    MESSAGE(STATUS "Boost_LIB_VERSION: ${Boost_LIB_VERSION}")
#    MESSAGE(STATUS "Boost_MAJOR_VERSION: ${Boost_MAJOR_VERSION}")
#    MESSAGE(STATUS "Boost_MINOR_VERSION: ${Boost_MINOR_VERSION}")
#    MESSAGE(STATUS "Boost_SUBMINOR_VERSION: ${Boost_SUBMINOR_VERSION}")
#    MESSAGE(STATUS "Boost_LIB_DIAGNOSTIC_DEFINITIONS: ${Boost_LIB_DIAGNOSTIC_DEFINITIONS}")
#    MESSAGE(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
#    MESSAGE(STATUS "Boost cached:")
#    MESSAGE(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
#    MESSAGE(STATUS "Boost_LIBRARY_DIR: ${Boost_LIBRARY_DIR}")
#endif()
#
#if(NOT Boost_FOUND)
#    MESSAGE(WARNING "Required Dodo dependency Boost (>=${_DODO_BOOST_MIN_VER}) could not be found!")
#    set(_DODO_FOUND FALSE)
#
#else()
#    LIST(APPEND _DODO_INCLUDE_DIRECTORIES_PUBLIC ${Boost_INCLUDE_DIRS})
#    LIST(APPEND _DODO_LINK_LIBRARIES_PUBLIC ${Boost_LIBRARIES})
#endif()
#
if(Boost_VERSION LESS 1.61)
    FIND_PACKAGE(Hana REQUIRED)
    LIST(APPEND _DODO_INCLUDE_DIRECTORIES_PUBLIC ${Hana_INCLUDE_DIRS})
endif()
#
#FIND_PACKAGE(graybat REQUIRED QUIET)
#    LIST(APPEND _DODO_INCLUDE_DIRECTORIES_PUBLIC ${graybat_INCLUDE_DIRS})
#    LIST(APPEND _DODO_LINK_LIBRARIES_PUBLIC ${graybat_LIBRARIES})

FIND_PACKAGE(dout)
if(dout_FOUND)
    LIST(APPEND _DODO_INCLUDE_DIRECTORIES_PUBLIC ${dout_INCLUDE_DIRS})
endif()


#-------------------------------------------------------------------------------
# Compiler settings.
#-------------------------------------------------------------------------------
if(MSVC)
    # Empty append to define it if it does not already exist.
    LIST(APPEND _DODO_COMPILE_OPTIONS_PUBLIC)
else()
    # Select C++ standard version.
    LIST(APPEND _DODO_COMPILE_OPTIONS_PUBLIC "-std=c++14")
endif()


set(_DODO_INCLUDE_DIRECTORY "${_DODO_ROOT_DIR}/include")
LIST(APPEND _DODO_INCLUDE_DIRECTORIES_PUBLIC "${_DODO_INCLUDE_DIRECTORY}")
set(_DODO_SUFFIXED_INCLUDE_DIR "${_DODO_INCLUDE_DIRECTORY}/dodo")

set(_DODO_LINK_LIBRARY)
LIST(APPEND _DODO_LINK_LIBRARIES_PUBLIC "${_DODO_LINK_LIBRARY}")

set(_DODO_FILES_OTHER "${_DODO_ROOT_DIR}/DodoConfig.cmake" "${_DODO_COMMON_FILE}" "${_DODO_ROOT_DIR}/.travis.yml" "${_DODO_ROOT_DIR}/README.md")

# Add all the source and include files in all recursive subdirectories and group them accordingly.
append_recursive_files_add_to_src_group("${_DODO_SUFFIXED_INCLUDE_DIR}" "${_DODO_SUFFIXED_INCLUDE_DIR}" "hpp" "_DODO_FILES_HEADER")
append_recursive_files_add_to_src_group("${_DODO_SUFFIXED_INCLUDE_DIR}" "${_DODO_SUFFIXED_INCLUDE_DIR}" "cpp" "_DODO_FILES_SOURCE")

#-------------------------------------------------------------------------------
# Set return values.
#-------------------------------------------------------------------------------
set(Dodo_COMPILE_OPTIONS ${_DODO_COMPILE_OPTIONS_PUBLIC})
set(Dodo_INCLUDE_DIR ${_DODO_INCLUDE_DIRECTORY})
set(Dodo_INCLUDE_DIRS ${_DODO_INCLUDE_DIRECTORIES_PUBLIC})
set(Dodo_LIBRARY ${_DODO_LINK_LIBRARY})
set(Dodo_LIBRARIES ${_DODO_LINK_FLAGS_PUBLIC})
LIST(APPEND Dodo_LIBRARIES ${_DODO_LINK_LIBRARIES_PUBLIC})

#TODO: Execute the following steps only after it is guaranteed that the lists exist!
#LIST(REMOVE_DUPLICATES Dodo_INCLUDE_DIRS)
#LIST(REMOVE_DUPLICATES Dodo_LIBRARIES)
#LIST(REMOVE_DUPLICATES Dodo_COMPILE_OPTIONS)
#LIST(REVERSE Dodo_INCLUDE_DIRS)

#-------------------------------------------------------------------------------
# Print the return values.
#-------------------------------------------------------------------------------
if(${DODO_DEBUG} GREATER 0)
    MESSAGE(STATUS "Dodo_FOUND: ${Dodo_FOUND}")
    MESSAGE(STATUS "Dodo_VERSION: ${Dodo_VERSION}")
    MESSAGE(STATUS "Dodo_COMPILE_OPTIONS: ${Dodo_COMPILE_OPTIONS}")
    MESSAGE(STATUS "Dodo_INCLUDE_DIR: ${Dodo_INCLUDE_DIR}")
    MESSAGE(STATUS "Dodo_INCLUDE_DIRS: ${Dodo_INCLUDE_DIRS}")
    MESSAGE(STATUS "Dodo_LIBRARY: ${Dodo_LIBRARY}")
    MESSAGE(STATUS "Dodo_LIBRARIES: ${Dodo_LIBRARIES}")
endif()

# Unset already set variables if not found.
if(NOT _DODO_FOUND)
    unset(Dodo_FOUND)
    unset(Dodo_VERSION)
    unset(Dodo_COMPILE_OPTIONS)
    unset(Dodo_INCLUDE_DIR)
    unset(Dodo_INCLUDE_DIRS)
    unset(Dodo_LIBRARY)
    unset(Dodo_LIBRARIES)

    unset(_DODO_FOUND)
    unset(_DODO_COMPILE_OPTIONS_PUBLIC)
    unset(_DODO_INCLUDE_DIRECTORY)
    unset(_DODO_INCLUDE_DIRECTORIES_PUBLIC)
    unset(_DODO_LINK_LIBRARY)
    unset(_DODO_LINK_LIBRARIES_PUBLIC)
    unset(_DODO_LINK_FLAGS_PUBLIC)
    unset(_DODO_COMMON_FILE)
    unset(_DODO_FILES_HEADER)
    unset(_DODO_FILES_SOURCE)
    unset(_DODO_FILES_OTHER)
    unset(_DODO_BOOST_MIN_VER)
else()
    # Make internal variables advanced options in the GUI.
    MARK_AS_ADVANCED(
        Dodo_INCLUDE_DIR
        Dodo_LIBRARY
        _DODO_COMPILE_OPTIONS_PUBLIC
        _DODO_INCLUDE_DIRECTORY
        _DODO_INCLUDE_DIRECTORIES_PUBLIC
        _DODO_LINK_LIBRARY
        _DODO_LINK_LIBRARIES_PUBLIC
        _DODO_LINK_FLAGS_PUBLIC
        _DODO_COMMON_FILE
        _DODO_FILES_HEADER
        _DODO_FILES_SOURCE
        _DODO_FILES_OTHER
        _DODO_BOOST_MIN_VER
        )
endif()

###############################################################################
# FindPackage options
###############################################################################

# Handles the REQUIRED, QUIET and version-related arguments for FIND_PACKAGE.
# NOTE: We do not check for Dodo_LIBRARIES because they can be empty.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    "Dodo"
    FOUND_VAR Dodo_FOUND
    REQUIRED_VARS Dodo_INCLUDE_DIR
    VERSION_VAR Dodo_VERSION
    )

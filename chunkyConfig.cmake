# - Config file for the chunky package
cmake_minimum_required(VERSION 3.0.2)
project(chunky)


#-------------------------------------------------------------------------------
# Directory of this file.
#-------------------------------------------------------------------------------
set(_CHUNKY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Normalize the path (e.g. remove ../)
GET_FILENAME_COMPONENT(_CHUNKY_ROOT_DIR "${_CHUNKY_ROOT_DIR}" ABSOLUTE)

#-------------------------------------------------------------------------------
# Set found to true initially and set it on false if a required dependency is missing.
#-------------------------------------------------------------------------------
set(_CHUNKY_FOUND TRUE)

#-------------------------------------------------------------------------------
# Common.
#-------------------------------------------------------------------------------
# Add common functions.
set(_CHUNKY_COMMON_FILE "${_CHUNKY_ROOT_DIR}/cmake/common.cmake")
INCLUDE("${_CHUNKY_COMMON_FILE}")


#-------------------------------------------------------------------------------
# Options.
#-------------------------------------------------------------------------------
OPTION(CHUNKY_ACC_CPU_B_SEQ_T_SEQ_ENABLE "Enable the serial CPU accelerator" ON)

# Drop-down combo box in cmake-gui.
set(CHUNKY_DEBUG "0" CACHE STRING "Debug level")
SET_PROPERTY(CACHE CHUNKY_DEBUG PROPERTY STRINGS "0;1;2")

#-------------------------------------------------------------------------------
# Find Boost.
#-------------------------------------------------------------------------------
set(_CHUNKY_BOOST_MIN_VER "1.58.0") # minimum version for basic features
if(${CHUNKY_DEBUG} GREATER 1)
    set(Boost_DEBUG ON)
    set(Boost_DETAILED_FAILURE_MSG ON)
endif()
FIND_PACKAGE(Boost ${_CHUNKY_BOOST_MIN_VER} QUIET)

if(${CHUNKY_DEBUG} GREATER 1)
    MESSAGE(STATUS "Boost in:")
    MESSAGE(STATUS "BOOST_ROOT : ${BOOST_ROOT}")
    MESSAGE(STATUS "BOOSTROOT : ${BOOSTROOT}")
    MESSAGE(STATUS "BOOST_INCLUDEDIR: ${BOOST_INCLUDEDIR}")
    MESSAGE(STATUS "BOOST_LIBRARYDIR: ${BOOST_LIBRARYDIR}")
    MESSAGE(STATUS "Boost_NO_SYSTEM_PATHS: ${Boost_NO_SYSTEM_PATHS}")
    MESSAGE(STATUS "Boost_ADDITIONAL_VERSIONS: ${Boost_ADDITIONAL_VERSIONS}")
    MESSAGE(STATUS "Boost_USE_MULTITHREADED: ${Boost_USE_MULTITHREADED}")
    MESSAGE(STATUS "Boost_USE_STATIC_LIBS: ${Boost_USE_STATIC_LIBS}")
    MESSAGE(STATUS "Boost_USE_STATIC_RUNTIME: ${Boost_USE_STATIC_RUNTIME}")
    MESSAGE(STATUS "Boost_USE_DEBUG_RUNTIME: ${Boost_USE_DEBUG_RUNTIME}")
    MESSAGE(STATUS "Boost_USE_DEBUG_PYTHON: ${Boost_USE_DEBUG_PYTHON}")
    MESSAGE(STATUS "Boost_USE_STLPORT: ${Boost_USE_STLPORT}")
    MESSAGE(STATUS "Boost_USE_STLPORT_DEPRECATED_NATIVE_IOSTREAMS: ${Boost_USE_STLPORT_DEPRECATED_NATIVE_IOSTREAMS}")
    MESSAGE(STATUS "Boost_COMPILER: ${Boost_COMPILER}")
    MESSAGE(STATUS "Boost_THREADAPI: ${Boost_THREADAPI}")
    MESSAGE(STATUS "Boost_NAMESPACE: ${Boost_NAMESPACE}")
    MESSAGE(STATUS "Boost_DEBUG: ${Boost_DEBUG}")
    MESSAGE(STATUS "Boost_DETAILED_FAILURE_MSG: ${Boost_DETAILED_FAILURE_MSG}")
    MESSAGE(STATUS "Boost_REALPATH: ${Boost_REALPATH}")
    MESSAGE(STATUS "Boost_NO_BOOST_CMAKE: ${Boost_NO_BOOST_CMAKE}")
    MESSAGE(STATUS "Boost out:")
    MESSAGE(STATUS "Boost_FOUND: ${Boost_FOUND}")
    MESSAGE(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    MESSAGE(STATUS "Boost_FIBER_FOUND: ${Boost_FIBER_FOUND}")
    MESSAGE(STATUS "Boost_FIBER_LIBRARY: ${Boost_FIBER_LIBRARY}")
    MESSAGE(STATUS "Boost_CONTEXT_FOUND: ${Boost_CONTEXT_FOUND}")
    MESSAGE(STATUS "Boost_CONTEXT_LIBRARY: ${Boost_CONTEXT_LIBRARY}")
    MESSAGE(STATUS "Boost_SYSTEM_FOUND: ${Boost_SYSTEM_FOUND}")
    MESSAGE(STATUS "Boost_SYSTEM_LIBRARY: ${Boost_SYSTEM_LIBRARY}")
    MESSAGE(STATUS "Boost_THREAD_FOUND: ${Boost_THREAD_FOUND}")
    MESSAGE(STATUS "Boost_THREAD_LIBRARY: ${Boost_THREAD_LIBRARY}")
    MESSAGE(STATUS "Boost_ATOMIC_FOUND: ${Boost_ATOMIC_FOUND}")
    MESSAGE(STATUS "Boost_ATOMIC_LIBRARY: ${Boost_ATOMIC_LIBRARY}")
    MESSAGE(STATUS "Boost_CHRONO_FOUND: ${Boost_CHRONO_FOUND}")
    MESSAGE(STATUS "Boost_CHRONO_LIBRARY: ${Boost_CHRONO_LIBRARY}")
    MESSAGE(STATUS "Boost_DATE_TIME_FOUND: ${Boost_DATE_TIME_FOUND}")
    MESSAGE(STATUS "Boost_DATE_TIME_LIBRARY: ${Boost_DATE_TIME_LIBRARY}")
    MESSAGE(STATUS "Boost_VERSION: ${Boost_VERSION}")
    MESSAGE(STATUS "Boost_LIB_VERSION: ${Boost_LIB_VERSION}")
    MESSAGE(STATUS "Boost_MAJOR_VERSION: ${Boost_MAJOR_VERSION}")
    MESSAGE(STATUS "Boost_MINOR_VERSION: ${Boost_MINOR_VERSION}")
    MESSAGE(STATUS "Boost_SUBMINOR_VERSION: ${Boost_SUBMINOR_VERSION}")
    MESSAGE(STATUS "Boost_LIB_DIAGNOSTIC_DEFINITIONS: ${Boost_LIB_DIAGNOSTIC_DEFINITIONS}")
    MESSAGE(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    MESSAGE(STATUS "Boost cached:")
    MESSAGE(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
    MESSAGE(STATUS "Boost_LIBRARY_DIR: ${Boost_LIBRARY_DIR}")
endif()

if(NOT Boost_FOUND)
    MESSAGE(WARNING "Required chunky dependency Boost (>=${_CHUNKY_BOOST_MIN_VER}) could not be found!")
    set(_CHUNKY_FOUND FALSE)

else()
    LIST(APPEND _CHUNKY_INCLUDE_DIRECTORIES_PUBLIC ${Boost_INCLUDE_DIRS})
    LIST(APPEND _CHUNKY_LINK_LIBRARIES_PUBLIC ${Boost_LIBRARIES})
endif()


#-------------------------------------------------------------------------------
# Compiler settings.
#-------------------------------------------------------------------------------
if(MSVC)
    # Empty append to define it if it does not already exist.
    LIST(APPEND _CHUNKY_COMPILE_OPTIONS_PUBLIC)
else()
    # Select C++ standard version.
    LIST(APPEND _CHUNKY_COMPILE_OPTIONS_PUBLIC "-std=c++1z")
endif()


set(_CHUNKY_INCLUDE_DIRECTORY "${_CHUNKY_ROOT_DIR}/include")
LIST(APPEND _CHUNKY_INCLUDE_DIRECTORIES_PUBLIC "${_CHUNKY_INCLUDE_DIRECTORY}")
set(_CHUNKY_SUFFIXED_INCLUDE_DIR "${_CHUNKY_INCLUDE_DIRECTORY}/chunky")

set(_CHUNKY_LINK_LIBRARY)
LIST(APPEND _CHUNKY_LINK_LIBRARIES_PUBLIC "${_CHUNKY_LINK_LIBRARY}")

set(_CHUNKY_FILES_OTHER "${_CHUNKY_ROOT_DIR}/chunkyConfig.cmake" "${_CHUNKY_COMMON_FILE}" "${_CHUNKY_ROOT_DIR}/.travis.yml" "${_CHUNKY_ROOT_DIR}/README.md")

# Add all the source and include files in all recursive subdirectories and group them accordingly.
append_recursive_files_add_to_src_group("${_CHUNKY_SUFFIXED_INCLUDE_DIR}" "${_CHUNKY_SUFFIXED_INCLUDE_DIR}" "hpp" "_CHUNKY_FILES_HEADER")
append_recursive_files_add_to_src_group("${_CHUNKY_SUFFIXED_INCLUDE_DIR}" "${_CHUNKY_SUFFIXED_INCLUDE_DIR}" "cpp" "_CHUNKY_FILES_SOURCE")

#-------------------------------------------------------------------------------
# Target.
#-------------------------------------------------------------------------------
if(NOT TARGET "chunky")
    ADD_LIBRARY("chunky" ${_CHUNKY_FILES_HEADER} ${_CHUNKY_FILES_SOURCE} ${_CHUNKY_FILES_OTHER})

    # Compile options.
    if(${CHUNKY_DEBUG} GREATER 1)
        MESSAGE(STATUS "_CHUNKY_COMPILE_OPTIONS_PUBLIC: ${_CHUNKY_COMPILE_OPTIONS_PUBLIC}")
    endif()
    LIST(LENGTH _CHUNKY_COMPILE_OPTIONS_PUBLIC _CHUNKY_COMPILE_OPTIONS_PUBLIC_LENGTH)
    if(${_CHUNKY_COMPILE_OPTIONS_PUBLIC_LENGTH} GREATER 0)
        TARGET_COMPILE_OPTIONS("chunky" PUBLIC ${_CHUNKY_COMPILE_OPTIONS_PUBLIC})
    endif()

    # Include directories.
    if(${CHUNKY_DEBUG} GREATER 1)
        MESSAGE(STATUS "_CHUNKY_INCLUDE_DIRECTORIES_PUBLIC: ${_CHUNKY_INCLUDE_DIRECTORIES_PUBLIC}")
    endif()
    LIST(LENGTH _CHUNKY_INCLUDE_DIRECTORIES_PUBLIC _CHUNKY_INCLUDE_DIRECTORIES_PUBLIC_LENGTH)
    if(${_CHUNKY_INCLUDE_DIRECTORIES_PUBLIC_LENGTH} GREATER 0)
        TARGET_INCLUDE_DIRECTORIES("chunky" PUBLIC ${_CHUNKY_INCLUDE_DIRECTORIES_PUBLIC})
    endif()

    # Link libraries.
    # There are no PUBLIC_LINK_FLAGS in CMAKE:
    # http://stackoverflow.com/questions/26850889/cmake-keeping-link-flags-of-internal-libs
    if(${CHUNKY_DEBUG} GREATER 1)
        MESSAGE(STATUS "_CHUNKY_LINK_LIBRARIES_PUBLIC: ${_CHUNKY_LINK_LIBRARIES_PUBLIC}")
    endif()
    LIST(LENGTH _CHUNKY_LINK_LIBRARIES_PUBLIC _CHUNKY_LINK_LIBRARIES_PUBLIC_LENGTH)
    if(${_CHUNKY_LINK_LIBRARIES_PUBLIC_LENGTH} GREATER 0)
        TARGET_LINK_LIBRARIES("chunky" PUBLIC ${_CHUNKY_LINK_LIBRARIES_PUBLIC} ${_CHUNKY_LINK_FLAGS_PUBLICI})
    endif()
endif()

#-------------------------------------------------------------------------------
# Set return values.
#-------------------------------------------------------------------------------
set(chunky_COMPILE_OPTIONS ${_CHUNKY_COMPILE_OPTIONS_PUBLIC})
set(chunky_INCLUDE_DIR ${_CHUNKY_INCLUDE_DIRECTORY})
set(chunky_INCLUDE_DIRS ${_CHUNKY_INCLUDE_DIRECTORIES_PUBLIC})
set(chunky_LIBRARY ${_CHUNKY_LINK_LIBRARY})
set(chunky_LIBRARIES ${_CHUNKY_LINK_FLAGS_PUBLIC})
LIST(APPEND chunky_LIBRARIES ${_CHUNKY_LINK_LIBRARIES_PUBLIC})

#-------------------------------------------------------------------------------
# Print the return values.
#-------------------------------------------------------------------------------
if(${CHUNKY_DEBUG} GREATER 0)
    MESSAGE(STATUS "chunky_FOUND: ${chunky_FOUND}")
    MESSAGE(STATUS "chunky_VERSION: ${chunky_VERSION}")
    MESSAGE(STATUS "chunky_COMPILE_OPTIONS: ${chunky_COMPILE_OPTIONS}")
    MESSAGE(STATUS "chunky_INCLUDE_DIR: ${chunky_INCLUDE_DIR}")
    MESSAGE(STATUS "chunky_INCLUDE_DIRS: ${chunky_INCLUDE_DIRS}")
    MESSAGE(STATUS "chunky_LIBRARY: ${chunky_LIBRARY}")
    MESSAGE(STATUS "chunky_LIBRARIES: ${chunky_LIBRARIES}")
endif()

# Unset already set variables if not found.
if(NOT _CHUNKY_FOUND)
    unset(chunky_FOUND)
    unset(chunky_VERSION)
    unset(chunky_COMPILE_OPTIONS)
    unset(chunky_INCLUDE_DIR)
    unset(chunky_INCLUDE_DIRS)
    unset(chunky_LIBRARY)
    unset(chunky_LIBRARIES)

    unset(_CHUNKY_FOUND)
    unset(_CHUNKY_COMPILE_OPTIONS_PUBLIC)
    unset(_CHUNKY_INCLUDE_DIRECTORY)
    unset(_CHUNKY_INCLUDE_DIRECTORIES_PUBLIC)
    unset(_CHUNKY_LINK_LIBRARY)
    unset(_CHUNKY_LINK_LIBRARIES_PUBLIC)
    unset(_CHUNKY_LINK_FLAGS_PUBLIC)
    unset(_CHUNKY_COMMON_FILE)
    unset(_CHUNKY_FILES_HEADER)
    unset(_CHUNKY_FILES_SOURCE)
    unset(_CHUNKY_FILES_OTHER)
    unset(_CHUNKY_BOOST_MIN_VER)
else()
    # Make internal variables advanced options in the GUI.
    MARK_AS_ADVANCED(
        chunky_INCLUDE_DIR
        chunky_LIBRARY
        _CHUNKY_COMPILE_OPTIONS_PUBLIC
        _CHUNKY_INCLUDE_DIRECTORY
        _CHUNKY_INCLUDE_DIRECTORIES_PUBLIC
        _CHUNKY_LINK_LIBRARY
        _CHUNKY_LINK_LIBRARIES_PUBLIC
        _CHUNKY_LINK_FLAGS_PUBLIC
        _CHUNKY_COMMON_FILE
        _CHUNKY_FILES_HEADER
        _CHUNKY_FILES_SOURCE
        _CHUNKY_FILES_OTHER
        _CHUNKY_BOOST_MIN_VER
        )
endif()

###############################################################################
# FindPackage options
###############################################################################

# Handles the REQUIRED, QUIET and version-related arguments for FIND_PACKAGE.
# NOTE: We do not check for chunky_LIBRARIES because they can be empty.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    "chunky"
    FOUND_VAR chunky_FOUND
    REQUIRED_VARS chunky_INCLUDE_DIR
    VERSION_VAR chunky_VERSION
    )

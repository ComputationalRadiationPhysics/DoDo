file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/include/chunky/version.hpp" chunky_VERSION_MAJOR_HPP REGEX "#define CHUNKY_VERSION_MAJOR ")
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/include/chunky/version.hpp"  chunky_VERSION_MINOR_HPP REGEX "#define CHUNKY_VERSION_MINOR ")
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/include/chunky/version.hpp"  chunky_VERSION_PATCH_HPP REGEX "#define CHUNKY_VERSION_PATCH ")

string(REGEX MATCH "([0-9]+)" chunky_VERSION_MAJOR  ${chunky_VERSION_MAJOR_HPP})
string(REGEX MATCH "([0-9]+)" chunky_VERSION_MINOR  ${chunky_VERSION_MINOR_HPP})
string(REGEX MATCH "([0-9]+)" chunky_VERSION_PATCH  ${chunky_VERSION_PATCH_HPP})

set(PACKAGE_VERSION "${chunky_VERSION_MAJOR}.${chunky_VERSION_MINOR}.${chunky_VERSION_PATCH}")

# Check whether the requested PACKAGE_FIND_VERSION is exactly the one requested
if("${PACKAGE_VERSION}" EQUAL "${PACKAGE_FIND_VERSION}")
    set(PACKAGE_VERSION_EXACT TRUE)
else()
    set(PACKAGE_VERSION_EXACT FALSE)
endif()

# Check whether the requested PACKAGE_FIND_VERSION is compatible
if("${PACKAGE_VERSION}" VERSION_LESS "${PACKAGE_FIND_VERSION}")
    set(PACKAGE_VERSION_COMPATIBLE FALSE)
else()
    set(PACKAGE_VERSION_COMPATIBLE TRUE)
    if ("${PACKAGE_VERSION}" VERSION_EQUAL "${PACKAGE_FIND_VERSION}")
        set(PACKAGE_VERSION_EXACT TRUE)
    endif()
endif()

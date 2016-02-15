#-------------------------------------------------------------------------------
# Set Project name.
#-------------------------------------------------------------------------------
get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)

################################################################################
# Build.
################################################################################
file(GLOB_RECURSE _SOURCES "src" "*.cpp")

# Always add all files to the target executable build call to add them to the build project.
ADD_EXECUTABLE(${PROJECT_NAME} ${_SOURCES})

# Set the link libraries for this library (adds libs, include directories, defines and compile options).
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Dodo_LIBRARIES})

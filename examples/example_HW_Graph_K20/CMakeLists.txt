#-------------------------------------------------------------------------------
# Set name.
#-------------------------------------------------------------------------------
get_filename_component(FolderName ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" FolderName ${FolderName})

################################################################################
# Build.
################################################################################
file(GLOB_RECURSE _SOURCES "src" "*.cpp")
file(GLOB_RECURSE _HEADERS "src" "*.hpp")

# Always add all files to the target executable build call to add them to the build project.
ADD_EXECUTABLE(${FolderName} ${_SOURCES} ${_HEADERS})

# Set the link libraries for this library (adds libs, include directories, defines and compile options).
TARGET_LINK_LIBRARIES(${FolderName} ${Dodo_LIBRARIES})

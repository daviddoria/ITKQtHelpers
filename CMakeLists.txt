cmake_minimum_required(VERSION 2.8)

project(ITKQtHelpers)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# This is needed for shared_ptr and the trick using enable_if and if_fundamental to allow scalars
# to be treated as the 0th component of a vector.
if(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

# Only include Helpers if it is not already available
get_property(FoundITKHelpers GLOBAL PROPERTY ITKHelpersIncludeDirs SET)
if(NOT FoundITKHelpers)
  add_subdirectory(ITKHelpers)
endif()

get_property(ITKHelpersIncludeDirs GLOBAL PROPERTY ITKHelpersIncludeDirs)
set(ITKQtHelpers_include_dirs ${ITKQtHelpers_include_dirs} ${ITKHelpersIncludeDirs})
get_property(ITKHelpersLibs GLOBAL PROPERTY ITKHelpersLibs)
set(ITKQtHelpers_libraries ${ITKQtHelpers_libraries} ${ITKHelpersLibs})

# Qt
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
list(APPEND compileflags "USE_QT_PARALLEL")
set(ITKQtHelpers_libraries ${ITKQtHelpers_libraries} ${ITK_LIBRARIES})

# ITK
FIND_PACKAGE(ITK REQUIRED ITKCommon ITKIOImageBase ITKTestKernel
             ITKImageIntensity ITKImageFeature ITKMathematicalMorphology
             ITKBinaryMathematicalMorphology ITKDistanceMap)
INCLUDE(${ITK_USE_FILE})
set(ITKQtHelpers_libraries ${ITKQtHelpers_libraries} ${ITK_LIBRARIES})

# Add non-compiled files to the project
add_custom_target(ITKQtHelpersSources SOURCES
ITKQtHelpers.h
ITKQtHelpers.hpp)

# Allow this project to be detected and used as a submodule
set(ITKQtHelpers_include_dirs ${ITKQtHelpers_include_dirs} ${CMAKE_CURRENT_SOURCE_DIR})
set_property(GLOBAL PROPERTY ITKQtHelpersIncludeDirs ${ITKQtHelpers_include_dirs})
set_property(GLOBAL PROPERTY ITKQtHelpersLibs ${ITKQtHelpers_libraries})

# Display where this code was used from (if it is used as a submodule, there may be multiple instances of this submodule in the project, only the first of which is used)
option(ITKQtHelpers_ShowSubmoduleLocation "Show the path from which ITKQtHelpers was used?" OFF)
if(ITKQtHelpers_ShowSubmoduleLocation)
  message("ITKQtHelpers used from ${CMAKE_CURRENT_SOURCE_DIR}")
endif(ITKQtHelpers_ShowSubmoduleLocation)

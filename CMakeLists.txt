cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(ITKQtHelpers_includes ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
endif()

# This is needed for shared_ptr and the trick using enable_if and if_fundamental to allow scalars
# to be treated as the 0th component of a vector.
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
list(APPEND compileflags "USE_QT_PARALLEL")

# ITK
FIND_PACKAGE(ITK REQUIRED ITKCommon ITKIOImageBase ITKTestKernel
             ITKImageIntensity ITKImageFeature ITKMathematicalMorphology
             ITKBinaryMathematicalMorphology ITKDistanceMap)
INCLUDE(${ITK_USE_FILE})

if(NOT TARGET ITKHelpers)
  add_subdirectory(ITKHelpers)
endif()

# This is not necessary because this is a template-only library (there is no cpp file to compile)
# add_library(libITKQtHelpers ITKQtHelpers.cpp)
# target_link_libraries(libITKQtHelpers ${ITK_LIBRARIES} ${QT_LIBRARIES})

cmake_minimum_required (VERSION 2.6)

project ("HelloTriangle")


set (TARGET_PLATFORM "win32")
#set (TARGET_INTERFACE "egl")
#set (TARGET_RENDERER "ogles20")



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)

set (Project_output "${PROJECT_BINARY_DIR}/output_${TARGET_PLATFORM}")

# Project configuration 
set (Project_VERSION_MAJOR 1)
set (Project_VERSION_MINOR 0)
set (Project_WORKING_DIR ${Project_output})
 
# configure a header file to pass some of the CMake settings to the source code
configure_file (  "${CMAKE_CURRENT_SOURCE_DIR}/config/HelloTriangleConfig.h.in"  "${CMAKE_CURRENT_BINARY_DIR}/HelloTriangleConfig.h"  )
if(WIN32)
configure_file (  "${CMAKE_CURRENT_SOURCE_DIR}/config/HelloTriangle.vcxproj.user"  "${CMAKE_CURRENT_BINARY_DIR}/HelloTriangle.vcxproj.user"  )
endif(WIN32)
 
# add the binary tree to the search path for include files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("./common" "./test")
include_directories("${CMAKE_SOURCE_DIR}/../externals/inc")

 
file(GLOB_RECURSE PROJECT_SOURCES_common "${CMAKE_CURRENT_BINARY_DIR}/HelloTriangleConfig.h" "./common/*.h" "./common/*.cpp" )
file(GLOB_RECURSE PROJECT_SOURCES_test "./test/*.h" "./test/*.cpp" )

file(GLOB EXTERNAL_DLLs "${CMAKE_SOURCE_DIR}/../externals/dlls/${TARGET_PLATFORM}/libEGL.dll")

#source_group(others   FILES ${OTHERS_SOURCES})
source_group(common   FILES ${PROJECT_SOURCES_common})
source_group(test   FILES ${PROJECT_SOURCES_test})

add_definitions(-DGL_GLEXT_PROTOTYPES)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)



link_directories("${CMAKE_SOURCE_DIR}/../externals/libs/${TARGET_PLATFORM}")
set (EXTRA_LIBS ${EXTRA_LIBS} libEGL)
 
# add the executable
add_executable(HelloTriangle ${PROJECT_SOURCES_common} ${PROJECT_SOURCES_test} )

target_link_libraries (HelloTriangle ${EXTRA_LIBS})

install (TARGETS HelloTriangle DESTINATION ${Project_output})
install (FILES ${EXTERNAL_DLLs} DESTINATION ${Project_output})

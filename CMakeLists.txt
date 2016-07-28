# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   ReTo
#   2016 (c) GMRV / Universidad Rey Juan Carlos
#   gmrv@gmrv.es
#   www.gmrv.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
cmake_minimum_required( VERSION 3.1 FATAL_ERROR )
project( ReTo VERSION 0.1.0 )
set( ReTo_VERSION_ABI 1)

# Disable in source building
if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
  message( FATAL "no in source building allowed." )
endif()

list( APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common )

# Default build type is Debug
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug )
endif( NOT CMAKE_BUILD_TYPE )

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
  add_definitions( -DDEBUG )
endif( )

include(GitExternal)

set( CMAKECOMMONTEMPLATE_DESCRIPTION "ReTo - Render Tools" )
set( COMMON_PROJECT_DOMAIN gmrv.es )

include( Common )

common_find_package(nvidiaOpenGL )
common_find_package(OpenGL REQUIRED SYSTEM)
common_find_package(GLEW REQUIRED SYSTEM)
common_find_package(Eigen3 REQUIRED SYSTEM)
common_find_package(GLUT SYSTEM)
common_find_package(Boost COMPONENTS unit_test_framework)
common_find_package(ZeroEQ)
common_find_package(Lexis)

list(APPEND RETO_DEPENDENT_LIBRARIES OpenGL GLEW Eigen3)

if(GLUT_FOUND)
  list(APPEND RETO_DEPENDENT_LIBRARIES GLUT)
endif()

if(ZEROEQ_FOUND)
  list(APPEND RETO_DEPENDENT_LIBRARIES ZeroEQ)
  common_find_package(Threads REQUIRED)
  list(APPEND RETO_DEPENDENT_LIBRARIES Threads)
endif()

if(LEXIS_FOUND)
  list(APPEND RETO_DEPENDENT_LIBRARIES Lexis)
endif()

common_find_package_post( )

add_subdirectory( reto )
add_subdirectory( examples )
add_subdirectory( tests )
add_subdirectory( bin )

configure_file(
  CMake/reto_generate_shaders.cmake
  ${PROJECT_BINARY_DIR}/${CMAKE_MODULE_INSTALL_PATH}/reto_generate_shaders.cmake
  COPYONLY
  )
install( FILES CMake/reto_generate_shaders.cmake
  DESTINATION ${CMAKE_MODULE_INSTALL_PATH} COMPONENT dev )

include( CPackConfig )
include( DoxygenRule )

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

common_find_package( Boost COMPONENTS unit_test_framework )
common_find_package_post( )

add_subdirectory( reto )
add_subdirectory( examples )
add_subdirectory( tests )

include( CPackConfig )
include( DoxygenRule )

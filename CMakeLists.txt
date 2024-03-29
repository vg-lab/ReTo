# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   ReTo
#   2017 (c) VG-Lab / Universidad Rey Juan Carlos
#   vg-lab@vg-lab.es
#   www.vg-lab.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

cmake_minimum_required( VERSION 3.1 FATAL_ERROR )
project( ReTo VERSION 0.3.7 )
set( ReTo_VERSION_ABI 5)

# Disable in source building
if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
  message( FATAL "no in source building allowed." )
endif()

list( APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common )

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/CMake/common/Common.cmake)
  message(FATAL_ERROR "CMake/common missing, run: git submodule update --init")
endif()

# Default build type is Debug
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug )
endif( NOT CMAKE_BUILD_TYPE )

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
  add_definitions( -DDEBUG )
endif( )

include(GitExternal)

set( CMAKECOMMONTEMPLATE_DESCRIPTION "ReTo - Render Tools" )
set( COMMON_PROJECT_DOMAIN vg-lab.es )


option( RETO_WITH_EXAMPLES "RETO_WITH_EXAMPLES" ON )

option( RETO_WITH_COMPUTE_SHADERS "enabling/disabling compute shaders" ON )
option( RETO_WITH_GEOMETRY_SHADERS "enabling/disabling geometry shaders" ON )
option( RETO_WITH_TESSELATION_SHADERS
  "enabling/disabling tesselation shaders" ON )
option( RETO_WITH_SUBPROGRAMS "enabling/disabling subprograms" ON )
option( RETO_WITH_OCC_QUERY "enabling/disabling occlusion query" ON )
option( RETO_WITH_TRANSFORM_FEEDBACK "enabling/disabling transform feedback" ON )

if ( RETO_WITH_COMPUTE_SHADERS )
  list(APPEND COMMON_FIND_PACKAGE_DEFINES RETO_COMPUTE_SHADERS)
endif( )

if ( RETO_WITH_GEOMETRY_SHADERS )
  list(APPEND COMMON_FIND_PACKAGE_DEFINES RETO_GEOMETRY_SHADERS)
endif( )

if ( RETO_WITH_TESSELATION_SHADERS )
  list(APPEND COMMON_FIND_PACKAGE_DEFINES RETO_TESSELATION_SHADERS)
endif( )

if ( RETO_WITH_SUBPROGRAMS )
  list(APPEND COMMON_FIND_PACKAGE_DEFINES RETO_SUBPROGRAMS)
endif( )

if ( RETO_WITH_OCC_QUERY )
  list(APPEND COMMON_FIND_PACKAGE_DEFINES RETO_OCC_QUERY)
endif( )

if ( RETO_WITH_TRANSFORM_FEEDBACK )
  list(APPEND COMMON_FIND_PACKAGE_DEFINES RETO_TRANSFORM_FEEDBACK)
endif( )


include( Common )

common_find_package(OpenGL REQUIRED SYSTEM)
common_find_package(GLEW REQUIRED SYSTEM)
common_find_package(Eigen3 REQUIRED SYSTEM)
common_find_package(GLUT SYSTEM)
common_find_package(Boost COMPONENTS unit_test_framework)
common_find_package(ZeroEQ)
common_find_package(Lexis)
common_find_package(FreeImage)

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

if(FREEIMAGE_FOUND)
  list(APPEND RETO_DEPENDENT_LIBRARIES FreeImage)
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

set( DOCS README.md LICENSE.txt )
install( FILES ${DOCS} DESTINATION share/ReTo COMPONENT dev )

set( DOXYGEN_MAINPAGE_MD README.md )
set( DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md )

include( DoxygenRule )

include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/glutExampleShaders.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/glutExampleShaders.h )

if ( GLUT_FOUND )

  set( RETOSPLINEDEMO_HEADERS )
  set( RETOSPLINEDEMO_SOURCES splineExample.cpp )
  set( RETOSPLINEDEMO_LINK_LIBRARIES
	  ReTo
    )
  common_application( ReToSplineDemo )

  set( RETOSHADERPROGRAM_HEADERS )
  set( RETOSHADERPROGRAM_SOURCES shaderProgram.cpp )
  set( RETOSHADERPROGRAM_LINK_LIBRARIES
	  ReTo
	  ${GLUT_LIBRARIES}
    )
  common_application( ReToShaderProgram )

  set( RETOGLUTEXAMPLE_HEADERS )
  set( RETOGLUTEXAMPLE_SOURCES glutExample.cpp )
  set( RETOGLUTEXAMPLE_LINK_LIBRARIES
	  ReTo
	  ${GLUT_LIBRARIES}
    )
  common_application( ReToGlutExample )

endif( )

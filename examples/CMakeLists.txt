include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/glutExampleShaders.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/glutExampleShaders.h )

if ( GLUT_FOUND )

  set( RETODEMOCUBES_HEADERS
    MyCube.h
  )
  set( RETODEMOCUBES_SOURCES
    MyCube.cpp
    demoCubes.cpp
  )
  set( RETODEMOCUBES_LINK_LIBRARIES
    ReTo
    ${GLUT_LIBRARIES}
  )
  common_application( ReToDemoCubes )

  set( RETOSHADERPROGRAM_HEADERS )
  set( RETOSHADERPROGRAM_SOURCES shaderProgram.cpp )
  set( RETOSHADERPROGRAM_LINK_LIBRARIES
	  ReTo
	  ${GLUT_LIBRARIES}
  )
  common_application( ReToShaderProgram )

  set( RETODEMOPICKING_HEADERS
    MyCube.h
  )
  set( RETODEMOPICKING_SOURCES
    MyCube.cpp
    pickDemo.cpp
  )
  set( RETODEMOPICKING_LINK_LIBRARIES
	  ReTo
	  ${GLUT_LIBRARIES}
  )
  common_application( ReToDemoPicking )

endif( )

set( RETOFILEREADER_HEADERS )
set( RETOFILEREADER_SOURCES fileReader.cpp )
set( RETOFILEREADER_LINK_LIBRARIES
  ReTo
)
common_application( ReToFileReader )

set( RETOSPLINEDEMO_HEADERS )
set( RETOSPLINEDEMO_SOURCES splineExample.cpp )
set( RETOSPLINEDEMO_LINK_LIBRARIES
  ReTo
)
common_application( ReToSplineDemo )

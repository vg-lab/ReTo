if( RETO_WITH_EXAMPLES )

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

    set( RETOTEXTUREREADER_HEADERS
      MyCube.h
    )
    set( RETOTEXTUREREADER_SOURCES
      MyCube.cpp
      textureReader.cpp
    )
    set( RETOTEXTUREREADER_LINK_LIBRARIES
      ReTo
      ${GLUT_LIBRARIES}
    )
    common_application( ReToTextureReader )

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


    set( RETODEMOPICKING2_HEADERS
      MyCube.h
    )
    set( RETODEMOPICKING2_SOURCES
      MyCube.cpp
      pickDemo2.cpp
    )
    set( RETODEMOPICKING2_LINK_LIBRARIES
      ReTo
      ${GLUT_LIBRARIES}
    )
    common_application( ReToDemoPicking2 )

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

endif( )

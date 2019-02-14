if( RETO_WITH_EXAMPLES )

  include_directories(
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
  )

  list( APPEND RETO_EXAMPLES_FILES
    color.vert pick.vert colorUniform.vert
    color.frag pick.frag colorUniform.frag
  )

  # configure_file(
  #   ${CMAKE_CURRENT_SOURCE_DIR}/glutExampleShaders.h.in
  #   ${CMAKE_CURRENT_BINARY_DIR}/glutExampleShaders.h )

  if ( GLUT_FOUND )

    set( RETODEMOCUBES_HEADERS MyCube.h )
    set( RETODEMOCUBES_SOURCES MyCube.cpp demoCubes.cpp )
    set( RETODEMOCUBES_LINK_LIBRARIES ReTo ${GLUT_LIBRARIES} )
    common_application( ReToDemoCubes NOHELP )
    list( APPEND RETO_EXAMPLES_FILES ${RETODEMOCUBES_SOURCES} )


    if (RETO_USE_FREEIMAGE)
      set( RETOTEXTUREREADER_HEADERS MyCube.h )
      set( RETOTEXTUREREADER_SOURCES MyCube.cpp textureReader.cpp )
      set( RETOTEXTUREREADER_LINK_LIBRARIES ReTo ${GLUT_LIBRARIES} )
      common_application( ReToTextureReader NOHELP )
      list( APPEND RETO_EXAMPLES_FILES ${RETOTEXTUREREADER_SOURCES} )
    endif( )

    set( RETOSHADERPROGRAM_HEADERS )
    set( RETOSHADERPROGRAM_SOURCES shaderProgram.cpp )
    set( RETOSHADERPROGRAM_LINK_LIBRARIES ReTo ${GLUT_LIBRARIES} )
    common_application( ReToShaderProgram NOHELP )
    list( APPEND RETO_EXAMPLES_FILES ${RETOSHADERPROGRAM_SOURCES} )

    set( RETODEMOPICKING_HEADERS MyCube.h )
    set( RETODEMOPICKING_SOURCES MyCube.cpp pickDemo.cpp )
    set( RETODEMOPICKING_LINK_LIBRARIES ReTo ${GLUT_LIBRARIES} )
    common_application( ReToDemoPicking NOHELP )
    list( APPEND RETO_EXAMPLES_FILES ${RETODEMOPICKING_SOURCES} )


    set( RETODEMOPICKING2_HEADERS MyCube.h )
    set( RETODEMOPICKING2_SOURCES MyCube.cpp pickDemo2.cpp )
    set( RETODEMOPICKING2_LINK_LIBRARIES ReTo ${GLUT_LIBRARIES} )
    common_application( ReToDemoPicking2 NOHELP )
    list( APPEND RETO_EXAMPLES_FILES ${RETODEMOPICKING2_SOURCES} )


    set( RETODEMORUBBERBAND_HEADERS MyCube.h )
    set( RETODEMORUBBERBAND_SOURCES MyCube.cpp rubberBandDemo.cpp )
    set( RETODEMORUBBERBAND_LINK_LIBRARIES ReTo ${GLUT_LIBRARIES} )
    common_application( ReToDemoRubberBand NOHELP )
    list( APPEND RETO_EXAMPLES_FILES ${RETODEMORUBBERBAND_SOURCES} )

    set( RETODEMOLASSO_HEADERS MyCube.h )
    set( RETODEMOLASSO_SOURCES MyCube.cpp lassoDemo.cpp )
    set( RETODEMOLASSO_LINK_LIBRARIES ReTo ${GLUT_LIBRARIES} )
    common_application( ReToDemoLasso NOHELP )
    list( APPEND RETO_EXAMPLES_FILES ${RETODEMOLASSO_SOURCES} )

    set( RETODEMOCLIPPINGPLANES_HEADERS MyCube.h )
    set( RETODEMOCLIPPINGPLANES_SOURCES MyCube.cpp clippingPlanesDemo.cpp )
    set( RETODEMOCLIPPINGPLANES_LINK_LIBRARIES ReTo ${GLUT_LIBRARIES} )
    common_application( ReToDemoClippingPlanes NOHELP )
    list( APPEND RETO_EXAMPLES_FILES ${RETODEMOCLIPPINGPLANES_SOURCES} )

  endif( )

  set( RETOFILEREADER_HEADERS )
  set( RETOFILEREADER_SOURCES fileReader.cpp )
  set( RETOFILEREADER_LINK_LIBRARIES ReTo )
  common_application( ReToFileReader NOHELP )
  list( APPEND RETO_EXAMPLES_FILES ${RETOFILEREADER_SOURCES} )

  set( RETOSPLINEDEMO_HEADERS )
  set( RETOSPLINEDEMO_SOURCES splineExample.cpp )
  set( RETOSPLINEDEMO_LINK_LIBRARIES ReTo )
  common_application( ReToSplineDemo NOHELP )
  list( APPEND RETO_EXAMPLES_FILES ${RETOSPLINEDEMO_SOURCES} )

  include(InstallFiles)
  install_files(share/ReTo/examples FILES ${RETO_EXAMPLES_FILES} COMPONENT
    examples )

endif( )

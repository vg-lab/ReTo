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
    
   endif() 
   
 endif() 
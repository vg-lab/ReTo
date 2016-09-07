source_group(\\ FILES CMakeLists.txt)

set(RETO_PUBLIC_HEADERS
  Camera.h
  ShaderProgram.h
  Pickable.h
  PickingSystem.h
  Spline.h
)

set(RETO_HEADERS )

set(RETO_SOURCES
  Camera.cpp
  ShaderProgram.cpp
  Pickable.cpp
  PickingSystem.cpp
  Spline.cpp
)

set(RETO_LINK_LIBRARIES
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
)

if ( ZEROEQ_FOUND )
  set(RETO_LINK_LIBRARIES ${RETO_LINK_LIBRARIES} ZeroEQ )
endif( )

if ( LEXIS_FOUND )
  set(RETO_LINK_LIBRARIES ${RETO_LINK_LIBRARIES} Lexis )
endif( )

common_library( ReTo )

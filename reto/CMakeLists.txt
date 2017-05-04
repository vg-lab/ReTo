source_group(\\ FILES CMakeLists.txt)

set(RETO_PUBLIC_HEADERS
  ObjParser.h
  Camera.h
  CameraController.h
  ShaderProgram.h
  Path.h
  Pickable.h
  PickingSystem.h
  TextureManager.h
)

set(RETO_HEADERS )

set(RETO_SOURCES
  ObjParser.cpp
  Camera.cpp
  CameraController.cpp
  ShaderProgram.cpp
  Path.cpp
  Pickable.cpp
  PickingSystem.cpp
  TextureManager.cpp
)

set(RETO_LINK_LIBRARIES
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${FREEIMAGE_LIBRARIES}
)

if ( ZEROEQ_FOUND )
  set(RETO_LINK_LIBRARIES ${RETO_LINK_LIBRARIES} ZeroEQ )
endif( )

if ( LEXIS_FOUND )
  set(RETO_LINK_LIBRARIES ${RETO_LINK_LIBRARIES} Lexis )
endif( )

common_library( ReTo )

source_group(\\ FILES CMakeLists.txt)

set(RETO_PUBLIC_HEADERS
  Camera.h
  ShaderProgram.h
  Pickable.h
  PickingSystem.h
)

set(RETO_HEADERS )

set(RETO_SOURCES
  Camera.cpp
  ShaderProgram.cpp
  Pickable.cpp
  PickingSystem.cpp
)

set(RETO_LINK_LIBRARIES
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
)

common_library( ReTo )

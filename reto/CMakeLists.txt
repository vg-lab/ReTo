source_group(\\ FILES CMakeLists.txt)

set(RETO_PUBLIC_HEADERS
  Camera.h
  #PickableObject.h
  ProgramShader.h
  Model.h
  Pickable.h
)

set(RETO_HEADERS )

set(RETO_SOURCES
  Camera.cpp
  #PickableObject.cpp
  ProgramShader.cpp
  Pickable.cpp
)

set(RETO_LINK_LIBRARIES
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
)

common_library( ReTo )

source_group(\\ FILES CMakeLists.txt)

set(RETO_PUBLIC_HEADERS
  Camera.h
  PickableObject.h
  ProgramShader.h
)

set(RETO_HEADERS )

set(RETO_SOURCES
  Camera.cpp
  PickableObject.cpp
  ProgramShader.cpp
)

common_library( ReTo )

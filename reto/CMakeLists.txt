source_group(\\ FILES CMakeLists.txt)

set(RETO_PUBLIC_HEADERS
  Camera.h
  ProgramShader.h
  Pickable.h
  PickingSystem.h
)

set(RETO_HEADERS )

set(RETO_SOURCES
  Camera.cpp
  ProgramShader.cpp
  Pickable.cpp
  PickingSystem.cpp
)

link_directories(${NVIDIA_OPENGL_gl_LIBRARY_PATH})

set(RETO_LINK_LIBRARIES
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
)

common_library( ReTo )

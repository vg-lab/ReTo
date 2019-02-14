source_group(\\ FILES CMakeLists.txt)

set(RETO_PUBLIC_HEADERS
  ObjParser.h
  CameraAnimation.h
  Camera.h
  AbstractCameraController.h
  OrbitalCameraController.h
  FreeCameraController.h
  ShaderProgram.h
  Pickable.h
  PickingSystem.h
  Spline.h
  TextureManager.h
  TransformFeedback.h
  Framebuffer.h
  SelectionSystem.h
  ClippingSystem.h
)

set(RETO_HEADERS )

set(RETO_SOURCES
  ObjParser.cpp
  CameraAnimation.cpp
  Camera.cpp
  AbstractCameraController.cpp
  OrbitalCameraController.cpp
  FreeCameraController.cpp
  ShaderProgram.cpp
  Pickable.cpp
  PickingSystem.cpp
  Spline.cpp
  TextureManager.cpp
  TransformFeedback.cpp
  Framebuffer.cpp
  SelectionSystem.cpp
  ClippingSystem.cpp
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

#if( MSVC )
#  add_definitions( -DGLEW_STATIC )
#endif( MSVC )

common_library( ReTo )

source_group(\\ FILES CMakeLists.txt)

set(RETO_PUBLIC_HEADERS
  ObjParser.h
  Camera.h
  ShaderProgram.h
  Pickable.h
  PickingSystem.h
  Spline.h
  TextureManager.h
  FrameRecorderWidget.h
)

set(RETO_HEADERS )

set(RETO_SOURCES
  resources.qrc
  ObjParser.cpp
  Camera.cpp
  ShaderProgram.cpp
  Pickable.cpp
  PickingSystem.cpp
  Spline.cpp
  TextureManager.cpp
  FrameRecorderWidget.cpp
  resources.qrc
)

set(RETO_LINK_LIBRARIES
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${FREEIMAGE_LIBRARIES}
  Qt5::Core
  Qt5::Widgets
  x264
  x265
  swscale
  avutil
  avformat
  avcodec
  ${FFMPEG_LIBRARIES}
  ${SDL_LIBRARY}
)

if ( ZEROEQ_FOUND )
  set(RETO_LINK_LIBRARIES ${RETO_LINK_LIBRARIES} ZeroEQ )
endif( )

if ( LEXIS_FOUND )
  set(RETO_LINK_LIBRARIES ${RETO_LINK_LIBRARIES} Lexis )
endif( )

add_definitions( -DRETO_SHARED )

common_library( ReTo )

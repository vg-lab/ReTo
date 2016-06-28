include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}
)

set( RETOGLUTCAMERA_HEADERS )
set( RETOGLUTCAMERA_SOURCES glutCamera.cpp )
set( RETOGLUTCAMERA_LINK_LIBRARIES ReTo )
common_application( ReToGlutCamera )

set( RETOQTCAMERA_HEADERS )
set( RETOQTCAMERA_SOURCES qtCamera.cpp )
set( RETOQTCAMERA_LINK_LIBRARIES ReTo )
common_application( ReToQtCamera )

set( RETOSHADERPROGRAM_HEADERS )
set( RETOSHADERPROGRAM_SOURCES test.cpp )
set( RETOSHADERPROGRAM_LINK_LIBRARIES 
	ReTo
#	${GLUT_LIBRARIES}
)
#message(${OPENGL_LIBRARIES})
common_application( ReToShaderProgram )
# Define public headers
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

# Define source files
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

if(HDF5_FOUND AND RETO_USE_HDF5)
  list(APPEND RETO_PUBLIC_HEADERS Reader/VasculatureReader.h)
endif()

if(WIN32)
  configure_file(
    "${PROJECT_SOURCE_DIR}/reto/ReToWinProps.rc.in"
    "${PROJECT_BINARY_DIR}/ReToWinProps.rc"
  )
  set(RETO_SOURCES ${RETO_SOURCES} "${PROJECT_BINARY_DIR}/ReToWinProps.rc")
endif()

# Create the includes strings for the unified header
set(HEADER_INCLUDES "")
foreach(header ${RETO_PUBLIC_HEADERS})
  string(APPEND HEADER_INCLUDES "#include <${PROJECT_NAME_LOWER}/${header}>\n")
endforeach()

# Generate the unified header
configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/unified_header.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}.h"
)

# Get the current Git commit for the version header
find_package(Git)
if(Git_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} --no-pager log -n 1 HEAD "--pretty=format:%h"
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    RESULT_VARIABLE git_result
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if(git_result EQUAL 0)
    message(STATUS "Last ${PROJECT_NAME} Git commit: ${GIT_COMMIT_HASH}")
  endif()
endif()

# Generate the version header
configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

# Create the target
add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Generate export header
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

# Add sources to the target
target_sources(${PROJECT_NAME}
  PRIVATE
    ${RETO_SOURCES}
  PUBLIC
    FILE_SET public_headers
    TYPE HEADERS
    BASE_DIRS
      ${PROJECT_SOURCE_DIR}
    FILES
      ${RETO_PUBLIC_HEADERS}
)

# Add generated sources to the target
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET public_headers_generated
    TYPE HEADERS
    BASE_DIRS
      ${PROJECT_BINARY_DIR}
    FILES
      # Generated headers
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}.h
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}_export.h
      ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

# Add compile definitions
target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    $<$<BOOL:${RETO_WITH_COMPUTE_SHADERS}>:RETO_COMPUTE_SHADERS>
    $<$<BOOL:${RETO_WITH_GEOMETRY_SHADERS}>:RETO_GEOMETRY_SHADERS>
    $<$<BOOL:${RETO_WITH_TESSELATION_SHADERS}>:RETO_TESSELATION_SHADERS>
    $<$<BOOL:${RETO_WITH_SUBPROGRAMS}>:RETO_SUBPROGRAMS>
    $<$<BOOL:${RETO_WITH_OCC_QUERY}>:RETO_OCC_QUERY>
    $<$<BOOL:${RETO_WITH_TRANSFORM_FEEDBACK}>:RETO_TRANSFORM_FEEDBACK>
    $<$<BOOL:${RETO_USE_GLUT}>:RETO_USE_GLUT>
    $<$<BOOL:${RETO_USE_FREEIMAGE}>:RETO_USE_FREEIMAGE>
    $<$<BOOL:${RETO_USE_LEXIS}>:RETO_USE_LEXIS>
    $<$<BOOL:${RETO_USE_ZEROEQ}>:RETO_USE_ZEROEQ>
  PRIVATE
    # $<$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>:DEBUG>
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    OpenGL::GL
    GLEW::GLEW
    Eigen3::Eigen
    "$<$<BOOL:${RETO_USE_GLUT}>:GLUT::GLUT>"
    "$<$<BOOL:${RETO_USE_LEXIS}>:Lexis>"
    "$<$<BOOL:${RETO_USE_ZEROEQ}>:ZeroEQ>"
  PRIVATE
    "$<$<BOOL:${RETO_USE_FREEIMAGE}>:freeimage>"
)
include_directories( ${CMAKE_BINARY_DIR}/include )
include_directories( ${PROJECT_SOURCE_DIR} )

if (${Boost_FOUND})
  if (${GLUT_FOUND})
    set(TEST_LIBRARIES
      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
      ReTo
      ${GLUT_LIBRARIES})
      include(CommonCTest)
    endif()
endif()

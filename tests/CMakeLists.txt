
configure_file(${PROJECT_SOURCE_DIR}/tests/testData.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/testData.h)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )


if (${Boost_FOUND})
  if (${GLUT_FOUND})
    set(TEST_LIBRARIES
      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
      ReTo
      ${GLUT_LIBRARIES})
      include(CommonCTest)
    endif()
endif()

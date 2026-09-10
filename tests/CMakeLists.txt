if(RETO_WITH_TESTS)

  # Configure the test data header
  configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/testData.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/testData.h
  )

  # Function to add a test
  function(add_project_test NAME SOURCE)
    add_executable(${NAME} ${SOURCE})
    target_include_directories(${NAME}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
    )
    target_link_libraries(${NAME} PRIVATE
      ${PROJECT_NAME}
      Boost::unit_test_framework
    )
    add_test(NAME ${NAME} COMMAND ${NAME})

    # Create all tests target
    if(NOT TARGET ${PROJECT_NAME}_all_tests)
      add_custom_target(${PROJECT_NAME}_all_tests)
    endif()

    # Add test to global tests target
    add_dependencies(${PROJECT_NAME}_all_tests ${NAME})
  endfunction()

  # Add tests
  add_project_test(ReToTestCamera Camera.cpp)
  add_project_test(ReToTestMeshLoader meshLoader.cpp)
  add_project_test(ReToTestSpline spline.cpp)

  if(RETO_USE_GLUT)
    if(UNIX AND NOT APPLE AND DEFINED ENV{DISPLAY} AND NOT "$ENV{DISPLAY}" STREQUAL "")
      add_project_test(ReToTestShaderProgram ShaderProgram.cpp)
      add_project_test(ReToTestTexture texture.cpp)
    else()
      message(WARNING "GLUT tests disabled: DISPLAY environment variable is not available")
    endif()
  endif()

endif()

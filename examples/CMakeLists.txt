if(RETO_WITH_EXAMPLES)

  # Function to create and install an example application
  function(add_project_example)
    set(options)
    set(oneValueArgs TARGET_NAME)
    set(multiValueArgs SOURCES LINK_LIBRARIES)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

    if(NOT ARG_TARGET_NAME OR NOT ARG_SOURCES)
      message(FATAL_ERROR "TARGET_NAME and SOURCES are required arguments")
    endif()

    add_executable(${ARG_TARGET_NAME} ${ARG_SOURCES})
    target_link_libraries(${ARG_TARGET_NAME} PRIVATE ${PROJECT_NAME} ${ARG_LINK_LIBRARIES})

    # Get current parent scope value and create a local copy
    set(PROJECT_EXAMPLES_FILES_VAR "${${PROJECT_NAME}_EXAMPLES_FILES}")
    set(current_examples ${PROJECT_EXAMPLES_FILES_VAR})
    
    # Convert to CMake list and append new sources
    list(APPEND current_examples ${ARG_SOURCES})
    
    # Remove duplicates and update parent scope
    list(REMOVE_DUPLICATES current_examples)
    set(${PROJECT_NAME}_EXAMPLES_FILES "${current_examples}" PARENT_SCOPE)

    # Create all examples target
    if(NOT TARGET ${PROJECT_NAME}_all_examples)
      add_custom_target(${PROJECT_NAME}_all_examples)
    endif()

    # Add example to global examples target
    add_dependencies(${PROJECT_NAME}_all_examples ${ARG_TARGET_NAME})
  endfunction()

  # Examples files
  list(APPEND RETO_EXAMPLES_FILES
    color.vert pick.vert colorUniform.vert
    color.frag pick.frag colorUniform.frag
  )

  # ReToFileReader
  add_project_example(
    TARGET_NAME ReToFileReader
    SOURCES 
      MyCube.cpp
      fileReader.cpp
  )

  # ReToSplineDemo
  add_project_example(
    TARGET_NAME ReToSplineDemo
    SOURCES 
      MyCube.cpp
      splineExample.cpp
  )

  if(RETO_USE_GLUT)
    # ReToDemoCubes
    add_project_example(
      TARGET_NAME ReToDemoCubes
      SOURCES 
        MyCube.cpp
        demoCubes.cpp
    )

    # ReToTextureReader
    add_project_example(
      TARGET_NAME ReToTextureReader
      SOURCES 
        MyCube.cpp
        textureReader.cpp
    )

    # ReToShaderProgram
    add_project_example(
      TARGET_NAME ReToShaderProgram
      SOURCES 
        MyCube.cpp
        shaderProgram.cpp
    )

    # ReToDemoPicking
    add_project_example(
      TARGET_NAME ReToDemoPicking
      SOURCES 
        MyCube.cpp
        pickDemo.cpp
    )

    # ReToDemoPicking2
    add_project_example(
      TARGET_NAME ReToDemoPicking2
      SOURCES 
        MyCube.cpp
        pickDemo2.cpp
    )

    # ReToDemoRubberBand
    add_project_example(
      TARGET_NAME ReToDemoRubberBand
      SOURCES 
        MyCube.cpp
        rubberBandDemo.cpp
    )

    # ReToDemoLasso
    add_project_example(
      TARGET_NAME ReToDemoLasso
      SOURCES 
        MyCube.cpp
        lassoDemo.cpp
    )

    # ReToDemoClippingPlanes
    add_project_example(
      TARGET_NAME ReToDemoClippingPlanes
      SOURCES 
        MyCube.cpp
        clippingPlanesDemo.cpp
    )

    # ReToDemoClippingPlanes2
    add_project_example(
      TARGET_NAME ReToDemoClippingPlanes2
      SOURCES 
        MyCube.cpp
        clippingPlanes2Demo.cpp
    )

    # ReToDemoClippingPlanesNoCSystem
    add_project_example(
      TARGET_NAME ReToDemoClippingPlanesNoCSystem
      SOURCES 
        MyCube.cpp
        clippingPlanesNoCSystemDemo.cpp
    )
  endif()

  # Define installation directory variables
  include(GNUInstallDirs)

  # Install examples source files
  install(FILES ${${PROJECT_NAME}_EXAMPLES_FILES}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME_LOWER}/examples
    COMPONENT ${PROJECT_NAME}_examples
  )

endif()
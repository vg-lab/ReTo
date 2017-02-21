if( NOT PYTHON_EXECUTABLE )
  find_package( PythonInterp REQUIRED )
  message( STATUS  "Using python: ${PYTHON_EXECUTABLE}")
endif( )

find_file( RETO_GENERATE_SHADERS reto_generate_shaders.py HINTS
  ${CMAKE_CURRENT_LIST_DIR}/../../../bin # from share/ReTo/CMake (!Windows)
  ${CMAKE_CURRENT_LIST_DIR}/../../bin # from ReTo/CMake (Windows)
  ${CMAKE_CURRENT_LIST_DIR}/../bin # from build dir
  )

if( NOT RETO_GENERATE_SHADERS )
  message( FATAL_ERROR "reto_generate_shaders.py not found" )
endif( )

function( reto_generate_shaders
    __RETO_GEN_SHADERS_DECLARATION__
    __RETO_GEN_SHADERS_NAMESPACE__
    __RETO_GEN_SHADERS_PATH__
    __RETO_GEN_SHADERS_FILE__
    )
  file(GLOB __SHADERS__ ${__RETO_GEN_SHADERS_PATH__}/* )

  add_custom_command(
    COMMAND ${PYTHON_EXECUTABLE} ${RETO_GENERATE_SHADERS}
    ARGS
    -d ${__RETO_GEN_SHADERS_DECLARATION__}
    -n ${__RETO_GEN_SHADERS_NAMESPACE__}
    -r ${__RETO_GEN_SHADERS_PATH__}
    -f ${__RETO_GEN_SHADERS_FILE__}
    DEPENDS ${__SHADERS__}
    OUTPUT ${__RETO_GEN_SHADERS_FILE__}
    MAIN_DEPENDENCY ${__SHADERS__}
  )
endfunction( )

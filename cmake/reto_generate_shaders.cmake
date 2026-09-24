find_package(Python REQUIRED COMPONENTS Interpreter)

# Find the script
include(GNUInstallDirs)
find_file(RETO_GENERATE_SHADERS reto_generate_shaders.py
  PATHS
    ${CMAKE_CURRENT_LIST_DIR}/../bin
    ${CMAKE_CURRENT_LIST_DIR}/bin
    ${CMAKE_INSTALL_DATADIR}/reto/bin
  NO_DEFAULT_PATH
)

if(NOT RETO_GENERATE_SHADERS)
  message(FATAL_ERROR "reto_generate_shaders.py not found")
endif()

function(reto_generate_shaders
  __RETO_GEN_SHADERS_DECLARATION__
  __RETO_GEN_SHADERS_NAMESPACE__
  __RETO_GEN_SHADERS_PATH__
  __RETO_GEN_SHADERS_FILE__
)
  file(
    GLOB __SHADERS__
    "${__RETO_GEN_SHADERS_PATH__}/*"
  )

  # Configure time
  execute_process(
    COMMAND ${Python_EXECUTABLE} ${RETO_GENERATE_SHADERS}
      -d ${__RETO_GEN_SHADERS_DECLARATION__}
      -n ${__RETO_GEN_SHADERS_NAMESPACE__}
      -r ${__RETO_GEN_SHADERS_PATH__}
      -f ${__RETO_GEN_SHADERS_FILE__}
    OUTPUT_VARIABLE _shader_gen_output
    RESULT_VARIABLE _shader_gen_result
    ERROR_VARIABLE _shader_gen_err
  )
  if(NOT _shader_gen_result EQUAL 0)
    message(FATAL_ERROR "Shader generation failed at configure time:\n${_shader_gen_err}")
  endif()

  # Build time
  add_custom_command(
    OUTPUT ${__RETO_GEN_SHADERS_FILE__}
    COMMAND ${Python_EXECUTABLE} ${RETO_GENERATE_SHADERS}
      -d ${__RETO_GEN_SHADERS_DECLARATION__}
      -n ${__RETO_GEN_SHADERS_NAMESPACE__}
      -r ${__RETO_GEN_SHADERS_PATH__}
      -f ${__RETO_GEN_SHADERS_FILE__}
    DEPENDS ${__SHADERS__}
    COMMENT "Generating shaders header"
  )
endfunction()

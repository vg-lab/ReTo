install( FILES reto_generate_shaders.py DESTINATION bin COMPONENT bin )
configure_file(
  reto_generate_shaders.py
  ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY
  )

install( FILES reto_generate_shaders.py DESTINATION bin COMPONENT bin )
configure_file(
  reto_generate_shaders.py
  ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY
)

install( FILES reto_shaders_import.py DESTINATION bin COMPONENT bin )
configure_file(
  reto_shaders_import.py
  ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY
)

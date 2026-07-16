include(GNUInstallDirs)

configure_file(
  reto_generate_shaders.py
  ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY
)

configure_file(
  reto_shaders_import.py
  ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY
)

install(
  FILES
    reto_generate_shaders.py
    reto_shaders_import.py
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME_LOWER}/bin
  COMPONENT ${PROJECT_NAME}_bin
)
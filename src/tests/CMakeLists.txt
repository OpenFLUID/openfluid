

ADD_SUBDIRECTORY(functions)

ADD_SUBDIRECTORY(observers)

ADD_SUBDIRECTORY(integration)
  
#CONFIGURE_FILE(tests-config.hpp.in ${PROJECT_BINARY_DIR}/src/tests/tests-config.hpp)
CONFIGURE_FILE(tests-config.hpp.in tests-config.hpp)
  
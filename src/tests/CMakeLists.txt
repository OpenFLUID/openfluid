

INCLUDE(OpenFLUIDHelpers)


ADD_SUBDIRECTORY(simulators)

ADD_SUBDIRECTORY(observers)

ADD_SUBDIRECTORY(integration)

  
CONFIGURE_FILE(tests-config.hpp.in tests-config.hpp)
  
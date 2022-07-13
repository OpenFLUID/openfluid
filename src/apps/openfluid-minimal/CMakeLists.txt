

ADD_EXECUTABLE(openfluid-minimal    
               main.cpp)

SET_TARGET_PROPERTIES(openfluid-minimal PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_BIN_DIR}")

TARGET_LINK_LIBRARIES(openfluid-minimal
                      openfluid-base
                      openfluid-core
                      openfluid-machine
                      openfluid-tools
                      openfluid-fluidx
                     )

IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()

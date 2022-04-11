

ADD_EXECUTABLE(openfluid-waretool    
               main.cpp)

SET_TARGET_PROPERTIES(openfluid-waretool PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_BIN_DIR}")

TARGET_LINK_LIBRARIES(openfluid-waretool
                      openfluid-tools
                     )

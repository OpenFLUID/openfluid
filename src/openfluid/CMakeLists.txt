

ADD_DEFINITIONS(-DBUILDINGDLL)

ADD_SUBDIRECTORY(base)
ADD_SUBDIRECTORY(buddies)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(machine)
ADD_SUBDIRECTORY(io)
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(market)
ADD_SUBDIRECTORY(builderext)
ADD_SUBDIRECTORY(guicommon)


INSTALL(FILES base.hpp buddies.hpp core.hpp machine.hpp io.hpp tools.hpp market.hpp 
              dllexport.hpp debug.hpp 
        DESTINATION ${INCLUDE_INSTALL_PATH})
       
CONFIGURE_FILE(openfluid.pc.in "${PKGCONFIG_OUTPUT_PATH}/openfluid.pc" @ONLY)

CONFIGURE_FILE(config.hpp.in "${PROJECT_BINARY_DIR}/src/openfluid/config.hpp" @ONLY)

INSTALL(FILES "${PKGCONFIG_OUTPUT_PATH}/openfluid.pc" 
        DESTINATION ${PKGCONFIG_INSTALL_PATH})

INSTALL(FILES "${PROJECT_BINARY_DIR}/src/openfluid/config.hpp" 
        DESTINATION ${INCLUDE_INSTALL_PATH})
               
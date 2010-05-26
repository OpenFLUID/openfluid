

ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")


ADD_SUBDIRECTORY(base)
ADD_SUBDIRECTORY(buddies)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(engine)
ADD_SUBDIRECTORY(io)
ADD_SUBDIRECTORY(tools)


INSTALL(FILES base.hpp buddies.hpp core.hpp engine.hpp io.hpp tools.hpp dllexport.hpp  
        DESTINATION ${INCLUDE_INSTALL_PATH})
       
CONFIGURE_FILE(openfluid.pc.in "${PKGCONFIG_OUTPUT_PATH}/openfluid.pc" @ONLY)

CONFIGURE_FILE(config.hpp.in "${PROJECT_BINARY_DIR}/src/openfluid/config.hpp" @ONLY)

INSTALL(FILES "${PKGCONFIG_OUTPUT_PATH}/openfluid.pc" 
        DESTINATION ${PKGCONFIG_INSTALL_PATH})

INSTALL(FILES "${PROJECT_BINARY_DIR}/src/openfluid/config.hpp" 
        DESTINATION ${INCLUDE_INSTALL_PATH})
               
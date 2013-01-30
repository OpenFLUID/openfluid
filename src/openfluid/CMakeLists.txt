

ADD_DEFINITIONS(-DBUILDINGDLL)

ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(fluidx)
ADD_SUBDIRECTORY(base)
ADD_SUBDIRECTORY(ware)
ADD_SUBDIRECTORY(machine)
ADD_SUBDIRECTORY(io)
ADD_SUBDIRECTORY(market)
ADD_SUBDIRECTORY(buddies)

IF(OPENFLUID_ENABLE_GUI)
  ADD_SUBDIRECTORY(builderext)
  IF (BUILD_APP_BUILDER)
    ADD_SUBDIRECTORY(guicommon)
  ENDIF()
ENDIF()


ADD_SUBDIRECTORY(landr)


INSTALL(FILES base.hpp buddies.hpp core.hpp machine.hpp io.hpp tools.hpp market.hpp 
              dllexport.hpp debug.hpp deprecation.hpp ware.hpp
        DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH})


CONFIGURE_FILE(OpenFLUIDConfig.cmake.in "${CMAKE_MODULES_OUTPUT_PATH}/OpenFLUIDConfig.cmake" @ONLY)
CONFIGURE_FILE(OpenFLUIDConfigVersion.cmake.in "${CMAKE_MODULES_OUTPUT_PATH}/OpenFLUIDConfigVersion.cmake" @ONLY)


CONFIGURE_FILE(openfluid.pc.in "${PKGCONFIG_OUTPUT_PATH}/openfluid.pc" @ONLY)
CONFIGURE_FILE(openfluid-gui.pc.in "${PKGCONFIG_OUTPUT_PATH}/openfluid-gui.pc" @ONLY)
CONFIGURE_FILE(openfluid-other.pc.in "${PKGCONFIG_OUTPUT_PATH}/openfluid-other.pc" @ONLY)

CONFIGURE_FILE(config.hpp.in "${PROJECT_BINARY_DIR}/src/openfluid/config.hpp" @ONLY)

INSTALL(FILES "${CMAKE_MODULES_OUTPUT_PATH}/OpenFLUIDConfig.cmake" 
        DESTINATION ${CMAKE_MODULES_INSTALL_PATH})
        
INSTALL(FILES "${CMAKE_MODULES_OUTPUT_PATH}/OpenFLUIDConfigVersion.cmake" 
        DESTINATION ${CMAKE_MODULES_INSTALL_PATH})        

INSTALL(FILES "${PKGCONFIG_OUTPUT_PATH}/openfluid.pc" 
              "${PKGCONFIG_OUTPUT_PATH}/openfluid-gui.pc"
              "${PKGCONFIG_OUTPUT_PATH}/openfluid-other.pc"
        DESTINATION ${PKGCONFIG_INSTALL_PATH})

INSTALL(FILES "${PROJECT_BINARY_DIR}/src/openfluid/config.hpp" 
        DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH})
               
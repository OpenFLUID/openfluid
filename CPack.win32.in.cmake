SET(CPACK_GENERATOR "NSIS")
SET(CPACK_SOURCE_GENERATOR "ZIP")
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/resources/packaging/windows;${CMAKE_MODULE_PATH}")

SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OpenFLUID-${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}")
  
SET(CPACK_PACKAGE_NAME "OpenFLUID")
  
SET(CPACK_NSIS_DISPLAY_NAME "OpenFLUID")
SET(CPACK_NSIS_HELP_LINK "${OPENFLUID_URL_OFFICIAL}")
SET(CPACK_NSIS_URL_INFO_ABOUT "${OPENFLUID_URL_OFFICIAL}")
SET(CPACK_NSIS_CONTACT "OpenFLUID contact <${OPENFLUID_EMAIL_CONTACT}>")  
SET(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/resources/packaging/windows\\\\openfluid_logo_NSIS.bmp")  
SET(CPACK_NSIS_MENU_LINKS 
    "${OPENFLUID_URL_OFFICIAL}" "OpenFLUID web site"
    "${OPENFLUID_URL_COMMUNITY}" "OpenFLUID community site")
SET(CPACK_PACKAGE_EXECUTABLES openfluid-builder;OpenFLUID-Builder
                              openfluid-devstudio;OpenFLUID-DevStudio)   
   
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME} ${OPENFLUID_VERSION_FULL}")
SET(CPACK_NSIS_MODIFY_PATH ON)


# This packaging includes Qt libs and GDAL libs
# It requires the OFBUILD_SUPPORT_DIR environment variable

 # support libraries
IF(OFBUILD_SUPPORT_DIR)
  FILE(TO_CMAKE_PATH "$ENV{OFBUILD_SUPPORT_DIR}" OFBUILD_SUPPORT_DIR_MOD)
  FILE(TO_CMAKE_PATH "$ENV{OFBUILD_SUPPORT_DIR}/bin" OFBUILD_SUPPORT_BINDIR_MOD)
  FILE(TO_CMAKE_PATH "$ENV{OFBUILD_SUPPORT_DIR}/lib" OFBUILD_SUPPORT_LIBDIR_MOD)

  # GDAL  
  INSTALL(DIRECTORY "${OFBUILD_SUPPORT_BINDIR_MOD}/" DESTINATION ${OFBUILD_BIN_INSTALL_PATH} FILES_MATCHING PATTERN "libgdal*.dll") 
  INSTALL(DIRECTORY "${OFBUILD_SUPPORT_LIBDIR_MOD}/" DESTINATION ${OFBUILD_BIN_INSTALL_PATH} FILES_MATCHING PATTERN "libgdal*.dll") 
ELSE()
  MESSAGE(WARNING "environment variable OFBUILD_SUPPORT_DIR is not set!")
ENDIF()


IF(OFBUILD_ENABLE_GUI)
  # Qt
  SET(WINDEPLOYQT_TARGETS)

  IF (QT_VERSION_MAJOR LESS 6)
    FIND_PROGRAM(WINDEPLOYQT_COMMAND NAMES windeployqt5 windeployqt5.exe windeployqt windeployqt.exe)
  ELSE()
    FIND_PROGRAM(WINDEPLOYQT_COMMAND NAMES windeployqt6 windeployqt6.exe windeployqt windeployqt.exe)
  ENDIF()

  IF(OFBUILD_ENABLE_APP_BUILDER)
    LIST(APPEND WINDEPLOYQT_TARGETS openfluid-builder)
  ENDIF()
  IF(OFBUILD_ENABLE_APP_DEVSTUDIO)
    LIST(APPEND WINDEPLOYQT_TARGETS openfluid-devstudio)
  ENDIF()

  FOREACH(CURRENT_TARGET ${WINDEPLOYQT_TARGETS})
    INSTALL(CODE
            "
            FILE(READ \"${CMAKE_BINARY_DIR}/${CURRENT_TARGET}_path.txt\" CURRENT_TARGET_PATH)
            EXECUTE_PROCESS(COMMAND ${WINDEPLOYQT_COMMAND}
                                        \"\${CURRENT_TARGET_PATH}\"
                                        --dir \"\${CMAKE_INSTALL_PREFIX}/${OFBUILD_BIN_INSTALL_PATH}\" 
                                        --libdir \"\${CMAKE_INSTALL_PREFIX}/${OFBUILD_BIN_INSTALL_PATH}\"
                                        --compiler-runtime
                                        -xml -network -concurrent
                                        --release
                                        --verbose 1
                                WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}\"
                                OUTPUT_FILE windeployqt_exec_${CURRENT_TARGET}.log 
                                ERROR_FILE windeployqt_exec_${CURRENT_TARGET}.err
                                RESULT_VARIABLE WDEPLOYQT_RESULT)
            IF(WDEPLOYQT_RESULT)
              MESSAGE(WARNING \"windeployqt failed for target: \${CURRENT_TARGET}. Check logs for details.\")
            ENDIF()
            ")
  ENDFOREACH()
ENDIF()

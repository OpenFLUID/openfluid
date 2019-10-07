

# desktop file on UNIX systems
IF(UNIX AND NOT APPLE)

  IF(OFBUILD_ENABLE_APP_BUILDER)
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/packaging/linux/openfluid-builder.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/openfluid-builder.desktop" @ONLY)

    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/openfluid-builder.desktop"
            DESTINATION "${OPENFLUID_DESKTOPENTRY_INSTALL_PATH}"
            PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                        GROUP_READ GROUP_EXECUTE
                        WORLD_READ WORLD_EXECUTE
            )
    ENDIF()

  IF(OFBUILD_ENABLE_APP_DEVSTUDIO)
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/packaging/linux/openfluid-devstudio.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/openfluid-devstudio.desktop" @ONLY)

    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/openfluid-devstudio.desktop"
            DESTINATION "${OPENFLUID_DESKTOPENTRY_INSTALL_PATH}"
            PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                        GROUP_READ GROUP_EXECUTE
                        WORLD_READ WORLD_EXECUTE
            )
    ENDIF()
ENDIF()





IF(OFBUILD_ENABLE_MARKET)
  # preparation of market datasets for tests
  EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "${CMAKE_CURRENT_SOURCE_DIR}/tools"
                                             "${CMAKE_COMMAND}"
                                             "-DREPOS_ROOT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/tests/miscdata/market/packages-src"
                                             "-DMARKETPLACE_ROOT_DIR=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/marketplace"
                                             "-DREPOS_VERSION=${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                             "-DREPOS_ARCH=${OFBUILD_SYSTEM_ARCH}"
                                              "-P" "oftest_marketpackbuild.cmake")
ENDIF()




# update of translations strings

IF(NOT LUPDATE_EXECUTABLE)
  MESSAGE(FATAL_ERROR "lupdate not found")
ENDIF()


FILE(MAKE_DIRECTORY ${OFBUILD_TRANSLATIONS_TSDIR})


FOREACH(_LANG ${OPENFLUID_TRANSLATIONS_LANGS})
  ADD_CUSTOM_TARGET(update-translation-strings-${_LANG}
                    COMMENT "Updating ${_LANG} translations for OpenFLUID"
                    COMMAND ${LUPDATE_EXECUTABLE}
                            ${OFBUILD_TRANSLATIONS_DIRSTOSCAN}
                            "-I" "${CMAKE_SOURCE_DIR}/src"
                            "-ts" "${OFBUILD_TRANSLATIONS_TSDIR}/${OPENFLUID_TRANSLATIONS_FILEROOT}-${_LANG}.ts" 
                            "-noobsolete")

  ADD_DEPENDENCIES(update-translations-strings update-translation-strings-${_LANG})
ENDFOREACH()



# build of translations

IF(NOT LRELEASE_EXECUTABLE)
  MESSAGE(FATAL_ERROR "lrelease not found")
ENDIF()



FILE(MAKE_DIRECTORY ${OFBUILD_DIST_TRANSLATIONS_DIR})

FOREACH(LANG ${OPENFLUID_TRANSLATIONS_LANGS})

  SET(LANG_TSFILE ${OFBUILD_TRANSLATIONS_TSDIR}/${OPENFLUID_TRANSLATIONS_FILEROOT}-${LANG}.ts)
  SET(LANG_QMFILE ${OFBUILD_DIST_TRANSLATIONS_DIR}/${OPENFLUID_TRANSLATIONS_FILEROOT}-${LANG}.qm)

  ADD_CUSTOM_COMMAND(DEPENDS ${LANG_TSFILE}
                     COMMAND ${LRELEASE_EXECUTABLE}
                             ${LANG_TSFILE}
                             -qm ${LANG_QMFILE}
                     OUTPUT ${LANG_QMFILE})

  ADD_CUSTOM_TARGET(release-translation-${LANG}
                    COMMENT "Releasing ${LANG} translation for OpenFLUID"
                    DEPENDS ${LANG_QMFILE})


  ADD_DEPENDENCIES(release-translations release-translation-${LANG})

  INSTALL(FILES "${LANG_QMFILE}"
          DESTINATION "${OPENFLUID_SHARE_TRANSLATIONS_INSTALL_PATH}"
         )

ENDFOREACH()



FIND_PACKAGE(PythonInterp)

FIND_PROGRAM(PYTHON3_EXECUTABLE NAMES python3)

IF(PYTHONINTERP_FOUND)

  # checkstyle tool without bracketing check (temporary)
  ADD_CUSTOM_TARGET(stylecheck
                    COMMENT "Checking source code style"
                    COMMAND "${PYTHON_EXECUTABLE}"
                            "${CMAKE_CURRENT_SOURCE_DIR}/tools/ofsrc-stylecheck.py"
                            "${CMAKE_SOURCE_DIR}"
                   )



  # cppcheck tool
  ADD_CUSTOM_TARGET(cppcheck
                    COMMENT "Static analysis of source code "
                    COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "${CMAKE_SOURCE_DIR}"
                            "${PYTHON_EXECUTABLE}"
                            "${CMAKE_CURRENT_SOURCE_DIR}/tools/ofsrc-cppcheck.py"
                            ${OFBUILD_CPPCHECK_EXTRA_OPTIONS}
                            "${CMAKE_SOURCE_DIR}"
                   )

ENDIF()


IF(PYTHON3_EXECUTABLE)

# rebuild completion tool
  ADD_CUSTOM_TARGET(rebuild-completion
  COMMENT "Rebuilding completion rules from API source code"
  COMMAND "${PYTHON3_EXECUTABLE}"
          "${CMAKE_CURRENT_SOURCE_DIR}/tools/ofsrc-completion-rebuild.py"
          "${CMAKE_SOURCE_DIR}"
  )

ENDIF()


# Packaging tools for OSX
IF(APPLE)
  CONFIGURE_FILE("tools/ofpack-osxdmgbuild.cmake.in" "${CMAKE_BINARY_DIR}/ofpack-osxdmgbuild.cmake" @ONLY)
  CONFIGURE_FILE("packaging/macosx/preinstall.in" "packaging/macosx/preinstall" @ONLY)
  CONFIGURE_FILE("packaging/macosx/postinstall.in" "packaging/macosx/postinstall" @ONLY)
  
  CONFIGURE_FILE("tools/ofpack-osx-brewcask.cmake.in" "${CMAKE_BINARY_DIR}/ofpack-osx-brewcask.cmake" @ONLY)
ENDIF()

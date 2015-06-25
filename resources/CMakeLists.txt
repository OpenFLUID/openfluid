

# desktop file on UNIX systems
IF(UNIX AND NOT APPLE)

  IF(BUILD_APP_BUILDER) 
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/packaging/openfluid-builder.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/openfluid-builder.desktop" @ONLY)

    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/openfluid-builder.desktop" 
            DESTINATION "${SHARE_DESKTOPENTRY_INSTALL_PATH}"
            PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                        GROUP_READ GROUP_EXECUTE
                        WORLD_READ WORLD_EXECUTE
            )
    ENDIF()
  
  IF(BUILD_APP_DEVSTUDIO) 
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/packaging/openfluid-devstudio.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/openfluid-devstudio.desktop" @ONLY)

    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/openfluid-devstudio.desktop" 
            DESTINATION "${SHARE_DESKTOPENTRY_INSTALL_PATH}"
            PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                        GROUP_READ GROUP_EXECUTE
                        WORLD_READ WORLD_EXECUTE
            )
    ENDIF()
ENDIF()





IF(OPENFLUID_ENABLE_MARKET)
  # preparation of market datasets for tests
  EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "${CMAKE_SOURCE_DIR}/cmake/internal"
                                             "${CMAKE_COMMAND}"
                                             "-DREPOS_ROOT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/tests/miscdata/market/packages-src"
                                             "-DMARKETPLACE_ROOT_DIR=${TESTS_OUTPUTDATA_PATH}/marketplace"
                                             "-DREPOS_VERSION=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" 
                                             "-DREPOS_ARCH=${OF_SYSTEM_ARCH}"
                                              "-P" "PackagingForMarket.cmake")
ENDIF()




# update of translations strings

IF(NOT LUPDATE_EXECUTABLE)
  MESSAGE(FATAL_ERROR "lupdate not found")
ENDIF()


FILE(MAKE_DIRECTORY ${OPENFLUID_TRANSLATIONS_TSDIR})


FOREACH(_LANG ${OPENFLUID_TRANSLATIONS_LANGS})
  ADD_CUSTOM_TARGET(update-translation-strings-${_LANG}
                    COMMENT "Updating ${_LANG} translations for OpenFLUID"
                    COMMAND ${LUPDATE_EXECUTABLE} 
                            ${OPENFLUID_TRANSLATIONS_DIRSTOSCAN}
                            "-I" "${CMAKE_SOURCE_DIR}/src"
                            "-ts" "${OPENFLUID_TRANSLATIONS_TSDIR}/${OPENFLUID_TRANSLATIONS_FILEROOT}-${_LANG}.ts")
     
  ADD_DEPENDENCIES(update-translations-strings update-translation-strings-${_LANG})    
ENDFOREACH()  



# build of translations

IF(NOT LRELEASE_EXECUTABLE)
  MESSAGE(FATAL_ERROR "lrelease not found")
ENDIF()

 
        
FILE(MAKE_DIRECTORY ${TRANSLATIONS_BUILD_DIR})        
        
FOREACH(LANG ${OPENFLUID_TRANSLATIONS_LANGS})
  
  SET(LANG_TSFILE ${OPENFLUID_TRANSLATIONS_TSDIR}/${OPENFLUID_TRANSLATIONS_FILEROOT}-${LANG}.ts)
  SET(LANG_QMFILE ${TRANSLATIONS_BUILD_DIR}/${OPENFLUID_TRANSLATIONS_FILEROOT}-${LANG}.qm)
        
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
          DESTINATION "${SHARE_TRANSLATIONS_PATH}" 
         )
         
ENDFOREACH()


# checkstyle tool
FIND_PACKAGE(PythonInterp)

IF(PYTHONINTERP_FOUND)

  ADD_CUSTOM_TARGET(stylecheck
                    COMMENT "Checking source code style"
                    COMMAND "${PYTHON_EXECUTABLE}" 
                            "${CMAKE_CURRENT_SOURCE_DIR}/tools/ofsrc-stylecheck.py"
                            "${CMAKE_SOURCE_DIR}"
                   )
ENDIF()


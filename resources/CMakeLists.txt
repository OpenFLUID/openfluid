

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
  
  IF(BUILD_APP_BUILDER) 
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/packaging/openfluid-devstudio.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/openfluid-devstudio.desktop" @ONLY)

    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/openfluid-devstudio.desktop" 
            DESTINATION "${SHARE_DESKTOPENTRY_INSTALL_PATH}"
            PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                        GROUP_READ GROUP_EXECUTE
                        WORLD_READ WORLD_EXECUTE
            )
    ENDIF()
ENDIF()






# preparation of market datasets for tests
EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "${CMAKE_SOURCE_DIR}/cmake/internal"
                                             "${CMAKE_COMMAND}"
                                             "-DREPOS_ROOT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/tests/miscdata/market/packages-src"
                                             "-DMARKETPLACE_ROOT_DIR=${TESTS_OUTPUTDATA_PATH}/marketplace"
                                             "-DREPOS_VERSION=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" 
                                             "-DREPOS_ARCH=${OF_SYSTEM_ARCH}"
                                              "-P" "PackagingForMarket.cmake")

                   
                                              
# build of translations
FIND_PROGRAM(LRELEASE_EXECUTABLE NAMES lrelease lrelease-qt4)

IF(NOT LRELEASE_EXECUTABLE)
  MESSAGE(FATAL_ERROR "lrelease not found")
ENDIF()

ADD_CUSTOM_TARGET(i18n-ts ALL)    
        
FILE(MAKE_DIRECTORY ${TRANSLATIONS_BUILD_DIR})        
        
FOREACH(LANG ${OPENFLUID_TRANSLATIONS_LANGS})
  
  SET(LANG_TSFILE ${OPENFLUID_TRANSLATIONS_TSDIR}/${OPENFLUID_TRANSLATIONS_FILEROOT}-${LANG}.ts)
  SET(LANG_QMFILE ${TRANSLATIONS_BUILD_DIR}/${OPENFLUID_TRANSLATIONS_FILEROOT}-${LANG}.qm)
        
  ADD_CUSTOM_COMMAND(DEPENDS ${LANG_TSFILE}
                     COMMAND ${LRELEASE_EXECUTABLE} 
                             ${LANG_TSFILE}
                             -qm ${LANG_QMFILE} 
                     OUTPUT ${LANG_QMFILE})
        
  ADD_CUSTOM_TARGET(i18n-ts-${LANG}
                    COMMENT "Building i18n-ts messages for ${LANG}"
                    DEPENDS ${LANG_QMFILE})


  ADD_DEPENDENCIES(i18n-ts i18n-ts-${LANG})
    
  INSTALL(FILES "${LANG_QMFILE}" 
          DESTINATION "${SHARE_TRANSLATIONS_PATH}" 
         )
         
ENDFOREACH()


# checkstyle tool
FIND_PACKAGE(PythonInterp)

IF(PYTHONINTERP_FOUND)
  ADD_CUSTOM_COMMAND(COMMAND "${PYTHON_EXECUTABLE}" 
                             "${CMAKE_CURRENT_SOURCE_DIR}/tools/ofsrc-stylecheck.py"
                             "${CMAKE_SOURCE_DIR}"
                     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nofile.checkstyke
                    )
        
  ADD_CUSTOM_TARGET(stylecheck
                    COMMENT "Checking source code style"
                    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nofile.checkstyke
                   )
ENDIF()




IF(BUILD_APP_BUILDER AND UNIX AND NOT APPLE)

  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/packaging/openfluid-builder.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/openfluid-builder.desktop" @ONLY)

  INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/openfluid-builder.desktop" 
          DESTINATION "${SHARE_DESKTOPENTRY_INSTALL_PATH}"
          PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                      GROUP_READ GROUP_EXECUTE
                      WORLD_READ WORLD_EXECUTE
          )

ENDIF()


EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "${CMAKE_SOURCE_DIR}/cmake/internal"
                                             "${CMAKE_COMMAND}"
                                             "-DREPOS_ROOT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/tests/datasets/market/packages-src"
                                             "-DMARKETPLACE_ROOT_DIR=${TESTS_OUTPUTDATA_PATH}/marketplace"
                                             "-DREPOS_VERSION=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" 
                                             "-DREPOS_ARCH=${OF_SYSTEM_ARCH}"
                                              "-P" "PackagingForMarket.cmake")

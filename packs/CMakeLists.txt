
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/resources/dist/README ${CMAKE_BINARY_DIR}/packs/readme.txt COPYONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/resources/dist/copyright ${CMAKE_BINARY_DIR}/packs/copyright.txt COPYONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/packs/welcome.txt ${CMAKE_BINARY_DIR}/packs/welcome.txt COPYONLY)


CONFIGURE_FILE(engine.linux.archives.cpack.in ${CMAKE_BINARY_DIR}/engine.linux.archives.cpack @ONLY)

CONFIGURE_FILE(engine.linux.ubuntu.cpack.in ${CMAKE_BINARY_DIR}/engine.linux.ubuntu.cpack @ONLY)

CONFIGURE_FILE(engine.win32.archives.cpack.in ${CMAKE_BINARY_DIR}/engine.win32.archives.cpack @ONLY)

CONFIGURE_FILE(engine.macosx32.archives.cpack.in ${CMAKE_BINARY_DIR}/engine.macosx32.archives.cpack @ONLY)
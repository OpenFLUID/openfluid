# Usage:
#  cmake -P ts_update.cmake
#
# Usage with extra search path for lupdate program:
#  cmake -DCMAKE_PREFIX_PATH=/path/to/dir -P ts_update.cmake
#
##


CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

FIND_PROGRAM(LUPDATE_EXECUTABLE NAMES lupdate lupdate-qt4)

IF(NOT LUPDATE_EXECUTABLE)
  MESSAGE(FATAL_ERROR "lupdate not found")
ENDIF()

INCLUDE(CMake.in.cmake)

IF (EXISTS CMake.in.local.cmake)
  INCLUDE(CMake.in.local.cmake)
ENDIF (EXISTS CMake.in.local.cmake)


FILE(MAKE_DIRECTORY ${OPENFLUID_TRANSLATIONS_TSDIR})

FOREACH(LANG ${OPENFLUID_TRANSLATIONS_LANGS})
  EXECUTE_PROCESS(COMMAND ${LUPDATE_EXECUTABLE} 
                          ${OPENFLUID_TRANSLATIONS_DIRSTOSCAN}
                          "-I" ./src
                          "-ts" "${OPENFLUID_TRANSLATIONS_TSDIR}/${OPENFLUID_TRANSLATIONS_FILEROOT}-${LANG}.ts"
                 )
ENDFOREACH()





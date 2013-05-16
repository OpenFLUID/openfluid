## OpenFLUID/src/apps/openfluid-builder/CMakeLists.txt


FILE(MAKE_DIRECTORY "${BUILDEREXT_OUTPUT_PATH}")

PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-2.4)
FIND_PACKAGE(GDAL REQUIRED)


FILE(READ "${PROJECT_SOURCE_DIR}/COPYING" BUILDER_LICENSE_TEXT)
STRING(REGEX REPLACE "\n" "\\\\n" BUILDER_LICENSE_TEXT "${BUILDER_LICENSE_TEXT}")
STRING(REGEX REPLACE "\"" "''" BUILDER_LICENSE_TEXT "${BUILDER_LICENSE_TEXT}")

FILE(READ "${PROJECT_SOURCE_DIR}/AUTHORS" BUILDER_AUTHORS_TEXT)
STRING(REGEX REPLACE "\n" "\\\\n" BUILDER_AUTHORS_TEXT "${BUILDER_AUTHORS_TEXT}")
STRING(REGEX REPLACE "\"" "''" BUILDER_AUTHORS_TEXT "${BUILDER_AUTHORS_TEXT}")


CONFIGURE_FILE(builderconfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/builderconfig.hpp" @ONLY)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")



INCLUDE_DIRECTORIES(${GTKMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${GDAL_LIBRARY_DIRS})

ADD_SUBDIRECTORY(builder-main)

IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))


INCLUDE_DIRECTORIES(${LibXML2_INCLUDE_DIRS})
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(builder-main builder-main/common)

ADD_EXECUTABLE(openfluid-builder main.cpp)

SET_TARGET_PROPERTIES(openfluid-builder PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-builder 
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${LibXML2_LIBRARIES}
                      ${GTKMM_LIBRARIES}
                      ${GDAL_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-tools
                      openfluid-guicommon
                      openfluid-builder-main
                      openfluid-fluidx)


ADD_SUBDIRECTORY(extensions)

ADD_DEPENDENCIES(openfluid-builder i18n)

INSTALL(TARGETS openfluid-builder
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})




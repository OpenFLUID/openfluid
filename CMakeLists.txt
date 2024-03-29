#
# CMake build for OpenFLUID
#
# Author : Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
#
# Usage for debug build:
#  cmake <path to sources>
#
# Usage for release build:
#  cmake <path to sources> -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<prefix for install path>
#


CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(OpenFLUID CXX)


################## build options ##################

OPTION(OFBUILD_ENABLE_GUI "enable build of UI libs and apps" ON)

OPTION(OFBUILD_ENABLE_DOCS "enable build of documentation" ON)

OPTION(OFBUILD_ENABLE_APP_CMD "enable build of command line program" ON)
OPTION(OFBUILD_ENABLE_APP_MINIMAL "enable build of openfluid-minimal program" ON)
OPTION(OFBUILD_ENABLE_APP_BUILDER "enable build of openfluid-builder application" ON)
OPTION(OFBUILD_ENABLE_APP_DEVSTUDIO "enable build of openfluid-devstudio application" ON)
OPTION(OFBUILD_ENABLE_APP_LOGEXPLORER "enable build of openfluid-logexplorer application" ON)
OPTION(OFBUILD_ENABLE_WARESDEV_BUILDER_INTEGRATION "enable integration of wares development in builder application" ON)
OPTION(OFBUILD_ENABLE_DOCALYZER "enable usage of docalyzer in wares builds and tests" ON)

OPTION(OFBUILD_ENABLE_SANITIZER "" OFF)

OPTION(OFBUILD_TESTS_ENABLE_HEAVY "enable build of heavy tests" OFF)
OPTION(OFBUILD_TESTS_ENABLE_FORTRAN "enable build of test simulators inetegrating Fortran code" OFF)
OPTION(OFBUILD_TESTS_ENABLE_EMBEDDING_R "enable build of test simulators embedding R code" OFF)
OPTION(OFBUILD_TESTS_ALLOW_NETWORK_REQUESTS "allow network requests in tests" ON)


################## project configuration ##################

# increase max length for paths (default value may be too short on Windows systems)
SET(CMAKE_OBJECT_PATH_MAX 300)

SET(OFBUILD_CXX_STANDARD 17)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/internal;${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# Set default build type (if not already set)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()
STRING(TOUPPER ${CMAKE_BUILD_TYPE} OFBUILD_BUILD_TYPE)

# Include internal CMake modules for os and hardware management
INCLUDE(OpenFLUIDSystemInfo)

# Include build configuration
INCLUDE(CMake.in.cmake)
INCLUDE(CMake.in.local.cmake OPTIONAL)

# Include internal CMake modules
ADD_SUBDIRECTORY(cmake)

SET(CMAKE_MODULE_PATH "${OFBUILD_DIST_CMAKE_MODULES_DIR};${CMAKE_MODULE_PATH}")
SET(OpenFLUID_DIR ${OFBUILD_DIST_CMAKE_MODULES_DIR})
INCLUDE(OpenFLUIDInternalHelpers)
INCLUDE(OpenFLUIDHelpers)

OPENFLUID_DETECT_CXX17()

FOREACH(_OPT ${OFBUILD_${OFBUILD_BUILD_TYPE}_COMPILE_OPTIONS})
  ADD_COMPILE_OPTIONS(${_OPT})
ENDFOREACH()

IF(OFBUILD_ENABLE_SANITIZER)
  OFBUILD_CONFIGURE_SANITIZER()
ENDIF()

SET(OFBUILD_ALL_COMPILE_FLAGS ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${OFBUILD_BUILD_TYPE}} ${OFBUILD_${OFBUILD_BUILD_TYPE}_COMPILE_OPTIONS})
STRING(REPLACE ";" " " OFBUILD_ALL_COMPILE_FLAGS_STR "${OFBUILD_ALL_COMPILE_FLAGS}")


IF(NOT OFBUILD_ENABLE_GUI)
  SET(OFBUILD_ENABLE_APP_BUILDER OFF)
  SET(OFBUILD_ENABLE_APP_DEVSTUDIO OFF)
  SET(OFBUILD_ENABLE_APP_LOGEXPLORER OFF)
  SET(OFBUILD_ENABLE_WARESDEV_BUILDER_INTEGRATION OFF)
ENDIF()


# Check of tools availability for docalyzer tests
IF(OFBUILD_ENABLE_DOCALYZER)
  FIND_PROGRAM(_DOCALYZER_PANDOC NAMES pandoc pandoc.exe)
  FIND_PROGRAM(_DOCALYZER_PDFLATEX NAMES pdflatex pdflatex.exe)
  FIND_PROGRAM(_DOCALYZER_BIBTEX NAMES bibtex bibtex.exe)
  IF(NOT (_DOCALYZER_PANDOC AND _DOCALYZER_PDFLATEX AND _DOCALYZER_BIBTEX))
    SET(OFBUILD_ENABLE_DOCALYZER OFF)
  ENDIF()
ENDIF()


# Force basic testing in debug mode
IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
  IF(NOT DEFINED OFBUILD_ENABLE_TESTING)
    SET(OFBUILD_ENABLE_TESTING ON)
  ENDIF()
ENDIF()

IF(OFBUILD_ENABLE_TESTING)
  SET(CMAKE_INSTALL_PREFIX "${OFBUILD_DIST_DIR}")
ELSE()
  SET(OFBUILD_TESTS_ENABLE_DOCALYZER ${OFBUILD_ENABLE_DOCALYZER})
  SET(OFBUILD_TESTS_ENABLE_FORTRAN OFF)
  SET(OFBUILD_TESTS_ENABLE_HEAVY OFF)
  SET(OFBUILD_TESTS_ENABLE_EMBEDDING_R OFF)
ENDIF()


# workaround to avoid conflicts between Boost libraries and Qt moc system
SET(CMAKE_AUTOMOC_MOC_OPTIONS "-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED")

SET(CMAKE_INCLUDE_CURRENT_DIR ON)



SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_DEBUG_LIBS OFF)
SET(Boost_USE_RELEASE_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)


# Detection of main dependencies
FIND_PACKAGE(Boost 1.53 REQUIRED)
FIND_PACKAGE(GDAL REQUIRED)
OPENFLUID_CHECK_GDAL_VERSION()
FIND_PACKAGE(CURL REQUIRED HTTP HTTPS)
IF(OFBUILD_ENABLE_GUI)
  FIND_PACKAGE(Qt6 COMPONENTS Core Widgets Svg QUIET)
  IF (NOT Qt6_FOUND)
    FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Svg REQUIRED)
    SET(QT_VERSION_MAJOR ${Qt5Core_VERSION_MAJOR})
    SET(QT_VERSION_MINOR ${Qt5Core_VERSION_MINOR})
    SET(QT_VERSION ${Qt5Core_VERSION})
  ELSE()
    SET(QT_VERSION_MAJOR ${Qt6Core_VERSION_MAJOR})
    SET(QT_VERSION_MINOR ${Qt6Core_VERSION_MINOR})
    SET(QT_VERSION ${Qt6Core_VERSION})  
  ENDIF()
  IF (QT_VERSION_MAJOR LESS 6)
    MESSAGE(STATUS "Found Qt5 (version ${Qt5_VERSION})")
  ELSE()
    MESSAGE(STATUS "Found Qt6 (version ${Qt6_VERSION})")
  ENDIF()
ENDIF()

IF(OFBUILD_ENABLE_DOCS)
  FIND_PACKAGE(Doxygen 1.8)
  FIND_PACKAGE(LATEX)
ENDIF()


IF(OFBUILD_ENABLE_GUI)
  # Detection of external translations tools and setup of common translations targets
  IF (QT_VERSION_MAJOR LESS 6)
    FIND_PROGRAM(LRELEASE_EXECUTABLE NAMES lrelease lrelease-qt5)
    FIND_PROGRAM(LUPDATE_EXECUTABLE NAMES lupdate lupdate-qt5)
  ELSE()
    FIND_PACKAGE(Qt6 COMPONENTS LinguistTools) # WARNING requires qtchoose manual setup externally FIXME see how to make it automatic
    SET(LRELEASE_EXECUTABLE lrelease)
    SET(LUPDATE_EXECUTABLE lupdate)
  ENDIF()
  
  ADD_CUSTOM_TARGET(update-translations-strings)
  ADD_CUSTOM_TARGET(release-translations ALL)
ENDIF()


FILE(MAKE_DIRECTORY "${OFBUILD_DIST_LIB_DIR}")
FILE(MAKE_DIRECTORY "${OFBUILD_DIST_BIN_DIR}")


STRING(TIMESTAMP OFBUILD_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")


################## testing ##################

IF(OFBUILD_ENABLE_TESTING)
  INCLUDE(CTest)
  ENABLE_TESTING()
  FILE(MAKE_DIRECTORY ${OFBUILD_TESTS_USERDATA_DIR})
  FILE(MAKE_DIRECTORY ${OFBUILD_TESTS_TEMP_DIR})

  # Check of server availability for tests
  IF(OFBUILD_TESTS_ALLOW_NETWORK_REQUESTS)
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E echo_append "-- Contacting ${OFBUILD_TESTS_DEV_HOST} for tests...")
    FILE(DOWNLOAD "http://${OFBUILD_TESTS_DEV_HOST}/" "${CMAKE_BINARY_DIR}/${OFBUILD_TESTS_DEV_HOST}.html"
                  TIMEOUT 10 STATUS OFBUILD_DEV_HOST_STATUS)
    LIST(GET OFBUILD_DEV_HOST_STATUS 0 OFBUILD_DEV_HOST_STATUS_0)
    IF(${OFBUILD_DEV_HOST_STATUS_0})
      EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E echo "failed")
      SET(OFBUILD_TESTS_ALLOW_NETWORK_REQUESTS OFF)
    ELSE()
      EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E echo "OK")
    ENDIF()
  ENDIF()
ENDIF()


IF(APPLE)
  SET(CMAKE_MACOSX_RPATH 1)
  SET(CMAKE_INSTALL_RPATH "@rpath/lib")
ENDIF(APPLE)


################## contents ##################

ADD_SUBDIRECTORY(resources)

ADD_SUBDIRECTORY(src)

ADD_CUSTOM_TARGET(update-example-docs)
ADD_SUBDIRECTORY(examples)

IF(OFBUILD_ENABLE_DOCS)
  ADD_SUBDIRECTORY(doc)
ENDIF()


################## packaging ##################

INCLUDE(CPack.in.cmake)


# Installation of common files
INSTALL(FILES "LICENSE" "AUTHORS.md" "README.md" "CHANGELOG.md"
        DESTINATION "${OPENFLUID_DOC_INSTALL_PATH}")


FILE(COPY "share/"
     DESTINATION "${OFBUILD_DIST_DIR}/${OFBUILD_SHARE_INSTALL_PATH}"
     PATTERN ".svn" EXCLUDE)

INSTALL(DIRECTORY "share"
        DESTINATION "." PATTERN ".svn" EXCLUDE)


################## display of main informations ##################

MESSAGE(STATUS "****************************************")
MESSAGE(STATUS "OpenFLUID version: ${OPENFLUID_VERSION_FULL} (numeric: ${OPENFLUID_VERSION_NUMERIC})")
MESSAGE(STATUS "System architecture: ${OFBUILD_ARCH_CLASS}")
MESSAGE(STATUS "Build type: ${OFBUILD_BUILD_TYPE}")
IF(OFBUILD_ENABLE_SANITIZER)
MESSAGE(STATUS "Sanitizer: enabled")
ELSE()
MESSAGE(STATUS "Sanitizer: disabled")
ENDIF()
MESSAGE(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "Compilation flags for ${CMAKE_BUILD_TYPE}: ${OFBUILD_ALL_COMPILE_FLAGS}")
MESSAGE(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "****************************************")

MESSAGE(STATUS "Build optional libraries")
IF(OFBUILD_ENABLE_GUI)
  MESSAGE(STATUS "  openfluid-ui-*: yes")
ELSE()
  MESSAGE(STATUS "  openfluid-ui-*: no")
ENDIF()

MESSAGE(STATUS "Build applications")
IF(OFBUILD_ENABLE_APP_CMD)
  MESSAGE(STATUS "  openfluid (cmd. line): yes")
ELSE()
  MESSAGE(STATUS "  openfluid (command line): no")
ENDIF()

IF(OFBUILD_ENABLE_APP_MINIMAL)
  MESSAGE(STATUS "  openfluid-minimal: yes")
ELSE()
  MESSAGE(STATUS "  openfluid-minimal: no")
ENDIF()

IF(OFBUILD_ENABLE_APP_BUILDER)
  MESSAGE(STATUS "  openfluid-builder: yes")
ELSE()
  MESSAGE(STATUS "  openfluid-builder: no")
ENDIF()

IF(OFBUILD_ENABLE_APP_DEVSTUDIO)
  MESSAGE(STATUS "  openfluid-devstudio: yes")
ELSE()
  MESSAGE(STATUS "  openfluid-devstudio: no")
ENDIF()

IF(OFBUILD_ENABLE_APP_LOGEXPLORER)
  MESSAGE(STATUS "  openfluid-logexplorer: yes")
ELSE()
  MESSAGE(STATUS "  openfluid-logexplorer: no")
ENDIF()

MESSAGE(STATUS "Build with integrations")
IF(OFBUILD_ENABLE_WARESDEV_BUILDER_INTEGRATION)
  MESSAGE(STATUS "  waresdev->openfluid-builder: yes")
ELSE()
  MESSAGE(STATUS "  waresdev->openfluid-builder: no")
ENDIF()

MESSAGE(STATUS "Build and run tests")
IF(OFBUILD_ENABLE_TESTING)
  MESSAGE(STATUS "  basic: yes")
ELSE()
  MESSAGE(STATUS "  basic: no")
ENDIF()
IF(OFBUILD_TESTS_ENABLE_HEAVY)
  MESSAGE(STATUS "  heavy: yes")
ELSE()
  MESSAGE(STATUS "  heavy: no")
ENDIF()
IF(OFBUILD_ENABLE_DOCALYZER)
  MESSAGE(STATUS "  docalizer: yes")
ELSE()
  MESSAGE(STATUS "  docalyzer: no")
ENDIF()
IF(OFBUILD_TESTS_ENABLE_FORTRAN)
  MESSAGE(STATUS "  fortran: yes")
ELSE()
  MESSAGE(STATUS "  fortran: no")
ENDIF()
IF(OFBUILD_TESTS_ENABLE_EMBEDDING_R)
  MESSAGE(STATUS "  R (embedded): yes")
ELSE()
  MESSAGE(STATUS "  R (embedded): no")
ENDIF()
IF(OFBUILD_TESTS_ALLOW_NETWORK_REQUESTS)
  MESSAGE(STATUS "  Network requests allowed: yes")
ELSE()
  MESSAGE(STATUS "  Network requests allowed: no")
ENDIF()

MESSAGE(STATUS "Build documentation")
IF(OFBUILD_ENABLE_DOCS AND DOXYGEN_FOUND)
  IF(PDFLATEX_COMPILER)
    MESSAGE(STATUS "  html : yes")
    MESSAGE(STATUS "  pdf : yes")
  ELSE()
    MESSAGE(STATUS "  html : yes")
    MESSAGE(STATUS "  pdf : no")
  ENDIF()
ELSE()
  MESSAGE(STATUS "  html : no")
  MESSAGE(STATUS "  pdf : no")
ENDIF()

MESSAGE(STATUS "****************************************")

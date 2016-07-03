#
# CMake build for OpenFLUID
#
# Author : Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
#
# Usage for debug build:
#  cmake <path to sources>
#
# Usage for debug build with heavy tests enabled:
#  cmake <path to sources> -DOFBUILD_ENABLE_HEAVYTESTING=1
#
# Usage for release build:
#  cmake <path to sources> -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<prefix for install path>
#


CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

PROJECT(OpenFLUID CXX)


# Check for C++11 compiler
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
IF(COMPILER_SUPPORTS_CXX11)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSE()
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  IF(COMPILER_SUPPORTS_CXX0X)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  ELSE()
    MESSAGE(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support C++11.")
  ENDIF()
ENDIF()


# Set policy for Qt on Windows
if(POLICY CMP0020)
 CMAKE_POLICY(SET CMP0020 OLD)
ENDIF()

# Set policy for LOCATION target property
if(POLICY CMP0026)
 CMAKE_POLICY(SET CMP0026 OLD)
ENDIF()


SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/internal;${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# Include internal CMake modules for os and hardware management
INCLUDE(OpenFLUIDSystemInfo)


# Include build configuration
INCLUDE(CMake.in.cmake)
INCLUDE(CMake.in.local.cmake OPTIONAL)

# Compute version as numeric
MATH(EXPR OPENFLUID_VERSION_NUMERIC "${OPENFLUID_VERSION_MAJOR} * 10000 + ${OPENFLUID_VERSION_MINOR} * 100 + ${OPENFLUID_VERSION_PATCH}")


ADD_SUBDIRECTORY(cmake)


# Include internal CMake modules
SET(CMAKE_MODULE_PATH "${OFBUILD_DIST_CMAKE_MODULES_DIR};${CMAKE_MODULE_PATH}")
SET(OpenFLUID_DIR ${OFBUILD_DIST_CMAKE_MODULES_DIR})
SET(OpenFLUIDHelpers_DIR ${OFBUILD_DIST_CMAKE_MODULES_DIR})
INCLUDE(OpenFLUIDInternalHelpers)
FIND_PACKAGE(OpenFLUIDHelpers)


# Set default build type (if not already set)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

STRING(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_STR)

IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
  IF(NOT DEFINED OFBUILD_ENABLE_TESTING)
    SET(OFBUILD_ENABLE_TESTING 1)
  ENDIF()
ELSE()
  SET(OFBUILD_ENABLE_TESTING 0)
  SET(OFBUILD_ENABLE_HEAVYTESTING 0)
ENDIF()


IF(OFBUILD_ENABLE_TESTING)
  SET(CMAKE_INSTALL_PREFIX "${OFBUILD_DIST_DIR}")
  SET(OFBUILD_BOOST_TEST_FRAMEWORK unit_test_framework)
ENDIF()


# Hack for avoiding conflicts between Boost libraries and Qt moc system
SET(CMAKE_AUTOMOC_MOC_OPTIONS "-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED")


SET(CMAKE_INCLUDE_CURRENT_DIR ON)


# Detection of main dependencies
INCLUDE(OpenFLUIDDetectQt)
FIND_PACKAGE(Boost 1.54 REQUIRED COMPONENTS ${OFBUILD_BOOST_TEST_FRAMEWORK})
FIND_PACKAGE(GDAL REQUIRED)
OPENFLUID_CHECK_GDAL_VERSION()
FIND_PACKAGE(RapidJSON REQUIRED)
FIND_PACKAGE(Doxygen)
FIND_PACKAGE(LATEX)


# Detection of external translations tools and setup of common translations targets
FIND_PROGRAM(LRELEASE_EXECUTABLE NAMES lrelease lrelease-qt4)
FIND_PROGRAM(LUPDATE_EXECUTABLE NAMES lupdate lupdate-qt4)
ADD_CUSTOM_TARGET(update-translations-strings)
ADD_CUSTOM_TARGET(release-translations ALL)


FILE(MAKE_DIRECTORY "${OFBUILD_DIST_LIB_DIR}")
FILE(MAKE_DIRECTORY "${OFBUILD_DIST_BIN_DIR}")
FILE(MAKE_DIRECTORY "${OFBUILD_DIST_SIMULATORS_DOC_DIR}")

IF(OFBUILD_ENABLE_TESTING)
  INCLUDE(CTest)
  ENABLE_TESTING()
  FILE(MAKE_DIRECTORY ${OFBUILD_TESTS_USERDATA_DIR})
  FILE(MAKE_DIRECTORY ${OFBUILD_TESTS_TEMP_DIR})
ENDIF()

ADD_SUBDIRECTORY(resources)

ADD_SUBDIRECTORY(src)

ADD_SUBDIRECTORY(examples)

IF(BUILD_DOCS)
  ADD_SUBDIRECTORY(doc)
ENDIF()



INCLUDE(CPack.in.cmake)


# Installation of common files
INSTALL(FILES "COPYING"
        "AUTHORS"
        "README.md"
        "CHANGELOG"
        DESTINATION "${OPENFLUID_DOC_INSTALL_PATH}")


FILE(COPY "share/"
     DESTINATION "${OFBUILD_DIST_DIR}/${OFBUILD_SHARE_INSTALL_PATH}"
     PATTERN ".svn" EXCLUDE)

INSTALL(DIRECTORY "share"
        DESTINATION "." PATTERN ".svn" EXCLUDE)


# Display of main informations about build configuration

MESSAGE(STATUS "****************************************")
MESSAGE(STATUS "OpenFLUID version: ${OPENFLUID_VERSION_FULL} (numeric: ${OPENFLUID_VERSION_NUMERIC})")
MESSAGE(STATUS "System architecture: ${OFBUILD_SYSTEM_ARCH}")
MESSAGE(STATUS "Distribution architecture: ${OFBUILD_DISTRO_ARCH}")
MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "Extra compiler flags for ${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS_${BUILD_TYPE_STR}}")
MESSAGE(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "****************************************")


MESSAGE(STATUS "Build optional libraries")
IF(OFBUILD_ENABLE_LANDR)
  MESSAGE(STATUS "  openfluid-landr : yes")
ELSE()
  MESSAGE(STATUS "  openfluid-landr : no")
ENDIF()

IF(OFBUILD_ENABLE_GUI)
  MESSAGE(STATUS "  openfluid-ui-* : yes")
ELSE()
  MESSAGE(STATUS "  openfluid-ui-* : no")
ENDIF()

IF(OFBUILD_ENABLE_MARKET)
  MESSAGE(STATUS "  openfluid-market : yes")
ELSE()
  MESSAGE(STATUS "  openfluid-market : no")
ENDIF()


MESSAGE(STATUS "Build applications")
IF(OFBUILD_ENABLE_APP_CMD)
  MESSAGE(STATUS "  openfluid (cmd. line) : yes")
ELSE()
  MESSAGE(STATUS "  openfluid (command line) : no")
ENDIF()

IF(OFBUILD_ENABLE_APP_MINIMAL)
  MESSAGE(STATUS "  openfluid-minimal : yes")
ELSE()
  MESSAGE(STATUS "  openfluid-minimal : no")
ENDIF()

IF(OFBUILD_ENABLE_APP_BUILDER AND OFBUILD_ENABLE_GUI)
  MESSAGE(STATUS "  openfluid-builder : yes")
ELSE()
  MESSAGE(STATUS "  openfluid-builder : no")
ENDIF()

IF(OFBUILD_ENABLE_APP_DEVSTUDIO AND OFBUILD_ENABLE_GUI)
  MESSAGE(STATUS "  openfluid-devstudio : yes")
ELSE()
  MESSAGE(STATUS "  openfluid-devstudio : no")
ENDIF()

IF(OFBUILD_ENABLE_APP_LOGEXPLORER AND OFBUILD_ENABLE_GUI)
  MESSAGE(STATUS "  openfluid-logexplorer : yes")
ELSE()
  MESSAGE(STATUS "  openfluid-logexplorer : no")
ENDIF()


MESSAGE(STATUS "Build tests")
IF(OFBUILD_ENABLE_TESTING)
  MESSAGE(STATUS "  basic : yes")
ELSE()
  MESSAGE(STATUS "  basic : no")
ENDIF()
IF(OFBUILD_ENABLE_HEAVYTESTING)
  MESSAGE(STATUS "  heavy : yes")
ELSE()
  MESSAGE(STATUS "  heavy : no")
ENDIF()


MESSAGE(STATUS "Build documentation")
IF(BUILD_DOCS AND DOXYGEN)
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

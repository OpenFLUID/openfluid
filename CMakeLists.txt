#
# CMake build for OpenFLUID
#
# Author : Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
#
# Usage for debug build:
#  cmake <path to sources>
#
# Usage for debug build with heavy tests enabled:
#  cmake <path to sources> -DOPENFLUID_ENABLE_HEAVYTESTING=1
#
# Usage for release build:
#  cmake <path to sources> -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<prefix for install path>
#


CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

PROJECT(OpenFLUID CXX)


SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/internal;${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
INCLUDE(OpenFLUIDSystemInfo)


INCLUDE(CMake.in.cmake)

IF (EXISTS ${CMAKE_SOURCE_DIR}/CMake.in.local.cmake)
  INCLUDE(CMake.in.local.cmake)
ENDIF (EXISTS ${CMAKE_SOURCE_DIR}/CMake.in.local.cmake)


GET_FILENAME_COMPONENT(BUILDNAME_COMPILER ${CMAKE_CXX_COMPILER} NAME)
SET(BUILDNAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${BUILDNAME_COMPILER}-v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

SET (CTEST_INITIAL_CACHE "
BUILDNAME:STRING=${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${BUILDNAME_COMPILER}-v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
")


ADD_SUBDIRECTORY(cmake)


SET(CMAKE_MODULE_PATH "${CMAKE_MODULES_OUTPUT_PATH};${CMAKE_MODULE_PATH}")
SET(OpenFLUID_DIR ${CMAKE_MODULES_OUTPUT_PATH})
INCLUDE(OpenFLUIDInternalHelpers)
INCLUDE(OpenFLUIDHelpers)


IF(NOT CMAKE_BUILD_TYPE) 
  SET(CMAKE_BUILD_TYPE "Debug")
ENDIF(NOT CMAKE_BUILD_TYPE)

STRING(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_STR)


IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
  SET(CMAKE_INSTALL_PREFIX "${BUILD_OUTPUT_PATH}")
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")


# hack for avoiding conflicts between boos libraries and Qt moc system
SET(CMAKE_AUTOMOC_MOC_OPTIONS "-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED")
SET(CMAKE_INCLUDE_CURRENT_DIR ON)


INCLUDE(OpenFLUIDDetectQt)
FIND_PACKAGE(Boost 1.35.0 REQUIRED COMPONENTS system filesystem date_time program_options regex iostreams unit_test_framework)
FIND_PACKAGE(GDAL REQUIRED)
FIND_PACKAGE(Doxygen)
FIND_PACKAGE(LATEX)


FILE(MAKE_DIRECTORY "${LIB_OUTPUT_PATH}")
FILE(MAKE_DIRECTORY "${BIN_OUTPUT_PATH}")
FILE(MAKE_DIRECTORY "${SIMULATORS_DOC_OUTPUT_PATH}")

IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  INCLUDE(CTest)
  ENABLE_TESTING()  
  FILE(MAKE_DIRECTORY ${OPENFLUID_TESTS_USERDATA_PATH})
  FILE(MAKE_DIRECTORY ${OPENFLUID_TESTS_TEMP_PATH})
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  


ADD_SUBDIRECTORY(src)  

ADD_SUBDIRECTORY(examples)

IF(BUILD_DOCS)
  ADD_SUBDIRECTORY(doc)
ENDIF()  

ADD_SUBDIRECTORY(resources)



INCLUDE(CPack.in.cmake)


INSTALL(FILES "COPYING"
        "AUTHORS"
        "README.md"   
        "CHANGELOG"
        DESTINATION "${DOC_INSTALL_PATH}")
  
  
FILE(COPY "share/" 
     DESTINATION "${BUILD_OUTPUT_PATH}/${SHARE_INSTALL_PATH}"
     PATTERN ".svn" EXCLUDE)
            
INSTALL(DIRECTORY "share" 
        DESTINATION "." PATTERN ".svn" EXCLUDE)
            
  

MESSAGE(STATUS "****************************************")
MESSAGE(STATUS "OpenFLUID version: ${FULL_VERSION}")
MESSAGE(STATUS "Build architecture: ${OF_SYSTEM_ARCH}")
MESSAGE(STATUS "Build name: ${BUILDNAME}")
MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")  
MESSAGE(STATUS "Compiler flags for build: ${CMAKE_CXX_FLAGS_${BUILD_TYPE_STR}}")
MESSAGE(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "****************************************")

IF(OPENFLUID_ENABLE_LANDR)
  MESSAGE(STATUS "Build openfluid-landr library : yes")  
ELSE()
  MESSAGE(STATUS "Build openfluid-landr library : no")  
ENDIF()

IF(BUILD_APP_CMD)
  MESSAGE(STATUS "Build openfluid application : yes")  
ELSE()
  MESSAGE(STATUS "Build openfluid application : no")  
ENDIF()

IF(BUILD_APP_MINIMAL)
  MESSAGE(STATUS "Build openfluid-minimal application : yes")  
ELSE()
  MESSAGE(STATUS "Build openfluid-minimal application : no")  
ENDIF()

IF(OPENFLUID_ENABLE_GUI)
  MESSAGE(STATUS "Build openfluid-ui libraries : yes")  
ELSE()
  MESSAGE(STATUS "Build openfluid-ui libraries : no")  
ENDIF()

IF(BUILD_APP_MINIMALGUI AND OPENFLUID_ENABLE_GUI)
  MESSAGE(STATUS "Build openfluid-minimal-gui application : yes")  
ELSE()
  MESSAGE(STATUS "Build openfluid-minimal-gui application : no")  
ENDIF()

IF(BUILD_APP_BUILDER AND OPENFLUID_ENABLE_GUI)
  MESSAGE(STATUS "Build openfluid-builder application : yes")  
ELSE()
  MESSAGE(STATUS "Build openfluid-builder application : no")  
ENDIF()

IF(BUILD_APP_DEVSTUDIO AND OPENFLUID_ENABLE_GUI)
  MESSAGE(STATUS "Build openfluid-devstudio application : yes")  
ELSE()
  MESSAGE(STATUS "Build openfluid-devstudio application : no")  
ENDIF()
  
IF(BUILD_APP_MARKETCLIENT AND OPENFLUID_ENABLE_GUI)
  MESSAGE(STATUS "Build openfluid-market-client application : yes")  
ELSE()
  MESSAGE(STATUS "Build openfluid-market-client application : no")  
ENDIF()  

IF(BUILD_DOCS AND DOXYGEN)
  IF(PDFLATEX_COMPILER)
    MESSAGE(STATUS "Build OpenFLUID documentation : html, pdf")
  ELSE()
    MESSAGE(STATUS "Build OpenFLUID documentation : html")
  ENDIF()
ELSE()
  MESSAGE(STATUS "Build OpenFLUID documentation : no") 
ENDIF()

IF(OPENFLUID_ENABLE_HEAVYTESTING)
  MESSAGE(STATUS "Run heavy tests : yes")  
ELSE()
  MESSAGE(STATUS "Run heavy tests : no")  
ENDIF()
  
MESSAGE(STATUS "****************************************")







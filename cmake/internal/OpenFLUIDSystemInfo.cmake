##
#
#  This file is part of OpenFLUID software
#  Copyright(c) 2007, INRA - Montpellier SupAgro
#
#
# == GNU General Public License Usage ==
#
#  OpenFLUID is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  OpenFLUID is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.
#
#
# == Other Usage ==
#
#  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
#  license, and requires a written agreement between You and INRA.
#  Licensees for Other Usage of OpenFLUID may use this file in accordance
#  with the terms contained in the written agreement between You and INRA.
#  
##


MESSAGE(STATUS "Performing internal architecture detection") 
TRY_RUN(OFBUILD_ARCH_RUN OFBUILD_ARCH_COMPILE
      ${CMAKE_CURRENT_BINARY_DIR}/cmake/internal/build
      ${CMAKE_CURRENT_LIST_DIR}/ArchForCMake.cpp
      CMAKE_FLAGS -DINCLUDE_DIRECTORIES:STRING=${CMAKE_SOURCE_DIR}/src
      RUN_OUTPUT_VARIABLE OFBUILD_ARCH_RUN_OUTPUT
      COMPILE_OUTPUT_VARIABLE OFBUILD_ARCH_COMPILE_OUTPUT)

IF(NOT OFBUILD_ARCH_COMPILE)
  MESSAGE(FATAL_ERROR "Cannot build internal architecture detection program\n\n ${COMPILE_OUTPUT_VARIABLE}")  
ENDIF()

IF(OFBUILD_ARCH_RUN)
  MESSAGE(FATAL_ERROR "Cannot run internal architecture detection program\n\n ${COMPILE_OUTPUT_VARIABLE}")  
ENDIF()

MESSAGE(STATUS "Performing internal architecture detection -- completed")


SET(OFBUILD_SYSTEM_ARCH ${OFBUILD_ARCH_RUN_OUTPUT})
SET(OFBUILD_SYSTEM_VERSION "")
SET(OFBUILD_SYSTEM_CODE "")
SET(OFBUILD_SYSTEM_IS_64BITS NO)
SET(OFBUILD_SYSTEM_IS_WINDOWS NO)
SET(OFBUILD_SYSTEM_IS_LINUX NO)
SET(OFBUILD_SYSTEM_IS_APPLE NO)

SET(OFBUILD_DISTRO_ARCH "")
SET(OFBUILD_DISTRO_ID "")
SET(OFBUILD_DISTRO_CODE "")
SET(OFBUILD_DISTRO_VERSION "")
SET(OFBUILD_DISTRO_IS_DEBIAN NO)
SET(OFBUILD_DISTRO_IS_UBUNTU NO)
SET(OFBUILD_DISTRO_IS_FEDORA NO)
SET(OFBUILD_DISTRO_IS_CENTOS NO)
SET(OFBUILD_LIBDIR_SUFFIX "")


IF("${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
  SET(OFBUILD_SYSTEM_IS_64BITS YES)
ENDIF()


IF(UNIX AND NOT CYGWIN)
  IF(NOT APPLE) 

    FIND_PROGRAM(LSB_RELEASE_PROG "lsb_release")
    IF(LSB_RELEASE_PROG-NOTFOUND)
      SET(OFBUILD_DISTRO_ARCH "noarch")
      SET(OFBUILD_DISTRO_ID "nodistro")
    ELSE()

      EXECUTE_PROCESS(COMMAND "${LSB_RELEASE_PROG}" "-is" OUTPUT_VARIABLE OFBUILD_DISTRO_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
      EXECUTE_PROCESS(COMMAND "${LSB_RELEASE_PROG}" "-cs" OUTPUT_VARIABLE OFBUILD_DISTRO_CODE OUTPUT_STRIP_TRAILING_WHITESPACE)
      EXECUTE_PROCESS(COMMAND "${LSB_RELEASE_PROG}" "-rs" OUTPUT_VARIABLE OFBUILD_DISTRO_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
    
      IF(OFBUILD_DISTRO_ID STREQUAL "Debian")
        SET(OFBUILD_SYSTEM_IS_LINUX YES)
        SET(OFBUILD_DISTRO_IS_DEBIAN YES)
        IF(OFBUILD_SYSTEM_IS_64BITS)
          SET(OFBUILD_DISTRO_ARCH "amd64")
        ELSE()
          SET(OFBUILD_DISTRO_ARCH "i386")
        ENDIF()
      ENDIF()    
    
      IF(OFBUILD_DISTRO_ID STREQUAL "Ubuntu")
        SET(OFBUILD_SYSTEM_IS_LINUX YES)
        SET(OFBUILD_DISTRO_IS_UBUNTU YES)
        IF(OFBUILD_SYSTEM_IS_64BITS)
          SET(OFBUILD_DISTRO_ARCH "amd64")
        ELSE()
          SET(OFBUILD_DISTRO_ARCH "i386")
        ENDIF()
      ENDIF()
      
      IF(OFBUILD_DISTRO_ID STREQUAL "CentOS")
        SET(OFBUILD_SYSTEM_IS_LINUX YES)        
        SET(OFBUILD_DISTRO_IS_CENTOS YES)        
        IF(OFBUILD_SYSTEM_IS_64BITS)
          SET(OFBUILD_DISTRO_ARCH "x86_64")
          SET(OFBUILD_LIBDIR_SUFFIX "64")
        ELSE()
          SET(OFBUILD_DISTRO_ARCH "i686")
        ENDIF()
      ENDIF()
      
      IF(OFBUILD_DISTRO_ID STREQUAL "Fedora")
        SET(OFBUILD_SYSTEM_IS_LINUX YES)        
        SET(OFBUILD_DISTRO_IS_FEDORA YES)        
        IF(OFBUILD_SYSTEM_IS_64BITS)
          SET(OFBUILD_DISTRO_ARCH "x86_64")
          SET(OFBUILD_LIBDIR_SUFFIX "64")
        ELSE()
          SET(OFBUILD_DISTRO_ARCH "i686")
        ENDIF()
      ENDIF()      
      
    ENDIF()
            
  ELSE()
    SET(OFBUILD_SYSTEM_IS_APPLE YES)
    SET(OFBUILD_SYSTEM_CODE "unknown")

    EXECUTE_PROCESS(COMMAND "sw_vers" "-productVersion" OUTPUT_VARIABLE OFBUILD_SYSTEM_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

    IF(${OFBUILD_SYSTEM_VERSION} MATCHES "10\.15.*")
      SET(OFBUILD_SYSTEM_CODE "catalina")
    ELSEIF(${OFBUILD_SYSTEM_VERSION} MATCHES "10\.14.*")
      SET(OFBUILD_SYSTEM_CODE "mojave")
    ELSEIF(${OFBUILD_SYSTEM_VERSION} MATCHES "10\.13.*")
      SET(OFBUILD_SYSTEM_CODE "high_sierra")
    ELSEIF(${OFBUILD_SYSTEM_VERSION} MATCHES "10\.12.*")
      SET(OFBUILD_SYSTEM_CODE "sierra")
    ELSEIF(${OFBUILD_SYSTEM_VERSION} MATCHES "10\.11.*")
      SET(OFBUILD_SYSTEM_CODE "el_capitan")
    ELSEIF(${OFBUILD_SYSTEM_VERSION} MATCHES "10\.10.*")
      SET(OFBUILD_SYSTEM_CODE "yosemite")
    ELSEIF(${OFBUILD_SYSTEM_VERSION} MATCHES "10\.9.*")
      SET(OFBUILD_SYSTEM_CODE "mavericks")
    ENDIF() 
  ENDIF()
ENDIF()

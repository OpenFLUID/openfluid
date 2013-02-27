##
# This file is part of OpenFLUID software
# Copyright (c) 2007-2010 INRA-Montpellier SupAgro
#
#
# == GNU General Public License Usage ==
#
# OpenFLUID is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# OpenFLUID is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.
#
# In addition, as a special exception, INRA gives You the additional right
# to dynamically link the code of OpenFLUID with code not covered
# under the GNU General Public License ("Non-GPL Code") and to distribute
# linked combinations including the two, subject to the limitations in this
# paragraph. Non-GPL Code permitted under this exception must only link to
# the code of OpenFLUID dynamically through the OpenFLUID libraries
# interfaces, and only for building OpenFLUID plugins. The files of
# Non-GPL Code may be link to the OpenFLUID libraries without causing the
# resulting work to be covered by the GNU General Public License. You must
# obey the GNU General Public License in all respects for all of the
# OpenFLUID code and other code used in conjunction with OpenFLUID
# except the Non-GPL Code covered by this exception. If you modify
# this OpenFLUID, you may extend this exception to your version of the file,
# but you are not obligated to do so. If you do not wish to provide this
# exception without modification, you must delete this exception statement
# from your version and license this OpenFLUID solely under the GPL without
# exception.
#
#
# == Other Usage ==
#
# Other Usage means a use of OpenFLUID that is inconsistent with the GPL
# license, and requires a written agreement between You and INRA.
# Licensees for Other Usage of OpenFLUID may use this file in accordance
# with the terms contained in the written agreement between You and INRA.
##

SET(OF_SYSTEM_ARCH "")
SET(OF_DISTRO_ID "")
SET(OF_DISTRO_CODE "")
SET(OF_DISTRO_VERSION "")
SET(OF_SYSTEM_IS_64BITS NO)
SET(OF_SYSTEM_IS_WINDOWS NO)
SET(OF_SYSTEM_IS_LINUX NO)
SET(OF_SYSTEM_IS_APPLE NO)
SET(OF_DISTRO_IS_UBUNTU NO)
SET(OF_DISTRO_IS_FEDORA NO)
SET(OF_DISTRO_IS_CENTOS NO)
SET(OF_LIBDIR_SUFFIX "")


IF("${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
  SET(OF_SYSTEM_IS_64BITS YES)
ENDIF("${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )


IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  SET(OF_SYSTEM_IS_WINDOWS YES)
  IF(OF_SYSTEM_IS_64BITS)
    SET(OF_SYSTEM_ARCH "win64")
  ELSE(OF_SYSTEM_IS_64BITS)
    SET(OF_SYSTEM_ARCH "win32")
  ENDIF(OF_SYSTEM_IS_64BITS)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")


IF(UNIX AND NOT CYGWIN)
  IF(NOT APPLE) 

    FIND_PROGRAM(LSB_RELEASE_PROG "lsb_release")
    IF(LSB_RELEASE_PROG-NOTFOUND)
      SET(OF_SYSTEM_ARCH "noarch")
      SET(OF_DISTRO_ID "nodistro")
    ELSE(LSB_RELEASE_PROG-NOTFOUND)

      EXECUTE_PROCESS(COMMAND "${LSB_RELEASE_PROG}" "-is" OUTPUT_VARIABLE OF_DISTRO_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
      EXECUTE_PROCESS(COMMAND "${LSB_RELEASE_PROG}" "-cs" OUTPUT_VARIABLE OF_DISTRO_CODE OUTPUT_STRIP_TRAILING_WHITESPACE)
      EXECUTE_PROCESS(COMMAND "${LSB_RELEASE_PROG}" "-rs" OUTPUT_VARIABLE OF_DISTRO_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
    
      IF(OF_DISTRO_ID STREQUAL "Ubuntu")
        SET(OF_SYSTEM_IS_LINUX YES)
        SET(OF_DISTRO_IS_UBUNTU YES)
        IF(OF_SYSTEM_IS_64BITS)
          SET(OF_SYSTEM_ARCH "amd64")
        ELSE(OF_SYSTEM_IS_64BITS)
          SET(OF_SYSTEM_ARCH "i386")
        ENDIF(OF_SYSTEM_IS_64BITS)
      ENDIF(OF_DISTRO_ID STREQUAL "Ubuntu")
      
      IF(OF_DISTRO_ID STREQUAL "CentOS")
        SET(OF_SYSTEM_IS_LINUX YES)        
        SET(OF_DISTRO_IS_CENTOS YES)        
        IF(OF_SYSTEM_IS_64BITS)
          SET(OF_SYSTEM_ARCH "x86_64")
          SET(OF_LIBDIR_SUFFIX "64")
        ELSE(OF_SYSTEM_IS_64BITS)
          SET(OF_SYSTEM_ARCH "i686")
        ENDIF(OF_SYSTEM_IS_64BITS)
      ENDIF(OF_DISTRO_ID STREQUAL "CentOS")
      
    ENDIF(LSB_RELEASE_PROG-NOTFOUND)
      
      
  ELSE(NOT APPLE)
    SET(OF_SYSTEM_IS_APPLE YES)
  ENDIF(NOT APPLE)
  
ENDIF(UNIX AND NOT CYGWIN)

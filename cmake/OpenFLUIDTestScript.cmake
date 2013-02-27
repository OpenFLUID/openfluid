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


FUNCTION(EXECUTE_COMMAND CMD)

  MESSAGE("${CMD}: ${ARGN}")

  IF(${CMD} STREQUAL "CHECK_FILE_EXIST")
    IF(NOT EXISTS ${ARGV1})
      MESSAGE(FATAL_ERROR "CHECK_FILE_EXIST: file ${ARGV1} not found")
    ENDIF()  
  ELSEIF(${CMD} STREQUAL "CHECK_DIRECTORY_EXIST")
    IF(NOT IS_DIRECTORY ${ARGV1})
      MESSAGE(FATAL_ERROR "CHECK_DIRECTORY_EXIST: directory ${ARGV1} not found")
    ENDIF()
  ELSEIF(${CMD} STREQUAL "REMOVE_FILE")
    IF(EXISTS ${ARGV1})
      FILE(REMOVE ${ARGV1})
    ENDIF()
  ELSEIF(${CMD} STREQUAL "REMOVE_DIRECTORY")
    IF(IS_DIRECTORY ${ARGV1})
      FILE(REMOVE_RECURSE ${ARGV1})
    ENDIF()
  ELSEIF(${CMD} STREQUAL "EMPTY_DIRECTORY")
    IF(IS_DIRECTORY ${ARGV1})
      FILE(REMOVE_RECURSE ${ARGV1})
      FILE(MAKE_DIRECTORY ${ARGV1})      
    ELSE()
      MESSAGE(FATAL_ERROR "EMPTY_DIRECTORY: directory ${ARGV1} not found")
    ENDIF()
  ELSE()
    MESSAGE(FATAL_ERROR "${CMD}: unknown command")
  ENDIF() 

ENDFUNCTION()


###########################################################################


FUNCTION(PARSE_COMMANDS)
  SET(CURRENT_ARG1 "")
  SET(CURRENT_ARG2 "")

  SET(CMD_EXPECTED 1)

  FOREACH(ELEM ${ARGN})
    SET(ELEM_PROCESSED 0)

    IF(${CMD_EXPECTED})
      IF(${ELEM} STREQUAL "CHECK_FILE_EXIST" OR
         ${ELEM} STREQUAL "CHECK_DIRECTORY_EXIST" OR
         ${ELEM} STREQUAL "REMOVE_FILE" OR
         ${ELEM} STREQUAL "REMOVE_DIRECTORY" OR
         ${ELEM} STREQUAL "EMPTY_DIRECTORY" OR
         ${ELEM} STREQUAL "CREATE_FILE" OR
         ${ELEM} STREQUAL "CREATE_DIR" OR
         ${ELEM} STREQUAL "COMPARE_FILES" OR
         ${ELEM} STREQUAL "COMPARE_DIRECTORIES")
        SET(CURRENT_CMD "${ELEM}")
        SET(CURRENT_ARG1 "")
        SET(CURRENT_ARG2 "")
        SET(ELEM_PROCESSED 1)
      ELSE()
        MESSAGE(FATAL_ERROR "command expected, but ${ELEM} found instead")
      ENDIF()
  
      SET(CMD_EXPECTED 0)
    ELSE()
      IF(${CURRENT_CMD} STREQUAL "COMPARE_FILES" OR
         ${CURRENT_CMD} STREQUAL "COMPARE_DIRECTORIES")
        IF(NOT CURRENT_ARG1)
          SET(CURRENT_ARG1 ${ELEM})
          SET(ELEM_PROCESSED 1)
        ELSEIF(NOT CURRENT_ARG2) 
          SET(CURRENT_ARG2 ${ELEM})
          SET(ELEM_PROCESSED 1)
          SET(CMD_EXPECTED 1)
          EXECUTE_COMMAND(${CURRENT_CMD} ${CURRENT_ARG1} ${CURRENT_ARG2})
        ELSE()
          MESSAGE(FATAL_ERROR "argument error for command ${CURRENT_CMD}")   
        ENDIF()
      ELSEIF(${CURRENT_CMD} STREQUAL "CHECK_FILE_EXIST" OR
             ${CURRENT_CMD} STREQUAL "CHECK_DIRECTORY_EXIST" OR
             ${CURRENT_CMD} STREQUAL "REMOVE_FILE" OR
             ${CURRENT_CMD} STREQUAL "REMOVE_DIRECTORY" OR
             ${CURRENT_CMD} STREQUAL "EMPTY_DIRECTORY" OR
             ${CURRENT_CMD} STREQUAL "CREATE_FILE" OR
             ${CURRENT_CMD} STREQUAL "CREATE_DIRECTORY")
        IF(NOT CURRENT_ARG1) 
          SET(CURRENT_ARG1 ${ELEM})
          SET(ELEM_PROCESSED 1)
          SET(CMD_EXPECTED 1)
          EXECUTE_COMMAND(${CURRENT_CMD} ${CURRENT_ARG1})
        ENDIF()
      ELSE()
        MESSAGE(FATAL_ERROR "argument expected for command ${CURRENT_CMD}")
      ENDIF()  
    ENDIF()
  ENDFOREACH()

  IF(NOT CMD_EXPECTED)
    MESSAGE(FATAL_ERROR "missing argument for command ${CURRENT_CMD}")
  ENDIF()

ENDFUNCTION()


###########################################################################
###########################################################################



PARSE_COMMANDS(${PRECMDS})

MESSAGE("TEST command: ${CMD}")
EXECUTE_PROCESS(COMMAND ${CMD}
                RESULT_VARIABLE CMD_RESULT)
IF(CMD_RESULT)
  MESSAGE(FATAL_ERROR "Test error")
ENDIF()                
                

PARSE_COMMANDS(${POSTCMDS})


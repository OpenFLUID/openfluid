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


FUNCTION(EXECUTE_COMMAND CMD)

  MESSAGE("${CMD}: ${ARGN}")

  IF(${CMD} STREQUAL "CHECK_FILE_EXIST")
    IF(NOT EXISTS ${ARGV1})
      MESSAGE(FATAL_ERROR "CHECK_FILE_EXIST: file ${ARGV1} not found")
    ENDIF()  

  ELSEIF(${CMD} STREQUAL "CHECK_FILE_EXIST_IN_ARCHIVE")
    SET(TMPDIR "TMPDIR_UNZIPPED_")
    STRING(RANDOM LENGTH 10 RANDOMSUFFIX)
    STRING(APPEND TMPDIR ${RANDOMSUFFIX})
    FILE(MAKE_DIRECTORY ${TMPDIR})
    EXECUTE_PROCESS(
      COMMAND ${CMAKE_COMMAND} -E tar x ${ARGV2} 
      WORKING_DIRECTORY ${TMPDIR}
    )

    IF(NOT EXISTS ${TMPDIR}/${ARGV1})
      MESSAGE(FATAL_ERROR "CHECK_FILE_EXIST_IN_ARCHIVE: file ${ARGV1} not found at archive root")
    ENDIF()
    FILE(REMOVE_RECURSE ${TMPDIR})

  ELSEIF(${CMD} STREQUAL "CHECK_FILE_CONTAINS")
    FILE(READ ${TMPDIR}/${ARGV1} TMPTXT)
    STRING(FIND "${TMPTXT}" "${ARGV2}" ISMATCH)
    IF(${ISMATCH} EQUAL -1)
      MESSAGE(FATAL_ERROR "CHECK_FILE_CONTAINS: file ${ARGV1} does not contain string ${ARGV2}")
    ENDIF()

  ELSEIF(${CMD} STREQUAL "CHECK_FILE_IDENTICAL")
    # generates consistent eol between files (can be replaced by option ignore_eol in cmake 3.14)
    CONFIGURE_FILE(${ARGV1}${ARGV3} ${ARGV3}_1_UNIX_EOL NEWLINE_STYLE UNIX)
    CONFIGURE_FILE(${ARGV2}${ARGV3} ${ARGV3}_2_UNIX_EOL NEWLINE_STYLE UNIX)
    EXECUTE_PROCESS(
      COMMAND ${CMAKE_COMMAND} -E compare_files ${ARGV3}_1_UNIX_EOL ${ARGV3}_2_UNIX_EOL
      RESULT_VARIABLE IDENTICAL_FILES OUTPUT_QUIET ERROR_QUIET
    )
    FILE(REMOVE_RECURSE ${ARGV3}_1_UNIX_EOL)
    FILE(REMOVE_RECURSE ${ARGV3}_2_UNIX_EOL)
    IF(IDENTICAL_FILES EQUAL "1")
      MESSAGE(FATAL_ERROR "CHECK_FILE_IDENTICAL: Contents of ${ARGV3} are not identical between ${ARGV1} and ${ARGV2}")
    ENDIF()

  ELSEIF(${CMD} STREQUAL "COMPARE_DIRECTORIES")
    IF(NOT IS_DIRECTORY ${ARGV1})
      MESSAGE(STATUS "COMPARE_DIRECTORIES: ${ARGV1} not found, comparison skipped.")
    ELSEIF(IS_DIRECTORY ${ARGV2})
      SET(METHOD ${ARGV3})
      IF(${METHOD} STREQUAL "custom_or_basic")
        IF(NOT "$ENV{OPENFLUID_COMPARE_TOOL}" STREQUAL "")
          MESSAGE("Using program provided in OPENFLUID_COMPARE_TOOL env var: $ENV{OPENFLUID_COMPARE_TOOL}")
          SET(METHOD "$ENV{OPENFLUID_COMPARE_TOOL}")
        ELSE()
          SET(METHOD "basic_output_check")
        ENDIF()
      ENDIF()
      IF(${METHOD} STREQUAL "basic_output_check")
        SET(_SUBPATHS "")
      
        FILE(GLOB_RECURSE _FILES "${ARGV1}/*")
        FOREACH(_FILEPATH ${_FILES})
          STRING(REPLACE "${ARGV1}/" "" _SUBPATH "${_FILEPATH}")
          LIST(APPEND _SUBPATHS ${_SUBPATH})
        ENDFOREACH()
        
        FILE(GLOB_RECURSE _FILES_2 "${ARGV2}/*")
        FOREACH(_FILEPATH ${_FILES_2})
          STRING(REPLACE "${ARGV2}/" "" _SUBPATH "${_FILEPATH}")
          LIST(APPEND _SUBPATHS ${_SUBPATH})
        ENDFOREACH()

        LIST(REMOVE_DUPLICATES _SUBPATHS)

        FOREACH(_SUBPATH ${_SUBPATHS})
          GET_FILENAME_COMPONENT(_FILEEXT ${_SUBPATH} LAST_EXT)
          GET_FILENAME_COMPONENT(_FILENAME ${_SUBPATH} NAME)
          IF (NOT ${_FILEEXT} STREQUAL ".log" AND NOT ${_FILEEXT} STREQUAL ".pdf" AND NOT ${_FILEEXT} STREQUAL ".kmz"
              AND NOT ${_FILENAME} STREQUAL "script.gnuplot") #TOIMPl check format from any other builtin observers
            GET_FILENAME_COMPONENT(_FILENAME ${_SUBPATH} NAME)
            EXECUTE_COMMAND("CHECK_FILE_IDENTICAL" "${ARGV1}/" "${ARGV2}/" "${_SUBPATH}")
          ENDIF()
        ENDFOREACH()
      ELSE()
        # first check if injected method found by calling it without args and expecting 0 as return code
        EXECUTE_PROCESS(COMMAND "${METHOD}" RESULT_VARIABLE METHOD_RETURNCODE)
        IF(NOT METHOD_RETURNCODE EQUAL "0")
          MESSAGE(WARNING "COMPARE_DIRECTORIES: Tool '${METHOD}' not responding, using fallback method")
          EXECUTE_COMMAND("COMPARE_DIRECTORIES" "${ARGV1}" "${ARGV2}" "basic_output_check")
        ELSE()
          EXECUTE_PROCESS(
            COMMAND "${METHOD}" "${ARGV1}" "${ARGV2}"
            RESULT_VARIABLE COMPARISON_RETURNCODE
          )
          IF(NOT COMPARISON_RETURNCODE EQUAL "0")
            MESSAGE(FATAL_ERROR "COMPARE_DIRECTORIES: Difference between ${ARGV1} and ${ARGV2} according to ${METHOD}")
          ENDIF()
        ENDIF()
      ENDIF()
    ELSE()
      MESSAGE(STATUS "COMPARE_DIRECTORIES: ${ARGV2} not found, comparison skipped.")
    ENDIF()
    
  ELSEIF(${CMD} STREQUAL "CHECK_DIRECTORY_EXIST")
    IF(NOT IS_DIRECTORY ${ARGV1})
      MESSAGE(FATAL_ERROR "CHECK_DIRECTORY_EXIST: directory ${ARGV1} not found")
    ENDIF()
  ELSEIF(${CMD} STREQUAL "CHECK_DIRECTORY_NOT_EXISTING")
    IF(IS_DIRECTORY ${ARGV1})
      MESSAGE(FATAL_ERROR "CHECK_DIRECTORY_NOT_EXISTING: directory ${ARGV1} found")
    ENDIF()
  ELSEIF(${CMD} STREQUAL "CREATE_DIRECTORY")
    FILE(MAKE_DIRECTORY ${ARGV1})
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
  ELSEIF(${CMD} STREQUAL "COPY_FILE")
    FILE(COPY ${ARGV1} DESTINATION ${ARGV2})
  ELSEIF(${CMD} STREQUAL "COPY_DIRECTORY")
    FILE(MAKE_DIRECTORY ${ARGV2})
    FILE(COPY ${ARGV1} DESTINATION ${ARGV2})
  ELSE()
    MESSAGE(FATAL_ERROR "${CMD}: unknown command")
  ENDIF() 

ENDFUNCTION()


###########################################################################


FUNCTION(PARSE_COMMANDS)
  SET(CURRENT_ARG1 "")
  SET(CURRENT_ARG2 "")
  SET(CURRENT_ARG3 "")

  SET(CMD_EXPECTED 1)

  FOREACH(ELEM ${ARGN})
    SET(ELEM_PROCESSED 0)

    IF(${CMD_EXPECTED})
      IF(${ELEM} STREQUAL "CHECK_FILE_EXIST" OR
         ${ELEM} STREQUAL "CHECK_FILE_EXIST_IN_ARCHIVE" OR
         ${ELEM} STREQUAL "CHECK_FILE_CONTAINS" OR
         ${ELEM} STREQUAL "CHECK_FILE_IDENTICAL" OR
         ${ELEM} STREQUAL "CHECK_DIRECTORY_EXIST" OR
         ${ELEM} STREQUAL "CHECK_DIRECTORY_NOT_EXISTING" OR
         ${ELEM} STREQUAL "REMOVE_FILE" OR
         ${ELEM} STREQUAL "REMOVE_DIRECTORY" OR
         ${ELEM} STREQUAL "COPY_FILE" OR
         ${ELEM} STREQUAL "COPY_DIRECTORY" OR
         ${ELEM} STREQUAL "EMPTY_DIRECTORY" OR
         ${ELEM} STREQUAL "CREATE_FILE" OR
         ${ELEM} STREQUAL "CREATE_DIRECTORY" OR
         ${ELEM} STREQUAL "COMPARE_FILES" OR
         ${ELEM} STREQUAL "COMPARE_DIRECTORIES")
        SET(CURRENT_CMD "${ELEM}")
        SET(CURRENT_ARG1 "")
        SET(CURRENT_ARG2 "")
        SET(CURRENT_ARG3 "")
        SET(ELEM_PROCESSED 1)
      ELSE()
        MESSAGE(FATAL_ERROR "command expected, but ${ELEM} found instead")
      ENDIF()
  
      SET(CMD_EXPECTED 0)
    ELSE()
      IF(${CURRENT_CMD} STREQUAL "COMPARE_FILES" OR
         ${CURRENT_CMD} STREQUAL "CHECK_FILE_EXIST_IN_ARCHIVE" OR
         ${CURRENT_CMD} STREQUAL "CHECK_FILE_CONTAINS" OR
         ${CURRENT_CMD} STREQUAL "COPY_FILE" OR
         ${CURRENT_CMD} STREQUAL "COPY_DIRECTORY")
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
      ELSEIF(${CURRENT_CMD} STREQUAL "CHECK_FILE_IDENTICAL" OR
             ${CURRENT_CMD} STREQUAL "COMPARE_DIRECTORIES")
        IF(NOT CURRENT_ARG1)
          SET(CURRENT_ARG1 ${ELEM})
          SET(ELEM_PROCESSED 1)
        ELSEIF(NOT CURRENT_ARG2) 
          SET(CURRENT_ARG2 ${ELEM})
          SET(ELEM_PROCESSED 1)
        ELSEIF(NOT CURRENT_ARG3)
          SET(CURRENT_ARG3 ${ELEM})
          SET(ELEM_PROCESSED 1)
          SET(CMD_EXPECTED 1)
          EXECUTE_COMMAND(${CURRENT_CMD} ${CURRENT_ARG1} ${CURRENT_ARG2} ${CURRENT_ARG3})
        ELSE()
          MESSAGE(FATAL_ERROR "argument error for command ${CURRENT_CMD}")   
        ENDIF()
      ELSEIF(${CURRENT_CMD} STREQUAL "CHECK_FILE_EXIST" OR
             ${CURRENT_CMD} STREQUAL "CHECK_DIRECTORY_EXIST" OR
             ${CURRENT_CMD} STREQUAL "CHECK_DIRECTORY_NOT_EXISTING" OR
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
  MESSAGE(FATAL_ERROR "Test error: ${CMD_RESULT}")
ENDIF()


PARSE_COMMANDS(${POSTCMDS})


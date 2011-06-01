#
# CMake script for extracting strings to translate
#
# Usage : cmake -P po_update.cmake
#


CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


FIND_PROGRAM(XGETTEXT_EXECUTABLE xgettext)
FIND_PROGRAM(MSGMERGE_EXECUTABLE msgmerge)
FIND_PROGRAM(MSGINIT_EXECUTABLE msginit)

#IF(XGETTEXT_EXECUTABLE-NOTFOUND OR MSGMERGE_EXECUTABLE-NOTFOUND OR MSGINIT_EXECUTABLE-NOTFOUND)
#  MESSAGE(FATAL_ERROR "Missing i18n tool(s)!")
#ENDIF(XGETTEXT_EXECUTABLE-NOTFOUND OR MSGMERGE_EXECUTABLE-NOTFOUND OR MSGINIT_EXECUTABLE-NOTFOUND)


INCLUDE(CMake.in.config)

IF (EXISTS CMake.in.local.config)
  INCLUDE(CMake.in.local.config)
ENDIF (EXISTS CMake.in.local.config)



FILE(GLOB_RECURSE ALL_HCPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.?pp)


FILE(WRITE ${PO_DIR}/POTFILES.in "")

FOREACH(HCPP_FILE ${ALL_HCPP_FILES})
  FILE(APPEND ${PO_DIR}/POTFILES.in "${HCPP_FILE}\n")
ENDFOREACH(HCPP_FILE ${ALL_HCCP_FILES})


EXECUTE_PROCESS(COMMAND ${XGETTEXT_EXECUTABLE} 
                "--files-from" "${PO_DIR}/POTFILES.in"
                "-o" "${PO_DIR}/openfluid.pot" 
                "--keyword=_" 
                "--package-name=${PO_PACKAGENAME}"
                "--package-version=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                "--copyright-holder=${PO_COPYRIGHTHOLDER}"
                "--msgid-bugs-address=${PO_BUGSADDRESS}"
                "--no-wrap") 
                
                
                
FILE(READ ${PO_DIR}/LINGUAS PO_LINGUAS)
STRING(REPLACE "\n" ";" PO_LINGUAS_STR ${PO_LINGUAS})


FOREACH(LINGUA ${PO_LINGUAS_STR})

  IF(EXISTS "${PO_DIR}/${LINGUA}.po")

    # update po file
  
    EXECUTE_PROCESS(COMMAND ${MSGMERGE_EXECUTABLE} 
                    "${LINGUA}" 
                    "${PO_PACKAGENAME}"
                    "-D" "${PO_DIR}"
                    "-o" "${PO_DIR}/${LINGUA}.po"
                    "--no-wrap")                    
                    
  
  ELSE(EXISTS "${PO_DIR}/${LINGUA}.po")
  
    # create po file
  
    EXECUTE_PROCESS(COMMAND ${MSGINIT_EXECUTABLE}
                    "--locale=${LINGUA}.UTF-8"
                    "--input=${PO_DIR}/openfluid.pot"
                    "--output-file=${PO_DIR}/${LINGUA}.po"
                    "--no-translator")

  ENDIF(EXISTS "${PO_DIR}/${LINGUA}.po")

ENDFOREACH(LINGUA ${PO_LINGUAS_STR})

 
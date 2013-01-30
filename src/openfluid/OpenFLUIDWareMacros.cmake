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


MACRO(_OPENFLUID_ADD_WARE_MESSAGES _WAREID _WARETYPE _WAREFILES _WARETARGET)
  MESSAGE(STATUS "***** ${_WARETYPE} ${_WAREID} *****")
  
  SET(_FILESMSG "  Files:")
  FOREACH(_WFILE ${_WAREFILES})
    SET(_FILESMSG "${_FILESMSG} ${_WFILE}")
  ENDFOREACH()

  MESSAGE(STATUS ${_FILESMSG})
  MESSAGE(STATUS "  Target: ${_WARETARGET}")

ENDMACRO()


# ==========================================================================


MACRO(OPENFLUID_SHOW_CMAKE_VARIABLES)
  MESSAGE("OpenFLUID_FOUND: " ${OpenFLUID_FOUND})
  MESSAGE("OpenFLUID_DIR: " ${OpenFLUID_DIR})
  MESSAGE("OpenFLUID_VERSION: " ${OpenFLUID_VERSION})
  MESSAGE("OpenFLUID_INCLUDE_DIR: " ${OpenFLUID_INCLUDE_DIR})
  MESSAGE("OpenFLUID_INCLUDE_DIRS: " ${OpenFLUID_INCLUDE_DIR})
  MESSAGE("OpenFLUID_LIBRARY_DIR: " ${OpenFLUID_LIBRARY_DIR})
  MESSAGE("OpenFLUID_LIBRARY_DIRS: " ${OpenFLUID_LIBRARY_DIR})
  MESSAGE("OpenFLUID_CORE_LIBRARY: " ${OpenFLUID_CORE_LIBRARY})
  MESSAGE("OpenFLUID_BASE_LIBRARY: " ${OpenFLUID_BASE_LIBRARY})
  MESSAGE("OpenFLUID_TOOLS_LIBRARY: " ${OpenFLUID_TOOLS_LIBRARY})
  MESSAGE("OpenFLUID_WARE_LIBRARY: " ${OpenFLUID_WARE_LIBRARY})
  MESSAGE("OpenFLUID_LIBRARIES: " ${OpenFLUID_LIBRARIES})
  MESSAGE("OpenFLUID_ENGINE_PROGRAM: " ${OpenFLUID_ENGINE_PROGRAM})
  MESSAGE("OpenFLUID_OBSERVER_FILENAME_SUFFIX: " ${OpenFLUID_OBSERVER_FILENAME_SUFFIX})
  MESSAGE("OpenFLUID_FUNCTION_FILENAME_SUFFIX: " ${OpenFLUID_FUNCTION_FILENAME_SUFFIX})
ENDMACRO()


# ==========================================================================


MACRO(OPENFLUID_ADD_FUNCTION _FUNCNAME)

  FIND_PACKAGE(OpenFLUID REQUIRED core base tools ware)
  FIND_PACKAGE(PkgConfig REQUIRED)
  PKG_CHECK_MODULES(GLIBMM REQUIRED glibmm-2.4)
  PKG_CHECK_MODULES(GTHREAD REQUIRED gthread-2.0)
  
  SET(${_FUNCNAME}_TARGET ${${_FUNCNAME}_ID}${OpenFLUID_FUNCTION_FILENAME_SUFFIX})

  _OPENFLUID_ADD_WARE_MESSAGES(${${_FUNCNAME}_ID} "Function" ${${_FUNCNAME}_CPP} ${${_FUNCNAME}_TARGET})

  INCLUDE_DIRECTORIES(${OpenFLUID_INCLUDE_DIRS} ${${_FUNCNAME}_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS})
  LINK_DIRECTORIES(${OpenFLUID_LIBRARY_DIRS} ${GLIBMM_LIBRARY_DIRS}) 


  ADD_LIBRARY(${${_FUNCNAME}_TARGET} MODULE ${${_FUNCNAME}_CPP})
  SET_TARGET_PROPERTIES(${${_FUNCNAME}_TARGET} PROPERTIES PREFIX "" SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})

  TARGET_LINK_LIBRARIES(${${_FUNCNAME}_TARGET} ${OpenFLUID_LIBRARIES} ${GLIBMM_LIBRARIES} ${GTHREAD_LIBRARIES})
  
ENDMACRO()


# ==========================================================================


MACRO(OPENFLUID_ADD_DEFAULT_FUNCTION)
  OPENFLUID_ADD_FUNCTION(FUNC)
ENDMACRO()


# ==========================================================================


MACRO(OPENFLUID_ADD_OBSERVER _OBSNAME)

  FIND_PACKAGE(OpenFLUID REQUIRED core base tools ware)
  FIND_PACKAGE(PkgConfig REQUIRED)
  PKG_CHECK_MODULES(GLIBMM REQUIRED glibmm-2.4)
  PKG_CHECK_MODULES(GTHREAD REQUIRED gthread-2.0)
  
  SET(${_OBSNAME}_TARGET ${${_OBSNAME}_ID}${OpenFLUID_OBSERVER_FILENAME_SUFFIX})

  _OPENFLUID_ADD_WARE_MESSAGES(${${_OBSNAME}_ID} "Observer" ${${_OBSNAME}_CPP} ${${_OBSNAME}_TARGET})

  INCLUDE_DIRECTORIES(${OpenFLUID_INCLUDE_DIRS} ${${_OBSNAME}_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS})
  LINK_DIRECTORIES(${OpenFLUID_LIBRARY_DIRS} ${GLIBMM_LIBRARY_DIRS}) 


  ADD_LIBRARY(${${_OBSNAME}_TARGET} MODULE ${${_OBSNAME}_CPP})
  SET_TARGET_PROPERTIES(${${_OBSNAME}_TARGET} PROPERTIES PREFIX "" SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})

  TARGET_LINK_LIBRARIES(${${_OBSNAME}_TARGET} ${OpenFLUID_LIBRARIES} ${GLIBMM_LIBRARIES} ${GTHREAD_LIBRARIES})
  
ENDMACRO()


# ==========================================================================


MACRO(OPENFLUID_ADD_DEFAULT_OBSERVER)
  OPENFLUID_ADD_OBSERVER(OBS)
ENDMACRO()




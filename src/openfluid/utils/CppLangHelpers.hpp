/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/

/**
  @file CppLangHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_UTILS_CPPLANGHELPERS_HPP__
#define __OPENFLUID_UTILS_CPPLANGHELPERS_HPP__


/**
  Macro to delete the five constructors and operators
*/
#define CPPCLASS_DELETE_FIVE(classname) \
    classname() = delete; \
    classname(const classname&) = delete; \
    classname& operator=(const classname&) = delete; \
    classname(classname &&) = delete; \
    classname& operator=(classname &&) = delete;


/**
  Macro to create the default five constructors and operators
*/
#define CPPCLASS_DEFAULT_FIVE(classname) \
    classname() = default; \
    classname(const classname&) = default; \
    classname& operator=(const classname&) = default; \
    classname(classname &&) = default; \
    classname& operator=(classname &&) = default;


// TODO review code to use CPPCLASS_DEFAULT_FIVE
// TODO review code to use CPPCLASS_DELETE_FIVE


#endif /* __OPENFLUID_UTILS_CPPLANGHELPERS_HPP__ */

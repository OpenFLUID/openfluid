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
  @file dllexport.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_DLLEXPORT_HPP__
#define __OPENFLUID_DLLEXPORT_HPP__


// Adapted from https://gcc.gnu.org/wiki/Visibility


// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define OPENFLUID_HELPER_DLL_IMPORT __declspec(dllimport)
  #define OPENFLUID_HELPER_DLL_EXPORT __declspec(dllexport)
  #define OPENFLUID_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define OPENFLUID_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define OPENFLUID_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define OPENFLUID_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define OPENFLUID_HELPER_DLL_IMPORT
    #define OPENFLUID_HELPER_DLL_EXPORT
    #define OPENFLUID_HELPER_DLL_LOCAL
  #endif
#endif


// =====================================================================
// =====================================================================


// Now we use the generic helper definitions above to define OPENFLUID_API, OPENFLUID_LOCAL and OPENFLUID_PLUGIN
// OPENFLUID_API is used for the public API symbols.
// OPENFLUID_LOCAL is used for non-api symbols.
// OPENFLUID_PLUGIN is used for plugins symbols (in simulators, observers, ...).

#ifdef OPENFLUID_DLL // lib is a DLL
  #ifdef OPENFLUID_DLL_EXPORTS // defined if we are building the DLL (instead of using it)
    #define OPENFLUID_API OPENFLUID_HELPER_DLL_EXPORT
  #else
    #define OPENFLUID_API OPENFLUID_HELPER_DLL_IMPORT
  #endif
  #define OPENFLUID_LOCAL OPENFLUID_HELPER_DLL_LOCAL
  #define OPENFLUID_PLUGIN OPENFLUID_HELPER_DLL_EXPORT
#else // lib is static
  #define OPENFLUID_API
  #define OPENFLUID_LOCAL
  #define OPENFLUID_PLUGIN OPENFLUID_HELPER_DLL_EXPORT
#endif


#endif /* __OPENFLUID_DLLEXPORT_HPP__ */

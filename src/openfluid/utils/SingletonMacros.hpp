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
  @file SingletonMacros.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UTILS_SINGLETONMACROS_HPP__
#define __OPENFLUID_UTILS_SINGLETONMACROS_HPP__


/**
  Macro for definition of a dynamic singleton,
  providing instance() and kill() methods to the class in which it is defined.
  It must be put in the .hpp file, at the beginning of the class.
  This implementation is not thread safe.

  Example of use to design an Example class as a dynamic singleton
  @snippet misc/singleton.cpp singletonmacros_def
  @snippet misc/singleton.cpp singletonmacros_init

  @see OPENFLUID_SINGLETON_INITIALIZATION
*/
#define OPENFLUID_SINGLETON_DEFINITION(T) \
  private: \
    T(T const&) = delete; \
    void operator=(T const&) = delete; \
  \
  protected: \
    static T* mp_Instance; \
  \
  public: \
    static T* instance() \
    { \
      if (!mp_Instance) \
        mp_Instance = new T(); \
      return mp_Instance; \
    }\
    static void kill()\
    {\
      if (mp_Instance)\
      {\
        delete mp_Instance;\
        mp_Instance = nullptr;\
      }\
    }


// =====================================================================
// =====================================================================


/**
  Macro for initialization of a dynamic singleton, must be put in the .cpp file.
  @see OPENFLUID_SINGLETON_DEFINITION
*/
#define OPENFLUID_SINGLETON_INITIALIZATION(T) \
  T* T::mp_Instance = nullptr;


#endif /* __OPENFLUID_UTILS_SINGLETONMACROS_HPP__ */

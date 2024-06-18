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
  @file KillableSingleton.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UTILS_KILLABLESINGLETON_HPP__
#define __OPENFLUID_UTILS_KILLABLESINGLETON_HPP__


namespace openfluid { namespace utils {

/**
  Singleton template class implementing a killable singleton design.
  This implementation is not thread safe.

  @warning This template class does not work in a cross-dll context on Windows.
  Due to this restriction, it MUST NOT BE USED in the OpenFLUID framework.

  Example of use to design an Example class as a killable singleton
  @snippet misc/singleton.cpp killablesingleton
*/
template<typename T>
class KillableSingleton
{

  private:

    KillableSingleton<T>(T const&) = delete;

    void operator=(T const&) = delete;


  protected:

    static T* mp_Instance;

    KillableSingleton<T>() = default;

    ~KillableSingleton<T>()
    { }


  public:

    /**
      Returns a pointer to the singleton object.
      Instantiates the class if it not has not been instantiated before or it has been killed before.
      @return A pointer to the singleton object
    */
    static T* instance()
    {
      if (!mp_Instance)
      {
        mp_Instance = new T();
      }

      return mp_Instance;
    }


    // =====================================================================
    // =====================================================================


    /**
      Kills the singleton class
    */
    static void kill()
    {
      if (mp_Instance)
      {
        delete mp_Instance;
        mp_Instance = nullptr;
      }
    }
};


template <typename T>
T *KillableSingleton<T>::mp_Instance = nullptr;


} }  //namespaces


#endif /* __OPENFLUID_UTILS_KILLABLESINGLETON_HPP__ */

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
  @file ProgramProxy.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QString>
#include <openfluid/dllexport.hpp>


#ifndef __OPENFLUID_UTILSQ_PROGRAMPROXY_HPP__
#define __OPENFLUID_UTILSQ_PROGRAMPROXY_HPP__


namespace openfluid { namespace utils {


template<typename T>
class ProgramProxy
{
  protected:

     static QString m_ExecutablePath;

     static QString m_Version;


  public:
    
    struct CommandInfos
    {
      QString Program;

      QStringList Args;

      QString joined(const QString& Sep = " ") const
      {
        return (Program + Sep + Args.join(Sep));
      }
    };


    static bool isAvailable()
    { 
      return false; 
    }

    static QString getVersion()
    { 
      return m_Version; 
    }

    static QString getExecutablePath()
    { 
      return m_ExecutablePath; 
    }

};


// =====================================================================
// =====================================================================


template<typename T> QString ProgramProxy<T>::m_ExecutablePath = "";

template<typename T>QString ProgramProxy<T>::m_Version = "";


} }  // namespaces


#endif /* __OPENFLUID_UTILSQ_PROGRAMPROXY_HPP__ */

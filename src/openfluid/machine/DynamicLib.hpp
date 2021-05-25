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
  @file DynamicLib.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_MACHINE_DYNAMICLIB_HPP__
#define __OPENFLUID_MACHINE_DYNAMICLIB_HPP__


#include <string>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API DynamicLib
{
  private:

    std::string m_LibPath;

    std::string m_LatestErrMsg;

    void* m_LibHandle;

    void storeErrorMessage();

    void* getVoidSymbol(const std::string& Name) const;


  public: 

    DynamicLib(const std::string& LibPath);

    DynamicLib(const DynamicLib&) = delete;
    DynamicLib(DynamicLib&&) = delete;

    DynamicLib& operator=(const DynamicLib& Other) = delete;
    DynamicLib& operator=(DynamicLib&& Other) = delete;

    ~DynamicLib();

    std::string getPath() const;

    std::string getLatestErrorMsg() const;

    bool load();

    bool unload();

    bool isLoaded() const;

    bool hasSymbol(const std::string& Name) const;
    
    template <class T>
    T getSymbol(const std::string& Name) const
    {
      return reinterpret_cast<T>(getVoidSymbol(Name));
    }
};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_DYNAMICLIB_HPP__ */

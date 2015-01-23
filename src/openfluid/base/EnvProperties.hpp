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
  @file EnvProperties.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_BASE_ENVPROPERTIES_HPP__
#define __OPENFLUID_BASE_ENVPROPERTIES_HPP__

#include <openfluid/dllexport.hpp>
#include <map>
#include <string>

namespace openfluid { namespace base {

/**
  Class containing execution environment information for funtions
*/
class OPENFLUID_API EnvironmentProperties
{
  private:

    typedef std::map<std::string,bool> KeyBoolMap_t;
    typedef std::map<std::string,std::string> KeyStringMap_t;

    KeyBoolMap_t m_BoolValues;
    KeyStringMap_t m_StringValues;

  public:

    /**
      Constructor
    */
    EnvironmentProperties();

    /**
      Destructor
    */
    virtual ~EnvironmentProperties();

    /**
      Assigns a boolean value to the corresponding key

      @return true if the assignment is correct
    */
    bool setValue(std::string Key, const bool Value);

    /**
      Assigns a string value to the corresponding key

      @return true if the assignment is correct
    */
    bool setValue(std::string Key, const std::string& Value);

    /**
      Gets the boolean value assigned to the key

      @return true if the value exists as a boolean
    */
    bool getValue(std::string Key, bool& Value) const;

    /**
      Gets the string value assigned to the key

      @return true if the value exists as a string
    */
    bool getValue(std::string Key, std::string& Value) const;


};


} } // namespaces

#endif /* __OPENFLUID_BASE_ENVPROPERTIES_HPP__ */

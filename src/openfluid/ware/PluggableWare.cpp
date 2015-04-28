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
  @file PluggableWare.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/ware/WareException.hpp>


namespace openfluid { namespace ware {

// =====================================================================
// =====================================================================

PluggableWare::PluggableWare(WareType WType)
: mp_WareEnv(NULL),m_WareType(WType),m_WareID(""),m_Initialized(false)
{

}

// =====================================================================
// =====================================================================


PluggableWare::~PluggableWare()
{

}


// =====================================================================
// =====================================================================


void PluggableWare::OPENFLUID_RaiseError(const std::string& Msg)
{
  throw WareException(openfluid::ware::WareException::computeContext(m_WareType,OPENFLUID_GetWareID()),Msg);
}


// =====================================================================
// =====================================================================


bool PluggableWare::OPENFLUID_GetRunEnvironment(const std::string& Key, std::string& Value)
{
  return mp_WareEnv->getValue(Key,Value);
}


// =====================================================================
// =====================================================================


bool PluggableWare::OPENFLUID_GetRunEnvironment(const std::string& Key, bool &Value)
{
  return mp_WareEnv->getValue(Key,Value);
}


// =====================================================================
// =====================================================================


void PluggableWare::initializeWare(const WareID_t& ID)
{
  if (!isLinked())
    throw openfluid::base::FrameworkException(
        openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION)
        .addInfos({{"wareid",ID}}),
        "initialized ware that is not fully linked");

  m_WareID = ID;
};


// =====================================================================
// =====================================================================


void PluggableWare::finalizeWare()
{

}


// =====================================================================
// =====================================================================


bool PluggableWare::isWellFormated(const openfluid::ware::WareParamKey_t& ParameterKey)
{
  if (ParameterKey.find_last_of('.') == ParameterKey.size() - 1)
    return false;

  if (ParameterKey.find_first_of('.') == 0)
    return false;

  return true;
}



}  }  // namespaces

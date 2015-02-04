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
  @file WareDescriptor.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/fluidx/WareDescriptor.hpp>

namespace openfluid {
namespace fluidx {

// =====================================================================
// =====================================================================

WareDescriptor::WareDescriptor() :
    m_WareType(NoWareType), m_Enabled(true)
{

}

// =====================================================================
// =====================================================================

WareDescriptor::~WareDescriptor()
{
}

// =====================================================================
// =====================================================================

void WareDescriptor::setParameter(const openfluid::ware::WareParamKey_t& Key,
                                  const openfluid::ware::WareParamValue_t& Value)
{
  m_Params[Key] = Value;
}

// =====================================================================
// =====================================================================

void WareDescriptor::setParameters(const openfluid::ware::WareParams_t& Params)
{
  openfluid::ware::WareParams_t::const_iterator it;

  for (it=Params.begin();it!=Params.end();++it)
    m_Params[it->first] = it->second;
}

// =====================================================================
// =====================================================================

openfluid::ware::WareParams_t WareDescriptor::getParameters()
{
  return m_Params;
}

// =====================================================================
// =====================================================================

bool WareDescriptor::isType(WareType MIType) const
{
  return (m_WareType == MIType);
}

// =====================================================================
// =====================================================================

WareDescriptor::WareType WareDescriptor::getType() const
{
  return m_WareType;
}

// =====================================================================
// =====================================================================

void WareDescriptor::eraseParameter(const openfluid::ware::WareParamKey_t& Key)
{
  m_Params.erase(Key);
}

// =====================================================================
// =====================================================================

void WareDescriptor::clearParameters()
{
  m_Params.clear();
}

// =====================================================================
// =====================================================================

bool WareDescriptor::hasParameter(std::string ParameterKey)
{
  return m_Params.count(ParameterKey);
}

// =====================================================================
// =====================================================================


void WareDescriptor::setEnabled(bool Enabled)
{
  m_Enabled = Enabled;
}


// =====================================================================
// =====================================================================


bool WareDescriptor::isEnabled() const
{
  return m_Enabled;
}




}
} // namespaces

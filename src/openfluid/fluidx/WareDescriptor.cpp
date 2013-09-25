/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file WareDescriptor.cpp
 \brief Implements ...

 \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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

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
  \file SimulatorSignature.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <boost/regex.hpp>

#include <openfluid/ware/SimulatorSignature.hpp>

namespace openfluid { namespace ware {


bool SignatureHandledTypedDataItem::getVariableNameAndType(const std::string SourceStr, std::string& VarName, openfluid::core::Value::Type& VarType)
{
  const boost::basic_regex<char> eVect("^([-.\\w]+)\\[\\]$"); //match "abc[]"
  const boost::basic_regex<char> eNone("[^[\\]][-.\\w]+"); //match "abc"
  const boost::basic_regex<char> eType("^([-.\\w]+)\\[(\\w+)\\]$");  //match "abc[type]"
  boost::smatch Type;

  if(boost::regex_match(SourceStr,Type,eVect) && Type.size() == 2)
  {
    VarName = Type[1];
    VarType = openfluid::core::Value::VECTOR;
    return true;
  }

  if(boost::regex_match(SourceStr,eNone))
  {
    VarName = SourceStr;
    VarType = openfluid::core::Value::NONE;
    return true;
  }

  if(boost::regex_match(SourceStr,Type,eType) && Type.size() == 3)
  {
    VarName = Type[1];
    return openfluid::core::Value::getValueTypeFromString(Type[2],VarType);
  }

  return false;
}


// =====================================================================
// =====================================================================


SignatureHandledTypedDataItem::SignatureHandledTypedDataItem(std::string DName,
                                                             openfluid::core::UnitClass_t UClass,
                                                             std::string DDescription,
                                                             std::string DUnit):
 SignatureHandledDataItem()
{
  UnitClass = UClass;
  Description = DDescription;
  DataUnit = DUnit;

  if(!getVariableNameAndType(DName,DataName,DataType))
    throw openfluid::base::FrameworkException("SignatureHandledTypedDataItem::SignatureHandledTypedDataItem",
                                              "Variable " + DName + " is not well formated.");
}



} } //namespaces


/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file WareSignature.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/dllexport.hpp>
#include <openfluid/config.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ware/WareIssues.hpp>
#include <openfluid/ware/WareSignature.hpp>


namespace openfluid { namespace ware {

SignatureDataItem::SignatureDataItem(const std::string& N, const std::string& D, const std::string& SI, 
  std::function<bool(const std::string&,std::string&,openfluid::core::Value::Type&)> Extractor):
  Description(D),SIUnit(SI)
{
  if (!Extractor(N,this->Name,this->DataType))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Variable " + N + " with optional type is not well formatted.");
  }
}


// =====================================================================
// =====================================================================


SignatureDataItem::SignatureDataItem(const std::string& N, const std::string& D, const std::string& SI,
  openfluid::core::Value::Type T, std::function<bool(const std::string&)> Validator):
  Name(N),Description(D),SIUnit(SI),DataType(T)
{
  if (!Validator(N))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Variable " + N + " is not well formatted.");
  }
}
} };


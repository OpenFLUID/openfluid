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
  @file ObserverSignatureSerializer.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/waresdev/ObserverSignatureSerializer.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace waresdev {


void ObserverSignatureSerializer::unserializeDataFromJSON(const openfluid::thirdparty::json& Json, 
                                                           openfluid::ware::ObserverSignature& Sign) const
{
  if (Json.contains("parameters")) //DIRTYCODE factorize
  {
    DataJsonConverter::unserializeParametersFromJSON(Json.at("parameters"),Sign.HandledData);
  }
}

openfluid::ware::ObserverSignature ObserverSignatureSerializer::fromJSON(const openfluid::thirdparty::json& Json) const
{
  openfluid::ware::ObserverSignature Sign = fromJSONBase(Json);

   if (!Json.contains("observer"))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Missing observer entry");
  }

  const auto JsonObs = Json.at("observer");

  if (JsonObs.contains("data"))
  {
    unserializeDataFromJSON(JsonObs.at("data"),Sign);
  }

  return Sign;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json ObserverSignatureSerializer::toJSON(const openfluid::ware::ObserverSignature& Sign) const
{
  openfluid::thirdparty::json Json = toJSONBase(Sign);

  Json["observer"] = openfluid::thirdparty::json::object();
  Json["observer"]["data"] = DataJsonConverter::serializeDataToJSON(Sign.HandledData);

  return Json;
}


// =====================================================================
// =====================================================================


std::string ObserverSignatureSerializer::toWareCPP(const openfluid::ware::ObserverSignature& Sign) const
{
  std::string CPP;
  
  CPP += CppWriter::getCPPHead("openfluid/ware/ObserverSignature.hpp","openfluid::ware::ObserverSignature");
  CPP += toWareCPPBase(Sign);

  CPP += CppWriter::toWareCPPParams(Sign.HandledData);
  
  CPP += CppWriter::getCPPTail();

  return CPP;
}


// =====================================================================
// =====================================================================


std::string ObserverSignatureSerializer::toWareCMake(const openfluid::ware::ObserverSignature& Sign) const
{
  std::string CMake;

  CMake += CppWriter::getHead("#");
  CMake += toWareCMakeBase(Sign);

  CMake += "SET(WARE_TYPE \"observer\")\n";
  CMake += "SET(WARE_IS_OBSERVER TRUE)\n";

  return CMake;
}


// =====================================================================
// =====================================================================


void ObserverSignatureSerializer::writeToBuildFiles(const openfluid::ware::ObserverSignature& Sign, 
                                                    const std::string& Path) const
{
  writeToWareCPPFile(Sign,
                     openfluid::tools::Path({Path,openfluid::config::WARESDEV_BUILD_MAINSIGN}).toGeneric());
  writeToParamsUICPPFile(Sign,
                         openfluid::tools::Path({Path,openfluid::config::WARESDEV_BUILD_PARAMSUISIGN}).toGeneric());
  writeToWareCMakeFile(Sign,
                       openfluid::tools::Path({Path,openfluid::config::WARESDEV_BUILD_MAININFO}).toGeneric());
  writeToParamsUICMakeFile(Sign,
                          openfluid::tools::Path({Path,openfluid::config::WARESDEV_BUILD_PARAMSUIINFO}).toGeneric());
}


} }  // namespaces

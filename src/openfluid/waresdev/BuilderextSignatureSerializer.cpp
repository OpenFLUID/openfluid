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
  @file BuilderextSignatureSerializer.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/waresdev/BuilderextSignatureSerializer.hpp>


namespace openfluid { namespace waresdev {


openfluid::builderext::BuilderExtensionSignature
BuilderextSignatureSerializer::fromJSON(const openfluid::thirdparty::json& Json) const
{
  openfluid::builderext::BuilderExtensionSignature Sign = fromJSONBase(Json);

  if (!Json.contains("builderext"))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Missing builderext entry");
  }

  Sign.MenuText = Json.at("builderext").value("menutext","");
  Sign.setCategoryFromString(Json.at("builderext").value("category",""));
  Sign.setModeFromString(Json.at("builderext").value("mode",""));

  return Sign;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json 
BuilderextSignatureSerializer::toJSON(const openfluid::builderext::BuilderExtensionSignature& Sign) const
{
  openfluid::thirdparty::json Json = toJSONBase(Sign);

  auto JsonBext = openfluid::thirdparty::json::object();
  JsonBext["mode"] = Sign.getModeAsString();
  JsonBext["category"] = Sign.getCategoryAsString();
  JsonBext["menutext"] = Sign.MenuText;
  Json["builderext"] = JsonBext;

  return Json;
}


// =====================================================================
// =====================================================================


std::string BuilderextSignatureSerializer::toCPP(const openfluid::builderext::BuilderExtensionSignature& Sign) const
{
  std::string CPP;
  
  CPP += getCPPHead("openfluid/builderext/BuilderExtensionSignature.hpp",
                    "openfluid::builderext::BuilderExtensionSignature");
  CPP += toCPPBase(Sign);
  
  CPP += "\n";

  CPP += getCPPAssignment("Mode","openfluid::builderext::ExtensionMode::"+
                                 openfluid::tools::toUpperCase(Sign.getModeAsString()));  
  CPP += getCPPAssignment("Category","openfluid::builderext::ExtensionCategory::"+
                                     openfluid::tools::toUpperCase(Sign.getCategoryAsString()));  
  CPP += getCPPAssignment("MenuText",Sign.MenuText,true);
  
  CPP += getCPPTail();

  return CPP;
}


} }  // namespaces

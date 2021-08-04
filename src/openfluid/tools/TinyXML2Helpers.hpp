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
  @file TinyXML2Helpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_TOOLS_TINYXML2HELPERS_HPP__
#define __OPENFLUID_TOOLS_TINYXML2HELPERS_HPP__


#include <string>

#include <tinyxml2.h>


namespace openfluid { namespace tools {


std::string getXMLAttribute(const tinyxml2::XMLElement* const Elt, const std::string& AttrName,
                              const std::string& DefaultValue = "")
{
  if (Elt != nullptr && Elt->Attribute(AttrName.c_str()) != nullptr) 
  {
    return std::string(Elt->Attribute(AttrName.c_str()));
  }
  return DefaultValue;
}


// =====================================================================
// =====================================================================


std::string getXMLText(const tinyxml2::XMLElement* const Elt, const std::string& DefaultValue = "")
{
  if (Elt != nullptr && Elt->GetText() != nullptr) 
  {
    return std::string(Elt->GetText());
  }
  return DefaultValue;
}


// =====================================================================
// =====================================================================


std::string getOpenFLUIDXMLFormat(const tinyxml2::XMLElement* const Elt)
{  
  return getXMLAttribute(Elt,"format");
}


// =====================================================================
// =====================================================================


tinyxml2::XMLElement* prepareOpenFLUIDXMLDoc(tinyxml2::XMLDocument& Doc, const std::string& FormatVersion)
{
  auto Decl = Doc.NewDeclaration();
  Doc.InsertFirstChild(Decl);
  auto OFElt = Doc.NewElement("openfluid");
  OFElt->SetAttribute("format",FormatVersion.c_str());
  Doc.InsertEndChild(OFElt);
  
  return OFElt;
}


} } // namespaces


#endif /* __OPENFLUID_TOOLS_TINYXML2HELPERS_HPP__ */

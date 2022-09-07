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
  @file BuilderextSignatureSerializer.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEV_BUILDEREXTSIGNATURESERIALIZER_HPP__
#define __OPENFLUID_WARESDEV_BUILDEREXTSIGNATURESERIALIZER_HPP__


#include <openfluid/waresdev/WareSignatureSerializer.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


class OPENFLUID_API BuilderextSignatureSerializer : 
  public WareSignatureSerializer<openfluid::builderext::BuilderExtensionSignature>
{
  public:

    BuilderextSignatureSerializer() : 
    WareSignatureSerializer<openfluid::builderext::BuilderExtensionSignature>()
    { }

    ~BuilderextSignatureSerializer()
    { }

    openfluid::builderext::BuilderExtensionSignature fromJSON(const openfluid::thirdparty::json& Json) const;

    openfluid::thirdparty::json toJSON(const openfluid::builderext::BuilderExtensionSignature& Sign) const;

    std::string toWareCPP(const openfluid::builderext::BuilderExtensionSignature& Sign) const;

    std::string toWareCMake(const openfluid::builderext::BuilderExtensionSignature& Sign) const;

    void writeToBuildFiles(const openfluid::builderext::BuilderExtensionSignature& Sign, const std::string& Path) const;
};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_BUILDEREXTSIGNATURESERIALIZER_HPP__ */

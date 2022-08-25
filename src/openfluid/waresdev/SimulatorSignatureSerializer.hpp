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
  @file SimulatorSignatureSerializer.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEV_SIMULATORSIGNATURESERIALIZER_HPP__
#define __OPENFLUID_WARESDEV_SIMULATORSIGNATURESERIALIZER_HPP__


#include <openfluid/waresdev/WareSignatureSerializer.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/thirdparty/XML.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


class OPENFLUID_API SimulatorSignatureSerializer : public WareSignatureSerializer<openfluid::ware::SimulatorSignature>
{
  private:

    static std::string getCPPDataString(const std::string Member, 
                                        const std::vector<openfluid::ware::SignatureDataItem>& Data);
    
    static std::string getCPPSpatialDataString(const std::string Member,
                                               const std::vector<openfluid::ware::SignatureSpatialDataItem>& Data);

    void unserializeParametersFromJSON(const openfluid::thirdparty::json& Json, 
                                       openfluid::ware::SimulatorSignature& Sign) const;

    void unserializeAttributesFromJSON(const openfluid::thirdparty::json& Json, 
                                       openfluid::ware::SimulatorSignature& Sign) const;

    void unserializeVariablesFromJSON(const openfluid::thirdparty::json& Json, 
                                      openfluid::ware::SimulatorSignature& Sign) const;

    void unserializeEventsFromJSON(const openfluid::thirdparty::json& Json, 
                                   openfluid::ware::SimulatorSignature& Sign) const;

    void unserializeExtrafilesFromJSON(const openfluid::thirdparty::json& Json, 
                                       openfluid::ware::SimulatorSignature& Sign) const;

    void unserializeDataFromJSON(const openfluid::thirdparty::json& Json, 
                                 openfluid::ware::SimulatorSignature& Sign) const;

    void unserializeSpatialGraphFromJSON(const openfluid::thirdparty::json& Json, 
                                         openfluid::ware::SimulatorSignature& Sign) const;

    void unserializeSchedulingFromJSON(const openfluid::thirdparty::json& Json, 
                                       openfluid::ware::SimulatorSignature& Sign) const;

    openfluid::thirdparty::json serializeParametersToJSON(const openfluid::ware::SimulatorSignature& Sign) const;

    openfluid::thirdparty::json serializeAttributesToJSON(const openfluid::ware::SimulatorSignature& Sign) const;

    openfluid::thirdparty::json serializeVariablesToJSON(const openfluid::ware::SimulatorSignature& Sign) const;

    openfluid::thirdparty::json serializeEventsToJSON(const openfluid::ware::SimulatorSignature& Sign) const;

    openfluid::thirdparty::json serializeExtrafilesToJSON(const openfluid::ware::SimulatorSignature& Sign) const;

    openfluid::thirdparty::json serializeDataToJSON(const openfluid::ware::SimulatorSignature& Sign) const;

    openfluid::thirdparty::json serializeSpatialGraphToJSON(const openfluid::ware::SimulatorSignature& Sign) const;

    openfluid::thirdparty::json serializeSchedulingToJSON(const openfluid::ware::SimulatorSignature& Sign) const;


  public:

    SimulatorSignatureSerializer() : 
    WareSignatureSerializer<openfluid::ware::SimulatorSignature>()
    { }

    ~SimulatorSignatureSerializer()
    { }

    openfluid::ware::SimulatorSignature fromJSON(const openfluid::thirdparty::json& Json) const;

    openfluid::thirdparty::json toJSON(const openfluid::ware::SimulatorSignature& Sign) const;

    std::string toCPP(const openfluid::ware::SimulatorSignature& Sign) const;
};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_SIMULATORSIGNATURESERIALIZER_HPP__ */

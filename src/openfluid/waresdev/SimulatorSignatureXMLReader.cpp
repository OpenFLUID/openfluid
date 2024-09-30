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
  @file SimulatorSignatureXMLReader.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <openfluid/thirdparty/XML.hpp>
#include <openfluid/waresdev/SimulatorSignatureXMLReader.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace waresdev {


class DataNodeFields
{
  public:

    std::string Name;

    std::string UnitsClass;

    std::string IOMode;

    std::string SIUnit;

    std::string Description;

    std::string Type;

    DataNodeFields(const openfluid::thirdparty::xml::XMLElement* const Elt, 
                  const std::vector<std::string>& ExpectedIOModes = {})
    {     
      if (Elt != nullptr)
      {      
        Name = openfluid::thirdparty::getXMLAttribute(Elt,"name");
        UnitsClass = openfluid::thirdparty::getXMLAttribute(Elt,"unitsclass");
        IOMode = openfluid::thirdparty::getXMLAttribute(Elt,"iomode");
        SIUnit = openfluid::thirdparty::getXMLAttribute(Elt,"siunit");
        Type = openfluid::thirdparty::getXMLAttribute(Elt,"type");
        Description = openfluid::thirdparty::getXMLText(Elt);
      }

      if (std::find(ExpectedIOModes.begin(),ExpectedIOModes.end(),IOMode) == ExpectedIOModes.end())
      {
        IOMode.clear();
      }
    }
};


// =====================================================================
// =====================================================================


bool SimulatorSignatureXMLReader::loadFromFile(const std::string& FilePath, 
                                               openfluid::ware::SimulatorSignature& Signature)
{
  Signature.clear();

  openfluid::thirdparty::xml::XMLDocument Doc;

  if (Doc.LoadFile(FilePath.c_str()) == openfluid::thirdparty::xml::XML_SUCCESS)
  {
    const auto Root = Doc.RootElement();

    if (Root != nullptr && std::string(Root->Name()) == "openfluid")
    {
      const auto GhostElt = Root->FirstChildElement("ghost-simulator");
      if (GhostElt != nullptr && GhostElt->Attribute("ID") != nullptr)
      {
        std::string GhostID = openfluid::thirdparty::getXMLAttribute(GhostElt,"ID");

        if (!boost::starts_with(openfluid::tools::FilesystemPath(FilePath).filename(),GhostID))
        {
          return false;
        }
        
        Signature.ID = GhostID;

        // infos
        const auto InfosElt = GhostElt->FirstChildElement("infos");
        if (InfosElt != nullptr)
        {
          for (auto Elt = InfosElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
          {
            std::string TagName(Elt->Name());
            std::string TagText = openfluid::thirdparty::getXMLText(Elt);

            if (TagName == "name")
            {
              Signature.Name = TagText;
            }
            else if (TagName == "description")
            {
              Signature.Description = TagText;
            }
            else if (TagName == "author")
            {
              Signature.Authors.push_back(
                std::pair<std::string,std::string>(openfluid::thirdparty::getXMLAttribute(Elt,"name"),
                                                   openfluid::thirdparty::getXMLAttribute(Elt,"email"))
              );
            }
            else if (TagName == "status")
            {
              std::string StatusStr = openfluid::tools::toLowerCase(TagText);

              if (StatusStr == "experimental")
              {
                Signature.Status = openfluid::ware::EXPERIMENTAL;
              }
              else if (StatusStr == "beta")
              {
                Signature.Status = openfluid::ware::BETA;
              }
              else if (StatusStr == "stable")
              {
                Signature.Status = openfluid::ware::STABLE;
              }
              else
              {
                return false;
              }
            }
            else if (TagName == "domain")
            {
              Signature.Domain = TagText;
            }
            else if (TagName == "process")
            {
              Signature.Process = TagText;
            }
            else if (TagName == "method")
            {
              Signature.Method = TagText;
            }
            else if (TagName == "version")
            {
              Signature.Version = TagText;
            }
          }
        }

        // data
        const auto DataElt = GhostElt->FirstChildElement("data");
        if (DataElt != nullptr)
        {
          for (auto Elt = DataElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
          {
            std::string TagName(Elt->Name());

            if (TagName == "parameter")
            {
              DataNodeFields Data(Elt,{"required","used"});

              if (Data.IOMode.empty() || Data.Name.empty())
              {
                return false;
              }

              openfluid::ware::SignatureDataItem Param;
              Param.Name = Data.Name;
              Param.SIUnit = Data.SIUnit;
              Param.Description = Data.Description;

              if (Data.IOMode == "required")
              {
                Signature.HandledData.RequiredParams.push_back(Param);
              }
              else if (Data.IOMode == "used")
              {
                Signature.HandledData.UsedParams.push_back(Param);
              }
            }
            else if (TagName == "extrafile")
            {
              DataNodeFields Data(Elt,{"required","used"});

              if (Data.IOMode.empty() || Data.Name.empty())
              {
                return false;
              }

              if (Data.IOMode == "required")
              {
                Signature.HandledData.RequiredExtraFiles.push_back(Data.Name);
              }
              else if (Data.IOMode == "used")
              {
                Signature.HandledData.UsedExtraFiles.push_back(Data.Name);
              }
            }
            else if (TagName == "events")
            {
              DataNodeFields Data(Elt);

              if (Data.UnitsClass.empty())
              {
                return false;
              }

              Signature.SimulatorHandledData.UsedEventsOnUnits.push_back(Data.UnitsClass);
            }
            else if (TagName == "variable")
            {
              DataNodeFields Data(Elt,{"required","used","produced","updated"});

              if (Data.IOMode.empty() || Data.Name.empty() || Data.UnitsClass.empty())
              {
                return false;
              }

              openfluid::ware::SignatureSpatialDataItem Variable;
              Variable.Name = Data.Name;
              Variable.UnitsClass = Data.UnitsClass;
              Variable.Description = Data.Description;
              Variable.SIUnit = Data.SIUnit;
              if (!Data.Type.empty())
              {
                if (!openfluid::core::Value::getValueTypeFromString(Data.Type,Variable.DataType))
                {
                  return false;
                }
              }

              if (Data.IOMode == "required")
              {
                Signature.HandledData.RequiredVars.push_back(Variable);
              }
              else if (Data.IOMode == "used")
              {
                Signature.HandledData.UsedVars.push_back(Variable);
              }
              else if (Data.IOMode == "produced")
              {
                Signature.SimulatorHandledData.ProducedVars.push_back(Variable);
              }
              else if (Data.IOMode == "updated")
              {
                Signature.SimulatorHandledData.UpdatedVars.push_back(Variable);
              }
            }
            else if (TagName == "attribute")
            {
              DataNodeFields Data(Elt,{"required","used","produced"});

              if (Data.IOMode.empty() || Data.Name.empty() || Data.UnitsClass.empty())
              {
                return false;
              }

              openfluid::ware::SignatureSpatialDataItem Attribute;
              Attribute.Name = Data.Name;
              Attribute.UnitsClass = Data.UnitsClass;
              Attribute.Description = Data.Description;
              Attribute.SIUnit = Data.SIUnit;

              if (Data.IOMode == "required")
              {
                Signature.HandledData.RequiredAttribute.push_back(Attribute);
              }
              else if (Data.IOMode == "used")
              {
                Signature.HandledData.UsedAttribute.push_back(Attribute);
              }
              else if (Data.IOMode == "produced")
              {
                Signature.SimulatorHandledData.ProducedAttribute.push_back(Attribute);
              }
            }
          }
        }

        // spatial graph
        const auto SpatialElt = GhostElt->FirstChildElement("spatialgraph");
        if (SpatialElt != nullptr)
        {
          for (auto Elt = SpatialElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
          {
            std::string TagName(Elt->Name());
            std::string TagText = openfluid::thirdparty::getXMLText(Elt);

            if (TagName == "description")
            {
              Signature.HandledUnitsGraph.UpdatedUnitsGraph = TagText;
            }
            else if (TagName == "unitsclass")
            {
              std::string NameStr = openfluid::thirdparty::getXMLAttribute(Elt,"name");
              if (NameStr.empty())
              {
                return false;
              }

              openfluid::ware::SignatureUnitsClassItem UnitsClassItem;
              UnitsClassItem.UnitsClass = NameStr;
              UnitsClassItem.Description = TagText;
              Signature.HandledUnitsGraph.UpdatedUnitsClass.push_back(UnitsClassItem);
            }
          }
        }

        // scheduling
        const auto SchedElt = GhostElt->FirstChildElement("scheduling");
        if (SchedElt != nullptr)
        {
          std::string ModeStr = openfluid::thirdparty::getXMLAttribute(SchedElt,"mode");

          if (ModeStr == "undefined")
          {
            Signature.TimeScheduling.setAsUndefined();
          }
          else if (ModeStr == "default")
          {
            Signature.TimeScheduling.setAsDefaultDeltaT();
          }
          else if (ModeStr == "fixed")
          {
            int Val = 0;

            if (SchedElt->QueryIntAttribute("value",&Val) == openfluid::thirdparty::xml::XML_SUCCESS)
            {
              Signature.TimeScheduling.setAsFixed(Val);
            }
            else
            {
              return false;
            }

            Signature.TimeScheduling.setAsFixed(Val);
          }
          else if (ModeStr == "range")
          {
            int Min = 0;
            int Max = 0;

            if (SchedElt->QueryIntAttribute("min",&Min) == openfluid::thirdparty::xml::XML_SUCCESS &&
                SchedElt->QueryIntAttribute("max",&Max) == openfluid::thirdparty::xml::XML_SUCCESS)
            {
              Signature.TimeScheduling.setAsRange(Min,Max);
            }
            else
            {
              return false;
            }
          }
          else
          {
            return false;
          }
        }
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }

  return true;
}


} }  // namespaces


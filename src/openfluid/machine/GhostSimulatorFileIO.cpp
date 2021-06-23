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
  @file GhostSimulatorFileIO.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <fstream>

#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QStringList>

#include <openfluid/machine/GhostSimulatorFileIO.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace machine {


class DataNodeFields
{
  public:

    std::string Name;

    std::string UnitsClass;

    std::string IOMode;

    std::string SIUnit;

    std::string Description;

    std::string Type;

    DataNodeFields(const QDomElement& Node, const QStringList& ExpectedIOModes = QStringList()):
      Name(Node.attribute("name").toStdString()),UnitsClass(Node.attribute("unitsclass").toStdString()),
      IOMode(Node.attribute("iomode").toStdString()),SIUnit(Node.attribute("siunit").toStdString()),
      Description(Node.text().toStdString()),Type(Node.attribute("type").toStdString())
    {
      if (!ExpectedIOModes.contains(QString::fromStdString(IOMode)))
      {
        IOMode.clear();
      }
    }
};


// =====================================================================
// =====================================================================


std::string escapeXMLEntities(const std::string& Str)
{
  return openfluid::tools::escapeXMLEntities(QString::fromStdString(Str)).toStdString();
}


// =====================================================================
// =====================================================================


std::string getXMLParameterString(const openfluid::ware::SignatureDataItem& Data,
                                  const std::string& IoModeStr)
{
  std::string XMLStr;

  XMLStr = "<parameter name=\"" + Data.DataName + "\" iomode=\""+IoModeStr+"\" siunit=\""
                                + escapeXMLEntities(Data.DataUnit) + "\">"
                                + escapeXMLEntities(Data.Description) + "</parameter>\n";

  return XMLStr;
}


// =====================================================================
// =====================================================================


std::string getXMLVariableString(const openfluid::ware::SignatureTypedSpatialDataItem& Data,
                                 const std::string& IoModeStr)
{
  std::string XMLStr;

  XMLStr = "<variable name=\"" + Data.DataName + "\" iomode=\""+IoModeStr+"\" " + "unitsclass=\"" + Data.UnitsClass;
  if (Data.DataType != openfluid::core::Value::NONE)
  {
    XMLStr += "\" type=\"" + openfluid::core::Value::getStringFromValueType(Data.DataType);
  }
  XMLStr += "\" siunit=\"" + escapeXMLEntities(Data.DataUnit) + "\">"
                           + escapeXMLEntities(Data.Description) + "</variable>\n";

  return XMLStr;
}


// =====================================================================
// =====================================================================


std::string getXMLAttributeString(const openfluid::ware::SignatureSpatialDataItem& Data,
                                  const std::string& IoModeStr)
{
  std::string XMLStr;

  XMLStr = "<attribute name=\"" + Data.DataName + "\" iomode=\""+IoModeStr+"\" " + "unitsclass=\"" + Data.UnitsClass
                                + "\" siunit=\"" + escapeXMLEntities(Data.DataUnit) + "\">"
                                + escapeXMLEntities(Data.Description) + "</attribute>\n";

  return XMLStr;
}


// =====================================================================
// =====================================================================


bool GhostSimulatorFileIO::saveToFile(const openfluid::ware::SimulatorSignature& Signature, const std::string& DirPath)
{
  const std::string Indent = "  ";

  std::ofstream OutFile;

  std::string OutFilename = openfluid::tools::Filesystem::joinPath({DirPath,Signature.ID+
                                                                            openfluid::config::SIMULATORS_GHOSTS_SUFFIX+
                                                                            openfluid::config::GHOSTS_EXT});

  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  OutFile << "<openfluid>\n";

  OutFile << Indent << "<ghost-simulator ID=\"" << Signature.ID << "\">\n";


  // infos

  OutFile << Indent << Indent << "<infos>\n";
  OutFile << Indent << Indent << Indent << "<name>" << escapeXMLEntities(Signature.Name) << "</name>\n";
  OutFile << Indent << Indent << Indent << "<description>" << escapeXMLEntities(Signature.Description)
                                        << "</description>\n";
  for (auto& Author: Signature.Authors)
  {
    OutFile << Indent << Indent << Indent << "<author name=\"" << escapeXMLEntities(Author.first)
                                          << "\" email=\"" << Author.second << "\" />\n";
  }
  OutFile << Indent << Indent << Indent << "<status>";
  if (Signature.Status == openfluid::ware::EXPERIMENTAL)
  {
    OutFile << "experimental";
  }
  else if (Signature.Status == openfluid::ware::BETA)
  {
    OutFile << "beta";
  }
  else if (Signature.Status == openfluid::ware::STABLE)
  {
    OutFile << "stable";
  }
  OutFile << "</status>\n";

  OutFile << Indent << Indent << Indent << "<domain>" << escapeXMLEntities(Signature.Domain) << "</domain>\n";
  OutFile << Indent << Indent << Indent << "<process>" << escapeXMLEntities(Signature.Process) << "</process>\n";
  OutFile << Indent << Indent << Indent << "<method>" << escapeXMLEntities(Signature.Method) << "</method>\n";
  OutFile << Indent << Indent << Indent << "<version>" << escapeXMLEntities(Signature.Version) << "</version>\n";
  OutFile << Indent << Indent << "</infos>\n";


  // data

  OutFile << Indent << Indent << "<data>\n";

  for (auto& Param: Signature.HandledData.RequiredParams)
  {
    OutFile << Indent << Indent << Indent << getXMLParameterString(Param,"required");
  }

  for (auto& Param: Signature.HandledData.UsedParams)
  {
    OutFile << Indent << Indent << Indent << getXMLParameterString(Param,"used");
  }

  for (auto& ExtraFile: Signature.HandledData.RequiredExtraFiles)
  {
    OutFile << Indent << Indent << Indent << "<extrafile name=\"" << ExtraFile << "\" iomode=\"required\" />\n";
  }

  for (auto& ExtraFile: Signature.HandledData.UsedExtraFiles)
  {
    OutFile << Indent << Indent << Indent << "<extrafile name=\"" << ExtraFile << "\" iomode=\"used\" />\n";
  }

  for (auto& EventClass: Signature.HandledData.UsedEventsOnUnits)
  {
    OutFile << Indent << Indent << Indent << "<events unitsclass=\"" << EventClass << "\" />\n";
  }

  for (auto& Var: Signature.HandledData.RequiredVars)
  {
    OutFile << Indent << Indent << Indent << getXMLVariableString(Var,"required");
  }

  for (auto& Var: Signature.HandledData.UsedVars)
  {
    OutFile << Indent << Indent << Indent << getXMLVariableString(Var,"used");
  }

  for (auto& Var: Signature.HandledData.ProducedVars)
  {
    OutFile << Indent << Indent << Indent << getXMLVariableString(Var,"produced");
  }

  for (auto& Var: Signature.HandledData.UpdatedVars)
  {
    OutFile << Indent << Indent << Indent << getXMLVariableString(Var,"updated");
  }

  for (auto& Var: Signature.HandledData.RequiredAttribute)
  {
    OutFile << Indent << Indent << Indent << getXMLAttributeString(Var,"required");
  }

  for (auto& Var: Signature.HandledData.UsedAttribute)
  {
    OutFile << Indent << Indent << Indent << getXMLAttributeString(Var,"used");
  }

  for (auto& Var: Signature.HandledData.ProducedAttribute)
  {
    OutFile << Indent << Indent << Indent << getXMLAttributeString(Var,"produced");
  }

  OutFile << Indent << Indent << "</data>\n";


  // spatial graph

  OutFile << Indent << Indent << "<spatialgraph>\n";

  OutFile << Indent << Indent << Indent << "<description>"
                                        << escapeXMLEntities(Signature.HandledUnitsGraph.UpdatedUnitsGraph)
                                        << "</description>\n";

  for (auto& UClass: Signature.HandledUnitsGraph.UpdatedUnitsClass)
  {
    OutFile << Indent << Indent << Indent << "<unitsclass name=\"" << UClass.UnitsClass << "\">"
                                            << escapeXMLEntities(UClass.Description) << "</unitsclass>\n";
  }

  OutFile << Indent << Indent << "</spatialgraph>\n";


  // scheduling

  std::string SchedStr;

  if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::UNDEFINED)
  {
    SchedStr = "mode=\"undefined\"";
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT)
  {
    SchedStr = "mode=\"default\"";
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED)
  {
    SchedStr = "mode=\"fixed\"";
    QString ExtraAttr(" value=\"%1\"");
    ExtraAttr = ExtraAttr.arg(Signature.TimeScheduling.Min);
    SchedStr += ExtraAttr.toStdString();
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
  {
    SchedStr = "mode=\"range\"";
    QString ExtraAttr(" min=\"%1\" max=\"%2\"");
    ExtraAttr = ExtraAttr.arg(Signature.TimeScheduling.Min).arg(Signature.TimeScheduling.Max);
    SchedStr += ExtraAttr.toStdString();
  }

  OutFile << Indent << Indent << "<scheduling "<< SchedStr << " />\n";


  OutFile << Indent << "</ghost-simulator>\n";

  OutFile << "</openfluid>\n";

  OutFile.close();

  return true;
}


// =====================================================================
// =====================================================================


bool GhostSimulatorFileIO::loadFromFile(const std::string& FilePath,openfluid::ware::SimulatorSignature& Signature)
{
  QDomDocument Doc;
  QDomElement Root;
  QString FilePathQStr = QString::fromStdString(FilePath);

  Signature.clear();


  QFile File(FilePathQStr);

  if (!File.open(QIODevice::ReadOnly))
  {
    return false;
  }

  bool Parsed = Doc.setContent(&File);
  File.close();

  if (Parsed)
  {
    Root = Doc.documentElement();

    if (!Root.isNull())
    {
      if (Root.tagName() == "openfluid")
      {
        QDomElement GhostNode = Root.firstChildElement();

        if (GhostNode.tagName() == "ghost-simulator" &&
            GhostNode.hasAttribute("ID"))
        {
          QString GhostID = GhostNode.attributeNode("ID").value();

          if (!QFileInfo(FilePathQStr).fileName().startsWith(GhostID))
          {
            return false;
          }

          Signature.ID = GhostID.toStdString();

          QDomElement CurrNode;

          for(CurrNode = GhostNode.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
          {
            // general infos
            if (CurrNode.tagName() == "infos")
            {
              QDomElement InfoNode;

              for (InfoNode = CurrNode.firstChildElement(); !InfoNode.isNull();
                   InfoNode = InfoNode.nextSiblingElement())
              {
                if (InfoNode.tagName() == "name")
                {
                  Signature.Name = InfoNode.text().toStdString();
                }
                else if (InfoNode.tagName() == "description")
                {
                  Signature.Description = InfoNode.text().toStdString();
                }
                else if (InfoNode.tagName() == "author")
                {
                  Signature.Authors.push_back(
                        std::pair<std::string,std::string>(InfoNode.attribute("name").toStdString(),
                                                           InfoNode.attribute("email").toStdString())
                    );
                }
                else if (InfoNode.tagName() == "status")
                {
                  std::string StatusStr = InfoNode.text().toLower().toStdString();

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
                else if (InfoNode.tagName() == "domain")
                {
                  Signature.Domain = InfoNode.text().toStdString();
                }
                else if (InfoNode.tagName() == "process")
                {
                  Signature.Process = InfoNode.text().toStdString();
                }
                else if (InfoNode.tagName() == "method")
                {
                  Signature.Method = InfoNode.text().toStdString();
                }
                else if (InfoNode.tagName() == "version")
                {
                  Signature.Version = InfoNode.text().toStdString();
                }
              }
            }

            // io data
            else if (CurrNode.tagName() == "data")
            {
              QDomElement DataNode;

              for (DataNode = CurrNode.firstChildElement(); !DataNode.isNull();
                  DataNode = DataNode.nextSiblingElement())
              {

                if (DataNode.tagName() == "parameter")
                {
                  DataNodeFields Data(DataNode,QStringList() << "required" << "used");

                  if (Data.IOMode.empty() || Data.Name.empty())
                  {
                    return false;
                  }

                  openfluid::ware::SignatureDataItem Param;
                  Param.DataName = Data.Name;
                  Param.DataUnit = Data.SIUnit;
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
                else if (DataNode.tagName() == "extrafile")
                {
                  DataNodeFields Data(DataNode,QStringList() << "required" << "used");

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
                else if (DataNode.tagName() == "events")
                {
                  DataNodeFields Data(DataNode);

                  if (Data.UnitsClass.empty())
                  {
                    return false;
                  }

                  Signature.HandledData.UsedEventsOnUnits.push_back(Data.UnitsClass);
                }
                else if (DataNode.tagName() == "variable")
                {
                  DataNodeFields Data(DataNode,QStringList() << "required" << "used" << "produced" << "updated");

                  if (Data.IOMode.empty() || Data.Name.empty() || Data.UnitsClass.empty())
                  {
                    return false;
                  }

                  openfluid::ware::SignatureTypedSpatialDataItem Variable;
                  Variable.DataName = Data.Name;
                  Variable.UnitsClass = Data.UnitsClass;
                  Variable.Description = Data.Description;
                  Variable.DataUnit = Data.SIUnit;
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
                    Signature.HandledData.ProducedVars.push_back(Variable);
                  }
                  else if (Data.IOMode == "updated")
                  {
                    Signature.HandledData.UpdatedVars.push_back(Variable);
                  }
                }
                else if (DataNode.tagName() == "attribute")
                {
                  DataNodeFields Data(DataNode,QStringList() << "required" << "used" << "produced");

                  if (Data.IOMode.empty() || Data.Name.empty() || Data.UnitsClass.empty())
                  {
                    return false;
                  }

                  openfluid::ware::SignatureSpatialDataItem Attribute;
                  Attribute.DataName = Data.Name;
                  Attribute.UnitsClass = Data.UnitsClass;
                  Attribute.Description = Data.Description;
                  Attribute.DataUnit = Data.SIUnit;

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
                    Signature.HandledData.ProducedAttribute.push_back(Attribute);
                  }
                }
              }

            }

            // spatial graph
            else if (CurrNode.tagName() == "spatialgraph")
            {
              QDomElement SpatialNode;

              for (SpatialNode = CurrNode.firstChildElement(); !SpatialNode.isNull();
                   SpatialNode = SpatialNode.nextSiblingElement())
              {
                if (SpatialNode.tagName() == "description")
                {
                  Signature.HandledUnitsGraph.UpdatedUnitsGraph = SpatialNode.text().toStdString();
                }
                else if (SpatialNode.tagName() == "unitsclass")
                {
                  std::string NameStr = SpatialNode.attribute("name").toStdString();
                  if (NameStr.empty())
                  {
                    return false;
                  }

                  openfluid::ware::SignatureUnitsClassItem UnitsClassItem;
                  UnitsClassItem.UnitsClass = NameStr;
                  UnitsClassItem.Description = SpatialNode.text().toStdString();
                  Signature.HandledUnitsGraph.UpdatedUnitsClass.push_back(UnitsClassItem);
                }
              }
            }

            // scheduling
            else if (CurrNode.tagName() == "scheduling")
            {
              QDomElement ScheduleNode;

              QString ModeStr = CurrNode.attribute("mode");

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
                QString ValStr = CurrNode.attribute("value");
                bool Converted = false;

                long Val = ValStr.toLong(&Converted);
                if (!Converted)
                {
                  return false;
                }

                Signature.TimeScheduling.setAsFixed(Val);
              }
              else if (ModeStr == "range")
              {
                QString MinStr = CurrNode.attribute("min");
                QString MaxStr = CurrNode.attribute("max");

                bool Converted = false;

                long Min = MinStr.toLong(&Converted);
                if (!Converted)
                {
                  return false;
                }

                long Max = MaxStr.toLong(&Converted);
                if (!Converted)
                {
                  return false;
                }

                Signature.TimeScheduling.setAsRange(Min,Max);
              }
              else
              {
                return false;
              }
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
  }
  else
  {
    return false;
  }

  return true;
}


} }  // namespaces


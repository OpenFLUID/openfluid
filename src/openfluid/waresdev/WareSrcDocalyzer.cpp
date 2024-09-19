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
  @file WareSrcDocalyzer.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
 */


#include <fstream>
#include <sstream>

#include <boost/algorithm/string/replace.hpp>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/TemplateProcessor.hpp>
#include <openfluid/utils/Process.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/utils/PandocProxy.hpp>
#include <openfluid/thirdparty/JSON.hpp>
#include <openfluid/waresdev/WareSignatureSerializer.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/waresdev/BuilderextSignatureSerializer.hpp>
#include <openfluid/waresdev/WareSrcDocalyzer.hpp>
#include <openfluid/config.hpp>


// =====================================================================
// =====================================================================


class DocalyzerWareSignature : public openfluid::ware::WareSignature
{
  public:

    openfluid::ware::WareType getType() const
    {
      return openfluid::ware::WareType::UNDEFINED;
    }
};


// =====================================================================
// =====================================================================


struct TexSignature
{
  std::ostringstream Text;

  bool IncludeEmptyFields = false;

  static std::string toFriendly(std::string Str)
  {
    boost::algorithm::replace_all(Str,"#","\\#");
    boost::algorithm::replace_all(Str,"$","\\$");
    boost::algorithm::replace_all(Str,"_","\\_");
    boost::algorithm::replace_all(Str,"&","\\&");
    boost::algorithm::replace_all(Str,"%","\\%");
    boost::algorithm::replace_all(Str,"{","\\{");
    boost::algorithm::replace_all(Str,"}","\\}");
    boost::algorithm::replace_all(Str,"~","$\\sim$");

    return Str;
  }

  static std::string tt(const std::string& Str)
  {
    if (Str.empty())
    {
      return std::string();
    }
    return "\\texttt{"+Str+"}";
  }

  static std::string math(const std::string& Str)
  {
    if (Str.empty())
    {
      return std::string();
    }
    return "$"+Str+"$";
  }

  void addInfo(const std::string& Key, const std::string& Value, const std::string& Default = "\\textit{unknown}")
  {
    if (!IncludeEmptyFields && Value.empty())
    {
      return;
    }

    auto V = Value;
    if (V.empty())
    {
      V = Default;
    }

    Text << "\\noindent \\textbf{" << Key << "} : " << V << "\\\\\n";
  }

  void addPeopleInfo(const std::string& Key, const openfluid::ware::WareSignature::PeopleList_t& Values, 
                     const std::string& Default = "\\textit{unknown}")
  {
    if (!IncludeEmptyFields && Values.empty())
    {
      return;
    }

    Text << "\\noindent \\textbf{" << Key << "} : ";

    if (Values.empty())
    {
      Text << Default << "\\\\\n";
    }
    else
    {
      bool First = true;
      for (const auto& V : Values)
      {
        if (!First)
        {
          Text << ", ";
        }
        First = false;
        Text << V.first;
        if (!V.second.empty())
        {
          Text << " <\\href{mailto:" << V.second << "}{" << V.second << "}>";
        }
      }
      Text << "\\\\\n";
    }
  }

  void startDataTable(const std::string& Title, const std::string& ColsFormat)
  {
    Text << "\\noindent \\textbf{" << Title << "}\n";
    Text << "\\vspace{.3em}\\\\\n";
    Text << "\\begin{tabularx}{\\linewidth}{" << ColsFormat << "}\n";
    Text << "\\hline\n";
  }

  void addDataEntry(const std::vector<std::string>& Values)
  {
    Text << openfluid::tools::join(Values," & ") << "\\\\\n";
  }

  void addMulticolumnDataEntry(int Span, const std::string& Format, const std::string& Value)
  {
    Text << "\\multicolumn{" << std::to_string(Span) << "}{" << Format << "}{"<< Value << "}\\\\\n";
  }

  void endDataTable()
  {
    Text << "\\hline\n";
    Text << "\\end{tabularx}\n";
    Text << "\\vspace{0.5em}\n\\\\\n";
  }

};


// =====================================================================
// =====================================================================


namespace openfluid { namespace waresdev {


std::string WareSrcDocalyzer::DocData::getTypeAsText() const
{
  if (Type == openfluid::ware::WareType::SIMULATOR)
  {
    return "Simulator";
  }
  else if (Type == openfluid::ware::WareType::OBSERVER)
  {
    return "Observer";
  }
  else if (Type == openfluid::ware::WareType::BUILDEREXT)
  {
    return "Builder-extension";
  }
  return "";
}


// =====================================================================
// =====================================================================


WareSrcDocalyzer::WareSrcDocalyzer(const std::string& SrcPath, const std::string& OutputPath, 
                                   WareSrcDocalyzerListener* Listener,
                                   std::vector<std::string> InputFormats) : 
  m_SrcPathObj(SrcPath), m_OutPathObj(OutputPath), 
  m_DocalyzerSharePathObj({openfluid::base::Environment::getInstallPrefix(),openfluid::config::INSTALL_SHARE_PATH,
                           "openfluid","docalyzer"}),
  mp_Listener(Listener),
  m_InputFormats(InputFormats)
{
  if (OutputPath.empty())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Output path empty");
  }
  if (m_InputFormats.empty())
  {
    m_InputFormats = {"tex","Rmd","md","readme"};
  }

}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::clear()
{
  m_WorkPathObj = openfluid::tools::Path();
  m_DocData.clear();
}


// =====================================================================
// =====================================================================


const std::vector<std::string> WareSrcDocalyzer::getMainFileRelativePath(const std::string& Format) const
{
  std::vector<std::string> FileToFind = {std::string("README.md")};
  if (Format != "readme")
  {
    const std::string MainFilename = openfluid::config::WARESDEV_DOC_MAIN_FILEROOT+"."+Format;
    FileToFind = {std::string("doc"),MainFilename};
  }

  return FileToFind;
}


// =====================================================================
// =====================================================================


std::string WareSrcDocalyzer::getGeneratedFileName(const std::string& FilePart, const std::string& FileExt)
{
  std::string Ext;

  if (!FileExt.empty())
  {
    Ext = "."+FileExt;
  }

  return "__docalyzer_"+FilePart+Ext;
}


// =====================================================================
// =====================================================================


const openfluid::tools::FilesystemPath WareSrcDocalyzer::getGeneratedFilePath(const std::string& FilePart, 
                                                                              const std::string& FileExt) const
{
  return m_WorkPathObj.fromThis({openfluid::config::WARESDEV_DOC_DIR,getGeneratedFileName(FilePart,FileExt)});
}


// =====================================================================
// =====================================================================


const std::string WareSrcDocalyzer::detectFormat() const
{
  for (const auto& IF : m_InputFormats)
  {
    const auto FileToFind = getMainFileRelativePath(IF);

    mp_Listener->stageMessage("Searching for "+ openfluid::tools::Filesystem::joinPath(FileToFind));
    if (m_SrcPathObj.fromThis(FileToFind).isFile())
    {
      return IF;
    }
  }

  return std::string();
}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::processSignature(bool IncludeEmptyFields)
{
  std::ifstream SignFile(m_SrcPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE).toGeneric(),
                                               std::ifstream::in);

  try 
  {
    auto Json = openfluid::thirdparty::json::parse(SignFile);
    auto Signature = openfluid::waresdev::WareSignatureSerializer<DocalyzerWareSignature>::fromJSONBase(Json);

    TexSignature TexDoc;
    TexDoc.IncludeEmptyFields = IncludeEmptyFields;

    TexDoc.addInfo("Name",TexSignature::toFriendly(Signature.Name));
    TexDoc.addInfo("Description",TexSignature::toFriendly(Signature.Description));
    
    if (!Signature.Tags.empty())
    {
      TexDoc.addInfo("Tags",openfluid::tools::join(Signature.Tags,", "));  
    }
    TexDoc.addInfo("Version",Signature.Version);
    TexDoc.addInfo("Status",Signature.getStatusAsString());

    TexDoc.addPeopleInfo("Author(s)",Signature.Authors);
    if (!Signature.Contacts.empty())
    {
      TexDoc.addPeopleInfo("Contact(s)",Signature.Contacts);
    }
    TexDoc.addInfo("License",Signature.License);

    if (!Signature.Links.empty())
    {
      TexDoc.Text << "\\noindent \\textbf{Related links}\n";
      TexDoc.Text << "\\begin{itemize}[topsep=-5px,partopsep=0px,noitemsep]\n";
      for (const auto& L : Signature.Links)
      {
        TexDoc.Text << "\\item " << L.first << " : \\href{" << L.second << "}{" << L.second << "}\n";
      }
      TexDoc.Text << "\\end{itemize}\\\n";
    }

    TexDoc.Text << "\\vspace{.5em}\\\\\n";

    if (m_DocData.Type == openfluid::ware::WareType::SIMULATOR)
    {
      auto SimSignature = openfluid::waresdev::SimulatorSignatureSerializer().fromJSON(Json);

      // -- Parameters
      if (!SimSignature.HandledData.RequiredParams.empty() ||
          !SimSignature.HandledData.UsedParams.empty())
      {
        TexDoc.startDataTable("Parameters","lllXr");
        for (const auto& D : SimSignature.HandledData.RequiredParams)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D.Name)),
                               openfluid::core::Value::getStringFromValueType(D.DataType),
                               "required",
                               TexSignature::toFriendly(D.Description),
                               TexSignature::math(D.SIUnit)});
        }
        for (const auto& D : SimSignature.HandledData.UsedParams)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D.Name)),
                               openfluid::core::Value::getStringFromValueType(D.DataType),
                               "used",
                               TexSignature::toFriendly(D.Description),
                               TexSignature::math(D.SIUnit)});
        }
        TexDoc.endDataTable();
      }

      // -- Parameters
      if (!SimSignature.SimulatorHandledData.RequiredExtraFiles.empty() ||
          !SimSignature.SimulatorHandledData.UsedExtraFiles.empty())
      {
        TexDoc.startDataTable("Extra files","ll");
        for (const auto& D : SimSignature.SimulatorHandledData.RequiredExtraFiles)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D)),"required"});
        }
        for (const auto& D : SimSignature.SimulatorHandledData.UsedExtraFiles)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D)),"used"});
        }
        TexDoc.endDataTable();
      }

      // -- Attributes
      if (!SimSignature.SimulatorHandledData.ProducedAttribute.empty() ||
          !SimSignature.SimulatorHandledData.RequiredAttribute.empty() ||
          !SimSignature.SimulatorHandledData.UsedAttribute.empty())
      {
        TexDoc.startDataTable("Attributes","llllXr");
        for (const auto& D : SimSignature.SimulatorHandledData.ProducedAttribute)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D.Name)),
                               openfluid::core::Value::getStringFromValueType(D.DataType),
                               "produced",
                               TexSignature::toFriendly(D.UnitsClass),
                               TexSignature::toFriendly(D.Description),
                               TexSignature::math(D.SIUnit)});
        }
        for (const auto& D : SimSignature.SimulatorHandledData.RequiredAttribute)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D.Name)),
                               openfluid::core::Value::getStringFromValueType(D.DataType),
                               "required",
                               TexSignature::toFriendly(D.UnitsClass),
                               TexSignature::toFriendly(D.Description),
                               TexSignature::math(D.SIUnit)});
        }
        for (const auto& D : SimSignature.SimulatorHandledData.UsedAttribute)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D.Name)),
                               openfluid::core::Value::getStringFromValueType(D.DataType),
                               "used",
                               TexSignature::toFriendly(D.UnitsClass),
                               TexSignature::toFriendly(D.Description),
                               TexSignature::math(D.SIUnit)});
        }
        TexDoc.endDataTable();
      }

      // -- Variables
      if (!SimSignature.SimulatorHandledData.ProducedVars.empty() ||
          !SimSignature.SimulatorHandledData.RequiredVars.empty() ||
          !SimSignature.SimulatorHandledData.UsedVars.empty() ||
          !SimSignature.SimulatorHandledData.UpdatedVars.empty())
      {
        TexDoc.startDataTable("Variables","llllXr");
        for (const auto& D : SimSignature.SimulatorHandledData.ProducedVars)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D.Name)),
                               openfluid::core::Value::getStringFromValueType(D.DataType),
                               "produced",
                               TexSignature::toFriendly(D.UnitsClass),
                               TexSignature::toFriendly(D.Description),
                               TexSignature::math(D.SIUnit)});
        }
        for (const auto& D : SimSignature.SimulatorHandledData.RequiredVars)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D.Name)),
                               openfluid::core::Value::getStringFromValueType(D.DataType),
                               "required",
                               TexSignature::toFriendly(D.UnitsClass),
                               TexSignature::toFriendly(D.Description),
                               TexSignature::math(D.SIUnit)});
        }
        for (const auto& D : SimSignature.SimulatorHandledData.UsedVars)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D.Name)),
                               openfluid::core::Value::getStringFromValueType(D.DataType),
                               "used",
                               TexSignature::toFriendly(D.UnitsClass),
                               TexSignature::toFriendly(D.Description),
                               TexSignature::math(D.SIUnit)});
        }
        for (const auto& D : SimSignature.SimulatorHandledData.UpdatedVars)
        {
          TexDoc.addDataEntry({TexSignature::tt(TexSignature::toFriendly(D.Name)),
                               openfluid::core::Value::getStringFromValueType(D.DataType),
                               "updated",
                               TexSignature::toFriendly(D.UnitsClass),
                               TexSignature::toFriendly(D.Description),
                               TexSignature::math(D.SIUnit)});
        }
        TexDoc.endDataTable();
      }

      // -- Events
      if (!SimSignature.SimulatorHandledData.UsedEventsOnUnits.empty())
      {
        TexDoc.startDataTable("Events","l");
        for (const auto& D : SimSignature.SimulatorHandledData.UsedEventsOnUnits)
        {
          TexDoc.addDataEntry({TexSignature::toFriendly(D)});
        }
        TexDoc.endDataTable();
      }

      // -- Spatial graph
      if (!SimSignature.HandledUnitsGraph.UpdatedUnitsGraph.empty() ||
          !SimSignature.HandledUnitsGraph.UpdatedUnitsClass.empty())
      {
        TexDoc.startDataTable("Spatial graph","lX");
        if (!SimSignature.HandledUnitsGraph.UpdatedUnitsGraph.empty())
        {
          TexDoc.addMulticolumnDataEntry(2,"X",
                                         TexSignature::toFriendly(SimSignature.HandledUnitsGraph.UpdatedUnitsGraph));
        }
        for (const auto& D : SimSignature.HandledUnitsGraph.UpdatedUnitsClass)
        {
          TexDoc.addDataEntry({D.UnitsClass,TexSignature::toFriendly(D.Description)});
        }
        TexDoc.endDataTable();
      }

      // -- Scheduling
      TexDoc.startDataTable("Scheduling","lX");
      if (SimSignature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT)
      {
        TexDoc.addMulticolumnDataEntry(2,"X","Scheduling uses the default time step");
      }
      else if (SimSignature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED)
      {
        TexDoc.addMulticolumnDataEntry(2,"X","Scheduling uses a fixed time step");
        TexDoc.addDataEntry({"Fixed value",std::to_string(SimSignature.TimeScheduling.Min)});
      }
      else if (SimSignature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
      {
        TexDoc.addMulticolumnDataEntry(2,"X","Scheduling uses a variable time step within a range");
        TexDoc.addDataEntry({"Minimum value",std::to_string(SimSignature.TimeScheduling.Min)});
        TexDoc.addDataEntry({"Maximum value",std::to_string(SimSignature.TimeScheduling.Max)});
      }
      else
      {
        TexDoc.addMulticolumnDataEntry(2,"X","Scheduling is undefined");
      }
      TexDoc.endDataTable();
    }
    else if (m_DocData.Type == openfluid::ware::WareType::BUILDEREXT)
    {
      auto BExtSignature = openfluid::waresdev::BuilderextSignatureSerializer().fromJSON(Json);

      std::string ModeTxt = "Unknown";
      std::string CategoryTxt = "Other";

      if (BExtSignature.Mode == openfluid::builderext::ExtensionMode::MODAL)
      {
        ModeTxt = "Modal";
      }
      else if (BExtSignature.Mode == openfluid::builderext::ExtensionMode::MODELESS)
      {
        ModeTxt = "Modeless";
      }
      else if (BExtSignature.Mode == openfluid::builderext::ExtensionMode::WORKSPACE)
      {
        ModeTxt = "Workspace panel";
      }

      if (BExtSignature.Category == openfluid::builderext::ExtensionCategory::MODEL)
      {
        ModeTxt = "Model";
      }
      else if (BExtSignature.Category == openfluid::builderext::ExtensionCategory::SPATIAL)
      {
        ModeTxt = "Spatial domain";
      }
      else if (BExtSignature.Category == openfluid::builderext::ExtensionCategory::RESULTS)
      {
        ModeTxt = "Results";
      }

      TexDoc.startDataTable("Configuration","lX");
      TexDoc.addDataEntry({"Mode",ModeTxt});
      TexDoc.addDataEntry({"Category",CategoryTxt});
      TexDoc.addDataEntry({"Menu label",TexSignature::toFriendly(BExtSignature.MenuText)});
      TexDoc.endDataTable();
    }

    m_DocData.Signature = TexDoc.Text.str();

  }
  catch (openfluid::thirdparty::json::exception& E)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error parsing signature file");
  }
}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::processLatexContent()
{
   auto MainFilePathObj = m_WorkPathObj.fromThis(getMainFileRelativePath("tex"));

   m_DocData.Content = openfluid::tools::Filesystem::readFile(MainFilePathObj);
}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::processRmarkdownContent()
{
  auto ExtProg =
      openfluid::utils::ExternalProgram::getRegisteredProgram(
        openfluid::utils::ExternalProgram::RegisteredPrograms::Rscript);

  if (!ExtProg.isFound())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Rscript program is required but not found in PATH");
  }


  // checking required R packages are installed
  for (const std::string& P : {std::string("knitr"),std::string("rmarkdown")})
  {
    openfluid::utils::Process::Command Cmd = 
    {
      .Program = ExtProg.getFullProgramPath(), 
      .Args = {m_DocalyzerSharePathObj.fromThis("R_checkpackage.R").toGeneric(),P}
    };
    Cmd.setOutErrFiles(m_WorkPathObj.toGeneric(),"checkpackage-"+P);

    if (openfluid::utils::Process::execute(Cmd) != 0)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
        openfluid::config::ERROR_MESSAGE_MISSING_R_PACKAGE+": "+P);
    }
  }

  openfluid::utils::Process::Command Cmd = 
  {
    .Program = ExtProg.getFullProgramPath(), 
    .Args = {m_DocalyzerSharePathObj.fromThis("R_rmd2latex.R").toGeneric(),
             m_WorkPathObj.fromThis(getMainFileRelativePath("Rmd")).toGeneric(),
             getGeneratedFilePath("precontent","md").toGeneric(),
             getGeneratedFilePath("content").toGeneric()},
    .WorkDir = m_WorkPathObj.fromThis(openfluid::config::WARESDEV_DOC_DIR).toGeneric()
  };
  Cmd.setOutErrFiles(m_WorkPathObj.toGeneric(),"process-content");

  if (openfluid::utils::Process::execute(Cmd) != 0)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"error processing Rmarkdown content");
  }
  
  m_DocData.Content = openfluid::tools::Filesystem::readFile(getGeneratedFilePath("content"));
}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::processMarkdownContent()
{
  // Render content in LaTeX

  if (!openfluid::utils::PandocProxy::isAvailable())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "pandoc program is required but not found in PATH");
  }

  openfluid::utils::Process::Command Cmd =
    openfluid::utils::PandocProxy::getCommand("markdown",
                                              m_WorkPathObj.fromThis(getMainFileRelativePath("md")).toGeneric(),
                                              "latex",
                                              getGeneratedFilePath("content").toGeneric(),
                                              true);
  Cmd.WorkDir = m_WorkPathObj.fromThis(openfluid::config::WARESDEV_DOC_DIR).toGeneric();
  Cmd.setOutErrFiles(m_WorkPathObj.toGeneric(),"process-content");

  if (openfluid::utils::Process::execute(Cmd) != 0)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"error processing markdown content");
  }

  m_DocData.Content = openfluid::tools::Filesystem::readFile(getGeneratedFilePath("content"));
}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::processReadmeContent()
{
  // Render README.md in LaTeX

  m_WorkPathObj.makeDirectory(openfluid::config::WARESDEV_DOC_DIR);

  if (!openfluid::utils::PandocProxy::isAvailable())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "pandoc program is required but not found in PATH");
  }

  openfluid::utils::Process::Command Cmd = 
    openfluid::utils::PandocProxy::getCommand("markdown",
                                              m_WorkPathObj.fromThis(getMainFileRelativePath("readme")).toGeneric(),
                                              "latex",
                                              getGeneratedFilePath("content").toGeneric(),
                                              true);
  Cmd.WorkDir = m_WorkPathObj.fromThis(openfluid::config::WARESDEV_DOC_DIR).toGeneric();
  Cmd.setOutErrFiles(m_WorkPathObj.toGeneric(),"process-content");

  if (openfluid::utils::Process::execute(Cmd) != 0)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"error processing README.md file");
  }

  m_DocData.Content = openfluid::tools::Filesystem::readFile(getGeneratedFilePath("content"));

}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::processContent(const std::string& Format)
{
  if (Format == "tex")
  {
    mp_Listener->stageMessage("Processing LaTeX content");
    processLatexContent();
  }
  else if (Format == "Rmd")
  {
    mp_Listener->stageMessage("Processing R Markdown content");
    processRmarkdownContent();
  }
  else if (Format == "md")
  {
    mp_Listener->stageMessage("Processing Markdown content");
    processMarkdownContent();
  }
  else if (Format == "readme")
  {
    mp_Listener->stageMessage("Processing README.md content");
    processReadmeContent();
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"unknown content format");
  }
}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::integrateDocument(const std::string& Format) const
{
  // Merge signature and content in a unique LaTeX doc

  std::string DocSource = "undefined source";
  std::string DocalyzerStr = "openfluid-docalyzer"; // root file name for layout and style files


  if (Format == "tex")
  {
    DocSource = "\\LaTeX\\ doc";
  }
  else if (Format == "Rmd")
  {
    DocSource = "R Markdown doc";
  }
  else if (Format == "md")
  {
    DocSource = "Markdown doc";
  }
  else if (Format == "readme")
  {
    DocSource = "README.md file";
  }

  // Manage standard or custom layout
  if (!m_WorkPathObj.fromThis({openfluid::config::WARESDEV_DOC_DIR,DocalyzerStr+".tex"}).isFile())
  {
    openfluid::tools::Filesystem::copyFile(m_DocalyzerSharePathObj.fromThis(DocalyzerStr+".tex").toGeneric(),
                                           m_WorkPathObj.fromThis({openfluid::config::WARESDEV_DOC_DIR,
                                                                   DocalyzerStr+".tex"}).toGeneric());
  }

  // Manage standard or custom style
  if (!m_WorkPathObj.fromThis({openfluid::config::WARESDEV_DOC_DIR,DocalyzerStr+".cls"}).isFile())
  {
    openfluid::tools::Filesystem::copyFile(m_DocalyzerSharePathObj.fromThis(DocalyzerStr+".cls").toGeneric(),
                                           m_WorkPathObj.fromThis({openfluid::config::WARESDEV_DOC_DIR,
                                                                   DocalyzerStr+".cls"}).toGeneric());
  }

  // Render integrated document using template
  openfluid::tools::TemplateProcessor::Data TplData = {
    {"docstyle",DocalyzerStr},
    {"wareid",TexSignature::toFriendly(m_DocData.ID)},
    {"waretype",TexSignature::toFriendly(m_DocData.getTypeAsText())},
    {"signature",m_DocData.Signature},
    {"content",m_DocData.Content},
    {"source",DocSource}
  };

  openfluid::tools::TemplateProcessor::Errors TplErrors;
  openfluid::tools::TemplateProcessor TplProc("##","##");
  TplProc.ignoreUnknown(true); // ignore unknown template patterns found in doc
  TplProc.renderFile(m_WorkPathObj.fromThis({openfluid::config::WARESDEV_DOC_DIR,DocalyzerStr+".tex"}).toGeneric(),
                     getGeneratedFilePath("integrated").toGeneric(),TplData,TplErrors);

}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::buildDocument() const
{
  // Build document into output format

  auto PdfExtProg =
      openfluid::utils::ExternalProgram::getRegisteredProgram(
        openfluid::utils::ExternalProgram::RegisteredPrograms::PdfLatex);

  if (!PdfExtProg.isFound())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "pdflatex program is required but not found in PATH");
  }

  openfluid::utils::Process::Command PdfCmd = 
  {
    .Program = PdfExtProg.getFullProgramPath(), 
    .Args = {"-shell-escape","-interaction=nonstopmode",
             "-output-directory",m_WorkPathObj.fromThis(openfluid::config::WARESDEV_DOC_DIR).toGeneric(),
             getGeneratedFilePath("integrated").toGeneric()},
    .WorkDir = m_WorkPathObj.fromThis(openfluid::config::WARESDEV_DOC_DIR).toGeneric()
  };
  
  auto BibExtProg =
      openfluid::utils::ExternalProgram::getRegisteredProgram(
        openfluid::utils::ExternalProgram::RegisteredPrograms::BibTex);

  if (!BibExtProg.isFound())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "bibtex program is required but not found in PATH");
  }

  openfluid::utils::Process::Command BibCmd = 
  {
    .Program = BibExtProg.getFullProgramPath(), 
    .Args = {getGeneratedFileName("integrated")},
    .WorkDir = m_WorkPathObj.fromThis(openfluid::config::WARESDEV_DOC_DIR).toGeneric()
  };
  BibCmd.setOutErrFiles(m_WorkPathObj.toGeneric(),"build-bib");

  int PassCount = 0;
  for (const std::string& Pass : {std::string("first"),std::string("second"),std::string("third")})
  {
    PassCount++;

    getGeneratedFilePath("integrated","pdf").removeFile();

    if (PassCount == 2)
    {
      mp_Listener->stageMessage("Processing bibliography");
      openfluid::utils::Process::execute(BibCmd);
    }
    
    mp_Listener->stageMessage("Building full document ("+Pass+" pass)");
    PdfCmd.setOutErrFiles(m_WorkPathObj.toGeneric(),"build-"+Pass);
    openfluid::utils::Process::execute(PdfCmd);
  }

  if (!getGeneratedFilePath("integrated","pdf").isFile())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "error building final document "+m_WorkPathObj.toGeneric());
  }

  openfluid::tools::Filesystem::copyFile(getGeneratedFilePath("integrated","pdf").toGeneric(),
                                         m_WorkPathObj.fromThis(m_DocData.ID+".pdf").toGeneric());
}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::releaseDocument() const
{
  std::string DocFilename(m_DocData.ID+".pdf");

  // Prepare output path
  m_OutPathObj.makeDirectory();
  m_OutPathObj.removeFile(DocFilename);

  // Copy built file in output Path
  openfluid::tools::Filesystem::copyFile(m_WorkPathObj.fromThis(DocFilename).toGeneric(),
                                         m_OutPathObj.fromThis(DocFilename).toGeneric());
}


// =====================================================================
// =====================================================================


void WareSrcDocalyzer::performDocalyze(bool KeepWorkData, bool IncludeEmptyFields)
{
  clear();

  mp_Listener->onDetectInputStart();

  // Detect input format
  const auto Format = detectFormat();
  if (Format.empty())
  {
    mp_Listener->onDetectInputEnd(openfluid::waresdev::WareSrcOpsListener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"cannot find main documentation file");
  }

  mp_Listener->onDetectInputEnd(openfluid::waresdev::WareSrcOpsListener::Status::OK_STATUS);


  // ----------


  mp_Listener->onPrepareStart();

  try
  {
    mp_Listener->stageMessage("Determining ware type and ID");
    const auto [Type,ID] = openfluid::waresdev::detectWareTypeAndID(
                              m_SrcPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE).toGeneric());
    if (Type == openfluid::ware::WareType::UNDEFINED || ID.empty())
    {
      mp_Listener->onPrepareEnd(openfluid::waresdev::WareSrcOpsListener::Status::ERROR_STATUS);
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"cannot determine ware type and/or ID");
    }

    m_DocData.ID = ID;
    m_DocData.Type = Type;

    mp_Listener->stageMessage("Creating work directory");
    m_WorkPathObj = m_OutPathObj.fromThis(openfluid::config::WARESDEV_DOCALYZER_WORK_DIRROOT+"_"+ID);
    m_WorkPathObj.makeDirectory();
    openfluid::tools::Filesystem::emptyDirectory(m_WorkPathObj.toGeneric());

    mp_Listener->stageMessage("Preparing doc files");
    if (Format == "readme")
    {
      openfluid::tools::Filesystem::copyFile(m_SrcPathObj.fromThis("README.md").toGeneric(),
                                             m_WorkPathObj.fromThis("README.md").toGeneric());
    }
    else
    {
      openfluid::tools::Filesystem::copyDirectory(m_SrcPathObj
                                                  .fromThis(openfluid::config::WARESDEV_DOC_DIR).toGeneric(),
                                                  m_WorkPathObj
                                                  .fromThis(openfluid::config::WARESDEV_DOC_DIR).toGeneric());
    }
  }
  catch (const openfluid::base::FrameworkException& E)
  {
    mp_Listener->onPrepareEnd(openfluid::waresdev::WareSrcOpsListener::Status::ERROR_STATUS);
    throw E;
  }
  
  mp_Listener->onPrepareEnd(openfluid::waresdev::WareSrcOpsListener::Status::OK_STATUS);


  // ----------


  mp_Listener->onBuildStart(); 

  try
  {
    mp_Listener->stageMessage("Processing signature");
    processSignature(IncludeEmptyFields);

    processContent(Format);
    
    mp_Listener->stageMessage("Integrating full document");
    integrateDocument(Format);

    buildDocument();
  }
  catch (const openfluid::base::FrameworkException& E)
  {
    mp_Listener->onBuildEnd(openfluid::waresdev::WareSrcOpsListener::Status::ERROR_STATUS);
    throw E;
  }

  mp_Listener->onBuildEnd(openfluid::waresdev::WareSrcOpsListener::Status::OK_STATUS);


  // ----------


  mp_Listener->onReleaseOutputStart();

  try
  {
    if (!m_OutPathObj.empty())
    {
      releaseDocument(); 
    }

    if (!KeepWorkData)
    {
      m_WorkPathObj.removeDirectory();
    }
  }
  catch (const openfluid::base::FrameworkException& E)
  {
    mp_Listener->onReleaseOutputEnd(openfluid::waresdev::WareSrcOpsListener::Status::ERROR_STATUS);
    throw E;
  }

  mp_Listener->onReleaseOutputEnd(openfluid::waresdev::WareSrcOpsListener::Status::OK_STATUS);

}


} }  // namespaces

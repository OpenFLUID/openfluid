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
  @file WareSrcMigrator.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
 */


#include <algorithm>
#include <filesystem>
#include <fstream>
#include <regex>

#include <boost/algorithm/string.hpp>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/TemplateProcessor.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include <openfluid/utils/Process.hpp>
#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/waresdev/ObserverSignatureSerializer.hpp>
#include <openfluid/waresdev/BuilderextSignatureSerializer.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/waresdev/WareSrcChecker.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>
#include <openfluid/waresdev/WareSrcMigrator.hpp>
#include <openfluid/thirdparty/JSON.hpp>
#include <openfluid/config.hpp>


/*
The migration process consists of 3 phases:
1) prepareMigration() : backup of original files if needed, retrieval of information 
   (ware type, id, extraction of signatures, extraction of doc if any)
2) processSources() : conversion of signature, conversion of CMake files, dispatching of existing files
3) processDoc() : processing of embedded documentation if any
*/


namespace openfluid { namespace waresdev {


std::vector<std::string> getFileLines(const openfluid::tools::Path& FileObj)
{
  std::vector<std::string> Lines;
  
  std::ifstream File(FileObj.toGeneric(),std::ios::in);

  if (File.is_open())
  {
    std::string L;
    while (std::getline(File,L))
    {
      Lines << L;
    }    
    File.close();
  }

  return Lines;
}


// =====================================================================
// =====================================================================


std::string extractBetweenTags(const std::string& Content, const std::string& BeginTag, const std::string& EndTag,
                               const bool WithTags = false)
{
  std::string::size_type Index = Content.find(BeginTag);

  if (Index != std::string::npos)
  {
    auto Copied = Content;
    Copied.erase(0,Index+BeginTag.length());

    Index = Copied.find(EndTag);
    if (Index != std::string::npos)
    {
      Copied.erase(Index,Copied.length()-Index);

      if (WithTags)
      {
        Copied.insert(0,BeginTag);
        Copied.append(EndTag);
      }

      return Copied;
    }
  }

  return std::string();
}


// =====================================================================
// =====================================================================

  
auto IsRelevantForCopy(const openfluid::tools::Path& P) 
{ 
  // rule to check that a file is relevant or must be ignored
  return (!P.filename().empty() && P.filename()[0] != '_' && P.filename()[0] != '.'); 
};


// =====================================================================
// =====================================================================


std::string adaptBuiderextSignature(const std::string& SignContent)
{
  auto AdaptedContent = SignContent;

  boost::replace_all(AdaptedContent,"MODE_UNKNOWN","ExtensionMode::UNKNOWN");
  boost::replace_all(AdaptedContent,"MODE_MODAL","ExtensionMode::WORKSPACE");
  boost::replace_all(AdaptedContent,"MODE_MODELESS","ExtensionMode::WORKSPACE");
  boost::replace_all(AdaptedContent,"MODE_WORKSPACE","ExtensionMode::WORKSPACE");

  boost::replace_all(AdaptedContent,"CAT_SPATIAL","ExtensionCategory::SPATIAL");
  boost::replace_all(AdaptedContent,"CAT_MODEL","ExtensionCategory::MODEL");
  boost::replace_all(AdaptedContent,"CAT_RESULTS","ExtensionCategory::RESULTS");
  boost::replace_all(AdaptedContent,"CAT_OTHER","ExtensionCategory::OTHER");

  return AdaptedContent;
}


// =====================================================================
// =====================================================================


std::string generateMigrationBlock(const std::string& LineComment, const std::string& EnclosedContent = "")
{
  std::string Str;

  Str += LineComment + " " + openfluid::config::MIGRATION_STRING + "(" + \
    openfluid::core::DateTime::now().getAsISOString()+")>-------\n";
  Str += EnclosedContent;
  Str += "\n" + LineComment + " -------<"+openfluid::config::MIGRATION_STRING;

  return Str;
}


// =====================================================================
// =====================================================================


template<class S>
std::vector<std::string> mergeWareshubInSignatureFile(const openfluid::tools::Path& WHFileObj,
                                                      const openfluid::tools::Path& SignFileObj)
{
  std::vector<std::string> Messages;
  if (WHFileObj.isFile())
  {
    std::ifstream WHFileStream;
    WHFileStream.open(WHFileObj.toGeneric(),std::ifstream::in);

    if (WHFileStream.is_open())
    {
      try
      {
        auto WHubDoc = openfluid::thirdparty::json::parse(WHFileStream);
        auto Signature = S().readFromJSONFile(SignFileObj.toGeneric());

        // tags
        auto Tags = WHubDoc.value("tags",std::vector<std::string>());
        for (const auto& T : Tags)
        {
          Signature.Tags.push_back(T);
        }

        // contacts
        auto Contacts = WHubDoc.value("contacts",std::vector<std::string>());
        for (const auto& C : Contacts)
        {
          // use part of the email before @ as contact name 
          Signature.Contacts.push_back({C.substr(0,C.find('@')),C});
        }

        // license
        Signature.License = WHubDoc.value("license","");

        // issues
        if (WHubDoc.contains("issues"))
        {
          const openfluid::thirdparty::json& Issues = WHubDoc["issues"];
          if (Issues.is_object())
          {
            std::set<unsigned int> CorrectIssuesIDs;
            std::vector<openfluid::ware::WareIssue> ValidIssues;
            std::vector<openfluid::ware::WareIssue> UnvalidIssues;
            for (const auto& [ID,Info] : Issues.items())
            {
              openfluid::ware::WareIssue Iss;

              std::string Title = Info.value("title","");
              if (!Title.empty())
              {
                Iss.Title = Title;
                Iss.Description = Info.value("description","");
                Iss.Creator = Info.value("creator","");
                Iss.CreatedAt = openfluid::core::DateTime::fromString(Info.value("date",""),"%Y-%m-%d");
                Iss.UpdatedAt = Iss.CreatedAt;
                Iss.IsOpen = (Info.value("state","") != "closed");

                // issue urgency in tags
                std::string Urgency = Info.value("urgency","");
                if (!Urgency.empty())
                {
                  Iss.Tags.push_back("urgency::" + Urgency);
                }

                // issue type in tags
                std::string Type = Info.value("type","");
                if (!Type.empty())
                {
                  Iss.Tags.push_back("type::" + Type);
                }

                // handle unvalid issues IDs
                int IntegerID;
                if (!openfluid::tools::toNumeric(ID, IntegerID))
                {
                  UnvalidIssues.push_back(Iss);
                }
                else
                {
                  if(IntegerID <= 0)
                  {
                    UnvalidIssues.push_back(Iss);
                  }
                  else
                  {
                    CorrectIssuesIDs.insert(IntegerID);
                    Iss.ID = IntegerID;
                    ValidIssues.push_back(Iss);
                  }
                }
              }
            }

            if(!Issues.empty())
            {
              // set valid IDs to unvalid issues
              unsigned int HighestIssueID = CorrectIssuesIDs.empty() ? 0 : *(--CorrectIssuesIDs.end());
              int CurrentValidID = HighestIssueID + 1;
              for(auto& NewValidIssue : UnvalidIssues)
              {
                NewValidIssue.ID = CurrentValidID;
                ValidIssues.push_back(NewValidIssue);
                CurrentValidID++;
              }

              for(const auto& Iss : ValidIssues)
              {
                try
                {
                  Signature.Issues.add(Iss); 
                }
                catch (openfluid::base::Exception& E)
                {
                  Messages.push_back(E.what());
                }
              }
            }
          }
        }

        S().writeToJSONFile(Signature,SignFileObj.toGeneric());
      }
      catch (openfluid::thirdparty::json::parse_error&)
      {
        // failure is not an error
        Messages.push_back("Json can not bet read");
      }
    }
  }
  return Messages;
}


// =====================================================================
// =====================================================================


WareSrcMigrator::WareSrcMigrator(const std::string& SrcPath, 
                                 WareSrcMigratorListener* Listener, const std::string& DestPath) :
  m_SrcPathObj(openfluid::tools::Path(SrcPath)), m_DestPathObj(openfluid::tools::Path(DestPath)),
  m_InPlace(DestPath.empty()), mp_Listener(Listener)
{
  if (!mp_Listener)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Listener can not be NULL");
  }
}


// =====================================================================
// =====================================================================


WareSrcMigrator::WareMigrationInfo WareSrcMigrator::prepareMigration()
{
  WareMigrationInfo Info;

  std::vector<openfluid::tools::Path> SrcFilesObjs;

  
  mp_Listener->onPrepareStart();

  if (m_InPlace)
  {
    // create backup directory for original files (only when in-place migration)
    m_SrcPathObj.makeDirectory(openfluid::config::WARESDEV_MIGRATION_ORIGINAL_DIR);

    // search for relevant files and directories to backup
    std::vector<openfluid::tools::Path> Entries;
    for (const auto& E : std::filesystem::directory_iterator{m_SrcPathObj.stdPath()})
    {
      const auto P = openfluid::tools::Path::fromStdPath(E.path());
      if (IsRelevantForCopy(P))
      {
        Entries.push_back(P);
      }
    }

    // copy files in the backup directory then remove them from the root directory
    for (const auto& SP : Entries)
    {
      const auto DP = m_SrcPathObj.fromThis({openfluid::config::WARESDEV_MIGRATION_ORIGINAL_DIR,SP.filename()});
      
      if (SP.isFile())
      {
        openfluid::tools::Filesystem::copyFile(SP.toGeneric(),DP.toGeneric());
        SP.removeFile();
      }
      else if (SP.isDirectory())
      {
        openfluid::tools::Filesystem::copyDirectory(SP.toGeneric(),DP.toGeneric(),false);
        SP.removeDirectory();
      }
    }

    m_DestPathObj = m_SrcPathObj;
    m_SrcPathObj = m_SrcPathObj.fromThis(openfluid::config::WARESDEV_MIGRATION_ORIGINAL_DIR);
    mp_Listener->stageMessage("ware will be migrated into the original source tree (original files are backed up)");
  }
  else
  {
    mp_Listener->stageMessage("ware will be migrated outside the original source tree");
  }


  for (auto const& E : std::filesystem::recursive_directory_iterator{m_SrcPathObj.stdPath()})
  {
    SrcFilesObjs.push_back(openfluid::tools::Path::fromStdPath(E.path()));
  }

  m_WorkPathObj = openfluid::tools::Path({m_DestPathObj.toGeneric(),openfluid::config::WARESDEV_MIGRATION_WORK_DIR});
  m_WorkPathObj.makeDirectory();

  if(openfluid::utils::GitProxy::isPathGitRepo(m_SrcPathObj.toGeneric()))
  {
    try
    {
      std::string CurrentBranchName = openfluid::utils::GitProxy::getCurrentBranchName(m_SrcPathObj.toGeneric());
      if(m_WorkPathObj.makeFile(openfluid::config::WARESDEV_BRANCH_INFO_FILE)) 
      {
        openfluid::tools::Filesystem::writeFile(CurrentBranchName, 
                openfluid::tools::Path({m_WorkPathObj.toGeneric(), openfluid::config::WARESDEV_BRANCH_INFO_FILE}));
      }
      else 
      {
        mp_Listener->stageMessage("Error while creating branch info file");
        openfluid::utils::log::error("Migration", "Error while creating branch info file");
      }
    }
    catch(openfluid::utils::GitOperationException& E)
    {
      mp_Listener->stageMessage(E.what());
      openfluid::utils::log::error("Migration", E.what());
    }
  }

  // 1st pass : search for simulator or observer
  for (const auto& FileObj : SrcFilesObjs)
  {
    if (openfluid::waresdev::IsCppFile(FileObj))
    {
      auto Content = openfluid::tools::Filesystem::readFile(FileObj);
      if (Info.WareType == openfluid::ware::WareType::UNDEFINED)
      {
        Info.SignatureContent = extractBetweenTags(Content,"BEGIN_SIMULATOR_SIGNATURE","END_SIMULATOR_SIGNATURE",true);

        if (!Info.SignatureContent.empty())
        {
          Info.WareType = openfluid::ware::WareType::SIMULATOR;
          Info.SignatureFile = FileObj;
          Info.TemplateData["WARETYPESTR"] = "Simulator";
          Info.TemplateData["SIGNATURENAMESPACE"] = "ware";
          Info.TemplateData["SIGNATURECLASS"] = "SimulatorSignature";
          Info.TemplateData["SIGNATURECONTENT"] = Info.SignatureContent;
          Info.CMakeDefaultWareVar = "SIM";
          Info.CMakeWareSuffix = "SIMULATOR";

          mp_Listener->stageMessage("ware type is simulator");
        }
      }

      if (Info.WareType == openfluid::ware::WareType::UNDEFINED)
      {
        Info.SignatureContent = extractBetweenTags(Content,"BEGIN_OBSERVER_SIGNATURE","END_OBSERVER_SIGNATURE",true);

        if (!Info.SignatureContent.empty())
        {
          Info.WareType = openfluid::ware::WareType::OBSERVER;
          Info.SignatureFile = FileObj;
          Info.TemplateData["WARETYPESTR"] = "Observer";
          Info.TemplateData["SIGNATURENAMESPACE"] = "ware";
          Info.TemplateData["SIGNATURECLASS"] = "ObserverSignature";
          Info.TemplateData["SIGNATURECONTENT"] = Info.SignatureContent;
          Info.CMakeDefaultWareVar = "OBS";
          Info.CMakeWareSuffix = "OBSERVER";

          mp_Listener->stageMessage("ware type is observer");
        }
      }
    }
  }

  // 2nd pass : search for builderext or parameterization UI
  for (const auto& FileObj : SrcFilesObjs)
  {
    if (openfluid::waresdev::IsCppFile(FileObj))
    {
      auto Content = openfluid::tools::Filesystem::readFile(FileObj);
      auto SignatureContent = extractBetweenTags(Content,"BEGIN_BUILDEREXT_SIGNATURE","END_BUILDEREXT_SIGNATURE",true);

      if (!SignatureContent.empty())
      {
        if (Info.WareType == openfluid::ware::WareType::UNDEFINED)
        {
          Info.SignatureFile = FileObj;
          Info.SignatureContent = SignatureContent;
          Info.WareType = openfluid::ware::WareType::BUILDEREXT;
          Info.TemplateData["WARETYPESTR"] = "Builderext";
          Info.TemplateData["SIGNATURENAMESPACE"] = "builderext";
          Info.TemplateData["SIGNATURECLASS"] = "BuilderExtensionSignature";
          Info.TemplateData["SIGNATURECONTENT"] = adaptBuiderextSignature(Info.SignatureContent);
          Info.CMakeDefaultWareVar = "BEXT";
          Info.CMakeWareSuffix = "BUILDEREXT";

          mp_Listener->stageMessage("ware type is Builder-extension");
        }
        else if (Info.WareType == openfluid::ware::WareType::SIMULATOR ||
                Info.WareType == openfluid::ware::WareType::OBSERVER)
        {
          Info.UISignatureFile = FileObj;
          Info.UISignatureContent = SignatureContent;

          mp_Listener->stageMessage("found parameterization UI");
        }
      }
    }
  }

  openfluid::base::Listener::Status Status = openfluid::base::Listener::Status::OK_STATUS;

  // 3rd pass for simulators only : search for sim2doc
  if (Info.WareType == openfluid::ware::WareType::SIMULATOR)
  {
    std::vector<std::string> Sim2DocFilesNotMigrated;
    for (const auto& FileObj : SrcFilesObjs)
    {
      if(openfluid::waresdev::IsCppFile(FileObj))
      {
        auto Content = openfluid::tools::Filesystem::readFile(FileObj);
        std::string DocContent = openfluid::tools::trim(extractBetweenTags(Content, 
                                                                           openfluid::config::SIM2DOC_BEGIN_TAG, 
                                                                           openfluid::config::SIM2DOC_END_TAG));
        if(DocContent.empty())
        {
          continue;
        }

        if(!Info.DocContent.empty())
        {
          Sim2DocFilesNotMigrated.push_back(FileObj.filename());
          continue;
        }
        
        Info.DocFile = FileObj;
        Info.DocContent = DocContent;
      }
    }

    if(!Sim2DocFilesNotMigrated.empty())
    {
      mp_Listener->stageMessage("multiple files with sim2doc tags where found. Converted file: " + 
                                Info.DocFile.filename() +
                                ". Unconverted file(s): " + openfluid::tools::join(Sim2DocFilesNotMigrated, ", "));
      Status = openfluid::base::Listener::Status::WARNING_STATUS;
    }
  }

  if (Info.WareType == openfluid::ware::WareType::UNDEFINED)
  {
    mp_Listener->onPrepareEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Unable to determine ware type");    
  }

  mp_Listener->onPrepareEnd(Status);

  return Info;
}


// =====================================================================
// =====================================================================


void WareSrcMigrator::processSignature(const WareSrcMigrator::WareMigrationInfo& Info, 
                                       const openfluid::tools::Path& WorkPathObj) const
{
  mp_Listener->onProcessSignatureStart();

  WorkPathObj.makeDirectory();

  // preparing exporter program templates
  mp_Listener->stageMessage("creating signature exporter");

  openfluid::tools::TemplateProcessor TplProc("%%","%%");
  openfluid::tools::TemplateProcessor::Errors TplErrors; 

  auto SignTplPathObj = openfluid::tools::Path({openfluid::base::Environment::getInstallPrefix(),
                                                openfluid::config::SHARE_WARESDEV_INSTALL_PATH,
                                                "migration","templates"});
  
  auto TplData = Info.TemplateData;
  TplData["SIGNATUREJSONPATH"] = WorkPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE).toGeneric();

  TplProc.renderFile(SignTplPathObj.fromThis("migrate_signature.cpp").toGeneric(),
                     WorkPathObj.fromThis("migrate_signature.cpp").toGeneric(),
                     TplData,TplErrors);

  TplProc.renderFile(SignTplPathObj.fromThis(openfluid::config::WARESDEV_SRC_CMAKESTDFILE).toGeneric(),
                     WorkPathObj.fromThis(openfluid::config::WARESDEV_SRC_CMAKESTDFILE).toGeneric(),
                     TplData,TplErrors);


  // configuring export program
  mp_Listener->stageMessage("configuring signature exporter");
  std::map<std::string,std::string> CMakeVars = openfluid::waresdev::initializeConfigureVariables();
  auto ConfigureCmd = openfluid::utils::CMakeProxy::getConfigureCommand(WorkPathObj.toGeneric(),
                                                                        WorkPathObj.toGeneric(),CMakeVars);
  ConfigureCmd.OutFile = WorkPathObj.fromThis("configure_log.out").toGeneric();
  ConfigureCmd.ErrFile = WorkPathObj.fromThis("configure_log.err").toGeneric();
  if (openfluid::utils::Process::execute(ConfigureCmd) != 0)
  {
    mp_Listener->onProcessSignatureEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error configuring signature export program");
  }

  // building export program
  mp_Listener->stageMessage("building signature exporter");
  auto BuildCmd = openfluid::utils::CMakeProxy::getBuildCommand(WorkPathObj.toGeneric());
  BuildCmd.OutFile = WorkPathObj.fromThis("build_log.out").toGeneric();
  BuildCmd.ErrFile = WorkPathObj.fromThis("build_log.err").toGeneric();
  if (openfluid::utils::Process::execute(BuildCmd) != 0)
  {
    mp_Listener->onProcessSignatureEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error building signature export program");
  }

  // executing export program
  mp_Listener->stageMessage("executing signature exporter");
  openfluid::utils::Process::Command ExecCmd;
  ExecCmd.Program = WorkPathObj.fromThis("migrate_signature").toGeneric();
  ExecCmd.OutFile = WorkPathObj.fromThis("exec_log.out").toGeneric();
  ExecCmd.ErrFile = WorkPathObj.fromThis("exec_log.err").toGeneric();
  if (openfluid::utils::Process::execute(ExecCmd) != 0)
  {
    mp_Listener->onProcessSignatureEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error executing signature export program");
  }

  // checking that the export program produces the signature file
  auto ExportedSignFileObj = WorkPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE);
  if (!ExportedSignFileObj.isFile())
  {
    mp_Listener->onProcessSignatureEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Exported signature file was not correctly written");
  }
  
  // integration of the wareshub.json content (if file exists) 
  auto WHubFileObj = m_SrcPathObj.fromThis(openfluid::config::WARESDEV_WARESHUB_FILE);
  if (WHubFileObj.isFile())
  {
    mp_Listener->stageMessage("processing wareshub file");

    std::vector<std::string> transmittedMessages;

    if (Info.WareType == openfluid::ware::WareType::SIMULATOR)
    {
      transmittedMessages = mergeWareshubInSignatureFile<openfluid::waresdev::SimulatorSignatureSerializer>(WHubFileObj,
        ExportedSignFileObj);
    }
    else if (Info.WareType == openfluid::ware::WareType::OBSERVER)
    {
      transmittedMessages = mergeWareshubInSignatureFile<openfluid::waresdev::ObserverSignatureSerializer>(WHubFileObj,
        ExportedSignFileObj);
    }
    else if (Info.WareType == openfluid::ware::WareType::BUILDEREXT)
    {
      transmittedMessages = mergeWareshubInSignatureFile<openfluid::waresdev::BuilderextSignatureSerializer>(
        WHubFileObj,ExportedSignFileObj);
    }
    for (const auto& M : transmittedMessages)
    {
      mp_Listener->stageMessage(M);
    }
  }


  mp_Listener->onProcessSignatureEnd(openfluid::base::Listener::Status::OK_STATUS);
}


// =====================================================================
// =====================================================================


void WareSrcMigrator::dispatchExistingFiles(const WareSrcMigrator::WareMigrationInfo& Info) const
{
  mp_Listener->onDispatchFilesStart();

  // ==== copy original sources files in correct location

  const auto CopyOptions = std::filesystem::copy_options::update_existing | 
                           std::filesystem::copy_options::recursive;

  auto DestSrcObj = m_DestPathObj.fromThis(openfluid::config::WARESDEV_SRC_DIR);

  for (const auto& E : std::filesystem::directory_iterator{m_SrcPathObj.stdPath()})
  {
    if (IsRelevantForCopy(openfluid::tools::Path::fromStdPath(E.path())))
    {
      auto Stem = E.path().filename().string();

      if (Stem != openfluid::config::WARESDEV_SRC_CMAKESTDFILE && 
          Stem != "CMake.in.config" && 
          Stem != openfluid::config::WARESDEV_WARESHUB_FILE)
      {
        if (Stem == openfluid::config::WARESDEV_DOC_DIR || Stem == openfluid::config::WARESDEV_TESTS_DIR)
        {
          mp_Listener->stageMessage("standard " + Stem + " " + std::string(E.is_directory() ? "directory" : "file"));
          try
          {
            openfluid::tools::Filesystem::copyDirectoryContent(E.path(), m_DestPathObj.stdPath()/Stem);
          }
          catch (std::exception & E)
          {
            //TODO use listener more cleverly with internal logs
            openfluid::utils::log::error("Migration", 
                                       "Dispatch failure: "+std::string(E.what()));
            mp_Listener->onDispatchFilesEnd(openfluid::base::Listener::Status::ERROR_STATUS);
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                      "Dispatch failure: "+std::string(E.what()));
          }
          catch (...)
          {
            openfluid::utils::log::error("Migration", 
                                       "Dispatch failure");
            mp_Listener->onDispatchFilesEnd(openfluid::base::Listener::Status::ERROR_STATUS);
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Dispatch failure");
          }
        }
        else
        {
          mp_Listener->stageMessage(Stem + " " + std::string(E.is_directory() ? "directory" : "file"));
          std::filesystem::copy(E.path(),DestSrcObj.stdPath()/Stem, CopyOptions);
        }
      }
    }
  }

  mp_Listener->onDispatchFilesEnd(openfluid::base::Listener::Status::OK_STATUS);


  mp_Listener->onCleanSrcFilesStart();

  // ==== remove old signature macros in CPP files

  // signature of the ware
  if (!Info.SignatureContent.empty())
  {
    mp_Listener->stageMessage("removing deprecated ware signature");

    auto SignFileObj = m_DestPathObj.fromThis({openfluid::config::WARESDEV_SRC_DIR,
                                               Info.SignatureFile.relativeTo(m_SrcPathObj.toGeneric())});
    std::string ReplStr = generateMigrationBlock("//",
                                                "// Here was located the former signature of the ware, "
                                                "this message should be manually cleaned");

    auto SignFileContent = openfluid::tools::Filesystem::readFile(SignFileObj);
    SignFileContent = openfluid::tools::replace(SignFileContent,Info.SignatureContent,ReplStr);
    openfluid::tools::Filesystem::writeFile(SignFileContent,SignFileObj);
    
  }

  // signature of the parameterization UI
  if (!Info.UISignatureContent.empty())
  {
    mp_Listener->stageMessage("removing deprecated parameterization UI signature");
    auto UISignFileObj = m_DestPathObj.fromThis({openfluid::config::WARESDEV_SRC_DIR,
                                                 Info.UISignatureFile.relativeTo(m_SrcPathObj.toGeneric())});

    std::string ReplStr = generateMigrationBlock("//",
                                                "// Here was located the former signature of the parameterization UI, "
                                                "this message should be manually cleaned");

    auto UISignFileContent = openfluid::tools::Filesystem::readFile(UISignFileObj);
    UISignFileContent = openfluid::tools::replace(UISignFileContent,Info.UISignatureContent,ReplStr);
    openfluid::tools::Filesystem::writeFile(UISignFileContent,UISignFileObj);
  }

  // ==== remove old signature macros in CPP files

  mp_Listener->stageMessage("removing deprecated link UID declarations");

  auto LinkUIDReplStr = generateMigrationBlock("//", "// Here was located the former link UID declaration, "
                                                     "this message should be manually cleaned");
  auto LinkUIDRegex = std::regex("DEFINE_WARE_LINKUID\\([\\s\\w_]*\\)",
                                 std::regex_constants::ECMAScript|std::regex_constants::icase);

  for (const auto& E : std::filesystem::recursive_directory_iterator{DestSrcObj.stdPath()})
  {
    auto CxxFileObj = openfluid::tools::Path::fromStdPath(E.path());
    if (IsCppFile(CxxFileObj))
    {
      auto CxxFileContent = openfluid::tools::Filesystem::readFile(CxxFileObj);

      if (std::regex_search(CxxFileContent,LinkUIDRegex))
      {
        CxxFileContent = std::regex_replace(CxxFileContent,LinkUIDRegex,LinkUIDReplStr);
        openfluid::tools::Filesystem::writeFile(CxxFileContent,CxxFileObj);
      }
    }
  }

  // ==== add migration tag around sim2doc

  // documentation of the ware
  if (!Info.DocContent.empty())
  {
    mp_Listener->stageMessage("deprecating ware sim2doc");

    auto DocFileObj = m_DestPathObj.fromThis({openfluid::config::WARESDEV_SRC_DIR,
                                              Info.DocFile.relativeTo(m_SrcPathObj.toGeneric())});
    auto DocFileContent = openfluid::tools::Filesystem::readFile(DocFileObj);
    std::string DocContentWithTags = openfluid::tools::trim(extractBetweenTags(DocFileContent, 
                                                                               openfluid::config::SIM2DOC_BEGIN_TAG, 
                                                                               openfluid::config::SIM2DOC_END_TAG, 
                                                                               true));

    std::string ReplStr = generateMigrationBlock("//",
                                                 "// Here is located the former sim2doc of the ware. "
                                                 "Ensure the doc content is correctly migrated in doc folder. \n" +
                                                 DocContentWithTags);

    DocFileContent = openfluid::tools::replace(DocFileContent, DocContentWithTags, ReplStr);
    openfluid::tools::Filesystem::writeFile(DocFileContent, DocFileObj);
  }

  // ==== fix builderext includes and namespaces

  mp_Listener->stageMessage("adapting specific includes and namespaces");

  std::string IncludeRegStr = 
    "^\\s*\\#include\\s*<(openfluid\\/builderext)\\/Pluggable(Modal|Modeless|Workspace|Parameterization|Feature)";
  std::regex IncludeRegex(IncludeRegStr,std::regex_constants::ECMAScript);

  std::string NamespRegStr = 
    "openfluid::builderext::(Pluggable(Modal|Modeless|Workspace|Parameterization|Feature)|FluidXUpdateFlags)";
  std::regex NamespRegex(NamespRegStr,std::regex_constants::ECMAScript);

  for (const auto& E : std::filesystem::recursive_directory_iterator{DestSrcObj.stdPath()})
  {
    auto CxxFileObj = openfluid::tools::Path::fromStdPath(E.path());
    if (IsCppFile(CxxFileObj))
    {
      auto CxxFileLines = getFileLines(CxxFileObj);
      auto Modified = false;

      for (auto& L : CxxFileLines)
      {
        // fix includes
        if (std::regex_search(L,IncludeRegex))
        {
          L = openfluid::tools::replace(L,"openfluid/builderext","openfluid/ui/builderext");
          Modified = true;
        }

        // fix namespaces
        if (std::regex_search(L,NamespRegex))
        {
          L = openfluid::tools::replace(L,"openfluid::builderext","openfluid::ui::builderext");
          Modified = true;
        }
      }

      if (Modified)
      {
        openfluid::tools::Filesystem::writeFile(openfluid::tools::join(CxxFileLines,"\n"),CxxFileObj);
      }
    }
  }


  mp_Listener->onCleanSrcFilesEnd(openfluid::base::Listener::Status::OK_STATUS);
}


// =====================================================================
// =====================================================================


std::pair<openfluid::tools::TemplateProcessor::Data,std::string> 
WareSrcMigrator::processCMakeFiles(const WareSrcMigrator::WareMigrationInfo& Info) const
{
  openfluid::tools::TemplateProcessor::Data TplData;
  std::string CMakePrepend;

  mp_Listener->onProcessCMakeConfigStart();

  if (Info.CMakeWareSuffix.empty() || Info.CMakeDefaultWareVar.empty())
  {
    mp_Listener->onProcessCMakeConfigEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Unable to detect required vars in CMake files");
  }

  // ==== process CMakeLists.txt

  mp_Listener->stageMessage("processing " + openfluid::config::WARESDEV_SRC_CMAKESTDFILE);

  auto CMakeWareVar = Info.CMakeDefaultWareVar;

  auto CMakeFileObj = m_SrcPathObj.fromThis(openfluid::config::WARESDEV_SRC_CMAKESTDFILE);
  if (CMakeFileObj.isFile())
  {
    // search for correct variable name
    auto CMakeContent = openfluid::tools::Filesystem::readFile(CMakeFileObj);
    if (!CMakeContent.empty())
    {
      // search for default
      const std::string DefaultRegStr = "OPENFLUID_ADD_DEFAULT_"+Info.CMakeWareSuffix+"\\([\\s\\n]*\\)";
      if (!std::regex_search(CMakeContent,std::regex(DefaultRegStr,std::regex_constants::icase)))
      {
        // search for custom if default is not found
        std::smatch Matches;
        const std::string VarRegStr = "OPENFLUID_ADD_"+Info.CMakeWareSuffix+"\\([\\s\\n]*(\\w*)[\\s\\n]*\\)";
        const std::regex VarRegex(VarRegStr,std::regex_constants::icase);

        if (std::regex_search(CMakeContent,Matches,VarRegex) && Matches.size() == 2)
        {
          CMakeWareVar = Matches[1].str();
        }
      }
    }
  }
  else
  {
    mp_Listener->onProcessCMakeConfigEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot find " +
                                                                      openfluid::config::WARESDEV_SRC_CMAKESTDFILE + 
                                                                      " file");
  }


  // ==== process CMake.in.config

  mp_Listener->stageMessage("processing CMake.in.config");

  auto ConfigFileObj = m_SrcPathObj.fromThis("CMake.in.config");

  if (ConfigFileObj.isFile())
  {
    auto ConfigLines = getFileLines(ConfigFileObj);

    if (!ConfigLines.empty())
    {
      for (std::vector<std::string>::size_type i=0; i<ConfigLines.size();i++)
      {
        ConfigLines[i] = openfluid::tools::trim(ConfigLines[i]);
      }

      // remove empty or commented lines
      ConfigLines.erase(std::remove_if(ConfigLines.begin(),ConfigLines.end(),
                                       [](const std::string& L) 
                                       {
                                         return (L.empty() || L[0] == '#');
                                       }), 
                        ConfigLines.end());

      const auto ConfigContent = openfluid::tools::join(ConfigLines," ");
      std::map<std::string,std::string> ConfigVariables;

      std::smatch Matches;
      std::string ValuesRegStr = "SET\\s*\\(("+CMakeWareVar+"|WARE)_(\\w*)\\s*([\\s\\w\\.\"\\=\\-\\$\\/\\{\\}]*)\\)";
      std::regex ValuesRegex(ValuesRegStr,std::regex_constants::ECMAScript|std::regex_constants::icase);

      auto ConfigStart = ConfigContent.begin();
      while (std::regex_search(ConfigStart,ConfigContent.end(),Matches,ValuesRegex))
      {
        ConfigStart = Matches[0].second;
        ConfigVariables[Matches.str(2)] = Matches.str(3);
      }

      // matching table between CMake variable prefixed with SIM_|OBS_|BEXT_ (keys) and template patterns (values)
      const std::map<std::string,std::string> VarsPatternsTable = 
      {
        {"INCLUDE_DIRS","INCDIRS"},
        {"LIBRARY_DIRS","LIBDIRS"},
        {"LINK_LIBS","LINKLIBS"},
        {"DEFINITIONS","WAREDEFS"},
        {"CPP","CPPFILES"},
        {"FORTRAN","FORTRANFILES"},
        {"OPENFLUID_COMPONENTS","OFCOMPONENTS"},
        {"PARAMSUI_ENABLED","PUIENABLED"},
        {"PARAMSUI_CPP","PUICPPFILES"},
        {"PARAMSUI_UI","PUIUIFILES"},
        {"PARAMSUI_RC","PUIRCFILES"},
        {"INSTALL_PATH","INSTALLPATH"},
        {"TRANSLATIONS_LANGS","I18NLANGS"},
        {"TRANSLATIONS_EXTRASCANS","I18NEXTRASCANS"}
      };

      // specific variables 
      for (auto& CV : ConfigVariables)
      {      
        if (CV.first == "INCLUDE_DIRS" || 
            CV.first == "LIBRARY_DIRS" ||
            CV.first == "LINK_LIBS" ||
            CV.first == "DEFINITIONS")
        {
          CV.second += " ${WARE_"+CV.first+"}";
        }

        auto it = VarsPatternsTable.find(CV.first);
        if (it != VarsPatternsTable.end())
        {
          TplData[it->second] = CV.second;
        } 
      }

      CMakePrepend = generateMigrationBlock("#","# Lines below should be manually adapted and cleaned\n\n" + 
                                                openfluid::tools::join(ConfigLines,"\n")) + "\n";
    }
    else
    {
      mp_Listener->onProcessCMakeConfigEnd(openfluid::base::Listener::Status::ERROR_STATUS);
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "CMake.in.config file is fully commented or empty");
    }
  }
  else
  {
    mp_Listener->onProcessCMakeConfigEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot find CMake.in.config file");
  }

  mp_Listener->onProcessCMakeConfigEnd(openfluid::base::Listener::Status::OK_STATUS);

  return {TplData,CMakePrepend};
}


// =====================================================================
// =====================================================================


void WareSrcMigrator::processSources(const WareSrcMigrator::WareMigrationInfo& Info) const
{
  auto SignWorkPathObj = m_WorkPathObj.fromThis("signature"); 


  // ==== process signature

  processSignature(Info,SignWorkPathObj);

  
  // ==== process CMake config

  auto [TplData,CMakePrepend] = processCMakeFiles(Info);


  // ====  create new structure using signature and CMake info
  
  mp_Listener->onCreateStructureStart();

  openfluid::waresdev::WareSrcFactory::Configuration Config;
  Config.WithParamsUI = !Info.UISignatureContent.empty();
  Config.ExtraTemplateData = TplData;

  mp_Listener->stageMessage("creating new source tree");
  
  auto ExportedSignFileObj = SignWorkPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE);
  if (!ExportedSignFileObj.isFile())
  {
    mp_Listener->onCreateStructureEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot find exported signature file");
  }

  if (Info.WareType == openfluid::ware::WareType::SIMULATOR)
  {
    auto Signature = openfluid::waresdev::SimulatorSignatureSerializer()
                     .readFromJSONFile(ExportedSignFileObj.toGeneric());
    openfluid::waresdev::WareSrcFactory::createSimulator(Signature,Config,m_DestPathObj.toGeneric(),false);
  }
  else if (Info.WareType == openfluid::ware::WareType::OBSERVER)
  {
    auto Signature = openfluid::waresdev::ObserverSignatureSerializer()
                     .readFromJSONFile(ExportedSignFileObj.toGeneric());
    openfluid::waresdev::WareSrcFactory::createObserver(Signature,Config,m_DestPathObj.toGeneric(),false);
  }
  else if (Info.WareType == openfluid::ware::WareType::BUILDEREXT)
  {
    auto Signature = openfluid::waresdev::BuilderextSignatureSerializer()
                     .readFromJSONFile(ExportedSignFileObj.toGeneric());
    openfluid::waresdev::WareSrcFactory::createBuilderext(Signature,Config,m_DestPathObj.toGeneric(),false);
  }
  else
  {
    mp_Listener->onCreateStructureEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Unknown ware type");
  }


  // ==== update CMakeLists.txt file in src directory

  mp_Listener->stageMessage("applying CMake configuration");
  
  auto CMakeFileObj = m_DestPathObj.fromThis({openfluid::config::WARESDEV_SRC_DIR,
                                              openfluid::config::WARESDEV_SRC_CMAKESTDFILE});
  auto NewCMakeContent = openfluid::tools::Filesystem::readFile(CMakeFileObj);
  openfluid::tools::Filesystem::writeFile(CMakePrepend+NewCMakeContent,CMakeFileObj);


  // ==== clean any C++ files in src directory

  mp_Listener->stageMessage("preparing src directory");

  auto SrcFilesToRemove = 
    openfluid::tools::Filesystem::findFiles(m_DestPathObj.fromThis(openfluid::config::WARESDEV_SRC_DIR).toGeneric(),
                                            true);

  for (const auto& FName : SrcFilesToRemove )
  {
    auto FObj = openfluid::tools::Path(FName);
    if (openfluid::waresdev::IsCppFile(FObj))
    {
      FObj.removeFile();
    }
  }

  mp_Listener->onCreateStructureEnd(openfluid::base::Listener::Status::OK_STATUS);


  // ==== dispatch existing files

  dispatchExistingFiles(Info);
}


// =====================================================================
// =====================================================================


void WareSrcMigrator::processDoc(const WareSrcMigrator::WareMigrationInfo& Info) const
{
  mp_Listener->onDocStart();

  auto DocWorkPathObj = m_DestPathObj.fromThis(openfluid::config::WARESDEV_DOC_DIR);

  if (!Info.DocContent.empty())
  {
    mp_Listener->stageMessage("integrated documentation found");

    // prevent overwriting of already existing main.tex file (if any)
    if (DocWorkPathObj.isFile("main.tex"))
    {
      mp_Listener->stageMessage("renaming original doc/main.tex into doc/original_main.tex");
      openfluid::tools::Filesystem::renameFile(DocWorkPathObj.fromThis("main.tex").toGeneric(),
                                               DocWorkPathObj.fromThis("original_main.tex").toGeneric());
    }

    openfluid::tools::Filesystem::writeFile(Info.DocContent+"\n",DocWorkPathObj.fromThis("main.tex"));
    mp_Listener->stageMessage("documentation exported in doc/main.tex");
  }
  else
  {
    mp_Listener->stageMessage("no documentation found");
  }

  mp_Listener->onDocEnd(openfluid::base::Listener::Status::OK_STATUS);
}


// =====================================================================
// =====================================================================


void WareSrcMigrator::performMigration()
{
  // ==== preliminary checks

  mp_Listener->onPrecheckStart();

  if (!openfluid::utils::CMakeProxy::isAvailable())
  {
    mp_Listener->onPrecheckEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "CMake program not available");
  }

  mp_Listener->stageMessage("CMake program found");

  if (!m_SrcPathObj.isDirectory())
  {
    mp_Listener->onPrecheckEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Path " + m_SrcPathObj.toGeneric() + 
                                              " is not a directory or does not exist");
  }

  mp_Listener->stageMessage("ware sources directory found");

  if (openfluid::waresdev::WareSrcChecker::tryDetectWareSrcVersion(m_SrcPathObj) >= 202000)
  {
    mp_Listener->onPrecheckEnd(openfluid::base::Listener::Status::ERROR_STATUS);
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Wares sources cannot be migrated to OpenFLUID 2.2.+ specification, "
                                              "detected as already compliant with 2.2.+");
  }

  mp_Listener->stageMessage("ware sources can be migrated");

  mp_Listener->onPrecheckEnd(openfluid::base::Listener::Status::OK_STATUS);


  // ==== go for migration process

  auto Info = prepareMigration();
  processSources(Info);
  processDoc(Info);

}


} }  // namespaces

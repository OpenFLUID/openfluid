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
  @file WareSrcChecker.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <iterator>
#include <algorithm>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/waresdev/ObserverSignatureSerializer.hpp>
#include <openfluid/waresdev/BuilderextSignatureSerializer.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>
#include <openfluid/waresdev/WareSrcChecker.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace waresdev {


WareSrcChecker::ReportingData::ReportingList InitializeReportingItemList(const std::vector<std::string>& Msgs)
{
  WareSrcChecker::ReportingData::ReportingList List;
  for (const auto& M : Msgs)
  {
    List.Items.push_back({M,WareSrcChecker::ReportingData::ReportingStatus::UNKNOWN, ""});
  }
  return List;
}


// =====================================================================
// =====================================================================


void WareSrcChecker::processReportingItem(WareSrcChecker::ReportingData::ReportingList& List,
                                          const std::string& Msg, std::function<bool()> Predicate,
                                          WareSrcChecker::ReportingData::ReportingStatus StatusIfFail,
                                          const std::string& SpecificInformation) const
{
  auto It = std::find_if(List.Items.begin(),List.Items.end(),[&Msg](const auto& II){ return II.Message == Msg; });

  // mark as disabled if should be ignored 
  if (std::find(m_IgnoredChecks.begin(),m_IgnoredChecks.end(),Msg) != m_IgnoredChecks.end())
  {
    (*It).Status = WareSrcChecker::ReportingData::ReportingStatus::DISABLED;
  }
  else if (It != List.Items.end())
  {
    (*It).Status = StatusIfFail;

    if (Predicate())
    {
      (*It).Status = WareSrcChecker::ReportingData::ReportingStatus::OK;
    }
    (*It).SpecificInformation = SpecificInformation;
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"internal checker error: check not found: "+Msg);
  }
}


// =====================================================================
// =====================================================================


WareSrcChecker::WareSrcChecker(const std::string& SrcPath, const ChecksList& IgnoredChecks) : 
  m_SrcPathObj(SrcPath), m_IgnoredChecks(IgnoredChecks)
{

}


// =====================================================================
// =====================================================================


void WareSrcChecker::updateWithPedanticCheck(ReportingData& RepData)
{
  auto MetaFileObj = m_SrcPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE);
  if (!MetaFileObj.exists()) // all pedantic checks are currently based on metadata file
  {
    return;
  }

  std::ifstream InFile(MetaFileObj.toGeneric(),std::ifstream::in);
  auto Json = openfluid::thirdparty::json::parse(InFile);

  // [w] non-empty description of ware
  processReportingItem(RepData.Categories["metainfo"],"ware_description_exists", 
                       [&](){return Json.value("description", "") != "";},
                       ReportingData::ReportingStatus::WARNING);

  bool IsDataDescr = true;
  bool IsDataUnit = true;
  std::string DescrInformation = "";
  std::string UnitInformation = "";
  std::map<std::string, std::vector<std::string>> CategoryByWaretype = 
    {
      {"simulator", {"parameters", "attributes", "variables"}}, 
      {"observer", {"parameters"}}
    };

  std::string FoundWaretype = "";

  for (const auto& Pair : CategoryByWaretype)
  {
    if (Json.value(Pair.first, openfluid::thirdparty::json::array()) != openfluid::thirdparty::json::array())
    {
      openfluid::thirdparty::json DataJson = Json.at(Pair.first).at("data");
      if (DataJson != openfluid::thirdparty::json::array())
      {
        for (const std::string& Cat : Pair.second)
        {
          std::string CatSingular = Cat;
          CatSingular.pop_back();
          for (const auto& SubCatJson : DataJson.at(Cat))  // used, required, updated...
          {
            for (const auto& ItemJson : SubCatJson)
            {
              if (ItemJson.value("description", "") == "")
              {
                if (DescrInformation != "")
                {
                  DescrInformation += ", ";
                }
                DescrInformation += CatSingular + " '"+ ItemJson.value("name", "?") + "'";
                IsDataDescr = false;
              }
              if (ItemJson.value("siunit", "") == "")
              {
                if (UnitInformation != "")
                {
                  UnitInformation += ", ";
                }
                UnitInformation += CatSingular + " '" + ItemJson.value("name", "?") + "'";
                IsDataUnit = false;
              }
            }
          }
        }
      }
    }
  }
  // [w] non-empty description of parameters/attributes/variables
  processReportingItem(RepData.Categories["metainfo"],"data_description_exists", [&](){return IsDataDescr;},
                                    ReportingData::ReportingStatus::WARNING, DescrInformation);

  // [w] non-empty SIUnit of parameters/attributes/variables
  processReportingItem(RepData.Categories["metainfo"],"data_unit_exists", [&](){return IsDataUnit;},
                                    ReportingData::ReportingStatus::WARNING, UnitInformation);
}


// =====================================================================
// =====================================================================


WareSrcChecker::ReportingData::ReportingList WareSrcChecker::performStructureCheck(bool OKToRun) const
{
  auto Data = InitializeReportingItemList({"cmakelists_exists","readme_exists",
                                           "srcdir_exists","docdir_exists","testsdir_exists"});

  if (OKToRun)
  { 
    // [e] CMakeLists.txt exists at root
    processReportingItem(Data,"cmakelists_exists",
                         [&](){return m_SrcPathObj.isFile(openfluid::config::WARESDEV_SRC_CMAKESTDFILE);},
                         ReportingData::ReportingStatus::ERROR_STATUS);

    // [w] README or README.md file exists
    auto IsAnyReadme = [&](){
      for (const auto& F : openfluid::config::WARESDEV_README_FILES)
      {
        if (m_SrcPathObj.isFile(F))
        {
          return true;
        }
      }
      return false;
    };
    processReportingItem(Data,"readme_exists",IsAnyReadme);

    // [w] src dir exists
    processReportingItem(Data,"srcdir_exists",
                         [&](){return m_SrcPathObj.isDirectory(openfluid::config::WARESDEV_SRC_DIR);});

    // [w] doc dir exists
    processReportingItem(Data,"docdir_exists",
                         [&](){return m_SrcPathObj.isDirectory(openfluid::config::WARESDEV_DOC_DIR);});

    // [w] tests dir exists
    processReportingItem(Data,"testsdir_exists",
                         [&](){return m_SrcPathObj.isDirectory(openfluid::config::WARESDEV_TESTS_DIR);});
  }

  return Data;
}


// =====================================================================
// =====================================================================


WareSrcChecker::ReportingData::ReportingList WareSrcChecker::performMetainfoCheck(bool OKToRun) const
{
  auto Data = InitializeReportingItemList({"file_iscorrect","id_iscorrect", "waretype_correct",
                                           "name_exists","authors_exist","contacts_exist","licence_exists",
                                           "rootdir_matchesid", 
                                           "ware_description_exists", "data_description_exists", "data_unit_exists"});

  if (OKToRun)
  {
    auto MetaFileObj = m_SrcPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE);

    // [e] metadata file is readable and well formed
    const auto Type = openfluid::waresdev::detectWareType(MetaFileObj.toGeneric());
    bool MetaIsReadable = (Type != openfluid::ware::WareType::UNDEFINED);
    processReportingItem(Data, "file_iscorrect", [&](){return MetaIsReadable;},
                         ReportingData::ReportingStatus::ERROR_STATUS);

    if (MetaIsReadable)
    {
      // [e] metadata file contains correct ware type
      processReportingItem(Data,"waretype_correct", [&](){return Type != openfluid::ware::WareType::OTHER;},
                                    ReportingData::ReportingStatus::ERROR_STATUS);

      std::ifstream InFile(MetaFileObj.toGeneric(),std::ifstream::in);
      auto Json = openfluid::thirdparty::json::parse(InFile);
      
      // [e] metadata file contains ID
      const auto IdFromMetadata = Json.value("id", "");
      processReportingItem(Data,"id_iscorrect", [&](){return IdFromMetadata != "";},
                                    ReportingData::ReportingStatus::ERROR_STATUS);

      // [w] name of containing directory matches ID
      const auto DirectoryName = m_SrcPathObj.filename();
      processReportingItem(Data,"rootdir_matchesid", [&](){return IdFromMetadata==DirectoryName;},
                                    ReportingData::ReportingStatus::WARNING);

      // [w] metadata file contains non-empty name
      processReportingItem(Data,"name_exists", [&](){return Json.value("name", "") != "";},
                                    ReportingData::ReportingStatus::WARNING);

      // [w] metadata file contains at least one author
      processReportingItem(Data,"authors_exist", [&](){return Json.value("authors", 
          openfluid::thirdparty::json::array()) != openfluid::thirdparty::json::array();}, 
        ReportingData::ReportingStatus::WARNING);

      // [w] metadata file contains at least one contact
      processReportingItem(Data,"contacts_exist", [&](){return Json.value("contacts", 
          openfluid::thirdparty::json::array()) != openfluid::thirdparty::json::array();},
        ReportingData::ReportingStatus::WARNING);
      
      // [w] metadata file contains non-empty license
      processReportingItem(Data,"licence_exists", [&](){return  Json.value("license", "") != "";},
                                    ReportingData::ReportingStatus::WARNING);
      }
    }

  return Data;
}


// =====================================================================
// =====================================================================


WareSrcChecker::ReportingData::ReportingList WareSrcChecker::performCodeCheck(bool OKToRun) const
{
  auto Data = InitializeReportingItemList({"migration_isclean_no_comments", "migration_isclean_no_sim2doc"});

  if (OKToRun)
  {
    bool HasMigrationTagInFile = false;
    bool HasSim2DocTagInFile = false;

    for (auto const& E : std::filesystem::recursive_directory_iterator{m_SrcPathObj.fromThis("src").stdPath()})
    {
      auto FileObj = openfluid::tools::Path::fromStdPath(E.path());
      if (openfluid::waresdev::IsCppFile(FileObj) || openfluid::waresdev::IsCMakeFile(FileObj))
      {
        auto Content = openfluid::tools::Filesystem::readFile(FileObj);

        // [w] MIGRATION tag in C++ or CMake files
        if (openfluid::tools::contains(Content, openfluid::config::MIGRATION_STRING))
        {
          HasMigrationTagInFile = true;
        }

        // [w] sim2doc tag in C++ files
        if (openfluid::tools::contains(Content, openfluid::config::SIM2DOC_BEGIN_TAG) ||
            openfluid::tools::contains(Content, openfluid::config::SIM2DOC_END_TAG))
        {
          HasSim2DocTagInFile = true;
        }
      }
    }
    
    processReportingItem(Data, "migration_isclean_no_comments", [&](){return !HasMigrationTagInFile;});
    processReportingItem(Data, "migration_isclean_no_sim2doc", [&](){return !HasSim2DocTagInFile;});
  }

  return Data;
}


// =====================================================================
// =====================================================================


WareSrcChecker::ReportingData WareSrcChecker::performCheck(bool Pedantic)
{
  ReportingData Data;
  auto BaseData = InitializeReportingItemList({"rootdir_exists","version_iscorrect", "no_migration_files"});

  
  // [e] ware src directory exists
  bool BaseIsOK = m_SrcPathObj.isDirectory();
  processReportingItem(BaseData,"rootdir_exists",[&](){return BaseIsOK;},ReportingData::ReportingStatus::ERROR_STATUS);

  if (BaseIsOK)
  {
    // [e] ware src is detected as 220000
    BaseIsOK = (tryDetectWareSrcVersion(m_SrcPathObj) >= 202000);
    bool HasMigrationFiles = hasMigrationFiles();

    if(BaseIsOK)
    {
      processReportingItem(BaseData, "no_migration_files", [&](){return !HasMigrationFiles;},
                                    ReportingData::ReportingStatus::WARNING);
    } 
    else 
    {
      processReportingItem(BaseData, "no_migration_files", [&](){return !HasMigrationFiles;},
                                    ReportingData::ReportingStatus::ERROR_STATUS);

      processReportingItem(BaseData, "version_iscorrect", [&](){return HasMigrationFiles;},
                         ReportingData::ReportingStatus::ERROR_STATUS);
    }
  }


  Data.Categories["base"] = BaseData;
  Data.Categories["structure"] = performStructureCheck(BaseIsOK);
  Data.Categories["metainfo"] = performMetainfoCheck(BaseIsOK);
  Data.Categories["code"] = performCodeCheck(BaseIsOK);


  if (Pedantic)
  {
    updateWithPedanticCheck(Data);
  }

  return Data;
}


// =====================================================================
// =====================================================================


unsigned int WareSrcChecker::tryDetectWareSrcVersion(const openfluid::tools::FilesystemPath& PathObj)
{
  // try detect >= 2.2.0
  if (PathObj.isFile(openfluid::config::WARESDEV_WAREMETA_FILE))
  {
    return 202000;
  } 
  else // try detect < 2.2.0
  {
    return 201000;
  }
}


// =====================================================================
// =====================================================================


bool WareSrcChecker::hasMigrationFiles() const
{
  openfluid::tools::FilesystemPath MigrationFolderPath = openfluid::tools::FilesystemPath(
    openfluid::tools::Filesystem::joinPath({m_SrcPathObj.toGeneric(), 
                                            openfluid::config::WARESDEV_MIGRATION_WORK_DIR}));

  openfluid::tools::FilesystemPath OriginalFolderPath = openfluid::tools::FilesystemPath(
    openfluid::tools::Filesystem::joinPath({m_SrcPathObj.toGeneric(), 
                                            openfluid::config::WARESDEV_MIGRATION_ORIGINAL_DIR}));

  return MigrationFolderPath.exists() && OriginalFolderPath.exists();
}


} }  // namespaces

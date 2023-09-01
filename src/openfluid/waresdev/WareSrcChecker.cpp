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
    List.Items.push_back({M,WareSrcChecker::ReportingData::ReportingStatus::UNKNOWN});
  }
  return List;
}


// =====================================================================
// =====================================================================


void WareSrcChecker::processReportingItem(WareSrcChecker::ReportingData::ReportingList& List,
                                          const std::string& Msg, std::function<bool()> Predicate,
                                          WareSrcChecker::ReportingData::ReportingStatus StatusIfFail) const
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
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"internal checker error");
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


void WareSrcChecker::updateWithPedanticCheck(ReportingData& /*RepData*/) const
{
  // [w] non-empty description of ware
  // TOIMPL to do

  // [w] non-empty description of parameters/attributes/variables
  // TOIMPL to do

  // [w] non-empty SIUnit of parameters/attributes/variables
  // TOIMPL to do
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
  auto Data = InitializeReportingItemList({"file_iscorrect","id_iscorrect",
                                           "name_exists","authors_exist","contacts_exist","licence_exists",
                                           "rootdir_matchesid"});

  if (OKToRun)
  {
    auto MetaFileObj = m_SrcPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE);

    // [e] metadata file is readable and well formed
    const auto Type = openfluid::waresdev::detectWareType(MetaFileObj.toGeneric());
    bool MetaIsReadable = (Type != openfluid::ware::WareType::UNDEFINED);
    processReportingItem(Data,"file_iscorrect",[&](){return MetaIsReadable;},ReportingData::ReportingStatus::ERROR_STATUS);

    if (MetaIsReadable)
    {
      // [e] metadata file contains correct ware type
      // TOIMPL to do

      // [e] metadata file contains ID
      // TOIMPL to do

      // [w] name of containing directory matches ID
      // TOIMPL to do

      // [w] metadata file contains non-empty name
      // TOIMPL to do

      // [w] metadata file contains at least one author
      // TOIMPL to do

      // [w] metadata file contains at least one contact
      // TOIMPL to do
      
      // [w] metadata file contains non-empty license
      // TOIMPL to do
      }
    }

  return Data;
}


// =====================================================================
// =====================================================================


WareSrcChecker::ReportingData::ReportingList WareSrcChecker::performCodeCheck(bool OKToRun) const
{
  auto Data = InitializeReportingItemList({"migration_isclean"});


  if (OKToRun)
  {
    // [w] MIGRATION tag in C++ or CMake files
    auto FindMigrationTagInFile = [&]()
    {
      for (auto const& E : std::filesystem::recursive_directory_iterator{m_SrcPathObj.fromThis("src").stdPath()})
      {
        auto FileObj = openfluid::tools::Path::fromStdPath(E.path());
        if (openfluid::waresdev::IsCppFile(FileObj) || openfluid::waresdev::IsCMakeFile(FileObj))
        {
          auto Content = openfluid::tools::Filesystem::readFile(FileObj);
          if (openfluid::tools::contains(Content,"[MIGRATION]"))
          {
            return false;
          }
        }
      }
      return true;
    };
    processReportingItem(Data,"migration_isclean",FindMigrationTagInFile);

    // [w] sim2doc tag in C++ files
    // TOIMPL to do
  }

  return Data;
}


// =====================================================================
// =====================================================================


WareSrcChecker::ReportingData WareSrcChecker::performCheck(bool Pedantic) const
{
  ReportingData Data;
  auto BaseData = InitializeReportingItemList({"rootdir_exists","version_iscorrect"});

  
  // [e] ware src directory exists
  bool BaseIsOK = m_SrcPathObj.isDirectory();
  processReportingItem(BaseData,"rootdir_exists",[&](){return BaseIsOK;},ReportingData::ReportingStatus::ERROR_STATUS);

  if (BaseIsOK)
  {
    // [e] ware src is detected as 220000
    BaseIsOK = (tryDetectWareSrcVersion(m_SrcPathObj) >= 202000);
    processReportingItem(BaseData,"version_iscorrect",[&](){return BaseIsOK;},ReportingData::ReportingStatus::ERROR_STATUS);
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

  // try detect < 2.2.0
  if (!PathObj.isFile(openfluid::config::WARESDEV_WAREMETA_FILE))
  {
    return 201000;
  }

  return 0;
}


} }  // namespaces

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
  @file MarketPackage.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#include <fstream>

#include <openfluid/config.hpp>
#include <openfluid/market/MarketPackage.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/FileDownloader.hpp>


namespace openfluid { namespace market {

const std::string MarketPackage::BUILDS_SUBDIR = "builds";
const std::string MarketPackage::DLOADS_SUBDIR = "downloads";
const std::string MarketPackage::LOG_FILENAME = "packages_install.log";

std::string MarketPackage::m_TempDir = "";
std::string MarketPackage::m_TempBuildsDir = "";
std::string MarketPackage::m_TempDownloadsDir = "";
std::string MarketPackage::m_MarketBagSimulatorDir = "";
std::string MarketPackage::m_MarketBagObserverDir = "";
std::string MarketPackage::m_MarketBagBuilderextDir = "";
std::string MarketPackage::m_MarketBagDatasetDir = "";
std::string MarketPackage::m_MarketBagBinSubDir = "";
std::string MarketPackage::m_MarketBagSrcSubDir = "";
std::string MarketPackage::m_LogFile = "";
bool MarketPackage::m_IsLogEnabled = false;

openfluid::utils::ExternalProgram MarketPackage::m_CMakeProgram =
    openfluid::utils::ExternalProgram::getRegisteredProgram(openfluid::utils::ExternalProgram::CMakeProgram);
std::string MarketPackage::m_SimulatorBuildConfigOptions = openfluid::config::MARKET_COMMONBUILDOPTS;
std::string MarketPackage::m_ObserverBuildConfigOptions = openfluid::config::MARKET_COMMONBUILDOPTS;
std::string MarketPackage::m_BuilderextBuildConfigOptions = openfluid::config::MARKET_COMMONBUILDOPTS;

bool MarketPackage::m_Initialized = false;


MarketPackage::MarketPackage(const openfluid::ware::WareID_t& ID, const std::string& PackageURL) :
  m_ID(ID), m_PackageURL(PackageURL),m_PackageFilename(openfluid::tools::Filesystem::filename(PackageURL)),
  m_Downloaded(false)
{

}


// =====================================================================
// =====================================================================


MarketPackage::~MarketPackage()
{

}


// =====================================================================
// =====================================================================


void MarketPackage::initialize(bool EnableLog = false)
{
  if (!m_CMakeProgram.isFound())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Required CMake program not found");

  // TODO
  //m_CommonBuildConfigOptions = openfluid::config::MARKET_COMMONBUILDOPTS;

  m_IsLogEnabled = EnableLog;

  resetLogFile();

  m_Initialized = true;
}


// =====================================================================
// =====================================================================


void MarketPackage::setWorksDirs(const std::string& TempDir, const std::string& MarketBagSimulatorDir,
                                 const std::string& MarketBagObserverDir, const std::string& MarketBagBuilderextDir,
                                 const std::string& MarketBagDatasetDir, const std::string& MarketBagBinSubDir,
                                 const std::string& MarketBagSrcSubDir)
{
  // Temporary directories
  m_TempDir = TempDir;
  m_TempBuildsDir = TempDir+"/"+BUILDS_SUBDIR;
  m_TempDownloadsDir = TempDir+"/"+DLOADS_SUBDIR;

  // Installation directories
  m_MarketBagSimulatorDir = MarketBagSimulatorDir;
  m_MarketBagObserverDir = MarketBagObserverDir;
  m_MarketBagBuilderextDir = MarketBagBuilderextDir;
  m_MarketBagDatasetDir = MarketBagDatasetDir;
  m_MarketBagBinSubDir = MarketBagBinSubDir;
  m_MarketBagSrcSubDir = MarketBagSrcSubDir;

  m_LogFile = TempDir+"/"+LOG_FILENAME;

  m_Initialized = false;
}


// =====================================================================
// =====================================================================


std::string MarketPackage::getCommonBuildOptions(const PackageInfo::PackageType& Type)
{
  if (Type == PackageInfo::SIM)
  {
    return m_SimulatorBuildConfigOptions;
  }
  else if (Type == PackageInfo::OBS)
  {
    return m_ObserverBuildConfigOptions;
  }
  else if (Type == PackageInfo::BUILD)
  {
    return m_BuilderextBuildConfigOptions;
  }
  return "";
}


// =====================================================================
// =====================================================================


void MarketPackage::setCommonBuildOptions(const PackageInfo::PackageType& Type, const std::string& BuildOptions)
{
  if (Type == PackageInfo::SIM)
  {
    m_SimulatorBuildConfigOptions = BuildOptions;
  }
  else if (Type == PackageInfo::OBS)
  {
    m_ObserverBuildConfigOptions = BuildOptions;
  }
  else if (Type == PackageInfo::BUILD)
  {
    m_BuilderextBuildConfigOptions = BuildOptions;
  }
}


// =====================================================================
// =====================================================================


std::string MarketPackage::composeFullBuildOptions(const PackageInfo::PackageType& Type,
                                                   const std::string& BuildOptions)
{
  std::string FullOptions = "";

  if (!BuildOptions.empty()) FullOptions = " " + BuildOptions;

  if (!getCommonBuildOptions(Type).empty()) FullOptions = " " + getCommonBuildOptions(Type) + FullOptions;

  return FullOptions;
}


// =====================================================================
// =====================================================================


void MarketPackage::appendToLogFile(const std::string& PackageName,
                                    const PackageInfo::PackageType& Type,
                                    const std::string& Action,
                                    const std::string& Str)
{
  if (m_IsLogEnabled)
  {
    std::string StrType;
    if (Type == PackageInfo::SIM) StrType = "SIM";
    else if (Type == PackageInfo::OBS) StrType = "OBS";
    else if (Type == PackageInfo::BUILD) StrType = "BEXT";
    else StrType = "DATA";

    std::ofstream LogFileStream;
    LogFileStream.open(m_LogFile.c_str(),std::ios_base::app);
    LogFileStream << "\n================================================================================\n";
    LogFileStream << "  [" << StrType << "] " << PackageName << " : " << Action << "\n";
    LogFileStream << "================================================================================\n";
    LogFileStream << Str << "\n";
    LogFileStream.close();
  }

}


// =====================================================================
// =====================================================================


void MarketPackage::appendToLogFile(const std::string& Str)
{
  if (m_IsLogEnabled)
  {
    std::ofstream LogFileStream;
    LogFileStream.open(m_LogFile.c_str(),std::ios_base::app);
    LogFileStream << Str << "\n";
    LogFileStream.close();
  }

}


// =====================================================================
// =====================================================================


void MarketPackage::resetLogFile()
{
  if (m_IsLogEnabled)
  {
    openfluid::tools::Filesystem::removeFile(m_LogFile);
    std::ofstream(m_LogFile.c_str()).close();
  }
}


// =====================================================================
// =====================================================================


void MarketPackage::download()
{

  if (!m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "package "+m_PackageFilename+" not initialized");

  m_PackageDest = m_TempDownloadsDir+"/"+m_PackageFilename;

  appendToLogFile(m_PackageFilename,getPackageType(),"downloading","");

  if (!openfluid::utils::FileDownloader::downloadToFile(m_PackageURL, m_PackageDest))
  {
    appendToLogFile("Error");
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "error while downloading package "+m_PackageFilename);
  }

  appendToLogFile("OK");

  m_Downloaded = true;

}


} } // namespaces





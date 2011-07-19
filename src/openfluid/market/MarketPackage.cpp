/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file MarketPackage.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/config.hpp>
#include <openfluid/market/MarketPackage.hpp>
#include <openfluid/tools/CURLDownloader.hpp>


namespace openfluid { namespace market {

const std::string MarketPackage::BUILDS_SUBDIR = "builds";
const std::string MarketPackage::DLOADS_SUBDIR = "downloads";
const std::string MarketPackage::LOG_FILENAME = "packages_install.log";

std::string MarketPackage::m_TempDir = "";
std::string MarketPackage::m_TempBuildsDir = "";
std::string MarketPackage::m_TempDownloadsDir = "";
std::string MarketPackage::m_MarketBagBinDir = "";
std::string MarketPackage::m_MarketBagSrcDir = "";
std::string MarketPackage::m_LogFile = "";
bool MarketPackage::m_IsLogEnabled = false;

std::string MarketPackage::m_CMakeCommand = "";
std::string MarketPackage::m_CommonBuildConfigOptions = openfluid::config::MARKET_COMMONBUILDOPTS;

bool MarketPackage::m_Initialized = false;


MarketPackage::MarketPackage(openfluid::base::FuncID_t ID, std::string PackageURL)
              : m_ID(ID), m_PackageURL(PackageURL)
{
  m_PackageFilename = boost::filesystem::path(PackageURL).leaf();
  m_Downloaded = false;
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
  std::string CMakeProgram = "";

#if defined __unix__ || defined __APPLE__
  CMakeProgram = "cmake";
#endif

#if WIN32
  CMakeProgram = "cmake.exe";
#endif

  if (CMakeProgram.empty())
    throw openfluid::base::OFException("OpenFLUID framework","MarketPackage::initialize()","Unsupported system platform");


  std::vector<std::string> CMakePaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar(CMakeProgram);

  if (!CMakePaths.empty())
  {
    m_CMakeCommand = boost::filesystem::path(CMakePaths[0]).string();
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework","MarketPackage::initialize()","Required CMake program not found");

  // TODO
  //m_CommonBuildConfigOptions = openfluid::config::MARKET_COMMONBUILDOPTS;

  m_IsLogEnabled = EnableLog;

  resetLogFile();

  m_Initialized = true;
}


// =====================================================================
// =====================================================================


void MarketPackage::setWorksDirs(std::string TempDir, std::string MarketBagBinDir, std::string MarketBagSrcDir)
{
  m_TempDir = boost::filesystem::path(TempDir).string();
  m_TempBuildsDir = boost::filesystem::path(TempDir+"/"+BUILDS_SUBDIR).string();
  m_TempDownloadsDir = boost::filesystem::path(TempDir+"/"+DLOADS_SUBDIR).string();
  m_MarketBagBinDir = boost::filesystem::path(MarketBagBinDir).string();
  m_MarketBagSrcDir = boost::filesystem::path(MarketBagSrcDir).string();
  m_LogFile = boost::filesystem::path(TempDir+"/"+LOG_FILENAME).string();

  m_Initialized = false;
}


// =====================================================================
// =====================================================================


std::string MarketPackage::composeFullBuildOptions(std::string BuildOptions)
{
  std::string FullOptions = "";

  if (!BuildOptions.empty()) FullOptions = " " + BuildOptions;

  if (!m_CommonBuildConfigOptions.empty()) FullOptions = " " + m_CommonBuildConfigOptions + FullOptions;

  return FullOptions;
}


// =====================================================================
// =====================================================================


void MarketPackage::appendToLogFile(const std::string& PackageName,
                                    const std::string& Action,
                                    const std::string& Str)
{
  if (m_IsLogEnabled)
  {
    std::ofstream LogFileStream;
    LogFileStream.open(boost::filesystem::path(m_LogFile).string().c_str(),std::ios_base::app);
    LogFileStream << "\n================================================================================\n";
    LogFileStream << "  " << PackageName << " : " << Action << "\n";
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
    LogFileStream.open(boost::filesystem::path(m_LogFile).string().c_str(),std::ios_base::app);
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
    boost::filesystem::remove(boost::filesystem::path(m_LogFile));
    std::ofstream(boost::filesystem::path(m_LogFile).string().c_str()).close();
  }
}


// =====================================================================
// =====================================================================


void MarketPackage::download()
{

  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","MarketPackage::download()","package "+m_PackageFilename+" not initialized");

  m_PackageDest = boost::filesystem::path(m_TempDownloadsDir+"/"+m_PackageFilename).string();

  appendToLogFile(m_PackageFilename,"downloading","");

  if (openfluid::tools::CURLDownloader::downloadToFile(m_PackageURL, m_PackageDest) != openfluid::tools::CURLDownloader::NO_ERROR)
  {
    appendToLogFile("Error");
    throw openfluid::base::OFException("OpenFLUID framework","MarketPackage::download()","error while downloading package "+m_PackageFilename);
  }

  appendToLogFile("OK");

  m_Downloaded = true;

}


} } // namespaces





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
  @file InfoTasks.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <iostream>

#include <boost/algorithm/string/join.hpp>

#include <openfluid/global.hpp>
#include <openfluid/config.hpp>
#include <openfluid/buildinfo.hpp>
#include <openfluid/thirdparty/JSON.hpp>
#include <openfluid/base/RunContextManager.hpp>

#include "InfoTasks.hpp"


int InfoTasks::processVersion() const
{
  if (m_Cmd.isOptionActive("numeric"))
  {
    std::cout << OPENFLUID_VERSION_NUMERIC << std::endl;
  }
  else if (m_Cmd.isOptionActive("no-status"))
  {
    std::cout << openfluid::config::VERSION_MAJOR << "."
              << openfluid::config::VERSION_MINOR << "."
              << openfluid::config::VERSION_PATCH << std::endl;
  }
  else
  {
    std::cout << openfluid::config::VERSION_FULL << std::endl;
  }

  return 0;
}


// =====================================================================
// =====================================================================


void printInfoTxt(const openfluid::thirdparty::json& JSON, const std::string& Prefix = "")
{
  for (const auto& Element : JSON.items())
  {
    if (Element.value().is_object())
    {
      printInfoTxt(Element.value(),Prefix+Element.key()+"/");
    }
    else
    {
      std::cout << Prefix <<  Element.key() << " : ";

      if (Element.value().is_string())
      {
        std::cout << Element.value().get<std::string>();
      }
      if (Element.value().is_boolean())
      {
        std::cout << (Element.value().get<bool>() ? "yes" : "no");
      }
      else if (Element.value().is_array())
      {        
        std::cout << boost::algorithm::join(Element.value().get<std::vector<std::string>>(),",");
      }
      std::cout << "\n";
    }
  }
}


// =====================================================================
// =====================================================================


int InfoTasks::processInfo() const
{
  std::string TmpFlags = openfluid::config::BUILD_COMPILATION_FLAGS;
  std::replace(TmpFlags.begin(),TmpFlags.end(),';',' ');
  
  openfluid::thirdparty::json JSON;
  
  JSON["build"]["system"]["processor"]["family"] = OPENFLUID_PROCESSOR_STRLABEL;
  JSON["build"]["system"]["os"]["family"] = OPENFLUID_OS_STRLABEL;
  JSON["build"]["type"] = openfluid::config::BUILD_TYPE;
  JSON["build"]["timestamp"] = openfluid::config::BUILD_TIMESTAMP;
  JSON["build"]["cmake"]["version"] = openfluid::config::BUILD_CMAKE_VERSION;
  JSON["build"]["c++"]["standard"] = openfluid::config::BUILD_CXX_STANDARD;
  JSON["build"]["compiler"]["id"] = openfluid::config::BUILD_COMPILER_ID;
  JSON["build"]["compiler"]["version"] = openfluid::config::BUILD_COMPILER_VERSION;
  JSON["build"]["compiler"]["flags"] = TmpFlags;
  JSON["build"]["openfluid"]["ui"] = openfluid::config::BUILD_OPENFLUID_UI;

  JSON["dependencies"]["nlohmannjson"]["version"] = openfluid::config::BUILD_LIB_nlohmannjson_VERSION;
  JSON["dependencies"]["tinyxml2"]["version"] = openfluid::config::BUILD_LIB_tinyxml2_VERSION;
  JSON["dependencies"]["boost"]["version"] = openfluid::config::BUILD_LIB_Boost_VERSION;
  JSON["dependencies"]["gdal"]["version"] = openfluid::config::BUILD_LIB_GDAL_VERSION;
  JSON["dependencies"]["libcurl"]["version"] = openfluid::config::BUILD_LIB_CURL_VERSION;
  if (openfluid::config::BUILD_OPENFLUID_UI)
  {
    JSON["dependencies"]["qt"]["version"] = openfluid::config::BUILD_LIB_QT_VERSION;
  }

  JSON["paths"]["install_prefix"] = openfluid::base::Environment::getInstallPrefix();
  JSON["paths"]["input"] = openfluid::base::RunContextManager::instance()->getInputDir();
  JSON["paths"]["output"] = openfluid::base::RunContextManager::instance()->getOutputDir();
  JSON["paths"]["search"]["simulators"] = openfluid::base::Environment::getSimulatorsDirs();
  JSON["paths"]["search"]["observers"] = openfluid::base::Environment::getObserversDirs();
  JSON["paths"]["temp"] = openfluid::base::Environment::getTempDir();


  if (m_Cmd.isOptionActive("format"))
  {
    if (m_Cmd.getOptionValue("format") == "json")
    {
      std::cout <<JSON.dump(2) << std::endl;
      return 0;
    }
    else if (m_Cmd.getOptionValue("format") == "txt")
    {
      printInfoTxt(JSON);
      return 0;
    }
    else
    {
      return error("unknown format");
    }
  }
  else
  {
    printInfoTxt(JSON);
    return 0;
  }

  return error();
}


// =====================================================================
// =====================================================================


int InfoTasks::process() const
{
  if (m_Cmd.getName() == "version")
  {
    return processVersion();
  } 
  else if (m_Cmd.getName() == "info")
  {
    return processInfo();
  }

  return unknownCommand();
}

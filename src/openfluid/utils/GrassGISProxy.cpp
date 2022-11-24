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
  @file GrassGISProxy.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <fstream>

#include <openfluid/utils/GrassGISProxy.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/base/Environment.hpp>


namespace openfluid { namespace utils {


GrassGISProxy::GrassGISProxy(const std::string& GISBase, const std::string& Location, const std::string& Mapset) :
  m_GISBase(GISBase), m_Location(Location), m_Mapset(Mapset)
{
  findGrassGISProgram();
}


// =====================================================================
// =====================================================================


GrassGISProxy::~GrassGISProxy()
{

}


// =====================================================================
// =====================================================================


void GrassGISProxy::findGrassGISProgram()
{
  if (m_ExecutablePath.empty())
  {
    m_ExecutablePath = 
      ExternalProgram::getRegisteredProgram(ExternalProgram::RegisteredPrograms::GrassGis).getFullProgramPath();

    if (!m_ExecutablePath.empty())
    {
      Process P(m_ExecutablePath,{"--help"});
      P.run();
      const auto OutLines = P.stdOutLines();

      if (!OutLines.empty() && openfluid::tools::startsWith(OutLines[0],"GRASS GIS "))
      {
        m_Version = OutLines[0];
        m_Version.erase(0,10);
      }
    }
  }
}


// =====================================================================
// =====================================================================


std::string GrassGISProxy::getCommandLine(const std::string& Command,
                                          const std::map<std::string,std::string>& Arguments,
                                          const std::vector<std::string>& Options)
{
  std::string Cmd = Command;

  for (auto& Arg : Arguments)
  {
    Cmd += " " + Arg.first + "=" + Arg.second;
  }

  for (auto& Opt : Options)
  {
    Cmd += " " + Opt;
  }

  return Cmd;
}


// =====================================================================
// =====================================================================


std::string GrassGISProxy::writeJobFile(const std::vector<std::string>& Lines, const std::string& DirPath)
{
  auto FullFilePath = openfluid::tools::Filesystem::makeUniqueFile(DirPath,"grassproxy-job.sh");

  if (!FullFilePath.empty())
  {
    auto JobFile = std::ofstream(FullFilePath,std::ios::out|std::ios::trunc);

    if (JobFile.is_open())
    {
      JobFile << "#!/bin/sh\n\n";

      JobFile << openfluid::tools::join(Lines,"\n") << "\n";

      JobFile.close();
    }

    // set file permissionsto 744 on job file
    auto JobFSP = openfluid::tools::Path(FullFilePath);
    std::filesystem::permissions(JobFSP.stdPath(),std::filesystem::perms::owner_all | 
                                                  std::filesystem::perms::group_read |
                                                  std::filesystem::perms::others_read);

    return FullFilePath;
  }

  return "";
}


// =====================================================================
// =====================================================================


bool GrassGISProxy::isAvailable()
{
  findGrassGISProgram();

  return (!m_ExecutablePath.empty() && !m_Version.empty());
}


// =====================================================================
// =====================================================================


int GrassGISProxy::executeGrassJob(const std::string& JobFilePath) const
{
  if (!isLocationExist())
  {
    return -99;
  }

  Process::Environment PcsEnv;
  PcsEnv.Vars["GRASS_BATCH_JOB"] = JobFilePath;

  Process::Command PcsCmd;
  
  PcsCmd.OutFile = m_OutputFile;
  if (PcsCmd.OutFile.empty())
  {
    PcsCmd.OutFile = JobFilePath+".out";
  }

  PcsCmd.ErrFile = m_ErrorFile;
  if (PcsCmd.ErrFile.empty())
  {
    PcsCmd.ErrFile = JobFilePath+".err";
  }
  
  PcsCmd.Program = "\""+m_ExecutablePath+"\"";
  PcsCmd.Args = {std::string("\""+m_GISBase+"/"+m_Location+"/"+m_Mapset+"\"")};

  Process P(PcsCmd,PcsEnv);
  P.run();

  return P.getExitCode();
}


// =====================================================================
// =====================================================================


int GrassGISProxy::executeGrassJobReturningData(const std::string& JobFilePath, std::vector<std::string>& Lines) const
{
  if (!isLocationExist())
  {
    return -99;
  }

  Process::Environment PcsEnv;
  PcsEnv.Vars["GRASS_BATCH_JOB"] = JobFilePath;

  Process::Command PcsCmd;
  PcsCmd.Program = "\""+m_ExecutablePath+"\"";
  PcsCmd.Args = {std::string("\""+m_GISBase+"/"+m_Location+"/"+m_Mapset+"\"")};

  Process P(PcsCmd,PcsEnv);
  P.run();

  if (P.getExitCode() == 0)
  {
    Lines = P.stdOutLines();
  }

  return P.getExitCode();
}


// =====================================================================
// =====================================================================


void GrassGISProxy::appendTask(const std::string& Command,
                               const std::map<std::string,std::string>& Arguments,
                               const std::vector<std::string>& Options)
{
  m_JobLines << getCommandLine(Command,Arguments,Options);
}


// =====================================================================
// =====================================================================


int GrassGISProxy::runJob()
{
  int RetValue = -1;

  std::string JobFilePath = writeJobFile(m_JobLines,openfluid::base::Environment::getTempDir());

  if (!JobFilePath.empty())
  {
    RetValue = executeGrassJob(JobFilePath);
    clearTasks();
  }

  return RetValue;
}


// =====================================================================
// =====================================================================


int GrassGISProxy::runSingleTask(const std::string& Command,
                                 const std::map<std::string,std::string>& Arguments,
                                 const std::vector<std::string>& Options) const
{
  int RetValue = -1;

  std::vector<std::string> Tasks;
  Tasks << getCommandLine(Command,Arguments,Options);

  std::string JobFilePath = writeJobFile(Tasks,openfluid::base::Environment::getTempDir());

  if (!JobFilePath.empty())
  {
    RetValue = executeGrassJob(JobFilePath);
  }

  return RetValue;
}


// =====================================================================
// =====================================================================


bool GrassGISProxy::isLocationExist() const
{
  std::string Path = m_GISBase+"/"+m_Location;

  return openfluid::tools::FilesystemPath(Path).isDirectory();
}


// =====================================================================
// =====================================================================


bool GrassGISProxy::createLocation(const std::string& EPSG) const
{
  Process::Command PcsCmd;

  PcsCmd.Program = "\""+m_ExecutablePath+"\"";
  PcsCmd.Args = {
    "-c",std::string("\""+EPSG+"\""),
    "-e",std::string("\""+m_GISBase+"/"+m_Location+"\"")
  };

  Process P(PcsCmd);
  P.run();

  return (P.getExitCode() == 0);
}


// =====================================================================
// =====================================================================


std::map<std::string,double> GrassGISProxy::region() const
{
  std::map<std::string,double> Values;

  std::vector<std::string> Tasks;
  Tasks << getCommandLine("g.region",{},{"-gucep"});

  auto JobFilePath = writeJobFile(Tasks,openfluid::base::Environment::getTempDir());

  if (!JobFilePath.empty())
  {
    std::vector<std::string> Lines;
    if (executeGrassJobReturningData(JobFilePath,Lines) == 0)
    {
      for (const auto& Ln : Lines)
      {
        std::vector<std::string> Parts = openfluid::tools::split(Ln,"=");

        if (Parts.size() == 2)
        {
          double ConvertedVal;
          bool ConvertOK = openfluid::tools::toNumeric<double>(Parts[1],ConvertedVal);
          if (ConvertOK)
          {
            Values[Parts[0]] = ConvertedVal;
          }
        }
      }
    }
  }

  return Values;
}


// =====================================================================
// =====================================================================


std::map<std::string,std::string> GrassGISProxy::gisenv() const
{
  std::map<std::string,std::string> Values;

  std::vector<std::string> Tasks;
  Tasks << getCommandLine("g.gisenv",{},{"-n","--q"});

  auto JobFilePath = writeJobFile(Tasks,openfluid::base::Environment::getTempDir());

  if (!JobFilePath.empty())
  {
    std::vector<std::string> Lines;
    if (executeGrassJobReturningData(JobFilePath,Lines) == 0)
    {
      for (const auto& Ln : Lines)
      {
        std::vector<std::string> Parts = openfluid::tools::split(Ln,"=");

        if (Parts.size() == 2)
        {
            Values[Parts[0]] = Parts[1];
        }
      }
    }
  }

  return Values;
}


// =====================================================================
// =====================================================================


std::vector<std::string> GrassGISProxy::mapsets() const
{
  std::vector<std::string> Values;

  std::vector<std::string> Tasks;
  Tasks << getCommandLine("g.mapsets",{{"separator","'\\n'"}},{"-l"});

  auto JobFilePath = writeJobFile(Tasks,openfluid::base::Environment::getTempDir());

  if (!JobFilePath.empty())
  {
    std::vector<std::string> Lines;
    if (executeGrassJobReturningData(JobFilePath,Lines) == 0)
    {
      for (const auto& Ln : Lines)
      {
        if (!openfluid::tools::trim(Ln).empty())
        {
          Values.push_back(Ln);
        }
      }
    }
  }

  return Values;
}


} }  // namespaces

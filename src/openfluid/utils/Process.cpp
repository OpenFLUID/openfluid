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
  @file Process.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <fstream>
#include <sstream>
#include <iterator>

#include <boost/process.hpp>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/utils/Process.hpp>


namespace openfluid { namespace utils {


std::string Process::Command::joined(const std::string& Sep) const
{
  std::stringstream JoinedArgs;
  std::copy(Args.begin(),Args.end(),std::ostream_iterator<std::string>(JoinedArgs,Sep.c_str()));
  return (Program + Sep + JoinedArgs.str());
}


// =====================================================================
// =====================================================================


void Process::Command::setOutFile(const std::string& DirPath, const std::string& NameBase)
{
  OutFile = openfluid::tools::Path({DirPath,NameBase+"_out.log"}).toGeneric();
}


// =====================================================================
// =====================================================================


void Process::Command::setErrFile(const std::string& DirPath, const std::string& NameBase)
{
  ErrFile = openfluid::tools::Path({DirPath,NameBase+"_err.log"}).toGeneric();
}


// =====================================================================
// =====================================================================


void Process::Command::setOutErrFiles(const std::string& DirPath, const std::string& NameBase)
{
  setOutFile(DirPath,NameBase);
  setErrFile(DirPath,NameBase);
}


// =====================================================================
// =====================================================================


Process::Process(const std::string& Program, const std::vector<std::string>& Args, const Environment& Env):
  m_Cmd{.Program=Program, .Args=Args, .OutFile=std::string(), .ErrFile=std::string()}, m_Env(Env)
{
  
}


// =====================================================================
// =====================================================================


Process::Process(const Command& Cmd, const Environment& Env) : 
  m_Cmd(Cmd), m_Env(Env)
{

}


// =====================================================================
// =====================================================================


void Process::reset()
{
  m_ExitCode = -1;
  m_ErrorMsg.clear();
  m_OutLines.clear();
  m_ErrLines.clear();
}


// =====================================================================
// =====================================================================


bool Process::run()
{
  // reset run configuration and data
  reset();

  try
  {
    boost::process::environment ProcessEnv;
    boost::process::ipstream StdOutStr;
    boost::process::ipstream StdErrStr;
    std::ofstream StdOutFile;
    std::ofstream StdErrFile;
    std::string Line;

    bool SinkOutToFile = !m_Cmd.OutFile.empty();
    bool SinkErrToFile = !m_Cmd.ErrFile.empty();

    // prepare environment
    if (m_Env.Inherits)
    {
      for(const auto& InheritedVar : boost::this_process::environment())
      {
        ProcessEnv[InheritedVar.get_name()] = InheritedVar.to_string();
      }
    }

    for (const auto& Var : m_Env.Vars)
    {
      ProcessEnv[Var.first] = Var.second;
    }

    boost::process::child BPC(boost::process::exe = m_Cmd.Program,
                              boost::process::args = m_Cmd.Args,
                              boost::process::std_out > StdOutStr, boost::process::std_err > StdErrStr,
                              ProcessEnv);

    // if out is redirected, create out file
    if (SinkOutToFile)
    {
      openfluid::tools::Path(openfluid::tools::Path(m_Cmd.OutFile).dirname()).makeDirectory();
      StdOutFile.open(m_Cmd.OutFile,std::ios::out);
    }

    // if error is redirected, create error file
    if (SinkErrToFile)
    {
      openfluid::tools::Path(openfluid::tools::Path(m_Cmd.ErrFile).dirname()).makeDirectory();
      StdErrFile.open(m_Cmd.ErrFile,std::ios::out);
    }

    while (BPC.running())
    {

      while(std::getline(StdOutStr,Line))
      {
        if (SinkOutToFile)
        {
          // if out is redirected, sink out lines in file
          StdOutFile << Line << "\n";
        }
        else
        {
          m_OutLines.push_back(Line);
        }
      }

      while(std::getline(StdErrStr,Line))
      {
        if (SinkErrToFile)
        {
          // if error is redirected, sink error lines in file
          StdErrFile << Line << "\n";
        }
        else
        {
          m_ErrLines.push_back(Line);
        }
      }
    }

    BPC.wait();
    m_ExitCode = BPC.exit_code();
  }
  catch(const boost::process::process_error& E)
  {
    m_ErrorMsg = std::string(E.what());
    return false;
  }
  catch(...)
  {
    return false;
  }

  return true;
}


// =====================================================================
// =====================================================================


int Process::execute(const std::string& Program, const std::vector<std::string>& Args, const Environment& Env)
{
  Command Cmd{.Program=Program, .Args=Args, .OutFile=std::string(), .ErrFile=std::string()};

  return execute(Cmd,Env);
}


// =====================================================================
// =====================================================================


int Process::execute(const Command& Cmd, const Environment& Env)
{
  Process P(Cmd,Env);
  
  P.run();

  return P.getExitCode();
}


} } // namespaces

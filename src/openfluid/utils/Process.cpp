/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <fstream>
#include <sstream>
#include <iterator>

#if (Boost_VERSION_MINOR > 85) && defined(OPENFLUID_OS_WINDOWS)
#include <winsock2.h> // FIXME check if useful or necessary, theorically useful to respect include order
#endif
#if (Boost_VERSION_MINOR > 85)
#include <unordered_map>
#include <boost/asio/io_context.hpp>
#include <boost/asio/readable_pipe.hpp>
#include <boost/asio/read.hpp>
#include <boost/process/v2/process.hpp>
#include <boost/process/v2/environment.hpp>
#include <boost/process/v2/stdio.hpp>
#include <boost/process/v2/start_dir.hpp>
#else
#include <boost/process.hpp>
#endif

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/base/InternalLogger.hpp>
#include <openfluid/utils/Process.hpp>


namespace openfluid { namespace utils {


std::string Process::Command::joined(const std::string& Sep) const
{
  return (Program + Sep + openfluid::tools::join(Args,Sep));
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
#if (Boost_VERSION_MINOR > 85)  
    std::ofstream StdOutFile;
    std::ofstream StdErrFile;
    std::string LineOut;
    std::string LineErr;
    std::unordered_map<std::string, std::string> ProcessEnv;
    boost::asio::io_context Ctx;
    boost::asio::readable_pipe PipeOut{Ctx};
    boost::asio::readable_pipe PipeErr{Ctx};

    bool SinkOutToFile = !m_Cmd.OutFile.empty();
    bool SinkErrToFile = !m_Cmd.ErrFile.empty();

    // prepare environment
    if (m_Env.Inherits)
    {
      for(const auto& InheritedVar : boost::process::v2::environment::current())
      {
        ProcessEnv[InheritedVar.key().string()] = InheritedVar.value().string();
      }
    }

    for (const auto& Var : m_Env.Vars)
    {
      ProcessEnv[Var.first] = Var.second;
    }
    // Hotfix for windows bug when work dir is empty
    std::string WorkDir;
    if (m_Cmd.WorkDir.empty())
    {
      WorkDir = "."; // warning boost :  If your path is relative, it may fail on posix, 
      //because the directory is changed before a call to execve. 
    }
    else
    {
        WorkDir = m_Cmd.WorkDir;
    }

    //                           boost::process::shell, <- not relevant in new system?
    boost::process::v2::process Proc(Ctx, m_Cmd.Program, m_Cmd.Args, 
                                     boost::process::v2::process_stdio{nullptr, PipeOut, PipeErr}, 
                                     boost::process::v2::process_start_dir(WorkDir),
                                     boost::process::v2::process_environment(ProcessEnv));

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

    boost::system::error_code Ec;
    boost::asio::read(PipeOut, boost::asio::dynamic_buffer(LineOut), Ec);
    if (!Ec && (Ec != boost::asio::error::eof))
    { 
      std::cout << "Boost process reading error in out stream: " << Ec.message() << std::endl;
      openfluid::base::log::error("Process",  "Boost process reading error in out stream");
    }
    boost::asio::read(PipeErr, boost::asio::dynamic_buffer(LineErr), Ec);
    if (!Ec && (Ec != boost::asio::error::eof))
    { 
      std::cout << "Boost process reading error in err stream: " << Ec.message() << std::endl;
      openfluid::base::log::error("Process",  "Boost process reading error in err stream");
    }
    
    Proc.wait();
    for (const auto& L : openfluid::tools::split(LineOut, "\n"))
    {
      if (SinkOutToFile)
      {
        // if out is redirected, sink out lines in file
        StdOutFile << L << "\n";
      }
      else
      {
        m_OutLines.push_back(L);
      }
    }

    for (const auto& L : openfluid::tools::split(LineErr, "\n"))
    {
      if (SinkErrToFile)
      {
        // if out is redirected, sink out lines in file
        StdErrFile << L << "\n";
      }
      else
      {
        m_ErrLines.push_back(L);
      }
    }

    m_ExitCode = Proc.exit_code();
#else
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
    // Hotfix for windows bug when work dir is empty
    std::string WorkDir;
    if (m_Cmd.WorkDir.empty())
    {
        WorkDir = ".";
    }
    else
    {
        WorkDir = m_Cmd.WorkDir;
    }
    
    if (!openfluid::tools::Path(WorkDir).isDirectory())
    {
      openfluid::base::log::error("Process", "Working directory does not exist: "+WorkDir);
      return false;
    }
    
    boost::process::child BPC(boost::process::exe = m_Cmd.Program,
                              boost::process::args = m_Cmd.Args,
                              boost::process::start_dir = WorkDir,
                              boost::process::std_out > StdOutStr, boost::process::std_err > StdErrStr,
                              boost::process::shell,
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
#endif
  }
  catch(const std::exception& E)
  {
    m_ErrorMsg = std::string(E.what());
    openfluid::base::log::error("Process", std::string("Boost process error: ")+E.what());
    return false;
  }
  catch(...)
  {
    // TODO for logging purposes
    openfluid::base::log::error("Process", "Boost process error");
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


// =====================================================================
// =====================================================================


int Process::system(const std::string& Program, const std::vector<std::string>& Args, const Environment& Env)
{
  Command Cmd{.Program=Program, .Args=Args, .OutFile=std::string(), .ErrFile=std::string()};

  return system(Cmd,Env);
}


// =====================================================================
// =====================================================================
 

int Process::system(const Command& Cmd, const Environment& Env)
{
#if (Boost_VERSION_MINOR > 85)
  boost::asio::io_context Ctx;
  std::unordered_map<std::string, std::string> ProcessEnv;
  // prepare environment
  if (Env.Inherits)
  {
    for(const auto& InheritedVar : boost::process::v2::environment::current())
    {
      ProcessEnv[InheritedVar.key().string()] = InheritedVar.value().string();
    }
  }

  for (const auto& Var : Env.Vars)
  {
    ProcessEnv[Var.first] = Var.second;
  }
  // Hotfix for windows bug when work dir is empty
  std::string WorkDir;
  if (Cmd.WorkDir.empty())
  {
    WorkDir = "."; // warning boost :  If your path is relative, it may fail on posix, 
    //because the directory is changed before a call to execve. 
  }
  else
  {
    WorkDir = Cmd.WorkDir;
  }
  boost::process::v2::process Proc(Ctx, Cmd.Program, Cmd.Args, 
                                     boost::process::v2::process_start_dir(WorkDir),
                                     boost::process::v2::process_environment(ProcessEnv));
  return Proc.wait();
#else
  boost::process::environment ProcessEnv;
  
  // prepare environment
  if (Env.Inherits)
  {
    for(const auto& InheritedVar : boost::this_process::environment())
    {
      ProcessEnv[InheritedVar.get_name()] = InheritedVar.to_string();
    }
  }

  for (const auto& Var : Env.Vars)
  {
    ProcessEnv[Var.first] = Var.second;
  }

  std::string WorkDir;
  if (Cmd.WorkDir.empty())
  {
      WorkDir = ".";
  }
  else
  {
      WorkDir = Cmd.WorkDir;
  }

  return boost::process::system(boost::process::exe = Cmd.Program,
                                boost::process::args = Cmd.Args,
                                boost::process::start_dir = WorkDir,
                                ProcessEnv);
#endif
}


} } // namespaces

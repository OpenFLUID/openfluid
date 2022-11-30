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
  @file Process.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UTILS_PROCESS_HPP__
#define __OPENFLUID_UTILS_PROCESS_HPP__


#include <string>
#include <vector>
#include <map>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace utils {

/**
  Management of processes to launch external programs

  <I>Example using the run() method with program and arguments</I>
  @snippet misc/process.cpp usage_run_progargs

  <I>Example using the run() method with fine tuning of command configuration</I>
  @snippet misc/process.cpp usage_run_cmd

  <I>Example using the execute() method</I>
  @snippet misc/process.cpp usage_execute
*/
class OPENFLUID_API Process
{
  public:

    /**
      Structure to hold execution environment for processes
    */
    struct OPENFLUID_API Environment 
    {
      /**
        Custom environment variables defined for a process.  
        If a variable already exists in parent environment when inherited, the parent variable is overidden.
      */
      std::map<std::string,std::string> Vars;

      /**
        Enable/disable inheritance of parent environment, enabled by default.
      */
      bool Inherits;

      Environment() : Inherits(true)
      { }
    };

    /**
      Structure to hold command configuration for processes
    */
    struct OPENFLUID_API Command
    {
      /**
        Path of the program to execute.
      */
      std::string Program;
      
      /**
        Program arguments
      */
      std::vector<std::string> Args;

      /**
        Path to the file for redirection of standard output stream (stdout) during program execution.
        No redirection is performed if empty.
      */
      std::string OutFile;

      /**
        Path to the file for redirection of standard error stream (stderr) during program execution.
        No redirection is performed if empty.
      */
      std::string ErrFile;

      /**
        Joins program and arguments as a single string
        @param[in] Sep The separator to use for joining
        @return The string after joining 
      */
      std::string joined(const std::string& Sep = " ") const;

      /**
        Sets the redirection file for output using normalized naming
        @param[in] DirPath The directory path for output file
        @param[in] NameBase The base to use for naming the output file
      */
      void setOutFile(const std::string& DirPath, const std::string& NameBase);

      /**
        Sets the redirection file for error using normalized naming
        @param[in] DirPath The directory path for error file
        @param[in] NameBase The base to use for naming the error file
      */
      void setErrFile(const std::string& DirPath, const std::string& NameBase);

      /**
        Sets the redirection files for output and error using normalized naming
        @param[in] DirPath The directory path for files
        @param[in] NameBase The base to use for naming the files
      */
      void setOutErrFiles(const std::string& DirPath, const std::string& NameBase);
    };


  private:

    const Command m_Cmd;

    const Environment m_Env;

    int m_ExitCode = -1;

    std::string m_ErrorMsg;

    std::vector<std::string> m_OutLines;

    std::vector<std::string> m_ErrLines;

    void reset();


  public:

    /**
      Constructor using program and arguments to configure the process
      @param[in] Program Path of the program to execute.
      @param[in] Args Program arguments (empty by default)
      @param[in] Env Execution environment (parent environment by default)
    */
    Process(const std::string& Program, const std::vector<std::string>& Args = {}, 
            const Environment& Env = Environment());

    /**
      Constructor using command structure to configure the process
      @param[in] Cmd Command configuration
      @param[in] Env Execution environment (parent environment by default)
    */
    Process(const Command& Cmd, const Environment& Env = {});

    /**
      Runs the process
      @return true if the process has been run, false otherwise. Do not confuse with exit code
    */
    bool run();

    /**
      Gets the exit code returned by the executed program when finished
      @return The exit code
    */
    int getExitCode() const
    {
      return m_ExitCode;
    }

    /**
      Gets the error message in case of execution problem
      @return The error message. Do not confuse with exit code
    */
    std::string getErrorMessage() const
    {
      return m_ErrorMsg;
    }

    /**
      Accesses to the lines of the standard output stream once the process have run.
      These lines set is empty if the redirection of the standard output stream has been enabled
      @return The output stream lines
    */
    const std::vector<std::string>& stdOutLines() const
    {
      return m_OutLines;
    }

    /**
      Accesses to the lines of the standard error stream once the process have run.
      These lines set is empty if the redirection of the standard error stream has been enabled
      @return The error stream lines
    */
    const std::vector<std::string>& stdErrLines() const
    {
      return m_ErrLines;
    }

    /**
      Accesses the execution environment used for the process
      @return The execution environment
    */
    const Environment& environment() const
    {
      return m_Env;
    }

    /**
      Accesses the command configuration used for the process
      @return The command configuration
    */
    const Command& command() const
    {
      return m_Cmd;
    }

    /**
      Executes a process using the given program, arguments and execution environment
      @param[in] Program Path of the program to execute.
      @param[in] Args Program arguments (empty by default)
      @param[in] Env Execution environment (parent environment by default)
    */
    static int execute(const std::string& Program, const std::vector<std::string>& Args = {}, 
                       const Environment& Env = Environment());

    /**
      Executes a process using the given command configuration and execution environment
      @param[in] Cmd Command configuration
      @param[in] Env Execution environment (parent environment by default)
    */
    static int execute(const Command& Cmd, const Environment& Env = Environment());

    /**
      Executes a process using the given program, arguments and execution environment
      with default standard i/o (stdout,stderr,stdin). It works as std::system with more options.
      @param[in] Program Path of the program to execute.
      @param[in] Args Program arguments (empty by default)
      @param[in] Env Execution environment (parent environment by default)
    */
    static int system(const std::string& Program, const std::vector<std::string>& Args = {}, 
                      const Environment& Env = Environment());

    /**
      Executes a process using the given command configuration and execution environment
      with default standard i/o (stdout,stderr,stdin). It works as std::system with more options.
      @param[in] Cmd Command configuration
      @param[in] Env Execution environment (parent environment by default)
    */
    static int system(const Command& Cmd, const Environment& Env = Environment());

};


} } // namespaces

#endif /* __OPENFLUID_UTILS_PROCESS_HPP__ */

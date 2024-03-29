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
  @file GrassGISProxy.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UTILS_GRASSGISPROXY_HPP__
#define __OPENFLUID_UTILS_GRASSGISPROXY_HPP__


#include <map>
#include <vector>

#include <openfluid/utils/ProgramProxy.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace utils {


class OPENFLUID_API GrassGISProxy : public ProgramProxy<GrassGISProxy>
{
  private:

    std::string m_GISBase;

    std::string m_Location;

    std::string m_Mapset;

    std::vector<std::string> m_JobLines;

    std::string m_OutputFile;

    std::string m_ErrorFile;

    static void findGrassGISProgram();

    static std::string getCommandLine(const std::string& Command,
                                      const std::map<std::string,std::string>& Arguments = {},
                                      const std::vector<std::string>& Options = {});

    static std::string writeJobFile(const std::vector<std::string>& Lines, const std::string& DirPath);

    int executeGrassJob(const std::string& JobFilePath) const;

    int executeGrassJobReturningData(const std::string& JobFilePath, std::vector<std::string>& Lines) const;


  public:

    GrassGISProxy() = delete;

    GrassGISProxy(const std::string& GISBase, const std::string& Location, const std::string& Mapset = "PERMANENT");

    virtual ~GrassGISProxy();

    static bool isAvailable();

    /**
      Sets the current location to use for next GRASS execution
      @param[in] Location the location directory
    */
    void setLocation(const std::string& Location)
    { 
      m_Location = Location; 
    }

    /**
      Sets the current mapset to use for next GRASS execution
      @param[in] Mapset the mapset directory
    */
    void setMapset(const std::string& Mapset)
    { 
      m_Mapset = Mapset;
    }

    /**
      Clears all appended tasks.
    */
    void clearTasks()
    { 
      m_JobLines.clear();
    }

    /**
      Appends a task to the current GRASS job.
      @param[in] Command the GRASS command to execute
      @param[in] Arguments a key-value map of arguments to pass to the GRASS command
      @param[in] Options options to pass to the GRASS command
    */
    void appendTask(const std::string& Command,
                    const std::map<std::string,std::string>& Arguments = {},
                    const std::vector<std::string>& Options = {});

    /**
      Runs a GRASS job made of appended tasks. Once finished, the task queue is cleared.
      @return the exit code of the execution
    */
    int runJob();

    /**
      Runs a single GRASS task.
      @param[in] Command the GRASS command to execute
      @param[in] Arguments a key-value map of arguments to pass to the GRASS command
      @param[in] Options options to pass to the GRASS command
      @return the exit code of the execution
    */
    int runSingleTask(const std::string& Command,
                      const std::map<std::string,std::string>& Arguments = {},
                      const std::vector<std::string>& Options = {}) const;

    /**
      Sets the file path where the standard output stream will be redirected.
      If not set or empty, the stream is redirected to a temporary file.
      @param[in] FullPath the full path for the output file
    */
    void setOutputFile(const std::string& FullPath)
    { 
      m_OutputFile = FullPath;
    }

    /**
      Sets the file path where the standard error stream will be redirected.
      If not set or empty, the stream is redirected to a temporary file.
      @param[in] FullPath the full path for the error file
    */
    void setErrorFile(const std::string& FullPath)
    { 
      m_ErrorFile = FullPath;
    }

    /**
      Returns the current command lines of the job
      @return a list of command lines
    */
    const std::vector<std::string>& jobLines() const
    { 
      return m_JobLines;
    }


    /**
      Returns true if the location already exists
      @return true  if the location already exists, false otherwise
    */
    bool isLocationExist() const;


    /**
      Creates the location defined for the current GRASS session, with the given EPSG code
      @param[in] EPSG the EPSG code for the location
      @return true  if the location has been created, false if it could not be created
              or if it already exists
    */
    bool createLocation(const std::string& EPSG) const;

    /**
      Returns the region informations as a key-value map,
      given by the results of the "g.region -gucep" GRASS command
      @return a key-value map about containing region informations
    */
    std::map<std::string,double> region() const;

    /**
      Returns the environment informations as a key-value map,
      given by the results of the "g.gisenv -n --q" GRASS command
      @return a key-value map about containing environment informations
    */
    std::map<std::string,std::string> gisenv() const;

    /**
      Returns the list of mapsets in the current location,
      given by the results of the "g.mapsets separator='\n' -l" GRASS command
      @return a vector of strings containing the mapsets list
    */
    std::vector<std::string> mapsets() const;

};


} }  // namespaces


#endif /* __OPENFLUID_UTILS_GRASSGISPROXY_HPP__ */


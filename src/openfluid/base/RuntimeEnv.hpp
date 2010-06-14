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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __RUNTIMEENV_HPP__
#define __RUNTIMEENV_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/tools.hpp>
#include <openfluid/base/EnvProperties.hpp>


#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


namespace openfluid { namespace base {


// =====================================================================
// =====================================================================


/**

*/
class DLLEXPORT RuntimeEnvironment
{
  private:

    static RuntimeEnvironment* mp_Singleton;

    std::string m_OutputDir;
    std::string m_InputDir;
    std::string m_UserDataDir;
    std::string m_TempDir;

    std::vector<std::string> m_PlugsDirs;

    std::string m_UserID;

    std::string m_HostName;

    bool m_ClearOutputDir;

    bool m_WriteResults;

    bool m_WriteSimReport;

    unsigned int m_ValuesBufferSize;

    bool m_IsUserValuesBufferSize;

    unsigned int m_FilesBufferSize;

    openfluid::base::EnvironmentProperties* mp_FuncEnv;

    openfluid::base::EnvironmentProperties m_ExtraProperties;

    boost::posix_time::ptime m_IgnitionDateTime;

    std::string m_SimulationID;

    boost::posix_time::time_duration m_EffectiveSimulationDuration;

    openfluid::core::DateTime m_StartTime;

    openfluid::core::DateTime m_EndTime;

    // TODO set correct type for time step
    int m_TimeStep;

    /**
      Default constructor
    */
    RuntimeEnvironment();

  public:

    static RuntimeEnvironment* getInstance();

    /**
      Destructor
    */
    ~RuntimeEnvironment();

    void setInputDir(const std::string InputDir)
      { m_InputDir = InputDir; mp_FuncEnv->setValue("dir.input",m_InputDir); };

    std::string getInputDir() const { return m_InputDir; };

    void setOutputDir(const std::string OutputDir)
      { m_OutputDir = OutputDir; mp_FuncEnv->setValue("dir.output",m_OutputDir); };

    std::string getOutputDir() const { return m_OutputDir; };

    void setDateTimeOutputDir();

    std::string getTempDir() const
      { return m_TempDir; mp_FuncEnv->setValue("dir.temp",m_OutputDir); };

    std::string getInputFullPath(std::string Filename) const
      { return boost::filesystem::path(m_InputDir + "/" + Filename).string(); };

    std::string getOutputFullPath(std::string Filename) const
      { return boost::filesystem::path(m_OutputDir + "/" + Filename).string(); };

    std::string getPluginFullPath(std::string Filename);

    void addExtraPluginsPaths(std::string SemicolonSeparatedPaths);

    std::vector<std::string> getPluginsPaths() const { return m_PlugsDirs; };

    openfluid::base::EnvironmentProperties& getExtraProperties()
      { return m_ExtraProperties; };

    bool isClearOutputDir() const { return m_ClearOutputDir; };

    void setClearOutputDir(bool ClearDir)
      { m_ClearOutputDir = ClearDir; mp_FuncEnv->setValue("mode.clearoutputdir",m_ClearOutputDir); };

    bool isWriteResults() const
      { return m_WriteResults; };

    void setWriteResults(bool WriteIt)
      { m_WriteResults = WriteIt; mp_FuncEnv->setValue("mode.saveresults",m_WriteResults); };

    bool isWriteSimReport() const { return m_WriteSimReport; };

    void setWriteSimReport(bool WriteIt)
      { m_WriteSimReport = WriteIt; mp_FuncEnv->setValue("mode.writereport",m_WriteSimReport); };

    openfluid::base::EnvironmentProperties* getFunctionEnvironment() const
      { return mp_FuncEnv; };


    boost::posix_time::ptime getIgnitionDateTime() const
      { return m_IgnitionDateTime; };

    std::string getSimulationID() const {return m_SimulationID; };

    void setSimulationID(const std::string SimID) { m_SimulationID = SimID; };

    boost::posix_time::time_duration getEffectiveSimulationDuration() const
      { return m_EffectiveSimulationDuration; };

    void setEffectiveSimulationDuration(const boost::posix_time::time_duration& TimeDuration)
      { m_EffectiveSimulationDuration = TimeDuration; };

    std::string getHostName() const {return m_HostName; };

    std::string getUserID() const {return m_UserID; };

    void setSimulationTimeInformation(openfluid::core::DateTime StartTime,
                                      openfluid::core::DateTime EndTime,
                                      int TimeStep);

    openfluid::core::DateTime getSimulationStartTime() const
      { return m_StartTime; };

    openfluid::core::DateTime getSimulationEndTime() const
      { return m_EndTime; };

    // TODO set correct type for time step
    int getSimulationTimeStep() const
      { return m_TimeStep; };

    void setValuesBufferSize(const unsigned int StepsNbr)
      {m_ValuesBufferSize = StepsNbr; m_IsUserValuesBufferSize = true;};

    unsigned int getValuesBufferSize() const
      { return m_ValuesBufferSize; };

    bool isUserValuesBufferSize() const { return m_IsUserValuesBufferSize; };

    void setFilesBufferSize(const unsigned int Bytes)
      {m_FilesBufferSize = Bytes; };

    unsigned int getFilesBufferSize() const
      { return m_FilesBufferSize; };

};


} } //namespaces

#endif




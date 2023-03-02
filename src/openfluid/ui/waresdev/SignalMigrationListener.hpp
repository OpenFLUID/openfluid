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
  @file SignalMigrationListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_SIGNALMIGRATIONLISTENER_HPP__
#define __OPENFLUID_UIWARESDEV_SIGNALMIGRATIONLISTENER_HPP__


#include <iostream>
#include <iomanip>

#include <openfluid/waresdev/WareSrcMigratorListener.hpp>
#include <openfluid/tools/Console.hpp>


class SignalMigrationListener : public QObject, public openfluid::waresdev::WareSrcMigratorListener
{
  Q_OBJECT;

  private:

    bool m_Verbose = false;

    bool m_IsFirstStageMsg = true;
    QString m_CurrentMsg = "";

    int m_Progress = 0;
    double m_ProgressRatio = 100/8; //TOIMPL variabilize steps ?

    void flushMsg(std::string channel="info")
    {
      if (channel == "info")
      {
        info(m_CurrentMsg);
      }
      else if (channel == "error")
      {
        error(m_CurrentMsg);
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Channel not recognized");
      }
      m_CurrentMsg = "";
    }

    void incrementProgress()
    {
      m_Progress += m_ProgressRatio;
      emit progressed(m_Progress);
    }


    void displayStatus(const openfluid::base::Listener::Status& Status)
    {
      m_CurrentMsg += " ";

      switch (Status)
      {
        case openfluid::base::Listener::Status::ERROR_STATUS :
          m_CurrentMsg += "[Error]";
          flushMsg("error");
          break;
        case openfluid::base::Listener::Status::OK_STATUS :
          m_CurrentMsg += "[OK]";
          flushMsg("info");
          break;
        case openfluid::base::Listener::Status::WARNING_STATUS :
          m_CurrentMsg += "[Warning]";
          flushMsg("info"); //TOIMPL add warning channel
          break;
      }


    };

    void displayStartMsg(const std::string& Msg)
    {    
      
      m_CurrentMsg += QString::fromStdString("-- " + Msg);
      if (m_Verbose)
      {
        m_CurrentMsg += " :";
      }
      
      m_IsFirstStageMsg = true;
    }

  signals:

    void info(QString Message);

    void warning(QString Message);

    void error(QString Message);

    void progressed(int Value);

    void finished();

  public:

    CPPCLASS_DEFAULT_FIVE(SignalMigrationListener)

    void setVerbose(bool Enabled)
    {
      m_Verbose = Enabled;
    }

    void onPrecheckStart()
    { 
      displayStartMsg("Preliminary checks");
    }

    void onPrecheckEnd(openfluid::base::Listener::Status Status)
    {
      displayStatus(Status);
      incrementProgress();
    }

    void onPrepareStart()
    { 
      displayStartMsg("Preparation");
    }

    void onPrepareEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
      incrementProgress();
    }

    void onProcessSignatureStart()
    {
      displayStartMsg("Processing ware signature");
    }

    void onProcessSignatureEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
      incrementProgress();
    }

    void onProcessCMakeConfigStart()
    { 
      displayStartMsg("Processing CMake configuration");
    }

    void onProcessCMakeConfigEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
    }

    void onCreateStructureStart()
    { 
      displayStartMsg("Creating new ware structure");
    }

    void onCreateStructureEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
      incrementProgress();
    }

    void onDispatchFilesStart()
    {
      displayStartMsg("Dispatching");
    }

    void onDispatchFilesEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
      incrementProgress();
    }

    void onCleanSrcFilesStart()
    { 
      displayStartMsg("Cleaning files");
    }

    void onCleanSrcFilesEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
      incrementProgress();
    }

    void onDocStart()
    { 
      displayStartMsg("Processing doc");
    }

    void onDocEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
      incrementProgress();
    }

    void stageMessage(const std::string& Msg)
    {
      if (m_Verbose)
      {
        if (!m_IsFirstStageMsg)
        {
          m_CurrentMsg += ",";
        }
        m_CurrentMsg += QString::fromStdString(" " + Msg);
      }
      m_IsFirstStageMsg = false;
    }

};


#endif /* __OPENFLUID_UIWARESDEV_SIGNALMIGRATIONLISTENER_HPP__ */
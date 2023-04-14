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
  @file WareMigrationListener.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <iostream>
#include <iomanip>

#include <openfluid/ui/waresdev/WareMigrationListener.hpp>


namespace openfluid { namespace ui { namespace waresdev {


void WareMigrationListener::flushMsg(const std::string& Channel)
{
  if (Channel == "info")
  {
    info(m_CurrentMsg);
  }
  else if (Channel == "warning")
  {
    warning(m_CurrentMsg);
  }
  else if (Channel == "error")
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


// =====================================================================
// =====================================================================


void WareMigrationListener::incrementProgress()
{
  m_Progress += m_ProgressRatio;
  emit progressed(m_Progress);
}


// =====================================================================
// =====================================================================


void WareMigrationListener::displayStatus(const openfluid::base::Listener::Status& Status)
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
      flushMsg("warning");
      break;
  }

};


// =====================================================================
// =====================================================================


void WareMigrationListener::displayStartMsg(const std::string& Msg)
{    
  
  m_CurrentMsg += QString::fromStdString("-- " + std::string(Msg));
  if (m_Verbose)
  {
    m_CurrentMsg += " :";
  }
  
  m_IsFirstStageMsg = true;
}


// =====================================================================
// =====================================================================


void WareMigrationListener::setVerbose(bool Enabled)
{
  m_Verbose = Enabled;
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onPrecheckStart()
{ 
  displayStartMsg("Preliminary checks");
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onPrecheckEnd(openfluid::base::Listener::Status Status)
{
  displayStatus(Status);
  incrementProgress();
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onPrepareStart()
{ 
  displayStartMsg("Preparation");
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onPrepareEnd(openfluid::base::Listener::Status Status)
{ 
  displayStatus(Status);
  incrementProgress();
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onProcessSignatureStart()
{
  displayStartMsg("Processing ware signature");
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onProcessSignatureEnd(openfluid::base::Listener::Status Status)
{ 
  displayStatus(Status);
  incrementProgress();
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onProcessCMakeConfigStart()
{ 
  displayStartMsg("Processing CMake configuration");
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onProcessCMakeConfigEnd(openfluid::base::Listener::Status Status)
{ 
  displayStatus(Status);
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onCreateStructureStart()
{ 
  displayStartMsg("Creating new ware structure");
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onCreateStructureEnd(openfluid::base::Listener::Status Status)
{ 
  displayStatus(Status);
  incrementProgress();
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onDispatchFilesStart()
{
  displayStartMsg("Dispatching");
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onDispatchFilesEnd(openfluid::base::Listener::Status Status)
{ 
  displayStatus(Status);
  incrementProgress();
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onCleanSrcFilesStart()
{ 
  displayStartMsg("Cleaning files");
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onCleanSrcFilesEnd(openfluid::base::Listener::Status Status)
{ 
  displayStatus(Status);
  incrementProgress();
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onDocStart()
{ 
  displayStartMsg("Processing doc");
}


// =====================================================================
// =====================================================================


void WareMigrationListener::onDocEnd(openfluid::base::Listener::Status Status)
{ 
  displayStatus(Status);
  incrementProgress();
}


// =====================================================================
// =====================================================================


void WareMigrationListener::stageMessage(const std::string& Msg)
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


} } }  // namespaces


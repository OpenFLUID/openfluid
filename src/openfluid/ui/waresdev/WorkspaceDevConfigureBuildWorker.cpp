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
  @file WorkspaceDevConfigureBuildWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <QObject>
#include <QProcess>
#include <QMetaType>

#include <openfluid/ui/waresdev/WorkspaceDevConfigureBuildWorker.hpp>
#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/utils/CMakeProxy.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevConfigureBuildWorker::WorkspaceDevConfigureBuildWorker(
    const WorkspaceDevDashboardTypes::WaresSelectionByType& Selection,
    openfluid::waresdev::WareSrcContainer::ConfigMode CMode,
    openfluid::waresdev::WareSrcContainer::BuildMode BMode) :
    WorkspaceDevProcessWorker(Selection), mp_Process(nullptr),
    m_ConfigMode(CMode), m_BuildMode(BMode)
{
  // be careful : anything performed here will be executed from the parent thread!
}


// =====================================================================
// =====================================================================


WorkspaceDevConfigureBuildWorker::~WorkspaceDevConfigureBuildWorker()
{
}


// =====================================================================
// =====================================================================


void WorkspaceDevConfigureBuildWorker::run()
{
  // mp_Process must be created here to be in the same thread
  mp_Process = new QProcess();

  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStdOut()));
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processStdErr()));


  for (auto& WType : m_Selection)
  {
    for (auto& WItem : WType.second)
    {
      openfluid::waresdev::WareSrcContainer Container(WItem.Path,WType.first,WItem.ID);

      Container.setConfigMode(m_ConfigMode);
      Container.setBuildMode(m_BuildMode);

      Container.prepareBuildDirectory();


      QString ConfigCommand = openfluid::utils::CMakeProxy::getConfigureCommand(Container.getBuildDirPath(),
                                                                                Container.getAbsolutePath(),
                                                                                Container.getConfigureVariables(),
                                                                                Container.getConfigureGenerator(),
                                                                                {Container.getConfigureExtraOptions()});

      writeMessage();
      writeMessage("====== "+tr("Configuring %1").arg(WItem.ID)+" ======");
      writeMessage();


      mp_Process->start(ConfigCommand);

      mp_Process->waitForStarted(-1);
      mp_Process->waitForReadyRead(-1);
      mp_Process->waitForFinished(-1);

      if (mp_Process->exitCode())
      {
        emit processed(WType.first,WItem.ID,"configure", false);
      }
      else
      {
        emit processed(WType.first,WItem.ID,"configure", true);

        writeMessage();
        writeMessage("====== "+tr("Building %1").arg(WItem.ID)+" ======");
        writeMessage();

        QString BuildCommand = openfluid::utils::CMakeProxy::getBuildCommand(Container.getBuildDirPath(),
                                                                             "install");

        mp_Process->start(BuildCommand);

        mp_Process->waitForStarted(-1);
        mp_Process->waitForReadyRead(-1);
        mp_Process->waitForFinished(-1);

        if (mp_Process->exitCode())
        {
          emit processed(WType.first,WItem.ID,"build", false);
        }
        else
          emit processed(WType.first,WItem.ID,"build", true);
      }
      emit wareCompleted();
    }
  }


  if (mp_Process->state() != QProcess::NotRunning)
    mp_Process->close();

  emit finished();

  delete mp_Process;
}


// =====================================================================
// =====================================================================


void WorkspaceDevConfigureBuildWorker::processStdOut()
{
  if (!mp_Process)
    return;

  mp_Process->setReadChannel(QProcess::StandardOutput);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine()).simplified();

    writeMessage(MsgLine);
  }
}


// =====================================================================
// =====================================================================


void WorkspaceDevConfigureBuildWorker::processStdErr()
{
  if (!mp_Process)
    return;

  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine()).simplified();

    writeMessage("<font style='color: red;'>"+MsgLine+"</font>");
  }
}


} } }  // namespaces


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
  @file WorkspaceDevBuildWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QObject>
#include <QProcess>
#include <QMetaType>

#include <openfluid/ui/waresdev/WorkspaceDevBuildWorker.hpp>
#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/tools/QtHelpers.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevBuildWorker::WorkspaceDevBuildWorker(
    const WorkspaceDevDashboardTypes::WaresSelectionByType& Selection,
    bool BuildWare, bool GenerateDoc,
    openfluid::waresdev::WareSrcContainer::ConfigMode CMode,
    openfluid::waresdev::WareSrcContainer::BuildMode BMode,
    unsigned int BJobs) :
    WorkspaceDevProcessWorker(Selection), mp_Process(nullptr),
    m_BuildWare(BuildWare), m_GenerateDoc(GenerateDoc),
    m_ConfigMode(CMode), m_BuildMode(BMode), m_BuildJobs(BJobs)
{
  // be careful : anything performed here will be executed from the parent thread!
}


// =====================================================================
// =====================================================================


WorkspaceDevBuildWorker::~WorkspaceDevBuildWorker()
{
}


// =====================================================================
// =====================================================================


void WorkspaceDevBuildWorker::run()
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
      Container.setBuildJobs(m_BuildJobs);

      Container.prepareBuildDirectory();

      QStringList ExtraOptionsList = openfluid::tools::convertArgsStringToList(Container.getConfigureExtraOptions());

      openfluid::utils::CMakeProxy::CommandInfos ConfigCommand = 
        openfluid::utils::CMakeProxy::getConfigureCommand(Container.getBuildDirPath(),
                                                          Container.getAbsolutePath(),
                                                          Container.getConfigureVariables(),
                                                          Container.getConfigureGenerator(),
                                                          ExtraOptionsList);

      writeMessage();
      writeMessage("====== "+tr("Configuring %1").arg(WItem.ID)+" ======");
      writeMessage();


      mp_Process->start(ConfigCommand.Program,ConfigCommand.Args);

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

        if (m_BuildWare)
        {

          writeMessage();
          writeMessage("====== "+tr("Building %1").arg(WItem.ID)+" ======");
          writeMessage();

          openfluid::utils::CMakeProxy::CommandInfos BuildCommand = 
            openfluid::utils::CMakeProxy::getBuildCommand(Container.getBuildDirPath(),
                                                          Container.getBuildTarget(),Container.getBuildJobs());

          mp_Process->start(BuildCommand.Program,BuildCommand.Args);

          mp_Process->waitForStarted(-1);
          mp_Process->waitForReadyRead(-1);
          mp_Process->waitForFinished(-1);

          if (mp_Process->exitCode())
          {
            emit processed(WType.first,WItem.ID,"build", false);
          }
          else
          {
            emit processed(WType.first,WItem.ID,"build", true);
          }
        }

        if (WType.first == openfluid::ware::WareType::SIMULATOR && m_GenerateDoc)
        {

          writeMessage();
          writeMessage("====== "+tr("Generating doc : %1").arg(WItem.ID)+" ======");
          writeMessage();

          openfluid::utils::CMakeProxy::CommandInfos DocCommand = 
            openfluid::utils::CMakeProxy::getBuildCommand(Container.getBuildDirPath(),Container.getGenerateDocTarget());

          mp_Process->start(DocCommand.Program,DocCommand.Args);

          mp_Process->waitForStarted(-1);
          mp_Process->waitForReadyRead(-1);
          mp_Process->waitForFinished(-1);

          if (mp_Process->exitCode())
          {
            emit processed(WType.first,WItem.ID,"doc", false);
          }
          else
          {
            emit processed(WType.first,WItem.ID,"doc", true);
          }
        }
      }
      emit wareCompleted();
    }
  }


  if (mp_Process->state() != QProcess::NotRunning)
  {
    mp_Process->close();
  }

  emit finished();

  delete mp_Process;
}


// =====================================================================
// =====================================================================


void WorkspaceDevBuildWorker::processStdOut()
{
  if (!mp_Process)
  {
    return;
  }

  mp_Process->setReadChannel(QProcess::StandardOutput);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine()).simplified();

    writeMessage(MsgLine);
  }
}


// =====================================================================
// =====================================================================


void WorkspaceDevBuildWorker::processStdErr()
{
  if (!mp_Process)
  {
    return;
  }

  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine()).simplified();

    writeMessage("<font style='color: red;'>"+MsgLine+"</font>");
  }
}


} } }  // namespaces


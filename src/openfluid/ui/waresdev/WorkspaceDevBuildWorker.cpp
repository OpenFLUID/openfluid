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
#include <openfluid/ui/waresdev/WareSrcUIContainer.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/ui/QtHelpers.hpp>


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
      openfluid::ui::waresdev::WareSrcUIContainer Container(WItem.Path,WType.first,WItem.ID);

      Container.setConfigMode(m_ConfigMode);
      Container.setBuildMode(m_BuildMode);
      Container.setBuildJobs(m_BuildJobs);

      Container.prepareBuildDirectory();

      // Removal of warnings
      std::map<std::string,std::string> ConfigVariables = Container.getConfigureVariables();
      const std::string FlagsKey = "CMAKE_CXX_FLAGS";
      if(ConfigVariables.find(FlagsKey) != ConfigVariables.end())
      {
        std::vector<std::string> FlagValues = openfluid::tools::split(ConfigVariables[FlagsKey], " ");
        if(std::find(FlagValues.begin(), FlagValues.end(), "-w") == FlagValues.end())
        {
          FlagValues.push_back("-w");
          ConfigVariables[FlagsKey] = "'" + openfluid::tools::join(FlagValues, " ") + "'";
        }
      }
      else 
      {
        ConfigVariables.insert({FlagsKey, "-w"});
      }

      QStringList ExtraOptionsList = 
        openfluid::ui::convertArgsStringToList(QString::fromStdString(Container.getConfigureExtraOptions()));

      auto ConfigCommand = 
      openfluid::utils::CMakeProxy::getConfigureCommand(Container.getBuildDirPath(),
                                                        Container.getAbsolutePath(),
                                                        ConfigVariables,
                                                        Container.getConfigureGenerator(),
                                                        openfluid::ui::toStdStringVector(ExtraOptionsList));

      writeMessage();
      writeMessage("====== "+tr("Configuring %1").arg(WItem.ID)+" ======");
      writeMessage();

      mp_Process->setWorkingDirectory(QString::fromStdString(Container.getBuildDirPath()));
      mp_Process->start(QString::fromStdString(ConfigCommand.Program),
                        openfluid::ui::toQStringList(ConfigCommand.Args));

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

          openfluid::utils::Process::Command BuildCommand = 
            openfluid::utils::CMakeProxy::getBuildCommand(Container.getBuildDirPath(),
                                                          Container.getBuildTarget(),
                                                          Container.getBuildJobs());

          mp_Process->start(QString::fromStdString(BuildCommand.Program),
                            openfluid::ui::toQStringList(BuildCommand.Args));

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

          openfluid::utils::Process::Command DocCommand = 
            openfluid::utils::CMakeProxy::getBuildCommand(Container.getBuildDirPath(),
                                                          Container.getGenerateDocTarget());

          mp_Process->start(QString::fromStdString(DocCommand.Program),
                            openfluid::ui::toQStringList(DocCommand.Args));

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

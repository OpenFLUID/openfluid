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
  @file WareSrcUIContainer.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#include <QCoreApplication>
#include <QMessageBox> // HACK

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/ui/QtHelpers.hpp>
#include <openfluid/ui/waresdev/WareSrcUIContainer.hpp>
#include <openfluid/ui/waresdev/OStreamMsgStream.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcUIContainer::WareSrcUIContainer(const QString& AbsolutePath, 
                                       openfluid::ware::WareType Type, const QString& WareID):
  QObject(),
  openfluid::waresdev::WareSrcContainer(AbsolutePath.toStdString(),Type, WareID.toStdString()), 
  mp_Process(new WareSrcProcess()),mp_Stream(new OStreamMsgStream()),
  mp_CurrentParser(new WareSrcMsgParserGcc())
{
  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStandardOutput()));
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processErrorOutput()));

  connect(mp_Process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinishedOutput(int)));
}


// =====================================================================
// =====================================================================


WareSrcUIContainer::~WareSrcUIContainer()
{
  if (mp_Process->state() != WareSrcProcess::NotRunning)
  {
    mp_Process->close();
  }

  delete mp_Process;
  delete mp_CurrentParser;
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::processStandardOutput()
{
  mp_Process->setReadChannel(WareSrcProcess::StandardOutput);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine());

    auto Message =
        mp_CurrentParser->parse(MsgLine,WareSrcMsgParser::WareSrcMsg::MessageType::MSG_STANDARD);

    mp_Stream->write(Message);

    if (Message.m_Type != WareSrcMsgParser::WareSrcMsg::MessageType::MSG_STANDARD)
    {
      m_Messages.append(Message);
    }
  }

}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::processErrorOutput()
{
  mp_Process->setReadChannel(WareSrcProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine());

    auto Message =
        mp_CurrentParser->parse(MsgLine,WareSrcMsgParser::WareSrcMsg::MessageType::MSG_WARNING);

    mp_Stream->write(Message);

    m_Messages.append(Message);
  }
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::processFinishedOutput(int ExitCode)
{
  QString ElapsedString =
      QString("execution time : %1").arg(QString::fromStdString(m_ProcessTimer.elapsedAsPrettyString()));

  if (!ExitCode)
  {
    auto Message = WareSrcMsgParser::WareSrcMsg(QString("\nCommand ended (%1)\n\n").arg(ElapsedString),
                                                WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND);
    mp_Stream->write(Message);
  }
  else
  {
    auto Message = 
      WareSrcMsgParser::WareSrcMsg(QString("\nCommand ended with error (%1)\n\n").arg(ElapsedString),
                                   WareSrcMsgParser::WareSrcMsg::MessageType::MSG_ERROR);
    mp_Stream->write(Message);
  }


  emit processFinished();

  if (mp_Process->getType() == WareSrcProcess::Type::CONFIGURE)
  {
    emit configureProcessFinished(m_Type,QString::fromStdString(m_ID));
  }
  else if (mp_Process->getType() == WareSrcProcess::Type::BUILD)
  {
    emit buildProcessFinished(m_Type,QString::fromStdString(m_ID));
  }

  mp_Process->setType(WareSrcProcess::Type::NONE);
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::setMsgStream(WareSrcMsgStream& Stream)
{
  if (mp_Stream)
  {
    delete mp_Stream;
  }
  mp_Stream = &Stream;
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::configure()
{
  if (!openfluid::utils::CMakeProxy::isAvailable())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "CMake program not available");
  }


  mp_Stream->clear();
  m_Messages.clear();


  prepareBuildDirectory();

  delete mp_CurrentParser;
  mp_CurrentParser = new WareSrcMsgParserCMake(QString::fromStdString(getAbsolutePath()));


  // === build and run command

  QStringList ExtraOptionsList = 
      openfluid::ui::convertArgsStringToList(QString::fromStdString(getConfigureExtraOptions()));

  openfluid::utils::Process::Command Cmd = 
    openfluid::utils::CMakeProxy::getConfigureCommand(m_BuildDirPath,getAbsolutePath(),
                                                      getConfigureVariables(),getConfigureGenerator(),
                                                      openfluid::ui::toStdStringVector(ExtraOptionsList));

  runCommand(Cmd, getConfigureEnvironment(), WareSrcProcess::Type::CONFIGURE);
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::build()
{
  if (!openfluid::utils::CMakeProxy::isAvailable())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "CMake program not available");
  }


  mp_Stream->clear();
  m_Messages.clear();


  // run configure if build dir does not exist
  if (!openfluid::tools::FilesystemPath(m_BuildDirPath).exists())
  {
    configure();

    while (!mp_Process->waitForFinished(200))  // TODO better to replace this by a threaded process
    {
      qApp->processEvents();
    }

    mp_Stream->write(
      WareSrcMsgParser::WareSrcMsg(
        "\n================================================================================\n\n\n",
        WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND
      )
    );
  }

  delete mp_CurrentParser;
  mp_CurrentParser = new WareSrcMsgParserGcc();


  // === build and run command

  openfluid::utils::Process::Command Cmd =
    openfluid::utils::CMakeProxy::getBuildCommand(m_BuildDirPath,getBuildTarget(),m_BuildJobs);

  runCommand(Cmd, getBuildEnvironment(), WareSrcProcess::Type::BUILD);
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::generateDoc()
{
  // TOIMPL
  QMessageBox::critical(nullptr,"Generate doc","not implemented",QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::runCommand(const openfluid::utils::Process::Command& Cmd, 
                                    const QProcessEnvironment& Env, WareSrcProcess::Type CmdType)
{
  if (mp_Process->state() != WareSrcProcess::NotRunning)
  {
    mp_Process->close();
  }

  m_ProcessTimer.restart();

  mp_Process->setType(CmdType);

  emit processLaunched();

  if (CmdType == WareSrcProcess::Type::CONFIGURE)
  {
    emit configureProcessLaunched(m_Type,QString::fromStdString(m_ID));
  }
  else if (CmdType == WareSrcProcess::Type::BUILD)
  {
    emit buildProcessLaunched(m_Type,QString::fromStdString(m_ID));
  }

  if (openfluid::base::PreferencesManager::instance()->isWaresdevShowCommandEnv("PATH"))
  {
    auto PATHMessage =
        WareSrcMsgParser::WareSrcMsg(
          QString("PATH=%1\n").arg(Env.value("PATH", "")),
          WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND);
    mp_Stream->write(PATHMessage);
  }

  auto CommandMessage = WareSrcMsgParser::WareSrcMsg(QString::fromStdString(Cmd.joined()+"\n"),
                                                     WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND);
  mp_Stream->write(CommandMessage);

  mp_Process->setProcessEnvironment(Env);
  mp_Process->start(QString::fromStdString(Cmd.Program),openfluid::ui::toQStringList(Cmd.Args));
}


// =====================================================================
// =====================================================================


QProcessEnvironment WareSrcUIContainer::getConfigureEnvironment() const
{
  // === set process environment
  QProcessEnvironment Env = QProcessEnvironment::systemEnvironment();

  // Set PATH env. var. if configured
  QString CustomPath = 
    QString::fromStdString(openfluid::base::PreferencesManager::instance()->getWaresdevConfigureEnv("PATH"));
  if (!CustomPath.isEmpty())
  {
    QByteArray ExistingPath = qgetenv("PATH");
    if (!ExistingPath.isNull())
    {
      CustomPath.replace("%%PATH%%", ExistingPath);
      Env.insert("PATH", CustomPath);
    }
  }

  return Env;
}


// =====================================================================
// =====================================================================


QProcessEnvironment WareSrcUIContainer::getBuildEnvironment() const
{
  QProcessEnvironment RunEnv = QProcessEnvironment::systemEnvironment();

  // Set PATH env. var. if configured
  QString CustomPath = 
    QString::fromStdString(openfluid::base::PreferencesManager::instance()->getWaresdevBuildEnv("PATH"));
  if (!CustomPath.isEmpty())
  {
    QByteArray ExistingPath = qgetenv("PATH");
    if (!ExistingPath.isNull())
    {
      CustomPath.replace("%%PATH%%", ExistingPath);
      RunEnv.insert("PATH", CustomPath);
    }
  }

  return RunEnv;
}


// =====================================================================
// =====================================================================


QList<WareSrcMsgParser::WareSrcMsg> WareSrcUIContainer::getMessages()
{
  return m_Messages;
}


// =====================================================================
// =====================================================================


bool WareSrcUIContainer::isProcessRunning() const
{
  return (mp_Process->state() != WareSrcProcess::NotRunning);
}


} } }  // namespaces


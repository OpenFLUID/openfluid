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
 @file WareSrcContainer.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/waresdev/WareSrcContainer.hpp>

#include <QDir>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/config.hpp>

#include <openfluid/waresdev/WareSrcMsgParser.hpp>
#include <openfluid/waresdev/OStreamMsgStream.hpp>


namespace openfluid { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcContainer::WareSrcContainer(const QString& AbsolutePath, openfluid::ware::WareType Type,
                                   const QString& WareName) :
    QObject(), m_AbsolutePath(AbsolutePath), m_Type(Type), m_Name(WareName), m_AbsoluteCMakeConfigPath(""),
        m_AbsoluteMainCppPath(""), m_AbsoluteUiParamCppPath(""), m_AbsoluteCMakeListsPath(""), m_AbsoluteJsonPath(""),
        m_CMakeProgramPath(""), mp_Stream(new openfluid::waresdev::OStreamMsgStream()), mp_Process(new QProcess()),
        mp_CurrentParser(new openfluid::waresdev::WareSrcMsgParserGcc())
{
  update();

  m_OFVersion = QString::fromStdString(openfluid::base::RuntimeEnvironment::instance()->getMajorMinorVersion());

  setConfigMode(CONFIG_RELEASE);
  setBuildMode(BUILD_WITHINSTALL);

  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStandardOutput()));
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processErrorOutput()));

  connect(mp_Process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinishedOutput(int)));
}


// =====================================================================
// =====================================================================


WareSrcContainer::~WareSrcContainer()
{
  if (mp_Process->state() != QProcess::NotRunning)
    mp_Process->close();

  delete mp_Process;
  delete mp_CurrentParser;
}


// =====================================================================
// =====================================================================


void WareSrcContainer::update()
{
  m_AbsoluteCMakeListsPath = m_AbsoluteJsonPath = m_AbsoluteCMakeConfigPath = m_AbsoluteMainCppPath =
      m_AbsoluteUiParamCppPath = "";

  QDir Dir(m_AbsolutePath);

  QString CMakeListsFilePath = Dir.absoluteFilePath("CMakeLists.txt");
  if (QFile::exists(CMakeListsFilePath))
    m_AbsoluteCMakeListsPath = CMakeListsFilePath;

  QString JsonFilePath = Dir.absoluteFilePath("wareshub.json");
  if (QFile::exists(JsonFilePath))
    m_AbsoluteJsonPath = JsonFilePath;

  QString CMakeConfigFilePath = Dir.absoluteFilePath(
      QString::fromStdString(openfluid::config::WARESDEV_CMAKE_USERFILE));

  if (QFile::exists(CMakeConfigFilePath))
  {
    m_AbsoluteCMakeConfigPath = CMakeConfigFilePath;

    QFile File(m_AbsoluteCMakeConfigPath);
    if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
      throw openfluid::base::FrameworkException(
          OPENFLUID_CODE_LOCATION, QString("Cannot open file %1").arg(m_AbsoluteCMakeConfigPath).toStdString());

    QByteArray CMakeFileContent = File.readAll();

    QString MainCppFilename = searchMainCppFileName(CMakeFileContent);
    if (!MainCppFilename.isEmpty())
    {
      QString MainCppFilePath = Dir.absoluteFilePath(MainCppFilename);

      if (QFile::exists(MainCppFilePath))
        m_AbsoluteMainCppPath = MainCppFilePath;
    }

    QString UiParamCppFilename = searchUiParamCppFileName(CMakeFileContent);
    if (!UiParamCppFilename.isEmpty())
    {
      QString UiParamCppFilePath = Dir.absoluteFilePath(UiParamCppFilename);

      if (QFile::exists(UiParamCppFilePath))
        m_AbsoluteUiParamCppPath = UiParamCppFilePath;
    }

  }
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::searchMainCppFileName(const QString& CMakeFileContent)
{
  QStringList Lines = CMakeFileContent.split('\n');

  QRegExp RE(
      QString("^\\s*SET\\s*\\((?:%1|%2|%3)\\s+([\\w_.-]+\\.cpp)").arg(
          QString::fromStdString(openfluid::config::WARESDEV_CMAKE_SIMCPPVAR)).arg(
          QString::fromStdString(openfluid::config::WARESDEV_CMAKE_OBSCPPVAR)).arg(
          QString::fromStdString(openfluid::config::WARESDEV_CMAKE_BEXTCPPVAR)));

  for (const QString& L : Lines)
  {
    if (RE.indexIn(L) > -1)
      return RE.cap(1);
  }

  return "";
}

// =====================================================================
// =====================================================================


QString WareSrcContainer::searchUiParamCppFileName(const QString& CMakeFileContent)
{
  QStringList Lines = CMakeFileContent.split('\n');

  QRegExp RE("^\\s*SET\\s*\\((?:SIM_PARAMSUI_CPP|OBS_PARAMSUI_CPP)\\s+([\\w_.-]+\\.cpp)");

  for (const QString& L : Lines)
  {
    if (RE.indexIn(L) > -1)
      return RE.cap(1);
  }

  return "";
}


// =====================================================================
// =====================================================================


QStringList WareSrcContainer::getDefaultFilesPaths()
{
  QStringList L;

  if (!m_AbsoluteCMakeConfigPath.isEmpty())
    L << m_AbsoluteCMakeConfigPath;

  if (!m_AbsoluteMainCppPath.isEmpty())
    L << m_AbsoluteMainCppPath;
  else
  {
    QDir Dir(m_AbsolutePath);

    QStringList NameFilters;
    NameFilters << "*.cpp";

    QString FirstCpp = Dir.entryList(NameFilters, QDir::Files, QDir::Name).value(0, "");

    if (!FirstCpp.isEmpty())
      L << Dir.absoluteFilePath(FirstCpp);
  }

  return L;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::getMainCppPath() const
{
  return m_AbsoluteMainCppPath;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::getUiParamCppPath() const
{
  return m_AbsoluteUiParamCppPath;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::getAbsolutePath() const
{
  return m_AbsolutePath;
}


// =====================================================================
// =====================================================================


openfluid::ware::WareType WareSrcContainer::getType() const
{
  return m_Type;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::getName() const
{
  return m_Name;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::getCMakeConfigPath() const
{
  return m_AbsoluteCMakeConfigPath;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::getCMakeListsPath() const
{
  return m_AbsoluteCMakeListsPath;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::getJsonPath() const
{
  return m_AbsoluteJsonPath;
}


// =====================================================================
// =====================================================================


void WareSrcContainer::setMsgStream(openfluid::waresdev::WareSrcMsgStream& Stream)
{
  if (mp_Stream)
    delete mp_Stream;
  mp_Stream = &Stream;
}


// =====================================================================
// =====================================================================


void WareSrcContainer::setConfigMode(ConfigMode Mode)
{
  m_ConfigMode = Mode;

  QString ConfigTag;
  switch (m_ConfigMode)
  {
    case CONFIG_RELEASE:
      ConfigTag = "-release";
      break;
    case CONFIG_DEBUG:
      ConfigTag = "-debug";
      break;
    default:
      ConfigTag = "";
      break;
  }

  m_BuildDirPath = QDir(m_AbsolutePath).filePath(QString("_build%1-%2").arg(ConfigTag).arg(m_OFVersion));
}


// =====================================================================
// =====================================================================


void WareSrcContainer::setBuildMode(BuildMode Mode)
{
  m_BuildMode = Mode;
}


// =====================================================================
// =====================================================================


void WareSrcContainer::findCMake()
{
  openfluid::utils::ExternalProgram CMakeProg = openfluid::utils::ExternalProgram::getRegisteredProgram(
      openfluid::utils::ExternalProgram::CMakeProgram);

  if (!CMakeProg.isFound())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "unable to find CMake program");

  m_CMakeProgramPath = CMakeProg.getFullProgramPath();
}


// =====================================================================
// =====================================================================


void WareSrcContainer::configure()
{
  if (m_CMakeProgramPath.isEmpty())
    findCMake();

  mp_Stream->clear();
  m_Messages.clear();

  QFile BuildDir(m_BuildDirPath);
  if (BuildDir.exists())
    openfluid::tools::emptyDirectoryRecursively(QString(m_BuildDirPath).toStdString());
  else if (!QDir().mkpath(m_BuildDirPath))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "unable to create build directory");

  QString Options = QString(" -DCMAKE_BUILD_TYPE=%1").arg(m_ConfigMode == CONFIG_RELEASE ? "Release" : "Debug");


  // Use OPENFLUID_INSTALL_PREFIX env. var. if defined
  QByteArray OpenFLUIDInstallPrefix = qgetenv("OPENFLUID_INSTALL_PREFIX");

  if (!OpenFLUIDInstallPrefix.isNull())
  {
    Options.prepend(" -DCMAKE_PREFIX_PATH=" + OpenFLUIDInstallPrefix + "/lib/cmake");
  }


  // Use specific options if configured
  QString ExtraOptions = openfluid::base::PreferencesManager::instance()->getWaresdevConfigOptions();
  if (!Options.isEmpty())
  {
    ExtraOptions = " " + ExtraOptions;
    Options.prepend(ExtraOptions);
  }


  // Use specific generator if configured
  QString Generator = openfluid::base::PreferencesManager::instance()->getWaresdevConfigGenerator();
  if (!Generator.isEmpty())
  {
    QString GeneratorOption = QString(" -G \"%1\"").arg(Generator);
    Options.prepend(GeneratorOption);
  }


  QProcessEnvironment RunEnv = QProcessEnvironment::systemEnvironment();

  // Set PATH env. var. if configured
  QString CustomPath = openfluid::base::PreferencesManager::instance()->getWaresdevConfigEnv("PATH");
  if (!CustomPath.isEmpty())
  {
    QByteArray ExistingPath = qgetenv("PATH");
    if (!ExistingPath.isNull())
    {
      CustomPath.replace("%%PATH%%", ExistingPath);
      RunEnv.insert("PATH", CustomPath);
    }
  }


  delete mp_CurrentParser;
  mp_CurrentParser = new openfluid::waresdev::WareSrcMsgParserCMake(m_AbsolutePath);


  QString Command = QString("\"%1\" -E chdir \"%2\" \"%1\" \"%3\" %4")
		                .arg(m_CMakeProgramPath)
		                .arg(m_BuildDirPath)
		                .arg(m_AbsolutePath)
		                .arg(Options);

  runCommand(Command, RunEnv);
}


// =====================================================================
// =====================================================================


void WareSrcContainer::build()
{
  if (m_CMakeProgramPath.isEmpty())
    findCMake();

  mp_Stream->clear();
  m_Messages.clear();

  if (!QFile(m_BuildDirPath).exists())
  {
    configure();
    mp_Process->waitForFinished(-1);
  }


  QProcessEnvironment RunEnv = QProcessEnvironment::systemEnvironment();

  // Set PATH env. var. if configured
  QString CustomPath = openfluid::base::PreferencesManager::instance()->getWaresdevBuildEnv("PATH");
  if (!CustomPath.isEmpty())
  {
    QByteArray ExistingPath = qgetenv("PATH");
    if (!ExistingPath.isNull())
    {
      CustomPath.replace("%%PATH%%", ExistingPath);
      RunEnv.insert("PATH", CustomPath);
    }
  }


  delete mp_CurrentParser;
  mp_CurrentParser = new openfluid::waresdev::WareSrcMsgParserGcc();


  QString Command = QString("\"%1\" -E chdir \"%2\" \"%1\" --build . %3")
		                .arg(m_CMakeProgramPath)
		                .arg(m_BuildDirPath)
		                .arg(m_BuildMode == BUILD_WITHINSTALL ? "--target install" : "");

  runCommand(Command, RunEnv);
}


// =====================================================================
// =====================================================================


void WareSrcContainer::processStandardOutput()
{
  mp_Process->setReadChannel(QProcess::StandardOutput);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine());

    WareSrcMsgParser::WareSrcMsg Message = mp_CurrentParser->parse(MsgLine,
                                                                   WareSrcMsgParser::WareSrcMsg::MSG_STANDARD);

    mp_Stream->write(Message);

    if (Message.m_Type != WareSrcMsgParser::WareSrcMsg::MSG_STANDARD)
      m_Messages.append(Message);
  }

}


// =====================================================================
// =====================================================================


void WareSrcContainer::processErrorOutput()
{
  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine());

    WareSrcMsgParser::WareSrcMsg Message = mp_CurrentParser->parse(MsgLine,
                                                                   WareSrcMsgParser::WareSrcMsg::MSG_WARNING);

    mp_Stream->write(Message);

    m_Messages.append(Message);
  }
}


// =====================================================================
// =====================================================================


void WareSrcContainer::processFinishedOutput(int ExitCode)
{
  if (!ExitCode)
  {
    WareSrcMsgParser::WareSrcMsg Message = WareSrcMsgParser::WareSrcMsg("\nCommand ended\n\n",
                                                                        WareSrcMsgParser::WareSrcMsg::MSG_COMMAND);
    mp_Stream->write(Message);
  }
  else
  {
    WareSrcMsgParser::WareSrcMsg Message = WareSrcMsgParser::WareSrcMsg("\nCommand ended with error\n\n",
                                                                        WareSrcMsgParser::WareSrcMsg::MSG_ERROR);
    mp_Stream->write(Message);
  }

  emit processFinished();
}


// =====================================================================
// =====================================================================


void WareSrcContainer::runCommand(const QString& Command, const QProcessEnvironment& Env)
{
  if (mp_Process->state() != QProcess::NotRunning)
    mp_Process->close();

  if (openfluid::base::PreferencesManager::instance()->isWaresdevShowCommandEnv("PATH"))
  {
    WareSrcMsgParser::WareSrcMsg PATHMessage = WareSrcMsgParser::WareSrcMsg(
        QString("PATH=%1\n").arg(Env.value("PATH", "")), WareSrcMsgParser::WareSrcMsg::MSG_COMMAND);
    mp_Stream->write(PATHMessage);
  }

  WareSrcMsgParser::WareSrcMsg CommandMessage = WareSrcMsgParser::WareSrcMsg(
      QString("%1\n").arg(Command), WareSrcMsgParser::WareSrcMsg::MSG_COMMAND);
  mp_Stream->write(CommandMessage);

  mp_Process->setProcessEnvironment(Env);
  mp_Process->start(Command);
}


// =====================================================================
// =====================================================================


QList<WareSrcMsgParser::WareSrcMsg> WareSrcContainer::getMessages()
{
  return m_Messages;
}


// =====================================================================
// =====================================================================


} } // namespaces

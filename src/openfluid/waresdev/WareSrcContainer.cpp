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
#include <QProcess>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/config.hpp>

namespace openfluid { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcContainer::WareSrcContainer(const QString& AbsolutePath, WareSrcManager::WareType Type, const QString& WareName,
                                   openfluid::waresdev::WareSrcMsgStream& Stream) :
    QObject(), m_AbsolutePath(AbsolutePath), m_AbsoluteCMakeConfigPath(""), m_AbsoluteMainCppPath(""), m_CMakePath(""),
    mp_Stream(&Stream), mp_Process(new QProcess())
{
  QDir Dir(AbsolutePath);

  QString CMakeFilePath = Dir.absoluteFilePath(QString::fromStdString(openfluid::config::WARESDEV_CMAKE_USERFILE));

  if (QFile::exists(CMakeFilePath))
  {
    m_AbsoluteCMakeConfigPath = CMakeFilePath;

    QFile File(m_AbsoluteCMakeConfigPath);
    if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
      throw openfluid::base::FrameworkException(
          "WareSrcContainer constructor", QString("Cannot open file %1").arg(m_AbsoluteCMakeConfigPath).toStdString());

    QString MainCppFilename = searchMainCppFileName(File.readAll());

    if (!MainCppFilename.isEmpty())
    {
      QString MainCppFilePath = Dir.absoluteFilePath(MainCppFilename);

      if (QFile::exists(MainCppFilePath))
        m_AbsoluteMainCppPath = MainCppFilePath;
    }
  }

  m_OFVersion = QString::fromStdString(openfluid::base::RuntimeEnvironment::instance()->getMajorMinorVersion());

  setConfigMode(CONFIG_RELEASE);
  setBuildMode(BUILD_WITHINSTALL);

  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processOutput()));
  connect(mp_Process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinishedOutput(int)));
}


// =====================================================================
// =====================================================================


WareSrcContainer::~WareSrcContainer()
{
  if (mp_Process->state() != QProcess::NotRunning)
    mp_Process->close();

  delete mp_Process;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::searchMainCppFileName(const QString& CMakeFileContent)
{
  QStringList Lines = CMakeFileContent.split('\n');

  QRegExp RE(
      QString("^\\s*SET\\s*\\((?:%1|%2|%3)\\s+(\\w+\\.cpp).*\\).*").arg(
          QString::fromStdString(openfluid::config::WARESDEV_CMAKE_SIMCPPVAR)).arg(
          QString::fromStdString(openfluid::config::WARESDEV_CMAKE_OBSCPPVAR)).arg(
          QString::fromStdString(openfluid::config::WARESDEV_CMAKE_BEXTCPPVAR)));

  foreach(QString L,Lines){ if (RE.indexIn(L) > -1)
  return RE.cap(1);
}

  return "";
}


// =====================================================================
// =====================================================================


QStringList WareSrcContainer::getDefaultFiles()
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


QString WareSrcContainer::getMainCppFile()
{
  return m_AbsoluteMainCppPath;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::getAbsolutePath() const
{
  return m_AbsolutePath;
}


// =====================================================================
// =====================================================================


void WareSrcContainer::setMsgStream(openfluid::waresdev::WareSrcMsgStream& Stream)
{
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
    throw openfluid::base::FrameworkException("WareSrcContainer::findCMake", "unable to find CMake program");

  m_CMakePath = CMakeProg.getFullProgramPath();
}


// =====================================================================
// =====================================================================


void WareSrcContainer::configure()
{
  if (m_CMakePath.isEmpty())
    findCMake();

  mp_Stream->clear();

  QFile BuildDir(m_BuildDirPath);
  if (BuildDir.exists())
    openfluid::tools::emptyDirectoryRecursively(QString(m_BuildDirPath).toStdString());
  else if (!QDir().mkpath(m_BuildDirPath))
    throw openfluid::base::FrameworkException("WareSrcContainer::configure", "unable to create build directory");

  QString Options = QString(" -DCMAKE_BUILD_TYPE=%1").arg(m_ConfigMode == CONFIG_RELEASE ? "Release" : "Debug");

#ifdef Q_OS_WIN32
  Options.prepend(" -G \"MinGW Makefiles\"");
#endif

  QString Command = QString("%1 -E chdir %2 %1 %3 %4").arg(m_CMakePath).arg(m_BuildDirPath).arg(m_AbsolutePath).arg(
      Options);

  runCommand(Command);
}


// =====================================================================
// =====================================================================


void WareSrcContainer::build()
{
  if (m_CMakePath.isEmpty())
    findCMake();

  mp_Stream->clear();

  if (!QFile(m_BuildDirPath).exists())
  {
    configure();
    mp_Process->waitForFinished(-1);
  }

  QString Command = QString("%1 -E chdir %2 %1 --build . %3").arg(m_CMakePath).arg(m_BuildDirPath).arg(
      m_BuildMode == BUILD_WITHINSTALL ? "--target install" : "");

  runCommand(Command);
}


// =====================================================================
// =====================================================================


void WareSrcContainer::processOutput()
{
//TODO fix accentuation pb, qprintable not sufficient
  mp_Stream->write(qPrintable(mp_Process->readAllStandardOutput()),
                   openfluid::waresdev::WareSrcMsgStream::MSG_STANDARD);
  mp_Stream->write(qPrintable(mp_Process->readAllStandardError()), openfluid::waresdev::WareSrcMsgStream::MSG_ERROR);
}


// =====================================================================
// =====================================================================


void WareSrcContainer::processFinishedOutput(int ExitCode)
{
  if (!ExitCode)
    mp_Stream->write(tr("Command ended\n\n"), openfluid::waresdev::WareSrcMsgStream::MSG_COMMAND);
  else
    mp_Stream->write(tr("Command ended with error\n\n"), openfluid::waresdev::WareSrcMsgStream::MSG_ERROR);
}


// =====================================================================
// =====================================================================


void WareSrcContainer::runCommand(const QString& Command)
{
  if (mp_Process->state() != QProcess::NotRunning)
    mp_Process->close();

  mp_Stream->write(QString("%1\n").arg(Command), openfluid::waresdev::WareSrcMsgStream::MSG_COMMAND);

  mp_Process->start(Command);
}


// =====================================================================
// =====================================================================


} }  // namespaces

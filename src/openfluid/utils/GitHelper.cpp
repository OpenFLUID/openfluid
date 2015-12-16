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
 @file GitHelper.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QDir>
#include <QTextStream>
#include <openfluid/utils/GitHelper.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/utils/ExternalProgram.hpp>

namespace openfluid { namespace utils {

QString GitHelper::m_GitPgm = "";

// =====================================================================
// =====================================================================


GitHelper::GitHelper()
{
  checkGitProgram();

  m_TmpPath = QString::fromStdString(openfluid::base::Environment::getTempDir());
}


// =====================================================================
// =====================================================================


GitHelper::~GitHelper()
{
  delete mp_Process;

  if (!m_AskPassFile.fileName().isEmpty())
    m_AskPassFile.remove();
}



// =====================================================================
// =====================================================================


bool GitHelper::checkGitProgram()
{
  if (m_GitPgm.isEmpty())
    m_GitPgm = ExternalProgram::getRegisteredProgram(ExternalProgram::GitProgram).getFullProgramPath();

  return (!m_GitPgm.isEmpty());
}


// =====================================================================
// =====================================================================


std::string GitHelper::getOpenfluidCurrentBranchName()
{
  return "openfluid-" + openfluid::base::Environment::getVersionMajorMinor();
}


// =====================================================================
// =====================================================================


void GitHelper::processStandardOutput()
{
  mp_Process->setReadChannel(QProcess::StandardOutput);

  while (mp_Process->canReadLine())
    emit info(QString::fromUtf8(mp_Process->readLine()));
}


// =====================================================================
// =====================================================================


void GitHelper::processErrorOutput()
{
  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
    emit error(QString::fromUtf8(mp_Process->readLine()));
}


// =====================================================================
// =====================================================================


void GitHelper::processErrorOutputAsInfo()
{
  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
    emit info(QString::fromUtf8(mp_Process->readLine()));
}


// =====================================================================
// =====================================================================


bool GitHelper::clone(const QString& FromUrl, const QString& ToPath, const QString& Username, const QString& Password,
  bool SslNoVerify)
{
  QString Url = FromUrl;
  QString Options = "";

  if (!Username.isEmpty() && !Url.contains("@"))
  {
    Url.replace("http://", QString("http://%1@").arg(Username));
    Url.replace("https://", QString("https://%1@").arg(Username));
  }

  if (!Password.isEmpty())
  {
    QString AskPassFileName;
    QString AskPassContent;

#if defined(OPENFLUID_OS_WINDOWS)
    AskPassFileName = "askpass.bat";
    AskPassContent = QString("@ echo off\necho %1\n").arg(Password);
#else
    AskPassFileName = "askpass.sh";
    AskPassContent = QString("#! /bin/sh\necho '%1'\n").arg(Password);
#endif

    QString AskPassFilePath = QDir(m_TmpPath).absoluteFilePath(AskPassFileName);

    m_AskPassFile.setFileName(AskPassFilePath);

    QDir TmpDir(m_TmpPath);
    if (!TmpDir.exists() && !TmpDir.mkpath(m_TmpPath))
    {
      emit error(tr("Unable to create temporary askpass directory"));
      qDebug("Unable to create temporary askpass directory");
      return false;
    }

    if (!m_AskPassFile.open(QFile::ReadWrite))
    {
      emit error(tr("Unable to create temporary askpass file"));
      qDebug("Unable to create temporary askpass file");
      return false;
    }

    QTextStream out(&m_AskPassFile);
    out << AskPassContent;

    m_AskPassFile.close();

    m_AskPassFile.setPermissions(QFile::ReadOwner | QFile::ExeOwner);

    Options = QString("-c core.askPass=\"%1\"").arg(AskPassFilePath);
  }

  if (SslNoVerify)
    Options += " -c http.sslverify=false";

  QString Cmd = QString("\"%1\" clone --progress %2 %3 %4").arg(m_GitPgm).arg(Options).arg(Url).arg(ToPath);

  mp_Process = new QProcess();

  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStandardOutput()));
  // git clone outputs all messages in error channel
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processErrorOutputAsInfo()));

  mp_Process->start(Cmd);

  mp_Process->waitForFinished(-1);
  mp_Process->waitForReadyRead(-1);

  int ErrCode = mp_Process->exitCode();

  delete mp_Process;
  mp_Process = nullptr;

  if (!m_AskPassFile.fileName().isEmpty())
    m_AskPassFile.remove();

  return (ErrCode == 0);
}


// =====================================================================
// =====================================================================


GitHelper::TreeStatusInfo GitHelper::status(const QString& Path)
{
  TreeStatusInfo TreeStatus;
  QDir PathDir(Path);

  if (!PathDir.exists(".git"))
    return TreeStatus;

  TreeStatus.m_IsGitTracked = true;

  mp_Process = new QProcess();
  mp_Process->setWorkingDirectory(Path);

  QString Cmd = QString("\"%1\" status --porcelain --ignored -b").arg(m_GitPgm);

  mp_Process->start(Cmd);

  mp_Process->waitForReadyRead(-1);
  mp_Process->waitForFinished(-1);

  QString Out = mp_Process->readAll();

  TreeStatus.m_BranchName = Out.section('\n', 0, 0).section(' ', 1).section("...", 0, 0);

  QStringList PathLines = Out.section('\n', 1).split('\n', QString::SkipEmptyParts);
  for (const QString& PathLine : PathLines)
  {
    QChar IndexLetter = PathLine.at(0);
    QChar WorkTreeLetter = PathLine.at(1);

    QString FilePath = PathLine;
    FilePath.remove(0, 3);

    FileStatusInfo FilePathStatus;

    if ((IndexLetter == 'D' && WorkTreeLetter == 'D') || (IndexLetter == 'A' && WorkTreeLetter == 'A')
        || IndexLetter == 'U'
        || WorkTreeLetter == 'U')
      FilePathStatus.m_IndexStatus = FileStatus::CONFLICT;
    else if (IndexLetter == '?')
      FilePathStatus.m_IndexStatus = FileStatus::UNTRACKED;
    else if (IndexLetter == '!')
      FilePathStatus.m_IndexStatus = FileStatus::IGNORED;
    else
    {
      if (WorkTreeLetter == 'M')
        FilePathStatus.m_IsDirty = true;

      if (IndexLetter == 'M')
        FilePathStatus.m_IndexStatus = FileStatus::MODIFIED;
      if (IndexLetter == 'A')
        FilePathStatus.m_IndexStatus = FileStatus::ADDED;
      // Deleted takes precedence over Added
      if (IndexLetter == 'D' || WorkTreeLetter == 'D')
        FilePathStatus.m_IndexStatus = FileStatus::DELETED;
    }

    TreeStatus.m_FileStatusByTreePath[FilePath] = FilePathStatus;
  }

  delete mp_Process;
  mp_Process = nullptr;

  return TreeStatus;
}


// =====================================================================
// =====================================================================


} } // namespaces

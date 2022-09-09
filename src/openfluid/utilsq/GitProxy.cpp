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
 @file GitProxy.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <QDir>
#include <QTextStream>

#include <openfluid/utilsq/GitProxy.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/utilsq/ExternalProgram.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace utils {


GitProxy::GitProxy()
{
  findGitProgram();

  // TODO use a subdir in tmp dir given by openfluid::tools::Filesystem::makeUniqueSubdirectory()
  m_TmpPath = QString::fromStdString(openfluid::base::Environment::getTempDir());
}


// =====================================================================
// =====================================================================


GitProxy::~GitProxy()
{
  delete mp_Process;

  if (!m_AskPassFile.fileName().isEmpty())
  {
    m_AskPassFile.remove();
  }
}


// =====================================================================
// =====================================================================


void GitProxy::findGitProgram()
{
  if (m_ExecutablePath.isEmpty())
  {
    m_ExecutablePath = ExternalProgram::getRegisteredProgram(
      ExternalProgram::RegisteredPrograms::Git).getFullProgramPath();

    if (!m_ExecutablePath.isEmpty())
    {
      QProcess Pcs;

      Pcs.start(m_ExecutablePath,{"--version"});

      Pcs.waitForReadyRead(-1);
      Pcs.waitForFinished(-1);

      QString Res = QString::fromUtf8(Pcs.readAll());

      if (Res.startsWith("git version "))
      {
        Res.remove(0,12);
        m_Version = Res;
      }
    }
  }
}


// =====================================================================
// =====================================================================


bool GitProxy::isAvailable()
{
  findGitProgram();

  return (!m_ExecutablePath.isEmpty() && !m_Version.isEmpty());
}


// =====================================================================
// =====================================================================


QString GitProxy::getCurrentOpenFLUIDBranchName()
{
  return "openfluid-" + QString::fromStdString(openfluid::base::Environment::getVersionMajorMinor());
}


// =====================================================================
// =====================================================================


void GitProxy::processStandardOutput()
{
  mp_Process->setReadChannel(QProcess::StandardOutput);

  while (mp_Process->canReadLine())
  {
    emit info(QString::fromUtf8(mp_Process->readLine()));
  }
}


// =====================================================================
// =====================================================================


void GitProxy::processErrorOutput()
{
  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    QString Msg = QString::fromUtf8(mp_Process->readLine());
    emit error(Msg);
  }
}


// =====================================================================
// =====================================================================


void GitProxy::processErrorOutputAsInfo()
{
  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    QString Msg = QString::fromUtf8(mp_Process->readLine());
    emit info(Msg);
  }
}


// =====================================================================
// =====================================================================


bool GitProxy::clone(const QString& FromUrl, const QString& ToPath,
                      const QString& Username, const QString& Password,
                      bool SslNoVerify)
{
  if (FromUrl.isEmpty() || ToPath.isEmpty())
  {
    emit error(tr("Empty remote url or empty destination path"));
    return false;
  }

  QString Url = FromUrl;
  QStringList Options;

  QProcessEnvironment PcsEnv = QProcessEnvironment::systemEnvironment();

  if (!Username.isEmpty() && !Url.contains("@"))
  {
    Url.replace("http://", QString("http://%1@").arg(Username));
    Url.replace("https://", QString("https://%1@").arg(Username));
  }

  if (!Password.isEmpty())
  {
    PcsEnv.insert("GIT_ASKPASS",QString::fromStdString(openfluid::config::GITASKPASS_APP));
    PcsEnv.insert(OFBUILD_GITASKPASS_ENVVAR,Password);
  }

  if (SslNoVerify)
  {
    Options.append("-c");
    Options.append("http.sslverify=false");
  }


  QStringList Args = {"clone","--progress"};
  Args.append(Options);
  Args.append(Url);
  Args.append(ToPath);

  mp_Process = new QProcess();
  mp_Process->setProcessEnvironment(PcsEnv);

  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStandardOutput()));
  // git clone outputs all messages in error channel
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processErrorOutputAsInfo()));

  mp_Process->start(m_ExecutablePath,Args);

  mp_Process->waitForFinished(-1);
  mp_Process->waitForReadyRead(-1);

  int ErrCode = mp_Process->exitCode();

  delete mp_Process;
  mp_Process = nullptr;

  return (ErrCode == 0);
}


// =====================================================================
// =====================================================================


GitProxy::TreeStatusInfo GitProxy::status(const QString& Path)
{
  TreeStatusInfo TreeStatus;
  QDir PathDir(Path);

  if (!PathDir.exists(".git"))
  {
    return TreeStatus;
  }

  TreeStatus.m_IsGitTracked = true;

  mp_Process = new QProcess();
  mp_Process->setWorkingDirectory(Path);

  QStringList Args = {"status","--porcelain","--ignored","-b"};

  mp_Process->start(m_ExecutablePath,Args);

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
    {
      FilePathStatus.m_IndexStatus = FileStatus::CONFLICT;
    }
    else if (IndexLetter == '?')
    {
      FilePathStatus.m_IndexStatus = FileStatus::UNTRACKED;
    }
    else if (IndexLetter == '!')
    {
      FilePathStatus.m_IndexStatus = FileStatus::IGNORED;
    }
    else
    {
      if (WorkTreeLetter == 'M')
      {
        FilePathStatus.m_IsDirty = true;
      }

      if (IndexLetter == 'M')
      {
        FilePathStatus.m_IndexStatus = FileStatus::MODIFIED;
      }
      if (IndexLetter == 'A')
      {
        FilePathStatus.m_IndexStatus = FileStatus::ADDED;
      }
      // Deleted takes precedence over Added
      if (IndexLetter == 'D' || WorkTreeLetter == 'D')
      {
        FilePathStatus.m_IndexStatus = FileStatus::DELETED;
      }
    }

    TreeStatus.m_FileStatusByTreePath[FilePath] = FilePathStatus;
  }

  delete mp_Process;
  mp_Process = nullptr;

  return TreeStatus;
}


// =====================================================================
// =====================================================================


QString GitProxy::statusHtml(const QString& Path, bool WithColorCodes)
{
  QDir PathDir(Path);

  if (!PathDir.exists(".git"))
  {
    return "";
  }

  mp_Process = new QProcess();
  mp_Process->setWorkingDirectory(Path);

  QStringList Args;

  if (WithColorCodes)
  {
    Args << "-c" << "color.status=always";
  }

  Args << "status";

  mp_Process->start(m_ExecutablePath,Args);

  mp_Process->waitForReadyRead(-1);
  mp_Process->waitForFinished(-1);

  QString Res = QString::fromUtf8(mp_Process->readAll());

  delete mp_Process;
  mp_Process = nullptr;

  return Res;
}


// =====================================================================
// =====================================================================


QString GitProxy::logHtml(const QString& Path, bool WithColorCodes)
{
  QDir PathDir(Path);

  if (!PathDir.exists(".git"))
  {
    return "";
  }

  mp_Process = new QProcess();
  mp_Process->setWorkingDirectory(Path);

  QStringList Args;
  Args << "log";

  if (WithColorCodes)
  {
    Args << "--color";
  }

  mp_Process->start(m_ExecutablePath,Args);

  mp_Process->waitForReadyRead(-1);
  mp_Process->waitForFinished(-1);

  QString Res = QString::fromUtf8(mp_Process->readAll());

  delete mp_Process;
  mp_Process = nullptr;

  return Res;
}


} } // namespaces

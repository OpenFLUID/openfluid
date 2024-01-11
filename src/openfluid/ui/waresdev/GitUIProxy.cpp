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
 @file GitUIProxy.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <fstream>

#include <QDir>
#include <QTextStream>

#include <openfluid/ui/waresdev/GitUIProxy.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace ui { namespace waresdev {


GitUIProxy::GitUIProxy() : openfluid::utils::GitProxy()
{
  // TODO use a subdir in tmp dir given by openfluid::tools::Filesystem::makeUniqueSubdirectory()
  m_TmpPath = QString::fromStdString(openfluid::base::Environment::getTempDir());
}


// =====================================================================
// =====================================================================


GitUIProxy::~GitUIProxy()
{
  delete mp_Process;
}


// =====================================================================
// =====================================================================


QString GitUIProxy::getCurrentOpenFLUIDBranchName()
{
  return "openfluid-" + QString::fromStdString(openfluid::base::Environment::getVersionMajorMinor());
}


// =====================================================================
// =====================================================================


void GitUIProxy::processStandardOutput()
{
  mp_Process->setReadChannel(QProcess::StandardOutput);

  while (mp_Process->canReadLine())
  {
    emit info(QString::fromUtf8(mp_Process->readLine()));
  }
}


// =====================================================================
// =====================================================================


void GitUIProxy::processErrorOutput()
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


void GitUIProxy::processErrorOutputAsInfo()
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


bool GitUIProxy::launchAuthCommand(QStringList Args, const QString& FromUrl, const QString& ToPath,
                               const QString& Username, const QString& Password,
                               bool SslNoVerify, const QString& WorkingDirectory, bool UsernameViaEnv)
{
  if (FromUrl.isEmpty() || ToPath.isEmpty())
  {
    std::string ErrorMsg = "Empty remote url or empty destination path";
    openfluid::utils::log::error("Git", ErrorMsg);
    emit error(tr(ErrorMsg.c_str()));
    return false;
  }

  // Manual check if .git/index.lock detected (meaning that another git process is already running)
  // HACK see if removable for release
  if (!WorkingDirectory.isEmpty())
  {
    openfluid::tools::FilesystemPath GitIndexLockPath = 
          openfluid::tools::FilesystemPath({WorkingDirectory.toStdString(), ".git", "index.lock"});
    if (GitIndexLockPath.exists())
    {
      std::string ErrorMsg = "Can not operate, git lock detected.";
      openfluid::utils::log::error("Git", ErrorMsg);
      emit error(tr(ErrorMsg.c_str()));
      return false;
    }
    else // HACK for debugging purpose, to remove before release
    {
      openfluid::utils::log::debug("Git", 
                                   "git lock not detected here: "+WorkingDirectory.toStdString());
      emit info(tr("git lock not detected here:")+QString::fromStdString(WorkingDirectory.toStdString()));
    }
  }

  QString Url = FromUrl;
  QStringList Options;

  QProcessEnvironment PcsEnv = QProcessEnvironment::systemEnvironment();

  if (!Username.isEmpty() && !Url.contains("@") && !UsernameViaEnv)
  {
    Url.replace("http://", QString("http://%1@").arg(Username));
    Url.replace("https://", QString("https://%1@").arg(Username));
  }

  if (!Password.isEmpty())
  {
    PcsEnv.insert("GIT_ASKPASS",QString::fromStdString(openfluid::config::GITASKPASS_APP));
    PcsEnv.insert(OFBUILD_GITASKPASS_ENVVAR,Password);
    if (UsernameViaEnv)
    {
      PcsEnv.insert(OFBUILD_GITASKUSER_ENVVAR,Username);
    }
  }

  if (SslNoVerify)
  {
    Options.append("-c");
    Options.append("http.sslverify=false");
  }

  Args.append(Options);
  Args.append(Url);
  Args.append(ToPath);

  mp_Process = new QProcess();
  if (!WorkingDirectory.isEmpty())
  {
    mp_Process->setWorkingDirectory(WorkingDirectory);
  }
  mp_Process->setProcessEnvironment(PcsEnv);

  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStandardOutput()));
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processErrorOutputAsInfo()));

  std::string GitCommand = m_LocalGitProgram + " ";
  for (auto& p : Args)
  {
    GitCommand += " " + p.toStdString();
  }

  openfluid::utils::log::info("Git", GitCommand);
  mp_Process->start(QString::fromStdString(m_LocalGitProgram),Args);
  if (!mp_Process->waitForStarted())
  {
      delete mp_Process;
      mp_Process = nullptr;
      openfluid::utils::log::error("Git", "Git failed start");
      return false;
  }
  mp_Process->waitForFinished(-1);
  mp_Process->waitForReadyRead(-1);

  QString Res = QString::fromUtf8(mp_Process->readAll());

  std::string ResContent = Res.toStdString();
  if (ResContent.empty())
  {
    ResContent = "/EMPTY/";
  }
  openfluid::utils::log::debug("Git", 
                             "Git process content: "+ResContent);
  
  int ErrCode = mp_Process->exitCode();

  delete mp_Process;
  mp_Process = nullptr;

  return (ErrCode == 0);
}


// =====================================================================
// =====================================================================


bool GitUIProxy::addSubmodule(const QString& FromUrl, const QString& ToPath, const QString& LocalGitRepoPath,
                              const QString& Username, const QString& Password,
                              bool SslNoVerify)
{
  QStringList Args = {"submodule", "add", "--progress"};
  return launchAuthCommand(Args, FromUrl, ToPath, Username, Password, SslNoVerify, LocalGitRepoPath, true);
}


// =====================================================================
// =====================================================================


bool cleanModuleDir(QString RootPath, openfluid::tools::FilesystemPath ModuleSubPath)
{
  // returns true if submodule was really deleted
  openfluid::tools::FilesystemPath GitSubmoduleTargetDirectory = openfluid::tools::FilesystemPath(
        {RootPath.toStdString(), ".git", "modules", ModuleSubPath.toGeneric()});
  if (GitSubmoduleTargetDirectory.isDirectory())
  {
    return GitSubmoduleTargetDirectory.removeDirectory();
  }
  return false;
}


// =====================================================================
// =====================================================================


std::pair<bool, QString>  GitUIProxy::removeSubmodule(const QString& MainPathString, const QString& SubmodulePathString)
{
  QString StandardOutput;

  // Removing a submodule included in git management system
  // reference: https://git-scm.com/docs/gitsubmodules#_forms
  /*
      A submodule can be deleted by running git rm <submodule path> && git commit. 
      This can be undone using git revert.
      The deletion removes the superproject’s tracking data, which are both the gitlink entry 
      and the section in the .gitmodules file. 
      The submodule’s working directory is removed from the file system, but the Git directory is kept around 
      as it to make it possible to checkout past commits without requiring fetching from another repository.
      To completely remove a submodule, manually delete $GIT_DIR/modules/<name>/.
  */
  // TOWATCH if Git command for submodule removal becomes more comprehensive in the future

  int SummaryStatusCode = 0;
  QString StatusTxt;

  // 1. remove submodule from git tracking
  QStringList Args;
  Args << "rm" << "-f" << SubmodulePathString;
  auto RmReturn = launchLocalCommand(MainPathString, Args);
  if (RmReturn.first != 0)
  {
    SummaryStatusCode = 1;
    StandardOutput += tr("Git rm command failed with error code %1\n").arg(RmReturn.first);
  }
  
  // 2. remove module folder in .git
  openfluid::tools::FilesystemPath DestSubPath(SubmodulePathString.toStdString());
  if (!cleanModuleDir(MainPathString, DestSubPath))
  {
    SummaryStatusCode = 1;
    StandardOutput += "Git manual removal of module failed\n";
  }

  // 3. remove .gitmodules if empty
  openfluid::tools::FilesystemPath SubmoduleFile({MainPathString.toStdString(), ".gitmodules"});
  if (SubmoduleFile.isFile())
  {
    std::ifstream SubmoduleFileStream;
    SubmoduleFileStream.open(SubmoduleFile.toGeneric(), std::ifstream::in);
    if (SubmoduleFileStream.peek() == std::ifstream::traits_type::eof())
    {
      // file empty, can be removed
      QProcess* Process = new QProcess();
      Process->setWorkingDirectory(MainPathString);

      Process->start(QString::fromStdString(m_LocalGitProgram),{"rm", "--cached", ".gitmodules"});
      Process->waitForReadyRead(-1);
      Process->waitForFinished(-1);
      int ErrCode = Process->exitCode();
      if (ErrCode != 0)
      {
        SummaryStatusCode = 1;
        StandardOutput += tr("Git rm command for .gitmodules failed with error code %1\n").arg(ErrCode);
      }
      delete Process;
      Process = nullptr;
      if (!SubmoduleFile.removeFile())
      {
        SummaryStatusCode = 1;
        StandardOutput += tr("Submodule file removal failed\n");
      }
    }
  }
  
  StatusTxt = StatusTxt + "\n" + RmReturn.second;
  if (SummaryStatusCode == 0)
  {
    StandardOutput += tr("Submodule successfully removed");
  }
  openfluid::utils::log::debug("Git", StandardOutput.toStdString());
  return std::pair(SummaryStatusCode, StandardOutput);
}


// =====================================================================
// =====================================================================


bool GitUIProxy::clone(const QString& FromUrl, const QString& ToPath,
                       const QString& Username, const QString& Password,
                       bool SslNoVerify, const QString& LocalGitRepoPath, bool WithoutVersioning)
{
  QStringList Args = {"clone", "--recurse-submodules", "--progress"};
  if (WithoutVersioning)
  {
    Args << "--depth=1";
  }
  return launchAuthCommand(Args, FromUrl, ToPath, Username, Password, SslNoVerify, LocalGitRepoPath, true);
}


// =====================================================================
// =====================================================================


bool GitUIProxy::checkout(const QString& Path, const QString& BranchName, bool New)
{
  //TOIMPL test this function?
  
  if (New)
  {
    QStringList Args = {"checkout", "-b", BranchName, "--progress"};
    launchLocalCommand(Path, Args);
  }
  else
  {
    QStringList Args = {"checkout", BranchName, "--progress"};
    launchLocalCommand(Path, Args);
  }
  QString Out = launchLocalCommand(Path, {"branch", "--show-current"}).second;
  QString CurrentBranch = Out.section("\n", 0, 0).section(" ", -1);
  return (CurrentBranch == BranchName);
}


// =====================================================================
// =====================================================================


GitUIProxy::TreeStatusInfo GitUIProxy::status(const QString& Path)
{
  TreeStatusInfo TreeStatus;
  QDir PathDir(Path);

  if (!PathDir.exists(".git"))
  {
    return TreeStatus;
  }

  TreeStatus.m_IsGitTracked = true;
  
  QStringList Args = {"status","--porcelain","--ignored","-b"};

  QString Out = launchLocalCommand(Path, Args).second;

  TreeStatus.m_BranchName = Out.section('\n', 0, 0).section(' ', 1).section("...", 0, 0);

#if (QT_VERSION_MAJOR < 6)
  QStringList PathLines = Out.section('\n', 1).split('\n', QString::SkipEmptyParts);
#else
  QStringList PathLines = Out.section('\n', 1).split('\n', Qt::SkipEmptyParts);
#endif
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


std::pair<int, QString> GitUIProxy::launchLocalCommand(const QString& Path, QStringList Args, bool RequiringDotGit)
{
  QDir PathDir(Path);

  if (RequiringDotGit && !PathDir.exists(".git"))
  {
    return std::pair(-1, QString());
  }

  QProcess LocalProcess;
  LocalProcess.setWorkingDirectory(Path);

  LocalProcess.start(QString::fromStdString(m_LocalGitProgram),Args);

  LocalProcess.waitForReadyRead(-1);
  LocalProcess.waitForFinished(-1);

  QString Res = QString::fromUtf8(LocalProcess.readAll());
  int ExitCode = LocalProcess.exitCode();

  return std::pair(ExitCode, Res);
}


// =====================================================================
// =====================================================================


QString GitUIProxy::statusHtml(const QString& Path, bool WithColorCodes)
{

  QStringList Args;

  if (WithColorCodes)
  {
    Args << "-c" << "color.status=always";
  }

  Args << "status";

  return launchLocalCommand(Path, Args).second;
}


// =====================================================================
// =====================================================================


QString GitUIProxy::logHtml(const QString& Path, bool WithColorCodes)
{

  QStringList Args;
  Args << "log";

  if (WithColorCodes)
  {
    Args << "--color";
  }

  return launchLocalCommand(Path, Args).second;
}


// =====================================================================
// =====================================================================


 QString GitUIProxy::init(const QString& Path)
{
  QStringList Args;
  Args << "init";
  return launchLocalCommand(Path, Args, false).second;
}


} } } // namespaces

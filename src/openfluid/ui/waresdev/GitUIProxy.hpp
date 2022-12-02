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
 @file GitUIProxy.hpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_GITUIPROXY_HPP__
#define __OPENFLUID_UIWARESDEV_GITUIPROXY_HPP__


#include <QString>
#include <QObject>
#include <QProcess>
#include <QMap>

#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// removal of submodule directory in .git via explicit rm operation
bool cleanModuleDir(QString RootPath, openfluid::tools::FilesystemPath DestSubPath);


class OPENFLUID_API GitUIProxy : public QObject, public openfluid::utils::GitProxy
{
  Q_OBJECT

  public:

    enum class FileStatus
    {
      TRACKED, UNTRACKED, IGNORED, CONFLICT, ADDED, DELETED, MODIFIED
    };

    struct FileStatusInfo
    {
        bool m_IsDirty = false;

        FileStatus m_IndexStatus = FileStatus::TRACKED;
    };

    struct TreeStatusInfo
    {
        bool m_IsGitTracked = false;

        QString m_BranchName = "";

        QMap<QString, FileStatusInfo> m_FileStatusByTreePath;
    };


  private:

    QProcess* mp_Process = nullptr;

    QString m_TmpPath;

    static void findGitProgram();


  private slots:

    void processStandardOutput();

    void processErrorOutput();

    void processErrorOutputAsInfo();


  signals:

    void info(const QString& Message);

    void error(const QString& Message);


  public:

    GitUIProxy();

    ~GitUIProxy();

    static QString getCurrentOpenFLUIDBranchName();

    bool launchAuthCommand(QStringList Args, const QString& FromUrl, const QString& ToPath,
                      const QString& Username = "", const QString& Password = "",
                      bool SslNoVerify = false, const QString& WorkingDirectory = "", bool UsernameViaEnv = false);

    bool addSubmodule(const QString& FromUrl, const QString& ToPath, const QString& LocalGitRepoPath,
               const QString& Username = "", const QString& Password = "",
               bool SslNoVerify = false);

    std::pair<bool, QString> removeSubmodule(const QString& Path, const QString& SubmodulePath);

    bool clone(const QString& FromUrl, const QString& ToPath,
               const QString& Username = "", const QString& Password = "",
               bool SslNoVerify = false, const QString& LocalGitRepoPath = "", bool WithoutVersioning = false);

    TreeStatusInfo status(const QString& Path);

    std::pair<int, QString> commandHtml(const QString& Path, QStringList Args, bool RequiringGit=true);

    QString statusHtml(const QString& Path, bool WithColorCodes);

    QString logHtml(const QString& Path, bool WithColorCodes);

    QString init(const QString& WorkingDirectory);

};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_GITUIPROXY_HPP__ */

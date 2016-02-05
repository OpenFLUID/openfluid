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
 @file GitHelper.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UTILS_GITHELPER_HPP__
#define __OPENFLUID_UTILS_GITHELPER_HPP__

#include <QString>
#include <QObject>
#include <QProcess>
#include <QMap>
#include <QFile>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace utils {


class OPENFLUID_API GitHelper: public QObject
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

    static QString m_GitPgm;

    QProcess* mp_Process = nullptr;

    QString m_TmpPath;

    QFile m_AskPassFile;


  private slots:

    void processStandardOutput();

    void processErrorOutput();

    void processErrorOutputAsInfo();


  signals:

    void info(const QString& Message);

    void error(const QString& Message);


  public:

    GitHelper();

    ~GitHelper();

    static bool checkGitProgram();

    static std::string getOpenfluidCurrentBranchName();

    bool clone(const QString& FromUrl, const QString& ToPath,
               const QString& Username = "", const QString& Password = "",
               bool SslNoVerify = false);

    TreeStatusInfo status(const QString& Path);

    QString statusHtml(const QString& Path, bool WithColorCodes);

    QString logHtml(const QString& Path, bool WithColorCodes);

};


} } // namespaces


#endif /* __OPENFLUID_UTILS_GITHELPER_HPP__ */

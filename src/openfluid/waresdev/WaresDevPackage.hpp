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
 @file WaresDevPackage.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __OPENFLUID_WARESDEV_WARESDEVPACKAGE_HPP__
#define __OPENFLUID_WARESDEV_WARESDEVPACKAGE_HPP__

#include <openfluid/dllexport.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/waresdev/WareSrcManager.hpp>

#include <QString>
#include <QMap>
#include <QDir>
#include <QProcess>


namespace openfluid { namespace waresdev {


class OPENFLUID_API WaresDevPackage: public QObject
{
  Q_OBJECT

  private slots:

    void processStandardOutput();

    void processErrorOutput();


  protected:

    static QString m_SevenZCmd;

    QString m_TempOfwdpSubDirPath;

    QString m_PackageFilePath;

    QStringList m_WaresPaths;

    QString m_Packagers;

    QString m_PackageDescription;

    QString m_PackageName;

    QDir m_PackageTempDir;

    QString m_PackageTempPath;

    QString m_ConfFilePath;

    QProcess* mp_Process = 0;

    WaresDevPackage(const QString& PackageFilePath, const QString& TempSubFolderName);

    void createAndLauchProcess(const QString& Command);


  public:

    virtual ~WaresDevPackage();

    QStringList getWaresPaths();

    static bool checkSevenZProgram();

    QString getPackagers();

    QString getPackageDescription();

    QDir getPackageTempDir();


  signals:

    void info(const QString& Message);

    void error(const QString& Message);

    void finished(bool Ok);

    void progressed(int Value);
};


// =====================================================================
// =====================================================================


class OPENFLUID_API WaresDevExportPackage: public WaresDevPackage
{
  Q_OBJECT

  private:

    void writeConfFile();

    void compress();


  public:

    WaresDevExportPackage(const QString& PackageFilePath, const QStringList& WaresPaths,
                          const QString& Packagers, const QString& PackageDescription);

  public slots:

    void exportToPackage();
};


// =====================================================================
// =====================================================================


class OPENFLUID_API WaresDevImportPackage: public WaresDevPackage
{
  Q_OBJECT

  private:

    QStringList m_SelectedWarePaths;

    void uncompress();

    void readConfFile();


  public:

    WaresDevImportPackage(const QString& PackageFilePath);

    void setSelectedWares(const QStringList& SelectedWarePaths);


  public slots:

    void fetchInformation();

    void copyWares();
};

} } // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESDEVPACKAGE_HPP__ */

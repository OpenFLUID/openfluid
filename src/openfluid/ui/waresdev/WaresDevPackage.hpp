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
 
  @author Aline LIBRES <aline.libres@gmail.com>
*/


#ifndef __OPENFLUID_UIWARESDEV_WARESDEVPACKAGE_HPP__
#define __OPENFLUID_UIWARESDEV_WARESDEVPACKAGE_HPP__


#include <QString>
#include <QMap>
#include <QDir>
#include <QProcess>

#include <openfluid/dllexport.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/utils/CMakeProxy.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WaresDevPackage: public QObject
{
  Q_OBJECT

  private slots:

    void processStandardOutput();

    void processErrorOutput();


  protected:

    QString m_PackageFilePath;

    QStringList m_WaresPaths;

    QString m_Packagers;

    QString m_PackageDescription;

    QString m_PackageName;

    QDir m_PackageTempDir;

    QString m_PackageTempPath;

    QString m_ConfFilePath;

    QProcess* mp_Process = nullptr;

    WaresDevPackage(const QString& PackageFilePath, const QString& TempSubFolderName);

    void createAndLauchProcess(const openfluid::utils::CMakeProxy::CommandInfos& Command);


  signals:

    void info(const QString& Message);

    void error(const QString& Message);

    void finished(bool Ok, const QString& Message);

    void progressed(int Value);


  public:

    virtual ~WaresDevPackage();

    QStringList getWaresPaths();

    QString getPackagers();

    QString getPackageDescription();

    QDir getPackageTempDir();

};


// =====================================================================
// =====================================================================


class OPENFLUID_API WaresDevExportPackage: public WaresDevPackage
{
  Q_OBJECT

  private:

    void writeConfFile();

    void compress();


  public slots:

    void exportToPackage();


  public:

    WaresDevExportPackage(const QString& PackageFilePath, const QStringList& WaresPaths,
                          const QString& Packagers, const QString& PackageDescription);

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


  public slots:

    void fetchInformation();

    void copyWares();


  public:

    WaresDevImportPackage(const QString& PackageFilePath);

    void setSelectedWares(const QStringList& SelectedWarePaths);

};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESDEVPACKAGE_HPP__ */

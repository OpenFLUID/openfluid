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
  @file GitImportWorker.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_UIWARESDEV_GITIMPORTWORKER_HPP__
#define __OPENFLUID_UIWARESDEV_GITIMPORTWORKER_HPP__


#include <QObject>
#include <QMap>
#include <QStringList>

#include <openfluid/dllexport.hpp>
#include <openfluid/utilsq/FluidHubAPIClient.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API GitImportWorker: public QObject
{
  Q_OBJECT


  protected:

    QString m_Username = "";
    QString m_Password = "";

    int m_Progress = 0;
    double m_ProgressRatio = 100;

    bool m_SslNoVerify;


    std::vector<std::pair<QString, QString>> m_ElementsToImport; // first: Git URL, second: local path

  protected slots:

    virtual bool importElement(const QString& GitUrl, const QString& ContextPath) = 0;


  signals:

    void info(const QString& Message);

    void error(const QString& Message);

    void finished(bool Ok, const QString& Message);

    void progressed(int Value);


  public slots:

    void setSelectedElements(const std::vector<std::pair<QString, QString>>& SelectedElements);

    bool runImports();


  public:

    GitImportWorker(bool SslNoVerify = false);

    ~GitImportWorker();

    void setupUser(const QString& Username, const QString& Password);

    void setProgressValues(int Progress, double ProgressRatio)
    {
      m_Progress = Progress;
      m_ProgressRatio = ProgressRatio;
    }

};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_GITIMPORTWORKER_HPP__ */

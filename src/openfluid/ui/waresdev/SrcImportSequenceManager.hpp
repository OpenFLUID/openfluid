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
  @file SrcImportSequenceManager.hpp

  @author Armel THÖNI <armel.thoni@inra.fr>
 */


#ifndef __OPENFLUID_UIWARESDEV_SRCIMPORTSEQUENCEMANAGER_HPP__
#define __OPENFLUID_UIWARESDEV_SRCIMPORTSEQUENCEMANAGER_HPP__


#include <QObject>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/waresdev/WaresImportWorker.hpp>
#include <openfluid/ui/waresdev/FragmentsImportWorker.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API SrcImportSequenceManager: public QObject
{
  Q_OBJECT


  private:

    QString m_Username;
    QString m_Password;
    QString m_RootPath;

    std::map<openfluid::ware::WareType, QStringList> m_SelectedWaresUrlByType;
    QStringList m_SelectedFragmentsUrl;

    bool m_FragmentsAsSubmodule = true;

    bool runWorker(GitImportWorker* Worker, std::vector<std::pair<QString, QString>> Elements);


  signals:

    void info(const QString& Message);

    void error(const QString& Message);

    void finished(bool Ok, const QString& Message);

    void progressed(int Value);


  public slots :

    bool onCloneRequest();


  public:

    SrcImportSequenceManager();

    ~SrcImportSequenceManager();

    void setSelectedWaresUrl(const std::map<openfluid::ware::WareType, QStringList>& SelectedWaresUrlByType);

    void setSelectedFragmentsUrl(const QStringList& SelectedFragmentsUrl);


    void setupUser(const QString& Username, const QString& Password);

    void setupFragmentStrategy(const QString& RootPath, bool AsSubmodule);

};

} } } //namespaces

#endif /* __OPENFLUID_UIWARESDEV_SRCIMPORTSEQUENCEMANAGER_HPP__ */

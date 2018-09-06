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
 @file WaresSrcImportDialog.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__


#include <QListWidget>
#include <QAbstractButton>
#include <QButtonGroup>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/config.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>


namespace Ui {
class WaresSrcImportDialog;
}

namespace openfluid { namespace waresdev {
class WaresDevImportPackage;
class WaresHubImportWorker;
} }


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WaresSrcImportDialog: public openfluid::ui::common::MessageDialog
{
  Q_OBJECT

  private slots :

    bool check();

    void onSourceChanged(QAbstractButton* ClickedButton);

    void onPackagePathButtonClicked();

    void onWareshubConnectButtonClicked();

    void import();


  private:

    Ui::WaresSrcImportDialog* ui;

    QButtonGroup m_SourceBtGroup;

    QString m_PackagePathLabelDefault = "<i>No package selected</i>";

    QList<QWidget*> m_WaresHubConnectionInfoWidgets;

    QString m_WaresHubButtonDisconnectLabel = tr("Disconnect");

    std::map<openfluid::ware::WareType, QListWidget*> m_ListWidgetsByWareType;

    std::map<QString, openfluid::ware::WareType> m_WareTypeConverter = {
        { QString::fromStdString(openfluid::config::SIMULATORS_PATH), openfluid::ware::WareType::SIMULATOR },
        { QString::fromStdString(openfluid::config::OBSERVERS_PATH), openfluid::ware::WareType::OBSERVER },
        { QString::fromStdString(openfluid::config::BUILDEREXTS_PATH), openfluid::ware::WareType::BUILDEREXT } };

    openfluid::waresdev::WaresDevImportPackage* mp_ImportFilePkg = nullptr;

    openfluid::waresdev::WaresHubImportWorker* mp_WaresHubImportWorker = nullptr;

    QMap<openfluid::ware::WareType, QStringList> m_AlreadySelectedWaresHubWares;

    void updatePackageInfo();

    void updatePackageWaresList();

    void updateWaresHubWaresList();

    QStringList getSelectedWares();

    std::map<openfluid::ware::WareType, QStringList> getSelectedWaresByType();


  public:

    WaresSrcImportDialog(QWidget* Parent);

    ~WaresSrcImportDialog();

};


} } } //namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__ */

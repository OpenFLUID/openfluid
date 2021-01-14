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

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Armel THONI <armel.thoni@inra.fr>
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
class FluidHubWaresImportWorker;
} }


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WaresSrcImportDialog: public openfluid::ui::common::MessageDialog
{
  Q_OBJECT

  private slots :

    bool check();

    void onSourceChanged(QAbstractButton* ClickedButton);

    void onPackagePathButtonClicked();

    void onHubConnectButtonClicked();

    void onHubLoginButtonClicked();

    void import();


  private:

    Ui::WaresSrcImportDialog* ui;

    bool m_IsConnectedToHub = false;
    bool m_IsLoggedInHub = false;

    QButtonGroup m_SourceBtGroup;

    QString m_PackagePathLabelDefault = "<i>No package selected</i>";

    QList<QWidget*> m_HubLoginWidgets;
    QList<QWidget*> m_HubLoginWidgetsAndButton;
    QList<QWidget*> m_HubConnectionInfoWidgets;


    QString m_HubButtonConnectLabel = tr("Go");
    QString m_HubButtonDisconnectLabel = tr("Quit");
    QString m_HubButtonLoginLabel = tr("Log in");
    QString m_HubButtonLogoutLabel = tr("Log out");

    std::map<openfluid::ware::WareType, QListWidget*> m_ListWidgetsByWareType;

    std::map<QString, openfluid::ware::WareType> m_WareTypeConverter = {
        { QString::fromStdString(openfluid::config::SIMULATORS_PATH), openfluid::ware::WareType::SIMULATOR },
        { QString::fromStdString(openfluid::config::OBSERVERS_PATH), openfluid::ware::WareType::OBSERVER },
        { QString::fromStdString(openfluid::config::BUILDEREXTS_PATH), openfluid::ware::WareType::BUILDEREXT } };

    openfluid::waresdev::WaresDevImportPackage* mp_ImportFilePkg = nullptr;

    openfluid::waresdev::FluidHubWaresImportWorker* mp_FluidHubWaresImportWorker = nullptr;

    QMap<openfluid::ware::WareType, QStringList> m_AlreadySelectedHubWares;

    void updatePackageInfo();

    void updatePackageWaresList();

    void updateHubWaresList();

    QStringList getSelectedWares();

    std::map<openfluid::ware::WareType, QStringList> getSelectedWaresByType();


  public:

    explicit WaresSrcImportDialog(QWidget* Parent);

    ~WaresSrcImportDialog();

};


} } } //namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__ */

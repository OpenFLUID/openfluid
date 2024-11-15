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
  @author Armel THÖNI <armel.thoni@inra.fr>
 */


#ifndef __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__


#include <QListWidget>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/config.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/ui/waresdev/AbstractSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WaresImportFilterWidget.hpp>


namespace Ui {
class WaresSrcImportDialog;
}


namespace openfluid { namespace ui { namespace waresdev {


class WaresDevImportPackage;
class WaresImportWorker;


class OPENFLUID_API WaresSrcImportDialog: public AbstractSrcImportDialog
{
  Q_OBJECT

  private slots :
  
    bool check();

    void onSourceChanged(QAbstractButton* ClickedButton);

    void onPackagePathButtonClicked();

    void onWaresListRefreshAsked();

    void updateWareSelectionCount();


  private:
    std::map<openfluid::ware::WareType, QListWidget*> m_ListWidgetsByWareType;

    std::map<openfluid::ware::WareType, std::map<openfluid::ware::WareID_t, QListWidgetItem*>> m_MapWidgetHub;
    std::map<openfluid::ware::WareType, std::map<openfluid::ware::WareID_t, QListWidgetItem*>> m_MapWidgetPackage;

    Ui::WaresSrcImportDialog* ui;

    QButtonGroup m_SourceBtGroup;

    QString m_PackagePathLabelDefault = "<i>No package selected</i>";


    std::map<openfluid::ware::WareType, WaresImportFilterWidget*> m_FilterWidgetsByWareType;

    std::map<QString, openfluid::ware::WareType> m_WareTypeConverter = {
        { QString::fromStdString(openfluid::config::SIMULATORS_PATH), openfluid::ware::WareType::SIMULATOR },
        { QString::fromStdString(openfluid::config::OBSERVERS_PATH), openfluid::ware::WareType::OBSERVER },
        { QString::fromStdString(openfluid::config::BUILDEREXTS_PATH), openfluid::ware::WareType::BUILDEREXT } };

    WaresDevImportPackage* mp_ImportFilePkg = nullptr;

    QMap<openfluid::ware::WareType, QStringList> m_AlreadySelectedHubWares;

    void updatePackageInfo();

    void updatePackageWaresList();

    void setItemChangedConnection(bool Connect);

    void toggleCheckSelectedWares(const QStringList& SelectedWares, bool Check);

    QStringList getSelectedWares();

    std::map<openfluid::ware::WareType, QStringList> getSelectedWaresByType();

    bool wareItemDisplay(const openfluid::ware::WareType Type, const QString WareId, QListWidgetItem* Item);

    bool isWareDisplayed(const openfluid::ware::WareType& Type, const QString WareId, const bool WareInWorkspace, 
                         const bool WareNotAuthorized);

  protected slots:

    void onHubConnectButtonClicked();

    void onImportAsked();


  protected:

    QLineEdit* hubUrlLineEdit();

    QLineEdit* usernameLineEdit();
  
    QLineEdit* passwordLineEdit();

    QPushButton* hubConnectButton();

    QPushButton* hubLoginButton();
    
    QLabel* hubUsernameLabel();

    void clearListWidgets();

    void updateHubElementsList();

    void customHideWidget(QWidget* Widget);
    
    void postConnectSetup();


  public:

    explicit WaresSrcImportDialog(QWidget* Parent);

    ~WaresSrcImportDialog();
    

};


} } } //namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__ */

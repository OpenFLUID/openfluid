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
  @file AbstractSrcImportDialog.hpp

  @author Armel THÖNI <armel.thoni@inra.fr>
 */


#ifndef __OPENFLUID_UIWARESDEV_ABSTRACTSRCIMPORTDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_ABSTRACTSRCIMPORTDIALOG_HPP__


#include <QListWidget>
#include <QLineEdit>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/config.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/waresdev/WaresImportFilterWidget.hpp>
#include <openfluid/ui/waresdev/SrcImportSequenceManager.hpp>
#include <openfluid/ui/waresdev/GitImportWorker.hpp>
#include <openfluid/ui/waresdev/HubManager.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class WaresImportWorker;


class OPENFLUID_API AbstractSrcImportDialog: public openfluid::ui::common::MessageDialog
{
  Q_OBJECT

  protected slots :

    virtual bool check() = 0;

    virtual void onHubConnectButtonClicked();
    
    void onHubLoginButtonClicked();

    virtual void onImportAsked() = 0;


  protected:

    openfluid::ui::waresdev::HubManager m_HubManager;

    QList<QWidget*> m_HubLoginWidgets;
    QList<QWidget*> m_HubConnectionInfoWidgets;
    QList<QWidget*> m_HubLoginWidgetsAndButton;

    QString m_HubButtonConnectLabel;
    QString m_HubButtonDisconnectLabel;
    QString m_HubButtonLoginLabel;
    QString m_HubButtonLogoutLabel;

    bool checkHubSection();

    QStringList getSelectedWares();

    virtual void updateHubElementsList() = 0;

    virtual QLineEdit* hubUrlLineEdit() = 0;
  
    virtual QLineEdit* usernameLineEdit() = 0;
  
    virtual QLineEdit* passwordLineEdit() = 0;

    virtual QPushButton* hubConnectButton() = 0;

    virtual QPushButton* hubLoginButton() = 0;

    virtual QLabel* hubUsernameLabel() = 0;

    virtual void clearListWidgets() = 0;

    virtual void customHideWidget(QWidget* Widget) = 0;

    virtual void postConnectSetup() = 0;

    QStringList getSelectedElementsFromListWidget(const QListWidget* ListWidget);

    void genericItemDisplay(bool AlreadyDisplayed, bool NotAuthorized, QListWidgetItem* Item, const QString& ElementId);
    
    void setupImportManagerThread(openfluid::ui::waresdev::SrcImportSequenceManager* SrcImportSequenceManager, 
                                  QThread* Thread, 
                                  openfluid::ui::waresdev::WaresSrcIOProgressDialog* ProgressDialogPtr);

    void runThread(QThread* Thread, openfluid::ui::waresdev::WaresSrcIOProgressDialog& ProgressDialog);
    

  public:

    explicit AbstractSrcImportDialog(QWidget* Parent);

    ~AbstractSrcImportDialog();

};


} } } //namespaces


#endif /* __OPENFLUID_UIWARESDEV_ABSTRACTSRCIMPORTDIALOG_HPP__ */

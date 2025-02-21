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
  @file AbstractSrcImportDialog.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inra.fr>
*/


#include <QFileDialog>
#include <QThread>
#include <QMessageBox>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/ui/waresdev/WaresImportWorker.hpp>
#include <openfluid/ui/waresdev/AbstractSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>


namespace openfluid { namespace ui { namespace waresdev {


AbstractSrcImportDialog::AbstractSrcImportDialog(QWidget* Parent) :
    openfluid::ui::common::MessageDialog(Parent)
{
  m_HubButtonConnectLabel = tr("Connect");
  m_HubButtonDisconnectLabel = tr("Disconnect");
  m_HubButtonLoginLabel = tr("Log in");
  m_HubButtonLogoutLabel = tr("Log out");
}


// =====================================================================
// =====================================================================


AbstractSrcImportDialog::~AbstractSrcImportDialog()
{
}


// =====================================================================
// =====================================================================


bool AbstractSrcImportDialog::checkHubSection()
{
  if (hubUrlLineEdit()->text().isEmpty())
  {
    setMessage(tr("No Hub URL defined"));
    hubConnectButton()->setEnabled(false);
    return false;
  }
  else
  {
    hubConnectButton()->setEnabled(true);
  }

  if (!m_HubManager.isConnected())
  {
    setMessage(tr("Not connected to a Hub"));
    return false;
  }
  setMessage("");
  return true;
}


// =====================================================================
// =====================================================================


void AbstractSrcImportDialog::onHubConnectButtonClicked()
{
  if (m_HubManager.isConnected())
  {
    m_HubManager.disconnectFromHub();

    clearListWidgets();

    hubConnectButton()->setText(m_HubButtonConnectLabel);

    for (auto& Widget : m_HubConnectionInfoWidgets)
    {
      Widget->setEnabled(true);
    }

    m_HubManager.logout();
    hubLoginButton()->setText(m_HubButtonLoginLabel);

    // Hide login fields
    hubUrlLineEdit()->setEnabled(true);
    for (auto& Widget : m_HubLoginWidgetsAndButton)
    {
      customHideWidget(Widget);
    }

    return;
  }
  else
  {
    QString HubUrl = hubUrlLineEdit()->text();

    if (HubUrl.isEmpty())
    {
      check();
      return;
    }

    m_HubManager.setUrl(HubUrl.toStdString());

    openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Connecting to Hub:"), true, this);

    try
    {
      connect(&m_HubManager, SIGNAL(finished(bool, const QString&)), &ProgressDialog,
        SLOT(finishAndQuit(bool, const QString&)));
      connect(&m_HubManager, SIGNAL(info(const QString&)), &ProgressDialog,
          SLOT(writeInfo(const QString&)));
    connect(&m_HubManager, SIGNAL(error(const QString&)), &ProgressDialog,
          SLOT(writeError(const QString&)));
      m_HubManager.asyncConnectToHub();
    }
    catch (std::exception& e)
    {
      ProgressDialog.writeError(e.what());
    }

    if (ProgressDialog.exec())
    {
      hubConnectButton()->setText(m_HubButtonDisconnectLabel);
      hubUrlLineEdit()->setEnabled(false);
      for (auto& Widget : m_HubLoginWidgetsAndButton)
      {
        Widget->setVisible(true);
        Widget->setEnabled(true);
      }

      if (m_HubManager.isV0ofAPI())
      {
        hubUsernameLabel()->setText(tr("Username"));
      }
      else
      {
        hubUsernameLabel()->setText(tr("User ID"));
        hubUsernameLabel()->setToolTip(tr("User ID for Hub can be the user email or username uid from LDAP"));
        usernameLineEdit()->setPlaceholderText(tr("Email or username"));
      }
    }
  
    updateHubElementsList();

    openfluid::base::PreferencesManager::instance()->setWaresdevImportHubUrl(hubUrlLineEdit()->text().toStdString());
    postConnectSetup();
    check();
  }
}


// =====================================================================
// =====================================================================


void AbstractSrcImportDialog::onHubLoginButtonClicked()
{
  if (m_HubManager.isLoggedIn())
  {
    hubLoginButton()->setText(m_HubButtonLoginLabel);
    for (auto& Widget : m_HubLoginWidgets)
    {
      Widget->setEnabled(true);
    }
    m_HubManager.logout();
    updateHubElementsList();
    return;
  }
  else
  {
    setMessage();
    if (m_HubManager.isConnected())
    {
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      bool OK = m_HubManager.login(usernameLineEdit()->text().toStdString(), passwordLineEdit()->text().toStdString());
      QApplication::restoreOverrideCursor();
      if (OK)
      {
        hubLoginButton()->setText(m_HubButtonLogoutLabel);
        for (auto& Widget : m_HubLoginWidgets)
        {
          Widget->setEnabled(false);
        }
        updateHubElementsList();

        openfluid::base::PreferencesManager::instance()->setWaresdevImportHubUsername(
          usernameLineEdit()->text().toStdString());  // username is added to preferences when validated by login
      }
      else
      {
        QMessageBox::warning(this, tr("Login error"), tr("Unable to log in with given information"));
        updateHubElementsList();
      }
    }
  }
}


// =====================================================================
// =====================================================================


void AbstractSrcImportDialog::genericItemDisplay(bool AlreadyDisplayed, 
                                                bool NotAuthorized, 
                                                QListWidgetItem* Item, 
                                                const QString& ElementId)
{
  if (!AlreadyDisplayed)
  {
    if (m_HubManager.isLoggedIn())
    {
      if (NotAuthorized)
      {
        Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
        Item->setToolTip(tr("You are not authorized to access to \"%1\"").arg(ElementId));
        Item->setForeground(QColor("orange"));
      }
      else
      {
        Item->setFlags(Item->flags() | Qt::ItemIsEnabled);
        Item->setToolTip("");
        Item->setForeground(QBrush());
      }
    }
    else
    {
      if (NotAuthorized)
      {
        Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
        Item->setToolTip(tr("You must be logged in to clone a ware"));
      }
    }
  }

  Item->setCheckState(Qt::Unchecked);
}


// =====================================================================
// =====================================================================


QStringList AbstractSrcImportDialog::getSelectedElementsFromListWidget(const QListWidget* ListWidget)
{
  QStringList Wares;
  for (const QListWidgetItem* Item : ListWidget->findItems("*", Qt::MatchWildcard))
  {
    if (Item->checkState() == Qt::Checked)
    {
      Wares << Item->data(Qt::UserRole).toString();
    }
  }
  return Wares;
}


// =====================================================================
// =====================================================================


void AbstractSrcImportDialog::setupImportManagerThread(
                                    openfluid::ui::waresdev::SrcImportSequenceManager* SrcImportSequenceManager, 
                                    QThread* Thread, 
                                    openfluid::ui::waresdev::WaresSrcIOProgressDialog* ProgressDialogPtr)
{
  SrcImportSequenceManager->moveToThread(Thread);

  connect(Thread, SIGNAL(started()), SrcImportSequenceManager, SLOT(onCloneRequest()));
  connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

  connect(SrcImportSequenceManager, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
  connect(SrcImportSequenceManager, SIGNAL(finished(bool, const QString&)), ProgressDialogPtr,
          SLOT(finish(bool, const QString&)));
  connect(SrcImportSequenceManager, SIGNAL(finished(bool, const QString&)), SrcImportSequenceManager, 
          SLOT(deleteLater()));

  connect(SrcImportSequenceManager, SIGNAL(info(const QString&)), ProgressDialogPtr, 
          SLOT(writeInfo(const QString&)));
  connect(SrcImportSequenceManager, SIGNAL(warning(const QString&)), ProgressDialogPtr, 
          SLOT(writeWarning(const QString&)));
  connect(SrcImportSequenceManager, SIGNAL(error(const QString&)), ProgressDialogPtr, 
          SLOT(writeError(const QString&)));
  connect(SrcImportSequenceManager, SIGNAL(progressed(int)), ProgressDialogPtr, SLOT(progress(int)));
  
}


// =====================================================================
// =====================================================================


void AbstractSrcImportDialog::runThread(QThread* Thread, 
                                       openfluid::ui::waresdev::WaresSrcIOProgressDialog& ProgressDialog)
{
  try
  {
    Thread->start();
  }
  catch (std::exception& e)
  {
    ProgressDialog.writeError(e.what());
  }

  if (ProgressDialog.exec())
  {
    accept();
  }
}

} } } //namespaces

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
  @file FragmentsSrcImportDialog.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inra.fr>
*/


#include <QFileDialog>
#include <QThread>
#include <QMessageBox>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/config.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/utilsq/GitProxy.hpp>
#include <openfluid/utilsq/CMakeProxy.hpp>
#include <openfluid/ui/waresdev/WaresImportWorker.hpp>
#include <openfluid/ui/waresdev/FragmentsSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/waresdev/AbstractSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/FragmentsImportWorker.hpp>
#include <openfluid/ui/waresdev/WareGitDialog.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>

#include "ui_FragmentsSrcImportDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


FragmentsSrcImportDialog::FragmentsSrcImportDialog(QWidget* Parent, QString WarePath, bool HasWareVersionControl) :
    AbstractSrcImportDialog(Parent), ui(new Ui::FragmentsSrcImportDialog), m_WarePath(WarePath), 
    m_HasWareVersionControl(HasWareVersionControl)
{
  ui->setupUi(this);

  setupMessageUi(tr("Import fragments sources"));

  // Contextual message: when not versioned, explain that only plain copy is available
  if (m_HasWareVersionControl)
  {
    ui->ContextInfoMessage->setText(tr("This ware is under versioning. Fragment can be added as submodule \n"
                                    "to keep its version tracking, or be merged with current code."));
  }
  else
  {
    ui->SubmoduleStratRadioButton->setEnabled(false);
    ui->MergeStratRadioButton->setChecked(true);
    ui->ContextInfoMessage->setText(tr(
      "This ware is not under versioning system, only plain copy of fragment is available.\n"
      "You can initialize the versioning system through the 'Git' > 'Init' option in contextual menu of the ware."
    )); 

  }

  m_GitLoginWidgets << ui->UsernameLineEdit << ui->PasswordLineEdit << ui->GitUrlLineEdit
                                  << ui->UsernameLabel << ui->PasswordLabel;
  m_HubConnectWidgets << ui->HubUrlLineEdit << ui->HubConnectButton;
  m_HubLoginWidgets << ui->HubUsernameLineEdit << ui->HubPasswordLineEdit
                                  << ui->HubUsernameLabel << ui->HubPasswordLabel;
  
  m_HubLoginWidgetsAndButton << m_HubLoginWidgets << ui->HubLoginButton;
  m_HubConnectionInfoWidgets << ui->HubUrlLineEdit << m_HubLoginWidgets;

  ui->HubConnectButton->setText(m_HubButtonConnectLabel);
  
  ui->HubUrlLineEdit->setText(
      QString::fromStdString(openfluid::base::PreferencesManager::instance()->getWaresdevImportHubUrl()));
  
  
  connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(check()));
  connect(ui->GitUrlLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(check()));
  connect(ui->PasswordLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(check()));
  connect(ui->UsernameLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(check()));

  connect(ui->HubLoginButton, SIGNAL(clicked()), this, SLOT(onHubLoginButtonClicked()));
  connect(ui->HubConnectButton, SIGNAL(clicked()), this, SLOT(onHubConnectButtonClicked()));
  connect(ui->HubUrlLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(check()));

  connect(ui->FragmentListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(check()));

  connect(buttonBox(), SIGNAL(accepted()), this, SLOT(onImportAsked()));

  if(!openfluid::utils::GitProxy::isAvailable())
  {
    setMessage(tr("Git program not found"));
  }

  check();
}


// =====================================================================
// =====================================================================


FragmentsSrcImportDialog::~FragmentsSrcImportDialog()
{

}


// =====================================================================
// =====================================================================


void FragmentsSrcImportDialog::clearListWidgets()
{
  ui->FragmentListWidget->clear();
}


// =====================================================================
// =====================================================================


void FragmentsSrcImportDialog::updateHubElementsList()
{
  clearListWidgets();

  if (!mp_HubManager)
  {
    return;
  }
  auto Mgr = openfluid::base::WorkspaceManager::instance();

  QString UserName = QString::fromStdString(mp_HubManager->getUsername());
  QString ErrStr;

  for (const auto& FragmentPair : mp_HubManager->getAvailableFragmentsWithDetails())
  {
    QString FragmentId = QString::fromStdString(FragmentPair.first);
    std::string WarePath = openfluid::tools::Filesystem::joinPath({Mgr->getWorkspacePath(),
                                                                   openfluid::config::FRAGMENTS_PATH,
                                                                   FragmentId.toStdString()});

    bool WareNotAuthorized = !openfluid::waresdev::hasUserAccess(UserName.toStdString(), 
                                                                  FragmentPair.second.ROUsers, 
                                                                  FragmentPair.second.RWUsers); 

    // fragment display

    QString WareUrl = QString::fromStdString(FragmentPair.second.GitUrl);
    QListWidgetItem* Item = new QListWidgetItem(FragmentId);
    Item->setData(Qt::UserRole, WareUrl);

    bool AlreadyDisplayed = fragmentItemDisplay(FragmentId, Item);

    genericItemDisplay(AlreadyDisplayed, WareNotAuthorized, Item, FragmentId);

    ui->FragmentListWidget->addItem(Item);
  }

  check();
}


// =====================================================================
// =====================================================================


QLineEdit* FragmentsSrcImportDialog::hubUrlLineEdit()
{
  return ui->HubUrlLineEdit;
}


// =====================================================================
// =====================================================================


QLineEdit* FragmentsSrcImportDialog::usernameLineEdit()
{
  return ui->HubUsernameLineEdit;
}


// =====================================================================
// =====================================================================


QLineEdit* FragmentsSrcImportDialog::passwordLineEdit()
{
  return ui->HubPasswordLineEdit;
}


// =====================================================================
// =====================================================================


QPushButton* FragmentsSrcImportDialog::hubConnectButton()
{
  return ui->HubConnectButton;
}


// =====================================================================
// =====================================================================


QPushButton* FragmentsSrcImportDialog::hubLoginButton()
{
  return ui->HubLoginButton;
}


// =====================================================================
// =====================================================================


QLabel* FragmentsSrcImportDialog::hubUsernameLabel()
{
  return ui->HubUsernameLabel;
}


// =====================================================================
// =====================================================================


void FragmentsSrcImportDialog::customHideWidget(QWidget* Widget)
{
  Widget->setEnabled(false);
}


// =====================================================================
// =====================================================================


void FragmentsSrcImportDialog::postConnectSetup()
{
  ui->HubUsernameLineEdit->setText(
      QString::fromStdString(openfluid::base::PreferencesManager::instance()->getWaresdevImportHubUsername()));
}


// =====================================================================
// =====================================================================


QStringList FragmentsSrcImportDialog::getSelectedFragments() 
{
  return getSelectedElementsFromListWidget(ui->FragmentListWidget);
}


// =====================================================================
// =====================================================================


bool FragmentsSrcImportDialog::check()
{
  if (ui->tabWidget->currentIndex() == 0)  // HUB TAB
  {
    if (!checkHubSection())
    {
      return false;
    }
    else if (mp_HubManager->isV0ofAPI())
    {
      setMessage(tr("Hub API version too old for fragment import"));
      return false;
    }
    else if (getSelectedFragments().isEmpty())
    {
      setMessage(tr("No fragment selected"));
      return false;
    }
  }
  else  // GIT TAB
  {
    // check if all fields are filled
    if (ui->GitUrlLineEdit->text().isEmpty())
    {
      setMessage(tr("No Git URL defined"));
      return false;
    }
    if (ui->UsernameLineEdit->text().isEmpty())
    {
      setMessage(tr("Missing username"));
      return false;
    }
    if (ui->PasswordLineEdit->text().isEmpty())
    {
      setMessage(tr("Missing password"));
      return false;
    }
  }

  setMessage();
  return true;
}


// =====================================================================
// =====================================================================


bool FragmentsSrcImportDialog::fragmentItemDisplay(const QString WareId, QListWidgetItem* Item)
{
  // returning true if display resolved
  if (isFragmentInWare(WareId.toStdString()))
  {
    Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
    Item->setToolTip(tr("\"%1\" fragment already exists in the ware").arg(WareId));
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


void FragmentsSrcImportDialog::onImportAsked()
{
  if (!check())
  {
    return;
  }

  QStringList FragmentUrls;
  QString Username;
  QString Password;

  if (ui->tabWidget->currentIndex() == 0)  // HUB TAB
  {
    FragmentUrls = getSelectedFragments();
    Username = QString::fromStdString(mp_HubManager->getUsername());
    Password = ui->HubPasswordLineEdit->text();
  }
  else  // GIT TAB
  {
    FragmentUrls << ui->GitUrlLineEdit->text();
    Username = ui->UsernameLineEdit->text();
    Password = ui->PasswordLineEdit->text();
  }
  bool AsSubmodule = ui->SubmoduleStratRadioButton->isChecked();

  openfluid::ui::waresdev::SrcImportSequenceManager* LocalSrcImportSequenceManager = 
    new openfluid::ui::waresdev::SrcImportSequenceManager();
  LocalSrcImportSequenceManager->setupFragmentStrategy(m_WarePath, AsSubmodule);

  openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Importing fragments sources:"), false, this);
  
  if (Username.isNull() || m_WarePath.isNull())
  {
    ProgressDialog.writeError("Missing required information for import, operation cancelled.");
    ProgressDialog.exec();
    return;
  }
  QThread* Thread = new QThread();
  LocalSrcImportSequenceManager->setSelectedFragmentsUrl(FragmentUrls);
  LocalSrcImportSequenceManager->setupUser(Username, Password);
  setupImportManagerThread(LocalSrcImportSequenceManager, Thread, &ProgressDialog);
  runThread(Thread, ProgressDialog);
}


// =====================================================================
// =====================================================================


bool FragmentsSrcImportDialog::isFragmentInWare(std::string FragmentName)
{
  openfluid::tools::FilesystemPath FragmentTheoricalPath({m_WarePath.toStdString(), 
                                                          openfluid::config::WARESDEV_SRC_DIR, 
                                                          openfluid::config::FRAGMENTS_PATH, 
                                                          FragmentName});
  return FragmentTheoricalPath.isDirectory();
}


} } } //namespaces

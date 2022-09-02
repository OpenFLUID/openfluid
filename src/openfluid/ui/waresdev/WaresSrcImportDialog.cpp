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
  @file WaresSrcImportDialog.cpp

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
#include <openfluid/utilsq/GitProxy.hpp>
#include <openfluid/utilsq/CMakeProxy.hpp>
#include <openfluid/ui/waresdev/WaresDevPackage.hpp>
#include <openfluid/ui/waresdev/WaresImportWorker.hpp>
#include <openfluid/ui/waresdev/WaresSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/waresdev/AbstractSrcImportDialog.hpp>
#include <openfluid/waresdev/WareSrcEnquirer.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>

#include "ui_WaresSrcImportDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WaresSrcImportDialog::WaresSrcImportDialog(QWidget* Parent) :
    AbstractSrcImportDialog(Parent), ui(new Ui::WaresSrcImportDialog)
{
  ui->setupUi(this);

  setupMessageUi(tr("Import wares sources"));

  m_ListWidgetsByWareType[openfluid::ware::WareType::SIMULATOR] = ui->SimListWidget;
  m_ListWidgetsByWareType[openfluid::ware::WareType::OBSERVER] = ui->ObsListWidget;
  m_ListWidgetsByWareType[openfluid::ware::WareType::BUILDEREXT] = ui->ExtListWidget;

  m_FilterWidgetsByWareType[openfluid::ware::WareType::SIMULATOR] = ui->SimFilterWidget;
  m_FilterWidgetsByWareType[openfluid::ware::WareType::OBSERVER] = ui->ObsFilterWidget;
  m_FilterWidgetsByWareType[openfluid::ware::WareType::BUILDEREXT] = ui->ExtFilterWidget;

  m_SourceBtGroup.addButton(ui->PackageRadioButton);
  m_SourceBtGroup.addButton(ui->HubRadioButton);
  m_SourceBtGroup.setExclusive(true);

  ui->PackagePathLabel->setText(m_PackagePathLabelDefault);

  m_HubLoginWidgets << ui->UsernameLineEdit << ui->PasswordLineEdit
                                  << ui->UsernameLabel << ui->PasswordLabel;
  m_HubLoginWidgetsAndButton << m_HubLoginWidgets << ui->HubLoginButton;
  m_HubConnectionInfoWidgets << ui->HubUrlLineEdit << m_HubLoginWidgets;

  ui->HubConnectButton->setText(m_HubButtonConnectLabel);
  
  ui->HubUrlLineEdit->setText(
      QString::fromStdString(openfluid::base::PreferencesManager::instance()->getWaresdevImportHubUrl()));

  connect(&m_SourceBtGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onSourceChanged(QAbstractButton*)));

  connect(ui->PackagePathButton, SIGNAL(clicked()), this, SLOT(onPackagePathButtonClicked()));
  connect(ui->HubConnectButton, SIGNAL(clicked()), this, SLOT(onHubConnectButtonClicked()));
  connect(ui->HubLoginButton, SIGNAL(clicked()), this, SLOT(onHubLoginButtonClicked()));

  connect(ui->HubUrlLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(check()));

  for (const auto& Pair : m_ListWidgetsByWareType)
  {
    connect(Pair.second, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(check()));
  }

  for (const auto& Pair : m_FilterWidgetsByWareType)
  {
    connect(Pair.second, SIGNAL(filterChanged()), this, SLOT(onWaresListRefreshAsked()));
  }
  
  connect(buttonBox(), SIGNAL(accepted()), this, SLOT(onImportAsked()));

  ui->PackageRadioButton->click();

  if(!openfluid::utils::GitProxy::isAvailable())
  {
    ui->HubRadioButton->setEnabled(false);
    ui->HubRadioButton->setToolTip(tr("Git program not found"));
  }

  if(!openfluid::utils::CMakeProxy::isAvailable())
  {
    ui->PackagePathButton->setEnabled(false);
    ui->HubRadioButton->setToolTip(tr("CMake program not found"));
    ui->HubConnectButton->click();
  }


  // Hide login fields
  for (auto& Widget : m_HubLoginWidgetsAndButton)
  {
    Widget->setVisible(false);
  }

  check();
}


// =====================================================================
// =====================================================================


WaresSrcImportDialog::~WaresSrcImportDialog()
{
  delete mp_ImportFilePkg;
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::clearListWidgets()
{
  for (const auto& WidgetPair : m_ListWidgetsByWareType)
  {
    WidgetPair.second->clear();
  }
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::updateHubElementsList()
{
  clearListWidgets();

  if (!mp_HubManager)
  {
    return;
  }

  auto Mgr = openfluid::base::WorkspaceManager::instance();

  QString UserName = QString::fromStdString(mp_HubManager->getUsername());
  QString ErrStr;

  for (const auto& Pair : m_ListWidgetsByWareType)
  {
    openfluid::ware::WareType Type = Pair.first;
    for (const auto& WarePair : mp_HubManager->getAvailableWaresWithDetails(Type))
    {
      QString WareId = QString::fromStdString(WarePair.first);
      std::string WarePath = Mgr->getWarePath(Type,WareId.toStdString());
      bool WareInWorkspace = openfluid::waresdev::isWareInCurrentWorkspace(WarePath);

      bool WareNotAuthorized = !openfluid::waresdev::hasUserAccess(UserName.toStdString(), 
                                                                   WarePair.second.ROUsers, 
                                                                   WarePair.second.RWUsers); 

      if (isWareDisplayed(Type, WareId, WareInWorkspace, WareNotAuthorized))
      {
        // ware display

        QString WareUrl = QString::fromStdString(WarePair.second.GitUrl);
        QListWidgetItem* Item = new QListWidgetItem(WareId);
        Item->setData(Qt::UserRole, WareUrl);

        bool AlreadyDisplayed = wareItemDisplay(Type, WareId, Item);
        genericItemDisplay(AlreadyDisplayed, WareNotAuthorized, Item, WareId);

        m_ListWidgetsByWareType[Type]->addItem(Item);
      }
    }
  }

  check();
}


// =====================================================================
// =====================================================================


QLineEdit* WaresSrcImportDialog::hubUrlLineEdit()
{
  return ui->HubUrlLineEdit;
}


// =====================================================================
// =====================================================================


QLineEdit* WaresSrcImportDialog::usernameLineEdit()
{
  return ui->UsernameLineEdit;
}


// =====================================================================
// =====================================================================


QLineEdit* WaresSrcImportDialog::passwordLineEdit()
{
  return ui->PasswordLineEdit;
}


// =====================================================================
// =====================================================================


QPushButton* WaresSrcImportDialog::hubConnectButton()
{
  return ui->HubConnectButton;
}


// =====================================================================
// =====================================================================


QPushButton* WaresSrcImportDialog::hubLoginButton()
{
  return ui->HubLoginButton;
}


// =====================================================================
// =====================================================================


QLabel* WaresSrcImportDialog::hubUsernameLabel()
{
  return ui->UsernameLabel;
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::customHideWidget(QWidget* Widget)
{
  Widget->setVisible(false);
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::postConnectSetup()
{
  updatePackageInfo();
}


// =====================================================================
// =====================================================================


QStringList WaresSrcImportDialog::getSelectedWares()
{
  QStringList Wares;
  for (const auto& Pair : m_ListWidgetsByWareType)
  {
    Wares << getSelectedElementsFromListWidget(Pair.second);
  }
  return Wares;
}


// =====================================================================
// =====================================================================


bool WaresSrcImportDialog::wareItemDisplay(const openfluid::ware::WareType Type, const QString WareId, 
                                           QListWidgetItem* Item)
{
  // returning true if display resolved
  std::string WarePath = openfluid::waresdev::WareSrcEnquirer::findWarePath(Type, WareId.toStdString());
  bool WareInWorkspace = openfluid::waresdev::isWareInCurrentWorkspace(WarePath);
  if (WareInWorkspace)
  {
    Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
    Item->setToolTip(tr("\"%1\" already exists in the workspace").arg(WareId));
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool WaresSrcImportDialog::check()
{
  if (ui->PackageRadioButton->isChecked() && (ui->PackagePathLabel->text().isEmpty()
                                              || ui->PackagePathLabel->text() == m_PackagePathLabelDefault))
  {
    setMessage(tr("No package file selected"));
    return false;
  }
  else if (ui->HubRadioButton->isChecked())
  {
    if (!checkHubSection())
    {
      return false;
    }
  }

  if (getSelectedWares().isEmpty())
  {
    setMessage(tr("No ware selected"));
    return false;
  }
  setMessage();
  return true;
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onSourceChanged(QAbstractButton* ClickedButton)
{
  if (ClickedButton == ui->PackageRadioButton)
  {
    ui->WaresGroupBox->setTitle(tr("Available wares in package"));

    for (auto& Widget : m_HubConnectionInfoWidgets)
    {
      Widget->setEnabled(false);
    }
    ui->HubConnectButton->setEnabled(false);
    for (const auto& Pair : m_FilterWidgetsByWareType)
    {
      Pair.second->showAuthorizationFilterCheckbox(false);
    }

    updatePackageWaresList();
  }
  else
  {
    ui->WaresGroupBox->setTitle(tr("Available wares on Hub"));
    ui->HubUrlLineEdit->setEnabled(!mp_HubManager || !mp_HubManager->isConnected());
    for (const auto& Pair : m_FilterWidgetsByWareType)
    {
      Pair.second->showAuthorizationFilterCheckbox(true);
    }

    updateHubElementsList();
    ui->UsernameLineEdit->setText(
      QString::fromStdString(openfluid::base::PreferencesManager::instance()->getWaresdevImportHubUsername()));
  }

  updatePackageInfo();
  check();
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onPackagePathButtonClicked()
{
  QString PackageFilePath = QFileDialog::getOpenFileName(this, tr("Select package file"),
                                                         QDir::homePath(),
                                                         tr("OpenFLUID wares packages (*.ofwdp)"));

  if (PackageFilePath.isEmpty())
  {
    return;
  }


  ui->PackagePathLabel->setText(PackageFilePath);


  QThread* Thread = new QThread();

  openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Fetching information from ware sources:"), true,
                                                                   this);

  try
  {
    mp_ImportFilePkg = new WaresDevImportPackage(PackageFilePath);

    mp_ImportFilePkg->moveToThread(Thread);

    connect(Thread, SIGNAL(started()), mp_ImportFilePkg, SLOT(fetchInformation()));
    connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

    connect(mp_ImportFilePkg, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
    connect(mp_ImportFilePkg, SIGNAL(finished(bool, const QString&)), &ProgressDialog,
            SLOT(finishAndQuit(bool, const QString&)));

    connect(mp_ImportFilePkg, SIGNAL(info(const QString&)), &ProgressDialog, SLOT(writeInfo(const QString&)));
    connect(mp_ImportFilePkg, SIGNAL(error(const QString&)), &ProgressDialog, SLOT(writeError(const QString&)));

    Thread->start();
  }
  catch (std::exception& e)
  {
    ProgressDialog.writeError(e.what());
  }

  ProgressDialog.exec();

  updatePackageInfo();
  updatePackageWaresList();
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onHubConnectButtonClicked()
{
  if (mp_HubManager && mp_HubManager->isConnected())
  {
    m_AlreadySelectedHubWares = QMap<openfluid::ware::WareType, QStringList>(getSelectedWaresByType());
  }
  AbstractSrcImportDialog::onHubConnectButtonClicked();
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onWaresListRefreshAsked()
{
  if (ui->PackageRadioButton->isChecked())
  {
    updatePackageWaresList();
  }
  else
  {
    updateHubElementsList();
  } 
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::updatePackageInfo()
{
  if (mp_ImportFilePkg)
  {
    ui->PackagersLabel->setText(mp_ImportFilePkg->getPackagers());
    ui->PackageDescriptionLabel->setText(mp_ImportFilePkg->getPackageDescription());
  }
  else
  {
    ui->PackagersLabel->setText("");
    ui->PackageDescriptionLabel->setText("");
  }

  ui->WaresTabWidget->setCurrentIndex(0);
}


// =====================================================================
// =====================================================================


bool WaresSrcImportDialog::isWareDisplayed(const openfluid::ware::WareType& Type, const QString WareId, 
                                           const bool WareInWorkspace, const bool WareNotAuthorized)
{
  if (!m_FilterWidgetsByWareType[Type]->areAlreadyImportedWaresEnabled() && WareInWorkspace)
  {
    return false;
  }
  if (!m_FilterWidgetsByWareType[Type]->areUnauthorizedWaresEnabled() && WareNotAuthorized)
  {
    return false;
  }
  QString FilterString = m_FilterWidgetsByWareType[Type]->getFilteringText();
  if (!FilterString.isEmpty() && !WareId.contains(FilterString, Qt::CaseInsensitive))
  {
    return false;
  }
  return true;
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::updatePackageWaresList()
{
  for (const auto& Pair : m_ListWidgetsByWareType)
  {
    Pair.second->clear();
  }

  if (!mp_ImportFilePkg)
  {
    return;
  }

  QDir WaresDevDir(QString::fromStdString(openfluid::base::WorkspaceManager::instance()->getWaresPath()));
  QDir PackageTempDir = mp_ImportFilePkg->getPackageTempDir();

  for (const QString& WarePath : mp_ImportFilePkg->getWaresPaths())
  {
    QFileInfo WareFileInfo(WarePath);

    QString WareName = WareFileInfo.fileName();
    QString WareTypeName = WareFileInfo.dir().dirName();

    bool WareInWorkspace = WaresDevDir.exists(PackageTempDir.relativeFilePath(WarePath));

    if (isWareDisplayed(m_WareTypeConverter[WareTypeName], WareName, WareInWorkspace, false))
    {
      QListWidgetItem* Item = new QListWidgetItem(WareName);
      Item->setData(Qt::UserRole, WareFileInfo.absoluteFilePath());

      if (WareInWorkspace)
      {
        Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
        Item->setCheckState(Qt::Unchecked);
        Item->setToolTip(QString(tr("\"%1\" already exists in the workspace")).arg(WareName));
      }
      else
      {
        Item->setCheckState(Qt::Checked);
        Item->setToolTip("");
      }

      if (m_WareTypeConverter.count(WareTypeName))
      {
        m_ListWidgetsByWareType[m_WareTypeConverter[WareTypeName]]->addItem(Item);
      }
    }
  }

  check();
}


// =====================================================================
// =====================================================================


std::map<openfluid::ware::WareType, QStringList> WaresSrcImportDialog::getSelectedWaresByType()
{
  std::map<openfluid::ware::WareType, QStringList> Wares;

  for (auto& Pair : m_ListWidgetsByWareType)
  {
    for (const QListWidgetItem* Item : Pair.second->findItems("*", Qt::MatchWildcard))
    {
      if (Item->checkState() == Qt::Checked)
      {
        Wares[Pair.first] << Item->data(Qt::UserRole).toString();
      }
    }
  }
  return Wares;
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onImportAsked()
{
  //FIXME black rectangle shortly covering progress dialog during initializing?
  if (!check())
  {
    return;
  }

  openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Importing wares sources:"), false, this);

  QThread* Thread = new QThread();

  if (ui->PackageRadioButton->isChecked())
  {
    connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));
    if (!mp_ImportFilePkg)
    {
      return;
    }

    mp_ImportFilePkg->setSelectedWares(getSelectedWares());

    mp_ImportFilePkg->moveToThread(Thread);

    connect(Thread, SIGNAL(started()), mp_ImportFilePkg, SLOT(copyWares()));

    connect(mp_ImportFilePkg, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
    connect(mp_ImportFilePkg, SIGNAL(finished(bool, const QString&)), &ProgressDialog,
            SLOT(finish(bool, const QString&)));

    connect(mp_ImportFilePkg, SIGNAL(info(const QString&)), &ProgressDialog, SLOT(writeInfo(const QString&)));
    connect(mp_ImportFilePkg, SIGNAL(error(const QString&)), &ProgressDialog, SLOT(writeError(const QString&)));
    connect(mp_ImportFilePkg, SIGNAL(progressed(int)), &ProgressDialog, SLOT(progress(int)));
    runThread(Thread, ProgressDialog);
  }
  else
  {
    if (!mp_HubManager)
    {
      return;
    }

    SrcImportSequenceManager* LocalSrcImportSequenceManager = new SrcImportSequenceManager();
    QString Username = QString::fromStdString(mp_HubManager->getUsername());
    QString Password = QString::fromStdString(mp_HubManager->getPassword());
    LocalSrcImportSequenceManager->setSelectedWaresUrl(getSelectedWaresByType());
    LocalSrcImportSequenceManager->setupUser(Username, Password);
    setupImportManagerThread(LocalSrcImportSequenceManager, Thread, &ProgressDialog);
    runThread(Thread, ProgressDialog);
    
    delete LocalSrcImportSequenceManager;
  }
  
}


} } } //namespaces

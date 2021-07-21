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
  @author Armel THONI <armel.thoni@inra.fr>
*/


#include <QFileDialog>
#include <QThread>
#include <QMessageBox>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/waresdev/WaresDevPackage.hpp>
#include <openfluid/waresdev/FluidHubWaresImportWorker.hpp>
#include <openfluid/ui/waresdev/WaresSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_WaresSrcImportDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WaresSrcImportDialog::WaresSrcImportDialog(QWidget* Parent) :
    openfluid::ui::common::MessageDialog(Parent), ui(new Ui::WaresSrcImportDialog)
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
  
  connect(buttonBox(), SIGNAL(accepted()), this, SLOT(import()));

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
  delete mp_FluidHubWaresImportWorker;
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
    if (ui->HubUrlLineEdit->text().isEmpty())
    {
      setMessage(tr("No Hub URL defined"));
      ui->HubConnectButton->setEnabled(false);
      return false;
    }
    else
    {
      setMessage("");
      ui->HubConnectButton->setEnabled(true);
    }

    if (!mp_FluidHubWaresImportWorker || !mp_FluidHubWaresImportWorker->isConnected())
    {
      setMessage(tr("Not connected to a Hub"));
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
    ui->HubUrlLineEdit->setEnabled(!m_IsConnectedToHub);
    for (const auto& Pair : m_FilterWidgetsByWareType)
    {
      Pair.second->showAuthorizationFilterCheckbox(true);
    }

    updateHubWaresList();
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
    mp_ImportFilePkg = new openfluid::waresdev::WaresDevImportPackage(PackageFilePath);

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
  if (m_IsConnectedToHub)
  {
    m_IsConnectedToHub = false;
    m_AlreadySelectedHubWares = QMap<openfluid::ware::WareType, QStringList>(getSelectedWaresByType());

    if (mp_FluidHubWaresImportWorker)
    {
      mp_FluidHubWaresImportWorker->disconnect();
    }

    for (const auto& WidgetPair : m_ListWidgetsByWareType)
    {
      WidgetPair.second->clear();
    }

    ui->HubConnectButton->setText(m_HubButtonConnectLabel);

    for (auto& Widget : m_HubConnectionInfoWidgets)
    {
      Widget->setEnabled(true);
    }

    m_IsLoggedInHub = false;
    ui->HubLoginButton->setText(m_HubButtonLoginLabel);

    // Hide login fields
    ui->HubUrlLineEdit->setEnabled(true);
    for (auto& Widget : m_HubLoginWidgetsAndButton)
    {
      Widget->setVisible(false);
    }

    return;
  }
  else
  {
    QString HubUrl = ui->HubUrlLineEdit->text();

    if (HubUrl.isEmpty())
    {
      return;
    }

    if (mp_FluidHubWaresImportWorker)
    {
      delete mp_FluidHubWaresImportWorker;
      mp_FluidHubWaresImportWorker = nullptr;
    }

    QThread* Thread = new QThread();

    openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Connecting to Hub:"), true, this);

    try
    {
      bool SslNoVerify = openfluid::base::PreferencesManager::instance()->isWaresdevGitSslNoVerify();
      mp_FluidHubWaresImportWorker = new openfluid::waresdev::FluidHubWaresImportWorker(HubUrl, SslNoVerify);

      mp_FluidHubWaresImportWorker->moveToThread(Thread);

      connect(Thread, SIGNAL(started()), mp_FluidHubWaresImportWorker, SLOT(connect()));
      connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

      connect(mp_FluidHubWaresImportWorker, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
      connect(mp_FluidHubWaresImportWorker, SIGNAL(finished(bool, const QString&)), &ProgressDialog,
              SLOT(finishAndQuit(bool, const QString&)));

      connect(mp_FluidHubWaresImportWorker, SIGNAL(info(const QString&)), &ProgressDialog,
              SLOT(writeInfo(const QString&)));
      connect(mp_FluidHubWaresImportWorker, SIGNAL(error(const QString&)), &ProgressDialog,
              SLOT(writeError(const QString&)));

      Thread->start();
    }
    catch (std::exception& e)
    {
      ProgressDialog.writeError(e.what());
    }

    if (ProgressDialog.exec())
    {
      ui->HubConnectButton->setText(m_HubButtonDisconnectLabel);
      m_IsConnectedToHub = true;

      ui->HubUrlLineEdit->setEnabled(false);
      for (auto& Widget : m_HubLoginWidgetsAndButton)
      {
        Widget->setVisible(true);
        Widget->setEnabled(true);
      }

      if (mp_FluidHubWaresImportWorker->isV0ofAPI())
      {
        ui->UsernameLabel->setText(tr("Username"));
      }
      else
      {
        ui->UsernameLabel->setText(tr("Email"));
      }
    }
  
    updatePackageInfo();
    updateHubWaresList();

    openfluid::base::PreferencesManager::instance()->setWaresdevImportHubUrl(ui->HubUrlLineEdit->text().toStdString());
    openfluid::base::PreferencesManager::instance()->setWaresdevImportHubUsername(
      ui->UsernameLineEdit->text().toStdString());
  }
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onHubLoginButtonClicked()
{
  if (m_IsLoggedInHub)
  {
    m_IsLoggedInHub = false;
    ui->HubLoginButton->setText(m_HubButtonLoginLabel);
    for (auto& Widget : m_HubLoginWidgets)
    {
      Widget->setEnabled(true);
    }
    mp_FluidHubWaresImportWorker->logout();
    updateHubWaresList();
    return;
  }
  else
  {
    setMessage();
    if (mp_FluidHubWaresImportWorker)
    {
      if (mp_FluidHubWaresImportWorker->login(ui->UsernameLineEdit->text(), ui->PasswordLineEdit->text()))
      {
        m_IsLoggedInHub = true;
        ui->HubLoginButton->setText(m_HubButtonLogoutLabel);
        for (auto& Widget : m_HubLoginWidgets)
        {
          Widget->setEnabled(false);
        }
        updateHubWaresList();
      }
      else
      {
        QMessageBox::warning(this, tr("Login error"), tr("Unable to log in with given information"));
      }
    }
  }
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
    updateHubWaresList();
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

  QDir WaresDevDir(openfluid::waresdev::WareSrcManager::instance()->getWaresdevPath());
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


void WaresSrcImportDialog::updateHubWaresList()
{
  for (const auto& Pair : m_ListWidgetsByWareType)
  {
    Pair.second->clear();
  }

  if (!mp_FluidHubWaresImportWorker)
  {
    return;
  }

  openfluid::waresdev::WareSrcManager* Mgr = openfluid::waresdev::WareSrcManager::instance();

  QString UserName = mp_FluidHubWaresImportWorker->getUsername();
  QString ErrStr;

  for (const auto& WidgetPair : m_ListWidgetsByWareType)
  {
    openfluid::ware::WareType Type = WidgetPair.first;

    for (const auto& WarePair : mp_FluidHubWaresImportWorker->getAvailableWaresWithDetails(Type))
    {
      QString WareId = QString::fromStdString(WarePair.first);
      std::set<std::string> Users = WarePair.second.ROUsers;

      bool WareInWorkspace = !Mgr->getWarePath(WareId, Type, ErrStr).isEmpty();
      bool WareNotAuthorized = !Users.count("*") && !Users.count(UserName.toStdString());

      if (isWareDisplayed(Type, WareId, WareInWorkspace, WareNotAuthorized))
      {

        QString WareUrl = QString::fromStdString(WarePair.second.GitUrl);
        QListWidgetItem* Item = new QListWidgetItem(WareId);

        Item->setData(Qt::UserRole, WareUrl);

        if (WareInWorkspace)
        {
          Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
          Item->setToolTip(tr("\"%1\" already exists in the workspace").arg(WareId));
        }
        else if (mp_FluidHubWaresImportWorker->isLoggedIn())
        {
          if (WareNotAuthorized)
          {
            Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
            Item->setToolTip(tr("You are not authorized to access to \"%1\"").arg(WareId));
            Item->setForeground(QColor("orange"));
          }
        }
        else
        {
          Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
          Item->setToolTip(tr("You must be logged in to clone a ware"));
        }

        Item->setCheckState(
            (m_AlreadySelectedHubWares.contains(Type) && !m_AlreadySelectedHubWares[Type].filter(
                QFileInfo(WareUrl).fileName()).isEmpty()) ? Qt::Checked : Qt::Unchecked);

        m_ListWidgetsByWareType[Type]->addItem(Item);
      }
    }
  }

  check();
}


// =====================================================================
// =====================================================================


QStringList WaresSrcImportDialog::getSelectedWares()
{
  QStringList Wares;

  for (auto& Pair : m_ListWidgetsByWareType)
  {
    for (const QListWidgetItem* Item : Pair.second->findItems("*", Qt::MatchWildcard))
    {
      if (Item->checkState() == Qt::Checked)
      {
        Wares << Item->data(Qt::UserRole).toString();
      }
    }
  }

  return Wares;
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


void WaresSrcImportDialog::import()
{
  if (!check())
  {
    return;
  }

  openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Importing wares sources:"), false, this);

  QThread* Thread = new QThread();
  connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

  if (ui->PackageRadioButton->isChecked())
  {
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
  }
  else
  {
    if (!mp_FluidHubWaresImportWorker)
    {
      return;
    }

    mp_FluidHubWaresImportWorker->setSelectedWaresUrl(getSelectedWaresByType());

    mp_FluidHubWaresImportWorker->moveToThread(Thread);

    connect(Thread, SIGNAL(started()), mp_FluidHubWaresImportWorker, SLOT(clone()));

    connect(mp_FluidHubWaresImportWorker, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
    connect(mp_FluidHubWaresImportWorker, SIGNAL(finished(bool, const QString&)), &ProgressDialog,
            SLOT(finish(bool, const QString&)));

    connect(mp_FluidHubWaresImportWorker, SIGNAL(info(const QString&)), &ProgressDialog, 
           SLOT(writeInfo(const QString&)));
    connect(mp_FluidHubWaresImportWorker, SIGNAL(error(const QString&)), &ProgressDialog, 
            SLOT(writeError(const QString&)));
    connect(mp_FluidHubWaresImportWorker, SIGNAL(progressed(int)), &ProgressDialog, SLOT(progress(int)));
  }


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

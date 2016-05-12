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
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QFileDialog>
#include <QThread>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/utils/GitHelper.hpp>
#include <openfluid/waresdev/WaresDevPackage.hpp>
#include <openfluid/waresdev/WaresHubImportWorker.hpp>
#include <openfluid/ui/waresdev/WaresSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_WaresSrcImportDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WaresSrcImportDialog::WaresSrcImportDialog(QWidget* Parent) :
    QDialog(Parent), ui(new Ui::WaresSrcImportDialog)
{
  ui->setupUi(this);

  m_ListWidgetsByWareType[openfluid::ware::WareType::SIMULATOR] = ui->SimListWidget;
  m_ListWidgetsByWareType[openfluid::ware::WareType::OBSERVER] = ui->ObsListWidget;
  m_ListWidgetsByWareType[openfluid::ware::WareType::BUILDEREXT] = ui->ExtListWidget;

  m_SourceBtGroup.addButton(ui->PackageRadioButton);
  m_SourceBtGroup.addButton(ui->WareshubRadioButton);
  m_SourceBtGroup.setExclusive(true);

  ui->PackagePathLabel->setText(m_PackagePathLabelDefault);

  m_WaresHubConnectionInfoWidgets << ui->WareshubUrlLineEdit << ui->UsernameLineEdit << ui->PasswordLineEdit
                                  << ui->SslNoVerifyCheckBox
                                  << ui->UsernameLabel << ui->PasswordLabel;

  ui->SslNoVerifyCheckBox->setChecked(openfluid::base::PreferencesManager::instance()->isWaresdevGitSslNoVerify());
  ui->WareshubUrlLineEdit->setText(
      openfluid::base::PreferencesManager::instance()->getWaresdevImportWaresHubLastUrl());
  ui->UsernameLineEdit->setText(
      openfluid::base::PreferencesManager::instance()->getWaresdevImportWaresHubLastUsername());

  connect(&m_SourceBtGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onSourceChanged(QAbstractButton*)));

  connect(ui->PackagePathButton, SIGNAL(clicked()), this, SLOT(onPackagePathButtonClicked()));
  connect(ui->WareshubConnectButton, SIGNAL(clicked()), this, SLOT(onWareshubConnectButtonClicked()));

  connect(ui->WareshubUrlLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(check()));

  for (const auto& Pair : m_ListWidgetsByWareType)
    connect(Pair.second, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(check()));

  connect(ui->ButtonBox, SIGNAL(accepted()), this, SLOT(import()));

  ui->PackageRadioButton->click();

  if(!openfluid::utils::GitHelper::checkGitProgram())
  {
    ui->WareshubRadioButton->setEnabled(false);
    ui->WareshubRadioButton->setToolTip(tr("Git program not found"));
  }

  if(!openfluid::waresdev::WaresDevPackage::checkCMakeProgram())
  {
    ui->PackagePathButton->setEnabled(false);
    ui->WareshubRadioButton->setToolTip(tr("CMake program not found"));
    ui->WareshubConnectButton->click();
  }

  check();
}


// =====================================================================
// =====================================================================


WaresSrcImportDialog::~WaresSrcImportDialog()
{
  delete mp_ImportFilePkg;
  delete mp_WaresHubImportWorker;
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
  else if (ui->WareshubRadioButton->isChecked())
  {
    if (ui->WareshubUrlLineEdit->text().isEmpty())
    {
      setMessage(tr("No WaresHub URL defined"));
      ui->WareshubConnectButton->setEnabled(false);
      return false;
    }
    else
    {
      setMessage("");
      ui->WareshubConnectButton->setEnabled(true);
    }

    if (!mp_WaresHubImportWorker || !mp_WaresHubImportWorker->isConnected())
    {
      setMessage(tr("Not connected to a WaresHub"));
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


void WaresSrcImportDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;").arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Import wares sources"));
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->MessageFrame->setStyleSheet(
        QString("background-color: %1").arg(openfluid::ui::config::DIALOGBANNER_WARNBGCOLOR));
    ui->MessageLabel->setText(Msg);
  }
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onSourceChanged(QAbstractButton* ClickedButton)
{
  if (ClickedButton == ui->PackageRadioButton)
  {
    ui->WaresGroupBox->setTitle(tr("Available wares in package"));

    for (auto& Widget : m_WaresHubConnectionInfoWidgets)
      Widget->setEnabled(false);
    ui->WareshubConnectButton->setEnabled(false);

    updatePackageWaresList();
  }
  else
  {
    ui->WaresGroupBox->setTitle(tr("Available wares on WaresHub site"));

    bool DisableConnInfos = (ui->WareshubConnectButton->text() == m_WaresHubButtonDisconnectLabel);
    for (auto& Widget : m_WaresHubConnectionInfoWidgets)
      Widget->setEnabled(!DisableConnInfos);
    ui->WareshubConnectButton->setEnabled(true);

    updateWaresHubWaresList();
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
    return;


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


void WaresSrcImportDialog::onWareshubConnectButtonClicked()
{
  if (ui->WareshubConnectButton->text() == m_WaresHubButtonDisconnectLabel)
  {
    m_AlreadySelectedWaresHubWares = QMap<openfluid::ware::WareType, QStringList>(getSelectedWaresByType());

    if (mp_WaresHubImportWorker)
      mp_WaresHubImportWorker->disconnect();

    for (const auto& WidgetPair : m_ListWidgetsByWareType)
      WidgetPair.second->clear();

    ui->WareshubConnectButton->setText(tr("Connect"));

    for (auto& Widget : m_WaresHubConnectionInfoWidgets)
      Widget->setEnabled(true);


    return;
  }

  QString WaresHubUrl = ui->WareshubUrlLineEdit->text();

  if (WaresHubUrl.isEmpty())
    return;

  if (!WaresHubUrl.contains("www.openfluid-project.org/resources/fluidhub-api/testing"))
    WaresHubUrl.append("/api");

  if (mp_WaresHubImportWorker)
  {
    delete mp_WaresHubImportWorker;
    mp_WaresHubImportWorker = nullptr;
  }

  QThread* Thread = new QThread();

  openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Connecting to WaresHub:"), true, this);

  try
  {
    mp_WaresHubImportWorker = new openfluid::waresdev::WaresHubImportWorker(WaresHubUrl,
                                                                            ui->UsernameLineEdit->text(),
                                                                            ui->PasswordLineEdit->text(),
                                                                            ui->SslNoVerifyCheckBox->isChecked());

    mp_WaresHubImportWorker->moveToThread(Thread);

    connect(Thread, SIGNAL(started()), mp_WaresHubImportWorker, SLOT(connect()));
    connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

    connect(mp_WaresHubImportWorker, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
    connect(mp_WaresHubImportWorker, SIGNAL(finished(bool, const QString&)), &ProgressDialog,
            SLOT(finishAndQuit(bool, const QString&)));

    connect(mp_WaresHubImportWorker, SIGNAL(info(const QString&)), &ProgressDialog,
            SLOT(writeInfo(const QString&)));
    connect(mp_WaresHubImportWorker, SIGNAL(error(const QString&)), &ProgressDialog,
            SLOT(writeError(const QString&)));

    Thread->start();
  }
  catch (std::exception& e)
  {
    ProgressDialog.writeError(e.what());
  }

  if (ProgressDialog.exec())
  {
    ui->WareshubConnectButton->setText(m_WaresHubButtonDisconnectLabel);

    for (auto& Widget : m_WaresHubConnectionInfoWidgets)
      Widget->setEnabled(false);
  }

  updatePackageInfo();
  updateWaresHubWaresList();

  openfluid::base::PreferencesManager::instance()->setWaresdevImportWaresHubLastUrl(ui->WareshubUrlLineEdit->text());
  openfluid::base::PreferencesManager::instance()->setWaresdevImportWaresHubLastUsername(ui->UsernameLineEdit->text());
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


void WaresSrcImportDialog::updatePackageWaresList()
{
  for (const auto& Pair : m_ListWidgetsByWareType)
    Pair.second->clear();

  if (!mp_ImportFilePkg)
    return;

  QDir WaresDevDir(openfluid::waresdev::WareSrcManager::instance()->getWaresdevPath());
  QDir PackageTempDir = mp_ImportFilePkg->getPackageTempDir();

  for (const QString& WarePath : mp_ImportFilePkg->getWaresPaths())
  {
    QFileInfo WareFileInfo(WarePath);

    QString WareName = WareFileInfo.fileName();
    QString WareTypeName = WareFileInfo.dir().dirName();

    QListWidgetItem* Item = new QListWidgetItem(WareName);
    Item->setData(Qt::UserRole, WareFileInfo.absoluteFilePath());

    if (WaresDevDir.exists(PackageTempDir.relativeFilePath(WarePath)))
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
      m_ListWidgetsByWareType[m_WareTypeConverter[WareTypeName]]->addItem(Item);
  }

  check();
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::updateWaresHubWaresList()
{
  for (const auto& Pair : m_ListWidgetsByWareType)
    Pair.second->clear();

  if (!mp_WaresHubImportWorker)
    return;

  openfluid::waresdev::WareSrcManager* Mgr = openfluid::waresdev::WareSrcManager::instance();
  QString UserName = ui->UsernameLineEdit->text();
  QString ErrStr;

  for (const auto& WidgetPair : m_ListWidgetsByWareType)
  {
    openfluid::ware::WareType Type = WidgetPair.first;

    for (const auto& WarePair : mp_WaresHubImportWorker->getAvailableWaresWithDetails(Type))
    {
      QString WareId = QString::fromStdString(WarePair.first);
      QString WareUrl = QString::fromStdString(WarePair.second.GitUrl);

      QListWidgetItem* Item = new QListWidgetItem(WareId);

      Item->setData(Qt::UserRole, WareUrl);

      std::set<std::string> Users = WarePair.second.ROUsers;

      if (!Mgr->getWarePath(WareId, Type, ErrStr).isEmpty())
      {
        Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
        Item->setToolTip(tr("\"%1\" already exists in the workspace").arg(WareId));
      }
      else if (!Users.count("*") && !Users.count(UserName.toStdString()))
      {
        Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
        Item->setToolTip(tr("User \"%2\" is not authorized to access to \"%1\"").arg(WareId).arg(UserName));
        Item->setBackgroundColor(QColor("orange"));
      }

      Item->setCheckState(
          (m_AlreadySelectedWaresHubWares.contains(Type) && !m_AlreadySelectedWaresHubWares[Type].filter(
              QFileInfo(WareUrl).fileName()).isEmpty()) ? Qt::Checked : Qt::Unchecked);

      m_ListWidgetsByWareType[Type]->addItem(Item);
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
        Wares << Item->data(Qt::UserRole).toString();
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
        Wares[Pair.first] << Item->data(Qt::UserRole).toString();
    }
  }

  return Wares;
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::import()
{
  if (!check())
    return;

  openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Importing wares sources:"), false, this);

  QThread* Thread = new QThread();
  connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

  if (ui->PackageRadioButton->isChecked())
  {
    if (!mp_ImportFilePkg)
      return;

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
    if (!mp_WaresHubImportWorker)
      return;

    mp_WaresHubImportWorker->setSelectedWaresUrl(getSelectedWaresByType());

    mp_WaresHubImportWorker->moveToThread(Thread);

    connect(Thread, SIGNAL(started()), mp_WaresHubImportWorker, SLOT(clone()));

    connect(mp_WaresHubImportWorker, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
    connect(mp_WaresHubImportWorker, SIGNAL(finished(bool, const QString&)), &ProgressDialog,
            SLOT(finish(bool, const QString&)));

    connect(mp_WaresHubImportWorker, SIGNAL(info(const QString&)), &ProgressDialog, SLOT(writeInfo(const QString&)));
    connect(mp_WaresHubImportWorker, SIGNAL(error(const QString&)), &ProgressDialog, SLOT(writeError(const QString&)));
    connect(mp_WaresHubImportWorker, SIGNAL(progressed(int)), &ProgressDialog, SLOT(progress(int)));
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
    accept();
}


} } } //namespaces

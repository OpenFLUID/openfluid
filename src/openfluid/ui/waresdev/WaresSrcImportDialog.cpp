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


#include <openfluid/ui/waresdev/WaresSrcImportDialog.hpp>

#include "ui_WaresSrcImportDialog.h"

#include <openfluid/waresdev/WaresDevPackage.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/config.hpp>

#include <QFileDialog>
#include <QThread>

namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WaresSrcImportDialog::WaresSrcImportDialog(QWidget* Parent) :
    QDialog(Parent), ui(new Ui::WaresSrcImportDialog)
{
  ui->setupUi(this);

  m_ListWidgetsByWareTypeName[openfluid::config::SIMULATORS_PLUGINS_SUBDIR] = ui->SimListWidget;
  m_ListWidgetsByWareTypeName[openfluid::config::OBSERVERS_PLUGINS_SUBDIR] = ui->ObsListWidget;
  m_ListWidgetsByWareTypeName[openfluid::config::BUILDEREXTS_PLUGINS_SUBDIR] = ui->ExtListWidget;

  ui->WareshubWidget->setVisible(false);

  onPackageSourceChoosen(true);

  connect(ui->PackageRadioButton, SIGNAL(toggled(bool)), this, SLOT(onPackageSourceChoosen(bool)));
  connect(ui->WareshubRadioButton, SIGNAL(toggled(bool)), this, SLOT(onGitSourceChoosen(bool)));

  connect(ui->PackagePathButton, SIGNAL(clicked()), this, SLOT(onPackagePathButtonClicked()));

  for (QListWidget* Widget : m_ListWidgetsByWareTypeName)
  {
    connect(Widget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(check()));
  }

  connect(ui->ButtonBox, SIGNAL(accepted()), this, SLOT(importPackage()));

  check();
}


// =====================================================================
// =====================================================================


bool WaresSrcImportDialog::check()
{
  if (ui->PackagePathLabel->text().isEmpty())
  {
    setMessage("No package file defined.");
    return false;
  }

  if (getSelectedWares().isEmpty())
  {
    setMessage("No ware selected.");
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


void WaresSrcImportDialog::onPackageSourceChoosen(bool Checked)
{
  if (!Checked)
    return;

  ui->WaresGroupBox->setTitle("Available wares in package");

  updatePackageInfo(mp_ImportFilePkg);
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onGitSourceChoosen(bool Checked)
{
  if (!Checked)
    return;

  ui->WaresGroupBox->setTitle("Available wares on wareshub site");

  updatePackageInfo(mp_ImportFilePkg);
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onPackagePathButtonClicked()
{
  QString PackageFilePath = QFileDialog::getOpenFileName(this, tr("Choose the package file to open"),
                                                         QDir::homePath(),
                                                         tr("OpenFLUID wares packages (*.ofwdp)"));

  if (PackageFilePath.isEmpty())
    return;


  ui->PackagePathLabel->setText(PackageFilePath);


  QThread* Thread = new QThread();

  openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog("Fetching ware sources information:", this);

  try
  {
    mp_ImportFilePkg = new openfluid::waresdev::WaresDevImportPackage(PackageFilePath);

    mp_ImportFilePkg->moveToThread(Thread);

    connect(Thread, SIGNAL(started()), mp_ImportFilePkg, SLOT(fetchInformation()));
    connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

    connect(mp_ImportFilePkg, SIGNAL(finished(bool)), Thread, SLOT(quit()));
    connect(mp_ImportFilePkg, SIGNAL(finished(bool)), &ProgressDialog, SLOT(finishAndQuit(bool)));

    connect(mp_ImportFilePkg, SIGNAL(info(const QString&)), &ProgressDialog, SLOT(writeInfo(const QString&)));
    connect(mp_ImportFilePkg, SIGNAL(error(const QString&)), &ProgressDialog, SLOT(writeError(const QString&)));
    connect(mp_ImportFilePkg, SIGNAL(progressed(int)), &ProgressDialog, SLOT(progress(int)));

    Thread->start();
  }
  catch (std::exception& e)
  {
    ProgressDialog.writeError(e.what());
  }

  ProgressDialog.exec();

  updatePackageInfo(mp_ImportFilePkg);
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::updatePackageInfo(openfluid::waresdev::WaresDevImportPackage* ImportPackage)
{
  if (!ImportPackage)
    return;

  ui->PackagersLabel->setText(ImportPackage->getPackagers());

  ui->PackageDescriptionLabel->setText(ImportPackage->getPackageDescription());

  updateWaresList(ImportPackage);

  ui->WaresTabWidget->setCurrentIndex(0);
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::updateWaresList(openfluid::waresdev::WaresDevImportPackage* ImportPackage)
{
  for (QListWidget* ListWidget : m_ListWidgetsByWareTypeName.values())
    ListWidget->clear();

  if (!ImportPackage)
    return;

  QDir WaresDevDir(openfluid::waresdev::WareSrcManager::instance()->getWaresdevPath());
  QDir PackageTempDir = ImportPackage->getPackageTempDir();

  for (const QString& WarePath : ImportPackage->getWaresPaths())
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
      Item->setToolTip(QString("%1 already exists in your workspace").arg(WareName));
    }
    else
    {
      Item->setCheckState(Qt::Checked);
      Item->setToolTip("");
    }

    m_ListWidgetsByWareTypeName[WareTypeName.toStdString()]->addItem(Item);
  }

  check();
}


// =====================================================================
// =====================================================================


QStringList WaresSrcImportDialog::getSelectedWares()
{
  QStringList Wares;

  for (QMap<std::string, QListWidget*>::iterator it = m_ListWidgetsByWareTypeName.begin();
      it != m_ListWidgetsByWareTypeName.end();
      ++it)
  {
    for (const QListWidgetItem* Item : it.value()->findItems("*", Qt::MatchWildcard))
    {
      if (Item->checkState() == Qt::Checked)
        Wares << Item->data(Qt::UserRole).toString();
    }
  }

  return Wares;
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::importPackage()
{
  if (!check())
    return;

  openfluid::waresdev::WaresDevImportPackage* Pkg = nullptr;

  if (ui->PackageRadioButton->isChecked())
    Pkg = mp_ImportFilePkg;
  else
    Pkg = mp_ImportGitPkg;

  if (!Pkg)
    return;

  Pkg->setSelectedWares(getSelectedWares());


  QThread* Thread = new QThread();

  openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog("Processing ware sources import:", this);

  Pkg->moveToThread(Thread);

  connect(Thread, SIGNAL(started()), Pkg, SLOT(copyWares()));
  connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

  connect(Pkg, SIGNAL(finished(bool)), Thread, SLOT(quit()));
  connect(Pkg, SIGNAL(finished(bool)), &ProgressDialog, SLOT(finish(bool)));

  connect(Pkg, SIGNAL(info(const QString&)), &ProgressDialog, SLOT(writeInfo(const QString&)));
  connect(Pkg, SIGNAL(error(const QString&)), &ProgressDialog, SLOT(writeError(const QString&)));
  connect(Pkg, SIGNAL(progressed(int)), &ProgressDialog, SLOT(progress(int)));

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


// =====================================================================
// =====================================================================


} } } //namespaces

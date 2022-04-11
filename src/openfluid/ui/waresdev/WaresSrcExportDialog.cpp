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
 @file WaresSrcExportDialog.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QFileDialog>
#include <QThread>

#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/waresdev/WaresDevPackage.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcExportDialog.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_WaresSrcExportDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WaresSrcExportDialog::WaresSrcExportDialog(QWidget* Parent) :
    openfluid::ui::common::MessageDialog(Parent), ui(new Ui::WaresSrcExportDialog)
{
  ui->setupUi(this);

  setupMessageUi(tr("Export wares sources"));

  m_ListWidgetsByWareType[openfluid::ware::WareType::SIMULATOR] = ui->SimListWidget;
  m_ListWidgetsByWareType[openfluid::ware::WareType::OBSERVER] = ui->ObsListWidget;
  m_ListWidgetsByWareType[openfluid::ware::WareType::BUILDEREXT] = ui->ExtListWidget;

  initWaresLists();

  connect(ui->PackagePathButton, SIGNAL(clicked()), this, SLOT(onPackagePathButtonClicked()));

  connect(ui->PackagePathLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(check()));

  for (QListWidget* Widget : m_ListWidgetsByWareType)
  {
    connect(Widget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(check()));
  }

  connect(buttonBox(), SIGNAL(accepted()), this, SLOT(exportToPackage()));

  check();
}


// =====================================================================
// =====================================================================


bool WaresSrcExportDialog::check()
{
  if (ui->PackagePathLineEdit->text().isEmpty())
  {
    setMessage(tr("No package file selected"));
    return false;
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


void WaresSrcExportDialog::initWaresLists()
{
  auto* Manager = openfluid::base::WorkspaceManager::instance();

  for (ListWidgetsByWareType_t::iterator it = m_ListWidgetsByWareType.begin(); it != m_ListWidgetsByWareType.end();
      ++it)
  {
    for (const QFileInfo& WareFileInfo : 
           QDir(QString::fromStdString(Manager->getWaresPath(it.key())))
             .entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot,QDir::Name))
    {
      QListWidgetItem* Item = new QListWidgetItem(WareFileInfo.fileName());
      Item->setData(Qt::UserRole, WareFileInfo.absoluteFilePath());
      Item->setCheckState(Qt::Unchecked);
      it.value()->addItem(Item);
    }
  }

  ui->WaresTabWidget->setCurrentIndex(0);
}


// =====================================================================
// =====================================================================


void WaresSrcExportDialog::onPackagePathButtonClicked()
{
  QString PackageFilePath = QFileDialog::getSaveFileName(this, tr("Export wares sources as package"),
                                                         QDir::homePath());

  if (PackageFilePath.isEmpty())
  {
    return;
  }

  if (!PackageFilePath.endsWith(".ofwdp"))
  {
    PackageFilePath.append(".ofwdp");
  }

  ui->PackagePathLineEdit->setText(PackageFilePath);
}


// =====================================================================
// =====================================================================


void WaresSrcExportDialog::exportToPackage()
{
  if (!check())
  {
    return;
  }

  QThread* Thread = new QThread();

  openfluid::waresdev::WaresDevExportPackage Pkg(ui->PackagePathLineEdit->text(),
                                                 getSelectedWares(),
                                                 ui->PackagersLineEdit->text(),
                                                 ui->PackageDescriptionTextEdit->toPlainText());

  openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Compression of wares sources:"), false, this);

  Pkg.moveToThread(Thread);

  connect(Thread, SIGNAL(started()), &Pkg, SLOT(exportToPackage()));
  connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

  connect(&Pkg, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
  connect(&Pkg, SIGNAL(finished(bool, const QString&)), &ProgressDialog, SLOT(finish(bool, const QString&)));

  connect(&Pkg, SIGNAL(info(const QString&)), &ProgressDialog, SLOT(writeInfo(const QString&)));
  connect(&Pkg, SIGNAL(error(const QString&)), &ProgressDialog, SLOT(writeError(const QString&)));
  connect(&Pkg, SIGNAL(progressed(int)), &ProgressDialog, SLOT(progress(int)));

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


// =====================================================================
// =====================================================================


QStringList WaresSrcExportDialog::getSelectedWares()
{
  QStringList Wares;

  for (ListWidgetsByWareType_t::iterator it = m_ListWidgetsByWareType.begin(); it != m_ListWidgetsByWareType.end();
      ++it)
  {
    for (const QListWidgetItem* Item : it.value()->findItems("*", Qt::MatchWildcard))
    {
      if (Item->checkState() == Qt::Checked)
      {
        Wares << Item->data(Qt::UserRole).toString();
      }
    }
  }

  return Wares;
}


} } } //namespaces

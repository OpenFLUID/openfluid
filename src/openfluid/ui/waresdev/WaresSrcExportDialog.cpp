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


#include <openfluid/ui/waresdev/WaresSrcExportDialog.hpp>

#include "ui_WaresSrcExportDialog.h"

#include <QFileDialog>


namespace openfluid { namespace ui { namespace waresdev {

// =====================================================================
// =====================================================================


WaresSrcExportDialog::WaresSrcExportDialog(QWidget* Parent) :
    QDialog(Parent), ui(new Ui::WaresSrcExportDialog)
{
  ui->setupUi(this);

  m_ListWidgetsByWareType[openfluid::waresdev::WareSrcManager::SIMULATOR] = ui->SimListWidget;
  m_ListWidgetsByWareType[openfluid::waresdev::WareSrcManager::OBSERVER] = ui->ObsListWidget;
  m_ListWidgetsByWareType[openfluid::waresdev::WareSrcManager::BUILDEREXT] = ui->ExtListWidget;

  initWaresLists();

  connect(ui->PackagePathButton, SIGNAL(clicked()), this, SLOT(onPackagePathButtonClicked()));
}


// =====================================================================
// =====================================================================

void WaresSrcExportDialog::initWaresLists()
{
  openfluid::waresdev::WareSrcManager* Manager = openfluid::waresdev::WareSrcManager::instance();

  for (ListWidgetsByWareType_t::iterator it = m_ListWidgetsByWareType.begin(); it != m_ListWidgetsByWareType.end();
      ++it)
  {
    for (const QString& WareId : QDir(Manager->getWareTypePath(it.key())).entryList(
                                                                                    QDir::Dirs | QDir::NoDotAndDotDot,
                                                                                    QDir::Name))
    {
      QListWidgetItem* Item = new QListWidgetItem(WareId);
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
  QString PackageFilePath = QFileDialog::getSaveFileName(this, tr("Set the package name and location"),
                                                         QDir::homePath());

  if (PackageFilePath.isEmpty())
    return;

  if (!PackageFilePath.endsWith(".ofwdp"))
    PackageFilePath.append(".ofwdp");

  ui->PackagePathLineEdit->setText(PackageFilePath);
}


// =====================================================================
// =====================================================================


QString WaresSrcExportDialog::getPackageFilePath()
{
  return ui->PackagePathLineEdit->text();
}


// =====================================================================
// =====================================================================


QString WaresSrcExportDialog::getPackagers()
{
  return ui->PackagersLineEdit->text();
}


// =====================================================================
// =====================================================================


QString WaresSrcExportDialog::getPackageDesciption()
{
  return ui->PackageDescriptionTextEdit->toPlainText();
}


// =====================================================================
// =====================================================================


QMap<openfluid::waresdev::WareSrcManager::WareType, QStringList> WaresSrcExportDialog::getSelectedWares()
{
  QMap<openfluid::waresdev::WareSrcManager::WareType, QStringList> Wares;

  for (ListWidgetsByWareType_t::iterator it = m_ListWidgetsByWareType.begin(); it != m_ListWidgetsByWareType.end();
      ++it)
  {
    for (const QListWidgetItem* Item : it.value()->findItems("*", Qt::MatchWildcard))
    {
      if (Item->checkState() == Qt::Checked)
        Wares[it.key()] << Item->text();
    }
  }

  return Wares;
}


// =====================================================================
// =====================================================================


} } } //namespaces

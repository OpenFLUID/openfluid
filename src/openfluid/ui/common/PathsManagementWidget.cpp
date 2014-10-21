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
  \file PathsManagementWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_PathsManagementWidget.h"
#include "PathsManagementWidget.hpp"

#include <QDir>
#include <QFileDialog>


namespace openfluid { namespace ui { namespace common {


PathsManagementWidget::PathsManagementWidget(QWidget* Parent):
  QWidget(Parent),ui(new Ui::PathsManagementWidget),
  m_AllowEmpty(true)
{
  ui->setupUi(this);

  ui->AddButton->setText("");
  ui->AddButton->setIcon(QIcon(":/ui/common/icons/add.png"));
  ui->AddButton->setIconSize(QSize(20,20));

  ui->RemoveButton->setText("");
  ui->RemoveButton->setIcon(QIcon(":/ui/common/icons/remove.png"));
  ui->RemoveButton->setIconSize(QSize(20,20));

  ui->UpButton->setText("");
  ui->UpButton->setIcon(QIcon(":/ui/common/icons/go-up.png"));
  ui->UpButton->setIconSize(QSize(20,20));

  ui->DownButton->setText("");
  ui->DownButton->setIcon(QIcon(":/ui/common/icons/go-down.png"));
  ui->DownButton->setIconSize(QSize(20,20));

  connect(ui->AddButton,SIGNAL(clicked()),this,SLOT(addPath()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removePath()));
  connect(ui->UpButton,SIGNAL(clicked()),this,SLOT(moveupPath()));
  connect(ui->DownButton,SIGNAL(clicked()),this,SLOT(movedownPath()));

}


// =====================================================================
// =====================================================================


PathsManagementWidget::~PathsManagementWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void PathsManagementWidget::addPath()
{
  QString SelectedDir = QFileDialog::getExistingDirectory(this,tr("Select search path"));

  if (SelectedDir !=  "")
  {
    ui->PathsListWidget->addItem(QDir::toNativeSeparators(SelectedDir));
    emit pathsUpdated();
  }
}


// =====================================================================
// =====================================================================


void PathsManagementWidget::removePath()
{
  if (!m_AllowEmpty && ui->PathsListWidget->count() < 2) return;

  if (ui->PathsListWidget->currentRow() >= 0)
  {
    delete ui->PathsListWidget->takeItem(ui->PathsListWidget->currentRow());
    emit pathsUpdated();
  }
}


// =====================================================================
// =====================================================================


void PathsManagementWidget::moveupPath()
{
  int Index = ui->PathsListWidget->currentRow();

  if (Index > 0)
  {
    QListWidgetItem *Item = ui->PathsListWidget->takeItem(Index);
    ui->PathsListWidget->insertItem(Index-1, Item);
    ui->PathsListWidget->setCurrentRow(Index-1);
    emit pathsUpdated();
  }
}


// =====================================================================
// =====================================================================


void PathsManagementWidget::movedownPath()
{
  int Index = ui->PathsListWidget->currentRow();

  if (Index < ui->PathsListWidget->count())
  {
    QListWidgetItem *Item = ui->PathsListWidget->takeItem(Index);
    ui->PathsListWidget->insertItem(Index+1, Item);
    ui->PathsListWidget->setCurrentRow(Index+1);
    emit pathsUpdated();
  }
}


// =====================================================================
// =====================================================================


QStringList PathsManagementWidget::getPathsList() const
{
  QStringList Paths;

  for (int i=0; i<ui->PathsListWidget->count(); i++)
    Paths.append(QDir::fromNativeSeparators(ui->PathsListWidget->item(i)->text()));

  return Paths;
}


// =====================================================================
// =====================================================================


void PathsManagementWidget::setPathsList(const QStringList& Paths)
{
  QListWidgetItem* ItemToErase = NULL;

  do
  {
    ItemToErase = ui->PathsListWidget->takeItem(0);
    if (ItemToErase)
    {
      delete ItemToErase;
      ItemToErase = NULL;
    }
  }
  while (ItemToErase);

  for (int i=0;i<Paths.count();i++)
    new QListWidgetItem(QDir::toNativeSeparators(Paths[i]),ui->PathsListWidget);
}


} } } // namespaces


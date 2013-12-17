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
  \file DatastoreWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_DatastoreWidget.h"
#include "DatastoreWidget.hpp"
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

#include <QMessageBox>


DatastoreWidget::DatastoreWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WorkspaceWidget(Parent,AFXDesc), ui(new Ui::DatastoreWidget), m_Datastore(AFXDesc.getDatastore())
{
  ui->setupUi(this);

  ui->AddButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddButton->setIconSize(QSize(20,20));

  ui->EditButton->setIcon(QIcon(":/icons/modify.png"));
  ui->EditButton->setIconSize(QSize(20,20));

  ui->RemoveButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveButton->setIconSize(QSize(20,20));


  connect(ui->AddButton,SIGNAL(clicked()),this,SLOT(addItem()));
  connect(ui->EditButton,SIGNAL(clicked()),this,SLOT(editItem()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removeItem()));

  connect(ui->DatastoreTableWidget->horizontalHeader(),SIGNAL(sectionResized(int, int, int)),
          ui->DatastoreTableWidget,SLOT(resizeRowsToContents()));

  refresh();
}


// =====================================================================
// =====================================================================


DatastoreWidget::~DatastoreWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void DatastoreWidget::refresh()
{
  ui->DatastoreTableWidget->setRowCount(0);

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items = m_Datastore.getItems();

  ui->DatastoreTableWidget->setRowCount(Items.size());

  int CurrentRow = 0;

  for (std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator it = Items.begin();
       it != Items.end(); ++it)
  {
    QTableWidgetItem *Item;

    Item = new QTableWidgetItem(QString::fromStdString((*it)->getID()));
    ui->DatastoreTableWidget->setItem(CurrentRow, 0, Item);

    Item = new QTableWidgetItem(QString::fromStdString(openfluid::core::UnstructuredValue::getStringFromValueType((*it)->getType())));
    ui->DatastoreTableWidget->setItem(CurrentRow, 1, Item);

    Item = new QTableWidgetItem(QString::fromStdString((*it)->getUnitClass()));
    ui->DatastoreTableWidget->setItem(CurrentRow, 2, Item);

    Item = new QTableWidgetItem(QString::fromStdString((*it)->getRelativePath()));
    ui->DatastoreTableWidget->setItem(CurrentRow, 3, Item);

    CurrentRow++;
  }

  ui->DatastoreTableWidget->resizeRowsToContents();
}

// =====================================================================
// =====================================================================


void DatastoreWidget::addItem()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void DatastoreWidget::editItem()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void DatastoreWidget::removeItem()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


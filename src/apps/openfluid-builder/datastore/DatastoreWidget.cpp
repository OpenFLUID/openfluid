/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


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


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
  \file SpatialDomainWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include "ui_SpatialDomainWidget.h"
#include "SpatialDomainWidget.hpp"
#include "UnitsClassWidget.hpp"
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

#include <QTableWidgetItem>


SpatialDomainWidget::SpatialDomainWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WorkspaceWidget(Parent,AFXDesc), ui(new Ui::SpatialDomainWidget),
  m_Domain(AFXDesc.getDomain()), m_ActiveClass("")
{
  ui->setupUi(this);

  ui->AddUnitsClassButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddUnitsClassButton->setIconSize(QSize(20,20));

  ui->AddUnitButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddUnitButton->setIconSize(QSize(20,20));

  ui->AddConnectionButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddConnectionButton->setIconSize(QSize(16,16));

  ui->RemoveConnectionButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveConnectionButton->setIconSize(QSize(16,16));

  ui->AddAttributeButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddAttributeButton->setIconSize(QSize(20,20));

  ui->EditAttributeButton->setIcon(QIcon(":/icons/modify.png"));
  ui->EditAttributeButton->setIconSize(QSize(20,20));

  ui->RemoveAttributeButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveAttributeButton->setIconSize(QSize(20,20));

  ui->ConnectionsTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

  connect(ui->IDsListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(updateUnitSelection(int)));

  refresh();
}


// =====================================================================
// =====================================================================


SpatialDomainWidget::~SpatialDomainWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refresh()
{
  std::set<std::string> Classes = m_Domain.getClassNames();

  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  // TODO remove classes that does not exist anymore

  for (std::set<std::string>::iterator it=Classes.begin(); it!=Classes.end(); ++it)
  {
    bool AlreadyExist = false;

    // searching for already existing unit class in class list
    for (int i=0;i<Layout->count();i++)
    {
      if (i!=Layout->count()-1)
      {
        UnitsClassWidget* ClassW = dynamic_cast<UnitsClassWidget*>(Layout->itemAt(i)->widget());

        if (ClassW->getClassName() == QString::fromStdString(*it))
          AlreadyExist = true;
      }
    }

    // Add if it does not already exist
    if (!AlreadyExist)
    {
      UnitsClassWidget* ClassW = new UnitsClassWidget(QString::fromStdString((*it)),ui->UnitsClassAreaContents);

      dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout())->insertWidget(Layout->count()-1,ClassW);
      connect(ClassW,SIGNAL(selectionRequested(QString)),this,SLOT(setSelectedClass(QString)));
    }
  }

  if (m_ActiveClass.isEmpty() && Layout->count()>1)
    setSelectedClass(dynamic_cast<UnitsClassWidget*>(Layout->itemAt(0)->widget())->getClassName());
  else
    setActiveClass("");
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::setSelectedClass(QString ClassName)
{
  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  for (int i=0;i<Layout->count();i++)
  {
    if (i!=Layout->count()-1)
    {
      UnitsClassWidget* ClassW = dynamic_cast<UnitsClassWidget*>(Layout->itemAt(i)->widget());

      if (ClassW->getClassName() != ClassName)
        ClassW->setSelected(false);
      else
        ClassW->setSelected(true);
    }
  }

  setActiveClass(ClassName);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::setActiveClass(const QString& ClassName)
{
  m_ActiveClass = ClassName;

  if (m_ActiveClass.isEmpty())
  {
    ui->StructureTabWidget->setVisible(false);
    ui->DataTabWidget->setVisible(false);
  }
  else
  {
    ui->StructureTabWidget->setVisible(true);
    ui->DataTabWidget->setVisible(true);
    ui->AddUnitButton->setText(tr("Add unit in %1 class").arg(ClassName));
  }

  refreshStructure();
  refreshData();
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refreshStructure()
{
  std::cout << __PRETTY_FUNCTION__ << " " << m_ActiveClass.toStdString() << std::endl;

  ui->IDsListWidget->clear();

  if (!m_ActiveClass.isEmpty())
  {
    std::set<int> IDs = m_Domain.getIDsOfClass(m_ActiveClass.toStdString());

    for (std::set<int>::iterator iti = IDs.begin(); iti != IDs.end(); ++iti)
    {
      QListWidgetItem* Item = new QListWidgetItem(QString("%1").arg(*iti));
      Item->setData(Qt::UserRole,(*iti));

      ui->IDsListWidget->addItem(Item);
    }

    ui->IDsListWidget->setCurrentRow(0);
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateUnitSelection(int Row)
{
  if (Row >= 0)
  {
    int ID = ui->IDsListWidget->item(Row)->data(Qt::UserRole).toInt();

    const openfluid::fluidx::AdvancedUnitDescriptor& UnitDesc = m_Domain.getUnit(m_ActiveClass.toStdString(),ID);

    // ======== Process order

    ui->PcsOrderSpinBox->setValue(UnitDesc.UnitDescriptor->getProcessOrder());


    // ======== Connections

    ui->ConnectionsTableWidget->setRowCount(0);

    int CurrentRow = 0;

    std::list<openfluid::core::UnitClassID_t>::iterator itconn;
    std::list<openfluid::core::UnitClassID_t> ConnUnits;

    std::pair<std::string,int> UnitClassID = std::make_pair(m_ActiveClass.toStdString(),ID);

    QTableWidgetItem *Item;

    // to connections
    ConnUnits = m_Domain.getUnitsToOf(UnitClassID);

    for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
    {

      ui->ConnectionsTableWidget->setRowCount(CurrentRow+1);

      Item = new QTableWidgetItem("To");
      ui->ConnectionsTableWidget->setItem(CurrentRow, 0, Item);

      Item = new QTableWidgetItem(QString::fromStdString((*itconn).first));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 1, Item);

      Item = new QTableWidgetItem(QString("%1").arg((*itconn).second));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 2, Item);

      CurrentRow++;
    }


    // from connections
    ConnUnits = m_Domain.getUnitsFromOf(UnitClassID);

    for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
    {

      ui->ConnectionsTableWidget->setRowCount(CurrentRow+1);

      Item = new QTableWidgetItem("From");
      ui->ConnectionsTableWidget->setItem(CurrentRow, 0, Item);

      Item = new QTableWidgetItem(QString::fromStdString((*itconn).first));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 1, Item);

      Item = new QTableWidgetItem(QString("%1").arg((*itconn).second));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 2, Item);

      CurrentRow++;
    }


    // parent of connections
    ConnUnits = m_Domain.getUnitsParentsOf(UnitClassID);

    for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
    {

      ui->ConnectionsTableWidget->setRowCount(CurrentRow+1);

      Item = new QTableWidgetItem("Child of");
      ui->ConnectionsTableWidget->setItem(CurrentRow, 0, Item);

      Item = new QTableWidgetItem(QString::fromStdString((*itconn).first));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 1, Item);

      Item = new QTableWidgetItem(QString("%1").arg((*itconn).second));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 2, Item);

      CurrentRow++;
    }


    // child of connections
    ConnUnits = m_Domain.getUnitsChildrenOf(UnitClassID);

    for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
    {

      ui->ConnectionsTableWidget->setRowCount(CurrentRow+1);

      Item = new QTableWidgetItem("Parent of");
      ui->ConnectionsTableWidget->setItem(CurrentRow, 0, Item);

      Item = new QTableWidgetItem(QString::fromStdString((*itconn).first));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 1, Item);

      Item = new QTableWidgetItem(QString("%1").arg((*itconn).second));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 2, Item);

      CurrentRow++;
    }

    ui->ConnectionsTableWidget->resizeRowsToContents();

  }
}


// =====================================================================
// =====================================================================



void SpatialDomainWidget::refreshData()
{

}

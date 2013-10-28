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
#include "AppTools.hpp"

#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/base/ProjectManager.hpp>


#include <QTableWidgetItem>
#include <QMessageBox>
#include <QTimer>


SpatialDomainWidget::SpatialDomainWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WorkspaceWidget(Parent,AFXDesc), ui(new Ui::SpatialDomainWidget),
  m_Domain(AFXDesc.getDomain()), m_Datastore(AFXDesc.getDatastore()),
  m_ActiveClass(""), m_IsFirstShow(true)
{
  ui->setupUi(this);

  ui->StructureTabWidget->setCurrentIndex(0);
  ui->DataTabWidget->setCurrentIndex(0);

  mp_MapScene = new MapScene(m_Domain,this);
  ui->GlobalMapView->setScene(mp_MapScene);

  ui->AddUnitsClassButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddUnitsClassButton->setIconSize(QSize(20,20));

  ui->AddUnitButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddUnitButton->setIconSize(QSize(20,20));

  ui->RemoveUnitButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveUnitButton->setIconSize(QSize(20,20));

  ui->AddConnectionButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddConnectionButton->setIconSize(QSize(16,16));

  ui->RemoveConnectionButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveConnectionButton->setIconSize(QSize(16,16));

  ui->AddAttributeButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddAttributeButton->setIconSize(QSize(20,20));

  ui->EditAttributesButton->setIcon(QIcon(":/icons/modify.png"));
  ui->EditAttributesButton->setIconSize(QSize(20,20));

  ui->RemoveAttributeButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveAttributeButton->setIconSize(QSize(20,20));

  ui->ConnectionsTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

  connect(ui->AddUnitsClassButton,SIGNAL(clicked()),this,SLOT(addUnitsClass()));

  connect(ui->AddUnitButton,SIGNAL(clicked()),this,SLOT(addUnit()));
  connect(ui->RemoveUnitButton,SIGNAL(clicked()),this,SLOT(removeUnit()));

  connect(ui->AddConnectionButton,SIGNAL(clicked()),this,SLOT(addConnection()));
  connect(ui->RemoveConnectionButton,SIGNAL(clicked()),this,SLOT(removeConnection()));

  connect(ui->AddAttributeButton,SIGNAL(clicked()),this,SLOT(addAttribute()));
  connect(ui->EditAttributesButton,SIGNAL(clicked()),this,SLOT(editAttributesValues()));
  connect(ui->RemoveAttributeButton,SIGNAL(clicked()),this,SLOT(removeAttribute()));

  connect(ui->IDsListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(updateUnitSelection(int)));

  connect(ui->UnitsIDsCheckBox,SIGNAL(toggled(bool)),mp_MapScene,SLOT(enableUnitsIDs(bool)));
  connect(ui->FitViewButton,SIGNAL(clicked()),ui->GlobalMapView,SLOT(fitViewToItems()));

  connect(mp_MapScene,SIGNAL(selectionChanged()),this,SLOT(updateSelectionFromMap()));

  refresh();
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::showEvent(QShowEvent *Event)
{

  if (m_IsFirstShow)
  {
    ui->GlobalMapView->fitViewToItems();
    m_IsFirstShow = false;
  }
  WorkspaceWidget::showEvent(Event);
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
  QStringList OriginalClassesList = StringSetToQStringList(m_Domain.getClassNames());

  QStringList ClassesList;

  // get display order for classes from project config file
  QVariant TmpList = openfluid::base::ProjectManager::getInstance()->getConfigValue("builder.spatial.unitsclasses","order");

  if (!TmpList.isValid())
  {
    openfluid::base::ProjectManager::getInstance()->setConfigValue("builder.spatial.unitsclasses","order",OriginalClassesList);
    ClassesList = OriginalClassesList;
  }
  else
  {
    ClassesList = TmpList.toStringList();

    // find classes in project config file that are not in original dataset
    QStringList ClassesToRemove;
    for (int i=0; i< ClassesList.size();i++)
    {
      if (!OriginalClassesList.contains(ClassesList[i]))
        ClassesToRemove.append(ClassesList[i]);
    }

    // remove them from classes to display
    for (int i=0; i< ClassesToRemove.size();i++)
      ClassesList.removeAll(ClassesToRemove[i]);

    // remove classes from original list if they exists in classes to display
    for (int i=0; i< ClassesList.size();i++)
    {
      if (OriginalClassesList.contains(ClassesList[i]))
        OriginalClassesList.removeAll(ClassesList[i]);
    }

    // add original dataset classes that are not already in the classes to display
    for (int i=0; i< OriginalClassesList.size();i++)
    {
      if (!ClassesList.contains(OriginalClassesList[i]))
        ClassesList.append(OriginalClassesList[i]);
    }

    openfluid::base::ProjectManager::getInstance()->setConfigValue("builder.spatial.unitsclasses","order",ClassesList);
  }


  // display classes widgets

  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  // TODO remove classes that does not exist anymore

  for (int i = 0; i<ClassesList.size(); i++)
  {
    bool AlreadyExist = false;

    // searching for already existing unit class in class list
    for (int j=0;j<Layout->count();j++)
    {
      if (j!=Layout->count()-1)
      {
        UnitsClassWidget* ClassW = dynamic_cast<UnitsClassWidget*>(Layout->itemAt(j)->widget());

        if (ClassW->getClassName() == ClassesList[i])
          AlreadyExist = true;
      }
    }

    // Add if it does not already exist
    if (!AlreadyExist)
    {
      UnitsClassWidget* ClassW = new UnitsClassWidget(ClassesList[i],
                                                      m_Datastore.getItems(ClassesList[i].toStdString(),
                                                                           openfluid::core::UnstructuredValue::GeoVectorValue),
                                                      ui->UnitsClassAreaContents);

      dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout())->insertWidget(Layout->count()-1,ClassW);
      connect(ClassW,SIGNAL(selectionRequested(QString)),this,SLOT(setSelectedClass(QString)));
      connect(ClassW,SIGNAL(styleChanged(QString)),this,SLOT(refreshMap()));
      connect(ClassW,SIGNAL(upClicked(QString)),this,SLOT(moveUnitsClassUp(QString)));
      connect(ClassW,SIGNAL(downClicked(QString)),this,SLOT(moveUnitsClassDown(QString)));
      connect(ClassW,SIGNAL(removeClicked(QString)),this,SLOT(removeUnitsClass(QString)));
    }
  }

  if (m_ActiveClass.isEmpty() && Layout->count()>1)
    setSelectedClass(dynamic_cast<UnitsClassWidget*>(Layout->itemAt(0)->widget())->getClassName());
  else
    setActiveClass("");

  refreshMap();
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

  refreshClassStructure();
  refreshClassData();
  updateUpDownButtons();
  mp_MapScene->setActiveLayer(ClassName);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refreshClassStructure()
{
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

    ui->RemoveUnitButton->setText(tr("Remove unit %1 from %2 class").arg(ID).arg(m_ActiveClass));

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

    // attributes

    ui->AttributesTableWidget->clearSelection();
    ui->AttributesTableWidget->selectRow(Row);

  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refreshClassData()
{
  ui->AttributesTableWidget->clear();

  if (!m_ActiveClass.isEmpty())
  {
    QStringList AttrNames = StringSetToQStringList(m_Domain.getAttributesNames(m_ActiveClass.toStdString()));
    QStringList IDs = IntSetToQStringList(m_Domain.getIDsOfClass(m_ActiveClass.toStdString()));

    ui->AttributesTableWidget->setColumnCount(AttrNames.size());
    ui->AttributesTableWidget->setRowCount(IDs.size());

    ui->AttributesTableWidget->setHorizontalHeaderLabels(AttrNames);
    ui->AttributesTableWidget->setVerticalHeaderLabels(IDs);

    for (int i=0;i<IDs.size();i++)
    {
      for (int j=0;j<AttrNames.size();j++)
      {
        ui->AttributesTableWidget->setItem(i,j,
                                           new QTableWidgetItem(QString::fromStdString(m_Domain.getAttribute(m_ActiveClass.toStdString(),
                                                                                                             IDs[i].toInt(),
                                                                                                             AttrNames[j].toStdString()))));
      }
    }
  }

  ui->AttributesTableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refreshMap()
{
  mp_MapScene->clear();

  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  for (int i=Layout->count()-2; i>=0;i--)
  {
    UnitsClassWidget* ClassW = dynamic_cast<UnitsClassWidget*>(Layout->itemAt(i)->widget());

    if (ClassW->getLayerSource() != NULL && ClassW->isLayerVisible())
    {
      mp_MapScene->addLayer(ClassW->getLayerSource(), -i,
                               ClassW->getLineWidth(),
                               ClassW->getLineColor(),
                               ClassW->getFillColor());
    }
  }

  mp_MapScene->setActiveLayer(m_ActiveClass);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::addUnitsClass()
{
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
  updateUpDownButtons();
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::moveUnitsClassUp(QString ClassName)
{
  int Position = getClassIndex(ClassName);

  if (Position < 1 ) return;

  int From = Position;
  int To = Position-1;

  UnitsClassWidget* W = (UnitsClassWidget*)(ui->UnitsClassAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(ui->UnitsClassAreaContents->layout()))->insertWidget(To,W);

  updateUpDownButtons();

  openfluid::base::ProjectManager::getInstance()->setConfigValue("builder.spatial.unitsclasses",
                                                                 "order",
                                                                 getClassesOrderedStringList());

  refreshMap();
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::moveUnitsClassDown(QString ClassName)
{
  int Position = getClassIndex(ClassName);

  if (Position < 0) return;

  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  if (Position > Layout->count()-2) return;

  int From = Position;
  int To = Position+1;

  UnitsClassWidget* W = (UnitsClassWidget*)(ui->UnitsClassAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(ui->UnitsClassAreaContents->layout()))->insertWidget(To,W);

  updateUpDownButtons();

  openfluid::base::ProjectManager::getInstance()->setConfigValue("builder.spatial.unitsclasses",
                                                                 "order",
                                                                 getClassesOrderedStringList());

  refreshMap();
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::removeUnitsClass(QString ClassName)
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),
                        QString(__PRETTY_FUNCTION__)+"\n"+ClassName,QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::addUnit()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::removeUnit()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::addConnection()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::removeConnection()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::addAttribute()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::editAttributesValues()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::removeAttribute()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateUpDownButtons()
{
  int LastIndex = ui->UnitsClassAreaContents->layout()->count()-2;

  for (int i=0;i<=LastIndex;i++)
  {
    UnitsClassWidget* W = (UnitsClassWidget*)(ui->UnitsClassAreaContents->layout()->itemAt(i)->widget());
    if (i==0 && LastIndex == 0)
    {
      W->setUpButtonEnabled(false);
      W->setDownButtonEnabled(false);
    }
    else if (i==0)
    {
      W->setUpButtonEnabled(false);
      W->setDownButtonEnabled(true);
    }
    else if (i==LastIndex)
    {
      W->setUpButtonEnabled(true);
      W->setDownButtonEnabled(false);
    }
    else
    {
      W->setUpButtonEnabled(true);
      W->setDownButtonEnabled(true);
    }
  }
}


// =====================================================================
// =====================================================================


int SpatialDomainWidget::getClassIndex(const QString& ClassName)
{
  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  for (int i=0;i<Layout->count()-1;i++)
  {
    UnitsClassWidget* ClassW = dynamic_cast<UnitsClassWidget*>(Layout->itemAt(i)->widget());

    if (ClassW->getClassName() == ClassName)
      return i;
  }

  return -1;
}


// =====================================================================
// =====================================================================


QStringList SpatialDomainWidget::getClassesOrderedStringList()
{
  QStringList StrList;

  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  for (int i=0;i<Layout->count()-1;i++)
  {
    UnitsClassWidget* ClassW = dynamic_cast<UnitsClassWidget*>(Layout->itemAt(i)->widget());
    StrList.append(ClassW->getClassName());
  }

  return StrList;
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateSelectionFromMap()
{
  if (!mp_MapScene->selectedItems().isEmpty())
  {
    int ID = dynamic_cast<MapItemGraphics*>(mp_MapScene->selectedItems().first())->getUnitID();

    for (int i=0; i<ui->IDsListWidget->count();i++)
    {
      if (ui->IDsListWidget->item(i)->data(Qt::UserRole).toInt() == ID)
      {
        ui->IDsListWidget->setCurrentRow(i);
        return;
      }
    }
  }
}

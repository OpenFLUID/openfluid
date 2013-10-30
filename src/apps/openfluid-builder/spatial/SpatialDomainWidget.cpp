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
#include "EditAttributesValuesDialog.hpp"
#include "EditAttributeNameDialog.hpp"

#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>


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
  ui->AddConnectionButton->setIconSize(QSize(20,20));

  ui->RemoveConnectionButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveConnectionButton->setIconSize(QSize(20,20));

  ui->AddAttributeButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddAttributeButton->setIconSize(QSize(20,20));

  ui->EditAttributesButton->setIcon(QIcon(":/icons/modify.png"));
  ui->EditAttributesButton->setIconSize(QSize(20,20));

  ui->RenameAttributeButton->setIcon(QIcon(":/icons/rename-attribute.png"));
  ui->RenameAttributeButton->setIconSize(QSize(20,20));

  ui->RemoveAttributeButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveAttributeButton->setIconSize(QSize(20,20));

  ui->AddEventButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddEventButton->setIconSize(QSize(20,20));

  ui->EditEventButton->setIcon(QIcon(":/icons/modify.png"));
  ui->EditEventButton->setIconSize(QSize(20,20));

  ui->RemoveEventsButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveEventsButton->setIconSize(QSize(20,20));


  ui->ConnectionsTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

  connect(ui->AddUnitsClassButton,SIGNAL(clicked()),this,SLOT(addUnitsClass()));

  connect(ui->AddUnitButton,SIGNAL(clicked()),this,SLOT(addUnit()));
  connect(ui->RemoveUnitButton,SIGNAL(clicked()),this,SLOT(removeUnit()));

  connect(ui->AddConnectionButton,SIGNAL(clicked()),this,SLOT(addConnection()));
  connect(ui->RemoveConnectionButton,SIGNAL(clicked()),this,SLOT(removeConnection()));

  connect(ui->AddAttributeButton,SIGNAL(clicked()),this,SLOT(addAttribute()));
  connect(ui->EditAttributesButton,SIGNAL(clicked()),this,SLOT(editAttributesValues()));
  connect(ui->RenameAttributeButton,SIGNAL(clicked()),this,SLOT(renameAttribute()));
  connect(ui->RemoveAttributeButton,SIGNAL(clicked()),this,SLOT(removeAttribute()));

  connect(ui->AddEventButton,SIGNAL(clicked()),this,SLOT(addEvent()));
  connect(ui->EditEventButton,SIGNAL(clicked()),this,SLOT(editEvent()));
  connect(ui->RemoveEventsButton,SIGNAL(clicked()),this,SLOT(removeEvents()));

  connect(ui->IDsListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(updateUnitSelection(int)));

  connect(ui->UnitsIDsCheckBox,SIGNAL(toggled(bool)),mp_MapScene,SLOT(enableUnitsIDs(bool)));
  connect(ui->FitViewButton,SIGNAL(clicked()),ui->GlobalMapView,SLOT(fitViewToItems()));

  connect(mp_MapScene,SIGNAL(selectionChanged()),this,SLOT(updateSelectionFromMap()));

  connect(ui->AttributesTableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(updateFluidXAttributeFromCellValue(int,int)));
  connect(ui->PcsOrderSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateFluidXProcessOrder(int)));

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

  // remove classes from display that does not exist anymore
  for (int j=0;j<Layout->count();j++)
  {
    if (j!=Layout->count()-1)
    {
      UnitsClassWidget* ClassW = dynamic_cast<UnitsClassWidget*>(Layout->itemAt(j)->widget());

      if (!ClassesList.contains(ClassW->getClassName()))
      {
        if (ClassW->getClassName() == m_ActiveClass)
          m_ActiveClass = "";

        Layout->takeAt(j)->widget()->deleteLater();
      }
    }
  }


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


  if (m_ActiveClass.isEmpty())
  {
    // active class is not defined, pick the first units class if any
    if (Layout->count()>1)
      setSelectedClass(dynamic_cast<UnitsClassWidget*>(Layout->itemAt(0)->widget())->getClassName());
    else
      setActiveClass("");
  }
  else
  {
    setSelectedClass(m_ActiveClass);
  }

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

    disconnect(ui->PcsOrderSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateFluidXProcessOrder(int)));
    ui->PcsOrderSpinBox->setValue(UnitDesc.UnitDescriptor->getProcessOrder());
    connect(ui->PcsOrderSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateFluidXProcessOrder(int)));


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
      // set connection code to 0 for "To"
      Item->setData(Qt::UserRole,0);
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
      // set connection code to 1 for "From"
      Item->setData(Qt::UserRole,1);
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
      // set connection code to 2 for "Child of"
      Item->setData(Qt::UserRole,2);
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
      // set connection code to 3 for "Parent of"
      Item->setData(Qt::UserRole,3);
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


void SpatialDomainWidget::refreshClassAttributes()
{
  disconnect(ui->AttributesTableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(updateFluidXAttributeFromCellValue(int,int)));

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

  connect(ui->AttributesTableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(updateFluidXAttributeFromCellValue(int,int)));
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refreshClassEvents()
{
  ui->EventsTableWidget->clearContents();
  ui->EventsTableWidget->setRowCount(0);

  if (!m_ActiveClass.isEmpty())
  {
    std::set<int> IDs = m_Domain.getIDsOfClass(m_ActiveClass.toStdString());

    std::set<int>::iterator it;
    std::set<int>::iterator itb = IDs.begin();
    std::set<int>::iterator ite = IDs.end();

    int RowIndex = 0;

    for (it=itb; it!=ite; ++it)
    {
      const std::list<openfluid::core::Event*>* EventsListPtr = &(m_Domain.getUnit(m_ActiveClass.toStdString(),*it).Events);

      if (!EventsListPtr->empty())
      {
        std::list<openfluid::core::Event*>::const_iterator Eit;
        std::list<openfluid::core::Event*>::const_iterator Eitb = EventsListPtr->begin();
        std::list<openfluid::core::Event*>::const_iterator Eite = EventsListPtr->end();

        for (Eit=Eitb; Eit!=Eite; ++Eit)
        {
          ui->EventsTableWidget->setRowCount(RowIndex+1);

          ui->EventsTableWidget->setItem(RowIndex,0,new QTableWidgetItem(QString::fromStdString((*Eit)->getDateTime().getAsISOString())));

          ui->EventsTableWidget->setItem(RowIndex,1,new QTableWidgetItem(QString("%1").arg(*it)));

          openfluid::core::Event::EventInfosMap_t Infos = (*Eit)->getInfos();
          openfluid::core::Event::EventInfosMap_t::iterator Iit;
          openfluid::core::Event::EventInfosMap_t::iterator Iitb = Infos.begin();
          openfluid::core::Event::EventInfosMap_t::iterator Iite = Infos.end();

          QString InfosStr;

          for (Iit=Iitb; Iit!=Iite; ++Iit)
          {
            if (Iit != Iitb) InfosStr += ", ";
            InfosStr += QString::fromStdString((*Iit).first)+"="+QString::fromStdString((*Iit).second);
          }

          ui->EventsTableWidget->setItem(RowIndex,2,new QTableWidgetItem(InfosStr));

          ui->EventsTableWidget->resizeRowToContents(RowIndex);

          RowIndex++;
        }
      }
    }
  }

  ui->EventsTableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refreshClassData()
{
  refreshClassAttributes();
  refreshClassEvents();
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
  // TODO
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
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  std::set<int> IDs = m_Domain.getIDsOfClass(ClassName.toStdString());

  std::set<int>::iterator it;
  std::set<int>::iterator itb = IDs.begin();
  std::set<int>::iterator ite = IDs.end();

  for (it =itb; it!= ite; ++it)
    m_Domain.deleteUnit(ClassName.toStdString(),*it);

  refresh();

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT |
               openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);

  QApplication::restoreOverrideCursor();
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
  int ID = ui->IDsListWidget->currentItem()->data(Qt::UserRole).toInt();

  bool OK = true;

  if (openfluid::guicommon::PreferencesManager::getInstance()->isSpatialUnitsRemovalConfirm())
  {
    OK = (QMessageBox::question(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                tr("You are removing the unit %1 of class %2.\n"
                                    "All connections and attributes units related to this unit will be lost.\n\n"
                                    "Proceed anyway?").arg(ID).arg(m_ActiveClass),
                                    QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
  }

  if (OK)
  {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_Domain.deleteUnit(m_ActiveClass.toStdString(),ID);
    if (!m_Domain.isClassNameExists(m_ActiveClass.toStdString())) m_ActiveClass = "";

    refresh();

    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT |
                 openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);
    QApplication::restoreOverrideCursor();
  }
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
  if (ui->ConnectionsTableWidget->selectedItems().isEmpty())
  {
    QMessageBox::critical(QApplication::activeWindow(),QString("OpenFLUID-builder"),tr("No connection selected for removal"),QMessageBox::Close);
  }
  else
  {
    bool OK = true;

    int SrcID = ui->IDsListWidget->currentItem()->data(Qt::UserRole).toInt();
    QString SrcClass = m_ActiveClass;

    int Row = ui->ConnectionsTableWidget->selectedItems().front()->row();

    int DestID = ui->ConnectionsTableWidget->item(Row,2)->text().toInt();
    QString DestClass = ui->ConnectionsTableWidget->item(Row,1)->text();

    QString ConnStr = ui->ConnectionsTableWidget->item(Row,0)->text();
    int ConnCode = ui->ConnectionsTableWidget->item(Row,0)->data(Qt::UserRole).toInt();

    if (openfluid::guicommon::PreferencesManager::getInstance()->isSpatialConnsRemovalConfirm())
    {
      OK = (QMessageBox::question(QApplication::activeWindow(),
                                  "OpenFLUID-Builder",
                                  tr("You are removing the \"%1\" connection\n"
                                     "between unit %2 of class %3 and unit %4 of class %5.\n\n"
                                     "Proceed?").arg(ConnStr).arg(SrcID).arg(SrcClass).arg(DestID).arg(DestClass),
                                      QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
    }

    if (OK)
    {
       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
       if (ConnCode == 0)
       {
         m_Domain.removeFromToRelation(make_pair(SrcClass.toStdString(),SrcID),
                                       make_pair(DestClass.toStdString(),DestID));
       }
       else if (ConnCode == 1)
       {
         m_Domain.removeFromToRelation(make_pair(DestClass.toStdString(),DestID),
                                       make_pair(SrcClass.toStdString(),SrcID));
       }
       else if (ConnCode == 2)
       {
         m_Domain.removeParentChildRelation(make_pair(DestClass.toStdString(),DestID),
                                            make_pair(SrcClass.toStdString(),SrcID));
       }
       else if (ConnCode == 3)
       {
         m_Domain.removeParentChildRelation(make_pair(SrcClass.toStdString(),SrcID),
                                            make_pair(DestClass.toStdString(),DestID));
       }

       ui->ConnectionsTableWidget->removeRow(Row);
       emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT);
       QApplication::restoreOverrideCursor();
    }
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::addAttribute()
{
  QStringList AttrsNames = StringSetToQStringList(m_Domain.getAttributesNames(m_ActiveClass.toStdString()));

  EditAttributeNameDialog AddDlg(EditAttributeNameDialog::EDIT_ADD,AttrsNames,this);
  if (AddDlg.exec() == QDialog::Accepted)
  {
    m_Domain.addAttribute(m_ActiveClass.toStdString(),
                          AddDlg.getNewName().toStdString(),
                          AddDlg.getDefaultValue().toStdString());
    refreshClassAttributes();

    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::editAttributesValues()
{
  if (!ui->AttributesTableWidget->selectedItems().isEmpty())
  {
    EditAttributesValuesDialog EditDlg;

    if (EditDlg.exec() == QDialog::Accepted)
    {
      if (EditDlg.getEditMode() == EditAttributesValuesDialog::EDIT_REPLACE)
      {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        foreach(QTableWidgetItem* Item,ui->AttributesTableWidget->selectedItems())
        {
          Item->setText(EditDlg.getReplaceValue());
        }
        QApplication::restoreOverrideCursor();
      }
      else
      {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        bool GlobalOK = true;
        bool LocalOK;

        // check if selected attributes values are numeric values (required for math operations)
        foreach(QTableWidgetItem* Item,ui->AttributesTableWidget->selectedItems())
        {
          Item->text().toDouble(&LocalOK);
          GlobalOK &= LocalOK;
        }

        if (GlobalOK)
        {
          if (EditDlg.getEditMode() == EditAttributesValuesDialog::EDIT_MULTIPLY)
          {
            double MultValue = EditDlg.getMultiplyValue();
            foreach(QTableWidgetItem* Item,ui->AttributesTableWidget->selectedItems())
            {
              double Value = Item->text().toDouble();
              Value *= MultValue;
              Item->setText(QString("%1").arg(Value,0,'g',12));
            }
          }
          else
          {
            double AddValue = EditDlg.getAddValue();
            foreach(QTableWidgetItem* Item,ui->AttributesTableWidget->selectedItems())
            {
              double Value = Item->text().toDouble();
              Value += AddValue;
              Item->setText(QString("%1").arg(Value,0,'g',12));
            }
          }

          QApplication::restoreOverrideCursor();
        }
        else
        {
          // at least one value is not numeric
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(this,"OpenFLUID-Builder",
                                tr("At least one attribute value is not a numeric value.\n"
                                   "Edition of attributes cannot be performed."),
                                QMessageBox::Close);
        }

      }
    }
  }
  else
  {
    // no attribute selected
    QMessageBox::critical(this,"OpenFLUID-Builder",
                          tr("No attribute value selected"),QMessageBox::Close);
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::renameAttribute()
{
  QStringList AttrsNames = StringSetToQStringList(m_Domain.getAttributesNames(m_ActiveClass.toStdString()));

  if (AttrsNames.isEmpty())
  {
    QMessageBox::critical(this,"OpenFLUID-Builder",
                          tr("There is no attribute in the %1 units class.\n"
                             "Rename cannot be performed.").arg(m_ActiveClass),
                          QMessageBox::Close);
  }
  else
  {
    EditAttributeNameDialog RenameDlg(EditAttributeNameDialog::EDIT_RENAME,AttrsNames,this);
    if (RenameDlg.exec() == QDialog::Accepted)
    {
      m_Domain.renameAttribute(m_ActiveClass.toStdString(),
                               RenameDlg.getSelectedOriginalName().toStdString(),
                               RenameDlg.getNewName().toStdString());

      refreshClassAttributes();

      emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);
    }
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::removeAttribute()
{
  bool OK = true;

  QStringList AttrsNames = StringSetToQStringList(m_Domain.getAttributesNames(m_ActiveClass.toStdString()));

  if (AttrsNames.isEmpty())
  {
    QMessageBox::critical(this,"OpenFLUID-Builder",
                          tr("There is no attribute in the %1 units class.\n"
                              "Rename cannot be performed.").arg(m_ActiveClass),
                              QMessageBox::Close);
  }
  else
  {
    EditAttributeNameDialog RemoveDlg(EditAttributeNameDialog::EDIT_REMOVE,AttrsNames,this);
    if (RemoveDlg.exec() == QDialog::Accepted)
    {
      if (openfluid::guicommon::PreferencesManager::getInstance()->isSpatialAttrsRemovalConfirm())
      {
        OK = (QMessageBox::question(QApplication::activeWindow(),
                                    "OpenFLUID-Builder",
                                    tr("You are removing the attribute %1 of class %2.\n"
                                        "All %1 values associated to units of class %2 will be lost.\n\n"
                                        "Proceed anyway?").arg(RemoveDlg.getSelectedOriginalName()).arg(m_ActiveClass),
                                        QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
      }

      if (OK)
      {
        m_Domain.deleteAttribute(m_ActiveClass.toStdString(),
                                 RemoveDlg.getSelectedOriginalName().toStdString());

        refreshClassAttributes();

        emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);
      }
    }
  }
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


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateFluidXAttributeFromCellValue(int Row, int Column)
{
  std::string Attr = ui->AttributesTableWidget->horizontalHeaderItem(Column)->text().toStdString();
  int ID = ui->AttributesTableWidget->verticalHeaderItem(Row)->text().toInt();
  std::string Value = ui->AttributesTableWidget->item(Row,Column)->text().toStdString();

  m_Domain.getAttribute(m_ActiveClass.toStdString(),ID,Attr) = Value;

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);

}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateFluidXProcessOrder(int PcsOrd)
{
  int ID = ui->IDsListWidget->currentItem()->text().toInt();
  (&const_cast<openfluid::fluidx::UnitDescriptor&>(m_Domain.getUnitDescriptor(m_ActiveClass.toStdString(),ID)))->getProcessOrder() = PcsOrd;

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::addEvent()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::editEvent()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::removeEvents()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


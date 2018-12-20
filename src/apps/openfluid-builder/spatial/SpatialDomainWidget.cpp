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
  @file SpatialDomainWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QTableWidgetItem>
#include <QMessageBox>
#include <QTimer>

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_SpatialDomainWidget.h"
#include "SpatialDomainWidget.hpp"
#include "UnitsClassWidget.hpp"
#include "EditAttributesValuesDialog.hpp"
#include "EditAttributeNameDialog.hpp"
#include "AddUnitsClassDialog.hpp"
#include "AddUnitToClassDialog.hpp"
#include "AddConnectionDialog.hpp"
#include "EditEventDialog.hpp"
#include "ProjectCentral.hpp"


// for storing Event IDs in QTableWidget
Q_DECLARE_METATYPE(openfluid::fluidx::EventID_t);

enum CustomRoles {
    EventIDRole = Qt::UserRole + 1
};


// =====================================================================
// =====================================================================


SpatialDomainWidget::SpatialDomainWidget(QWidget* Parent, openfluid::fluidx::FluidXDescriptor& FXDesc):
  WorkspaceWidget(Parent,FXDesc), ui(new Ui::SpatialDomainWidget),
  m_Domain(FXDesc.spatialDomain()), m_Datastore(FXDesc.datastore()),
  m_ActiveClass("")
{
  ui->setupUi(this);

  ui->StructureTabWidget->setCurrentIndex(0);
  ui->DataTabWidget->setCurrentIndex(0);

  mp_MapScene = new MapScene(m_Domain,this);
  ui->GlobalMapView->setScene(mp_MapScene);

  ui->AddUnitsClassButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddUnitsClassButton->setIconSize(QSize(20,20));

  ui->AddUnitButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddUnitButton->setIconSize(QSize(20,20));

  ui->RemoveUnitButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveUnitButton->setIconSize(QSize(20,20));

  ui->AddConnectionButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddConnectionButton->setIconSize(QSize(20,20));

  ui->RemoveConnectionButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveConnectionButton->setIconSize(QSize(20,20));

  ui->AddAttributeButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddAttributeButton->setIconSize(QSize(20,20));

  ui->EditAttributesButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->EditAttributesButton->setIconSize(QSize(20,20));

  ui->RenameAttributeButton->setIcon(openfluid::ui::common::getIcon("rename-column","/builder"));
  ui->RenameAttributeButton->setIconSize(QSize(20,20));

  ui->RemoveAttributeButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveAttributeButton->setIconSize(QSize(20,20));

  ui->AddEventButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddEventButton->setIconSize(QSize(20,20));

  ui->EditEventButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->EditEventButton->setIconSize(QSize(20,20));

  ui->RemoveEventsButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveEventsButton->setIconSize(QSize(20,20));

  ui->ConnectionsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


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
  connect(ui->EventsTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editEvent()));
  connect(ui->RemoveEventsButton,SIGNAL(clicked()),this,SLOT(removeEvents()));

  connect(ui->IDsListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(updateUnitSelection(int)));
  connect(ui->IDsListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(updateSelectionFromIDsList()));
  connect(ui->IDsListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(refreshUnitField()));

  // map view
  // TODO to reactivate when units IDs display on map will be fully functional
  ui->UnitsIDsCheckBox->setVisible(false);
  ui->GlobalMapView->enableAutomaticView(ui->AutomaticViewCheckBox->isChecked());

  connect(ui->UnitsIDsCheckBox,SIGNAL(toggled(bool)),mp_MapScene,SLOT(enableUnitsIDs(bool)));
  connect(ui->FitViewButton,SIGNAL(clicked()),ui->GlobalMapView,SLOT(fitViewToItems()));
  connect(ui->AutomaticViewCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableAutomaticView(bool)));
  connect(ui->GlobalMapView,SIGNAL(automaticViewEnabled(bool)),
          ui->AutomaticViewCheckBox,SLOT(setChecked(bool)));

  connect(mp_MapScene,SIGNAL(selectionChanged()),this,SLOT(updateSelectionFromMap()));


  connect(ui->AttributesTableWidget,SIGNAL(cellChanged(int,int)),this,
          SLOT(updateFluidXAttributeFromCellValue(int,int)));
  connect(ui->PcsOrderSpinBox,SIGNAL(valueChanged(int)),this,
          SLOT(updateFluidXProcessOrder(int)));

  connect(ui->AttributesTableWidget,SIGNAL(itemSelectionChanged()),this,
          SLOT(updateMapFromAttributesTableChange()));
  connect(ui->AttributesTableWidget,SIGNAL(itemSelectionChanged()),this,
          SLOT(updateIDsListSelectionFromAttributesTableChange()));

  refresh();

  // all map layers are set visible at startup only
  setAllMapLayersVisible();

}


// =====================================================================
// =====================================================================


SpatialDomainWidget::~SpatialDomainWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::enableAutomaticView(bool Enabled)
{
  ui->GlobalMapView->enableAutomaticView(Enabled);
  ui->FitViewButton->setEnabled(!Enabled);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refresh()
{
  // -------- prepare classes list --------

  QStringList OriginalClassesList = openfluid::tools::toQStringList(m_Domain.getClassNames());

  QStringList ClassesList;

  // get display order for classes from project config file
  QVariant TmpList =
      openfluid::base::RunContextManager::instance()->getProjectConfigValue("builder.spatial.unitsclasses","order");

  if (!TmpList.isValid())
  {
    openfluid::base::RunContextManager::instance()->setProjectConfigValue("builder.spatial.unitsclasses","order",
                                                                          OriginalClassesList);
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

    openfluid::base::RunContextManager::instance()->setProjectConfigValue("builder.spatial.unitsclasses","order",
                                                                          ClassesList);
  }


  // -------- display classes widgets ---------

  // remove classes from display that does not exist anymore

  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  // search for classes to remove from layout
  QList<int> ItemsToRemove;

  for (int j=0;j<Layout->count();j++)
  {
    if (Layout->itemAt(j)->widget() != nullptr)
    {

      UnitsClassWidget* ClassW = dynamic_cast<UnitsClassWidget*>(Layout->itemAt(j)->widget());

      if (!ClassesList.contains(ClassW->getClassName()))
      {
        if (ClassW->getClassName() == m_ActiveClass)
          m_ActiveClass = "";

        ItemsToRemove.append(j);
      }
    }
  }

  // effectively remove classes from layout
  QListIterator<int> itTR(ItemsToRemove);
  itTR.toBack();
  while (itTR.hasPrevious())
    Layout->takeAt(itTR.previous())->widget()->deleteLater();


  // Add of new classes to layout
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
      UnitsClassWidget* ClassW =
          new UnitsClassWidget(ClassesList[i],
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
  {//  ui->GlobalMapView->fitViewToItems();

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


void SpatialDomainWidget::updateRemoveUnitButtonText(int ID)
{
    if (ID >= 0 && ui->IDsListWidget->selectedItems().size() == 1)
    {
      ui->RemoveUnitButton->setText(tr("Remove unit %1 from %2 class").arg(ID).arg(m_ActiveClass));
    }
    else
    {
      ui->RemoveUnitButton->setText(tr("Remove selected units from %2 class").arg(m_ActiveClass));
    }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateUnitSelection(int Row)
{
  if (Row >= 0)
  {
    openfluid::core::UnitID_t ID = ui->IDsListWidget->item(Row)->data(Qt::UserRole).toInt();

    updateRemoveUnitButtonText(ID);

    const openfluid::fluidx::SpatialUnitDescriptor& UnitDesc = m_Domain.spatialUnit(m_ActiveClass.toStdString(),ID);

    // ======== Process order

    disconnect(ui->PcsOrderSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateFluidXProcessOrder(int)));
    ui->PcsOrderSpinBox->setValue(UnitDesc.getProcessOrder());
    connect(ui->PcsOrderSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateFluidXProcessOrder(int)));


    // ======== Connections

    ui->ConnectionsTableWidget->setRowCount(0);

    int CurrentRow = 0;

    std::list<openfluid::core::UnitClassID_t>::iterator itconn;
    std::list<openfluid::core::UnitClassID_t> ConnUnits;

    std::pair<std::string,int> UnitClassID = std::make_pair(m_ActiveClass.toStdString(),ID);

    QTableWidgetItem *Item;

    // to connections
    ConnUnits = m_Domain.toSpatialUnits(UnitClassID);

    for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
    {

      ui->ConnectionsTableWidget->setRowCount(CurrentRow+1);

      Item = new QTableWidgetItem("To");
      // set connection code to 0 for "To"
      Item->setData(Qt::UserRole,BUILDER_CONNCODE_TO);
      ui->ConnectionsTableWidget->setItem(CurrentRow, 0, Item);

      Item = new QTableWidgetItem(QString::fromStdString((*itconn).first));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 1, Item);

      Item = new QTableWidgetItem(QString("%1").arg((*itconn).second));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 2, Item);

      CurrentRow++;
    }


    // from connections
    ConnUnits = m_Domain.getFromSpatialUnits(UnitClassID);

    for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
    {

      ui->ConnectionsTableWidget->setRowCount(CurrentRow+1);

      Item = new QTableWidgetItem("From");
      // set connection code to 1 for "From"
      Item->setData(Qt::UserRole,BUILDER_CONNCODE_FROM);
      ui->ConnectionsTableWidget->setItem(CurrentRow, 0, Item);

      Item = new QTableWidgetItem(QString::fromStdString((*itconn).first));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 1, Item);

      Item = new QTableWidgetItem(QString("%1").arg((*itconn).second));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 2, Item);

      CurrentRow++;
    }


    // parent of connections
    ConnUnits = m_Domain.parentSpatialUnits(UnitClassID);

    for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
    {

      ui->ConnectionsTableWidget->setRowCount(CurrentRow+1);

      Item = new QTableWidgetItem("Child of");
      // set connection code to 2 for "Child of"
      Item->setData(Qt::UserRole,BUILDER_CONNCODE_CHILDOF);
      ui->ConnectionsTableWidget->setItem(CurrentRow, 0, Item);

      Item = new QTableWidgetItem(QString::fromStdString((*itconn).first));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 1, Item);

      Item = new QTableWidgetItem(QString("%1").arg((*itconn).second));
      ui->ConnectionsTableWidget->setItem(CurrentRow, 2, Item);

      CurrentRow++;
    }


    // child of connections
    ConnUnits = m_Domain.getChildSpatialUnits(UnitClassID);

    for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
    {

      ui->ConnectionsTableWidget->setRowCount(CurrentRow+1);

      Item = new QTableWidgetItem("Parent of");
      // set connection code to 3 for "Parent of"
      Item->setData(Qt::UserRole,BUILDER_CONNCODE_PARENTOF);
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


openfluid::core::UnitID_t SpatialDomainWidget::getUnitIDFromAttributesTableRow(int Row)
{
  return (openfluid::core::UnitID_t)ui->AttributesTableWidget->
                       verticalHeaderItem(Row)->text().toInt();
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refreshUnitField()
{
  unsigned int NItems = ui->IDsListWidget->selectedItems().size();
  if (NItems > 1)
  {
    // Disable unit fields if several items selected
    ui->UnitWidget->setEnabled(false);
    updateRemoveUnitButtonText(-1); //-1 calls the multiselection message
  }
  else
  {
    ui->UnitWidget->setEnabled(true);
    if (NItems == 1)
    {
      updateRemoveUnitButtonText(ui->IDsListWidget->selectedItems().last()->data(Qt::UserRole).toInt());
    }
  }
}


// =====================================================================
// =====================================================================


openfluid::core::UnitID_t SpatialDomainWidget::getUnitIDFromIDsListRow(int Row)
{
  return (openfluid::core::UnitID_t)ui->IDsListWidget->item(Row)->data(Qt::UserRole).toInt();
}


// =====================================================================
// =====================================================================


std::set<openfluid::core::UnitID_t> SpatialDomainWidget::getAttributeSelectedUnitSet()
{
  std::set<openfluid::core::UnitID_t> AttSelectedIDs;
  for (QTableWidgetItem* Item : ui->AttributesTableWidget->selectedItems())
  {
    AttSelectedIDs.insert(getUnitIDFromAttributesTableRow(Item->row()));
  }
  return AttSelectedIDs;
}


// =====================================================================
// =====================================================================


bool SpatialDomainWidget::isIDsListAndAttributeSelDiscrepancy()
{
  QList<QListWidgetItem *> IDsListSelItems = ui->IDsListWidget->selectedItems();
  bool IsDiscrepancy = true;
  std::set<openfluid::core::UnitID_t> IDsListSelIDs;
  for (QListWidgetItem* Item : IDsListSelItems)
  {
    IDsListSelIDs.insert(getUnitIDFromIDsListRow(ui->IDsListWidget->row(Item)));
  }

  if (IDsListSelIDs == getAttributeSelectedUnitSet())
  {
      IsDiscrepancy = false;
  }
  return IsDiscrepancy;
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateSelectionFromIDsList()
{
  // Update attribute table if change detected
  if (isIDsListAndAttributeSelDiscrepancy())
  {
    ui->AttributesTableWidget->clearSelection();
    disconnect(ui->AttributesTableWidget,SIGNAL(itemSelectionChanged()),this,
               SLOT(updateIDsListSelectionFromAttributesTableChange()));
    for (QListWidgetItem* Item : ui->IDsListWidget->selectedItems())
    {
      ui->AttributesTableWidget->selectRow(ui->IDsListWidget->row(Item));
    }
    connect(ui->AttributesTableWidget,SIGNAL(itemSelectionChanged()),this,
            SLOT(updateIDsListSelectionFromAttributesTableChange()));
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refreshClassAttributes()
{
  disconnect(ui->AttributesTableWidget,SIGNAL(cellChanged(int,int)),this,
             SLOT(updateFluidXAttributeFromCellValue(int,int)));

  ui->AttributesTableWidget->clear();

  if (!m_ActiveClass.isEmpty())
  {
    QStringList AttrNames = openfluid::tools::toQStringList(m_Domain.getAttributesNames(m_ActiveClass.toStdString()));
    QStringList IDs = openfluid::tools::toQStringList(m_Domain.getIDsOfClass(m_ActiveClass.toStdString()));

    ui->AttributesTableWidget->setColumnCount(AttrNames.size());
    ui->AttributesTableWidget->setRowCount(IDs.size());

    ui->AttributesTableWidget->setHorizontalHeaderLabels(AttrNames);
    ui->AttributesTableWidget->setVerticalHeaderLabels(IDs);

    for (int i=0;i<IDs.size();i++)
    {
      for (int j=0;j<AttrNames.size();j++)
      {
        ui->AttributesTableWidget
          ->setItem(i,j,new QTableWidgetItem(
              QString::fromStdString(m_Domain.getAttribute(m_ActiveClass.toStdString(),IDs[i].toInt(),
                                                           AttrNames[j].toStdString()))));
      }
    }
  }

  ui->AttributesTableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

  connect(ui->AttributesTableWidget,SIGNAL(cellChanged(int,int)),
          this,SLOT(updateFluidXAttributeFromCellValue(int,int)));
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
      const auto& EventsList = m_Domain.spatialUnit(m_ActiveClass.toStdString(),*it).events();

      if (!EventsList.empty())
      {

        for (const auto& Event : EventsList)
        {

          ui->EventsTableWidget->setRowCount(RowIndex+1);

          // storage of the event ID for edition and removal
          QTableWidgetItem *Item =
              new QTableWidgetItem(QString::fromStdString(Event.event().getDateTime().getAsISOString()));
          Item->setData(EventIDRole,QVariant::fromValue<openfluid::fluidx::EventID_t>(Event.getID()));

          ui->EventsTableWidget->setItem(RowIndex,0,Item);

          // unit ID
          ui->EventsTableWidget->setItem(RowIndex,1,new QTableWidgetItem(QString("%1").arg(*it)));


          // associated infos
          openfluid::core::Event::EventInfosMap_t Infos = Event.event().getInfos();
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

    ClassW->linkToDatastoreItem(m_Datastore.getItems(ClassW->getClassName().toStdString(),
                                  openfluid::core::UnstructuredValue::GeoVectorValue));

    if (ClassW->layerSource() != nullptr && ClassW->isLayerVisible())
    {
      mp_MapScene->addLayer(ClassW->layerSource(),
                            -i,
                            ClassW->getLineWidth(),
                            ClassW->getLineColor(),
                            ClassW->getFillColor());
    }
  }

  mp_MapScene->setActiveLayer(m_ActiveClass);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::setAllMapLayersVisible()
{
  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  for (int i=Layout->count()-2; i>=0;i--)
  {
    UnitsClassWidget* ClassW = dynamic_cast<UnitsClassWidget*>(Layout->itemAt(i)->widget());

    ClassW->setLayerVisible();
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::addUnitsClass()
{
  QStringList ExistingClasses = openfluid::tools::toQStringList(m_Domain.getClassNames());

  AddUnitsClassDialog AddDlg(ExistingClasses,this);
  if (AddDlg.exec() == QDialog::Accepted)
  {
    openfluid::fluidx::SpatialUnitDescriptor UDesc;
    UDesc.setUnitsClass(AddDlg.getClassName().toStdString());
    UDesc.setID(AddDlg.getUnitID());
    UDesc.setProcessOrder(AddDlg.getUnitPcsOrd());
    m_Domain.addUnit(UDesc);

    updateUpDownButtons();

    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT);

    refresh();
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::moveUnitsClassUp(QString ClassName)
{
  int Position = getClassIndex(ClassName);

  if (Position < 1 )
    return;

  int From = Position;
  int To = Position-1;

  UnitsClassWidget* W = static_cast<UnitsClassWidget*>(ui->UnitsClassAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(ui->UnitsClassAreaContents->layout()))->insertWidget(To,W);

  updateUpDownButtons();

  openfluid::base::RunContextManager::instance()->setProjectConfigValue("builder.spatial.unitsclasses",
                                                                        "order",
                                                                        getClassesOrderedStringList());

  refreshMap();
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::moveUnitsClassDown(QString ClassName)
{
  int Position = getClassIndex(ClassName);

  if (Position < 0)
    return;

  QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(ui->UnitsClassAreaContents->layout());

  if (Position > Layout->count()-2)
    return;

  int From = Position;
  int To = Position+1;

  UnitsClassWidget* W = static_cast<UnitsClassWidget*>(ui->UnitsClassAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(ui->UnitsClassAreaContents->layout()))->insertWidget(To,W);

  updateUpDownButtons();

  openfluid::base::RunContextManager::instance()->setProjectConfigValue("builder.spatial.unitsclasses",
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
  QStringList AttrsNames = openfluid::tools::toQStringList(m_Domain.getAttributesNames(m_ActiveClass.toStdString()));

  QStringList UnitsIDs = openfluid::tools::toQStringList(m_Domain.getIDsOfClass(m_ActiveClass.toStdString()));

  AddUnitToClassDialog AddDlg(m_ActiveClass,AttrsNames,UnitsIDs,this);
  if (AddDlg.exec() == QDialog::Accepted)
  {
    int ID = AddDlg.getUnitID();

    // create unit in FluidX descriptor
    openfluid::fluidx::SpatialUnitDescriptor UDesc;
    UDesc.setUnitsClass(m_ActiveClass.toStdString());
    UDesc.setID(ID);
    UDesc.setProcessOrder(AddDlg.getUnitPcsOrd());

    m_Domain.addUnit(UDesc);


    // create attributes in FluidX descriptor
    QMap<QString,QString> Attrs = AddDlg.getAttributes();
    QMapIterator<QString, QString> it(Attrs);

    while (it.hasNext())
    {
      it.next();
      m_Domain.setAttribute(m_ActiveClass.toStdString(),AddDlg.getUnitID(),
                           it.key().toStdString(),it.value().toStdString());
    }

    // add unit in ui
    refreshClassStructure();

    // add unit attributes in ui, and restore current attribute selection
    refreshClassAttributes();
    ui->AttributesTableWidget->selectRow(ui->IDsListWidget->currentRow());

    refreshMap();

    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT |
                 openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::removeUnit()
{
  QList<QListWidgetItem *> IDsListSelectedItems = ui->IDsListWidget->selectedItems();

  bool OK = true;

  if (openfluid::base::PreferencesManager::instance()->isBuilderSpatialUnitsRemovalConfirm())
  {
    QString ToRemoveString;
    if (IDsListSelectedItems.size() == 1)
    {
      int ID = IDsListSelectedItems.last()->data(Qt::UserRole).toInt();
      ToRemoveString = tr("You are removing the unit %1 of class %2.\n"
                          "All connections and attributes units related to this unit will be lost.\n\n"
                         ).arg(ID).arg(m_ActiveClass);
    }
    else
    {
      ToRemoveString = tr("You are removing several units of class %2.\n"
                          "All connections and attributes units related to these units will be lost.\n\n"
                         ).arg(m_ActiveClass);
    }

    OK = (QMessageBox::question(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                ToRemoveString + tr("Proceed anyway?"),
                                     QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
  }

  if (OK)
  {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    for (QListWidgetItem* Item : IDsListSelectedItems)
    {
      m_Domain.deleteUnit(m_ActiveClass.toStdString(), Item->data(Qt::UserRole).toInt());
    }
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
  openfluid::core::UnitID_t SrcID = ui->IDsListWidget->currentItem()->data(Qt::UserRole).toInt();

  AddConnectionDialog AddDlg(m_ActiveClass, QString("%1").arg(SrcID),
                             &m_Domain,
                             this);

  if (AddDlg.exec() == QDialog::Accepted)
  {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    ConnectionCode ConnCode = AddDlg.getConnectionCode();
    std::string SrcClass = m_ActiveClass.toStdString();
    std::string DestClass = AddDlg.getDestinationClass().toStdString();
    openfluid::core::UnitID_t DestID = AddDlg.getDestinationID().toInt();

    if (ConnCode == BUILDER_CONNCODE_TO)
    {
      m_Domain.addFromToRelation(make_pair(SrcClass,SrcID),
                                 make_pair(DestClass,DestID));
    }
    else if (ConnCode == BUILDER_CONNCODE_FROM)
    {
      m_Domain.addFromToRelation(make_pair(DestClass,DestID),
                                 make_pair(SrcClass,SrcID));
    }
    else if (ConnCode == BUILDER_CONNCODE_PARENTOF)
    {
      m_Domain.addParentChildRelation(make_pair(SrcClass,SrcID),
                                      make_pair(DestClass,DestID));
    }
    else if (ConnCode == BUILDER_CONNCODE_CHILDOF)
    {
      m_Domain.addParentChildRelation(make_pair(DestClass,DestID),
                                      make_pair(SrcClass,SrcID));
    }

    updateUnitSelection(ui->IDsListWidget->currentRow());

    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT);

    QApplication::restoreOverrideCursor();
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::removeConnection()
{
  if (ui->ConnectionsTableWidget->selectedItems().isEmpty())
  {
    QMessageBox::critical(QApplication::activeWindow(),QString("OpenFLUID-builder"),
                          tr("No connection selected for removal"),QMessageBox::Close);
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

    if (openfluid::base::PreferencesManager::instance()->isBuilderSpatialConnsRemovalConfirm())
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

       if (ConnCode == BUILDER_CONNCODE_TO)
       {
         m_Domain.removeFromToRelation(make_pair(SrcClass.toStdString(),SrcID),
                                       make_pair(DestClass.toStdString(),DestID));
       }
       else if (ConnCode == BUILDER_CONNCODE_FROM)
       {
         m_Domain.removeFromToRelation(make_pair(DestClass.toStdString(),DestID),
                                       make_pair(SrcClass.toStdString(),SrcID));
       }
       else if (ConnCode == BUILDER_CONNCODE_PARENTOF)
       {
         m_Domain.removeParentChildRelation(make_pair(SrcClass.toStdString(),SrcID),
                                            make_pair(DestClass.toStdString(),DestID));
       }
       else if (ConnCode == BUILDER_CONNCODE_CHILDOF)
       {
         m_Domain.removeParentChildRelation(make_pair(DestClass.toStdString(),DestID),
                                            make_pair(SrcClass.toStdString(),SrcID));
       }

       updateUnitSelection(ui->IDsListWidget->currentRow());

       emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT);

       QApplication::restoreOverrideCursor();
    }
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::addAttribute()
{
  EditAttributeNameDialog AddDlg(EditAttributeNameDialog::EDIT_ADD,
                                 ProjectCentral::instance()->attributesLists().value(m_ActiveClass),this);

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
        for (QTableWidgetItem* Item : ui->AttributesTableWidget->selectedItems())
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
        for (QTableWidgetItem* Item : ui->AttributesTableWidget->selectedItems())
        {
          Item->text().toDouble(&LocalOK);
          GlobalOK &= LocalOK;
        }

        if (GlobalOK)
        {
          if (EditDlg.getEditMode() == EditAttributesValuesDialog::EDIT_MULTIPLY)
          {
            double MultValue = EditDlg.getMultiplyValue();
            for (QTableWidgetItem* Item : ui->AttributesTableWidget->selectedItems())
            {
              double Value = Item->text().toDouble();
              Value *= MultValue;
              Item->setText(QString("%1").arg(Value,0,'g',12));
            }
          }
          else
          {
            double AddValue = EditDlg.getAddValue();
            for (QTableWidgetItem* Item : ui->AttributesTableWidget->selectedItems())
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
  QStringList AttrsNames = openfluid::tools::toQStringList(m_Domain.getAttributesNames(m_ActiveClass.toStdString()));

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

  QStringList AttrsNames = openfluid::tools::toQStringList(m_Domain.getAttributesNames(m_ActiveClass.toStdString()));

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
      if (openfluid::base::PreferencesManager::instance()->isBuilderSpatialAttrsRemovalConfirm())
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
    UnitsClassWidget* W = static_cast<UnitsClassWidget*>(ui->UnitsClassAreaContents->layout()->itemAt(i)->widget());
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
  QList<QGraphicsItem *> MapSelectedItems = mp_MapScene->selectedItems();
  bool IsDiscrepancy = true;
  std::set<openfluid::core::UnitID_t> MapSelectedIDs;
  for (QGraphicsItem* Item : MapSelectedItems)
  {
    MapSelectedIDs.insert(dynamic_cast<MapItemGraphics*>(Item)->getUnitID());
  }

  if (MapSelectedIDs == getAttributeSelectedUnitSet()) {
      IsDiscrepancy = false;
  }

  if (!MapSelectedItems.isEmpty() && IsDiscrepancy)
  {
    disconnect(ui->AttributesTableWidget,SIGNAL(itemSelectionChanged()),this,
        SLOT(updateMapFromAttributesTableChange()));
    ui->AttributesTableWidget->clearSelection();
    for (QGraphicsItem* Item : MapSelectedItems)
    {
      openfluid::core::UnitID_t IDMap = dynamic_cast<MapItemGraphics*>(Item)->getUnitID();
      for (int i=0; i<ui->AttributesTableWidget->rowCount();i++)
      {
        if (getUnitIDFromAttributesTableRow(i) == IDMap)
        {
            ui->AttributesTableWidget->selectRow(i);
        }
      }
    }
    connect(ui->AttributesTableWidget,SIGNAL(itemSelectionChanged()),this,
          SLOT(updateMapFromAttributesTableChange()));
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateIDsListSelectionFromAttributesTableChange()
{
  QList<QTableWidgetItem *> AttSelItems = ui->AttributesTableWidget->selectedItems();
  if (!AttSelItems.isEmpty() && isIDsListAndAttributeSelDiscrepancy())
  {
    int CurrentIDRow = ui->IDsListWidget->currentRow();
    bool IsCurrentRowInSelection = false;
    disconnect(ui->IDsListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(updateSelectionFromIDsList()));
    ui->IDsListWidget->clearSelection();

    for (QTableWidgetItem* Item : AttSelItems)
    {
      ui->IDsListWidget->item(Item->row())->setSelected(true);
      if (Item->row()==CurrentIDRow)
      {
          IsCurrentRowInSelection = true;
      }
    }

    QList<QListWidgetItem*> IDsListSelected = ui->IDsListWidget->selectedItems();
    if (!IsCurrentRowInSelection && IDsListSelected.size() == 1)
    {
        ui->IDsListWidget->setCurrentItem(IDsListSelected.first());
    }
    connect(ui->IDsListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(updateSelectionFromIDsList()));
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateMapFromAttributesTableChange()
{
  disconnect(mp_MapScene,SIGNAL(selectionChanged()),this,SLOT(updateSelectionFromMap()));
  mp_MapScene->clearSelection();

  for (QTableWidgetItem* Item : ui->AttributesTableWidget->selectedItems())
  {
    // Update map based on selection change
    for(QGraphicsItem* MapItem : mp_MapScene->items())
    {
      if (dynamic_cast<MapItemGraphics*>(MapItem)->getUnitID() ==
          getUnitIDFromAttributesTableRow(Item->row()))
      {
        MapItem->setSelected(true);
      }
    }
  }
  connect(mp_MapScene,SIGNAL(selectionChanged()),this,SLOT(updateSelectionFromMap()));
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateFluidXAttributeFromCellValue(int Row, int Column)
{
  std::string Value = ui->AttributesTableWidget->item(Row,Column)->text().toStdString();
  std::string Attr = ui->AttributesTableWidget->horizontalHeaderItem(Column)->text().toStdString();
  int ID = ui->AttributesTableWidget->verticalHeaderItem(Row)->text().toInt();


  if (Value.empty())
  {
    QMessageBox::critical(QApplication::activeWindow(),QString("OpenFLUID-builder"),
                              tr("Spatial attribute cannot be empty"),QMessageBox::Close);

    // disconnect to avoid emission of cellChanged() signal
    disconnect(ui->AttributesTableWidget,SIGNAL(cellChanged(int,int)),this,
            SLOT(updateFluidXAttributeFromCellValue(int,int)));

    // revert to previous non-empty value
    ui->AttributesTableWidget->item(Row,Column)
      ->setText(QString::fromStdString(m_Domain.getAttribute(m_ActiveClass.toStdString(),ID,Attr)));

    // reconnect to take into account later editing of the cell value
    connect(ui->AttributesTableWidget,SIGNAL(cellChanged(int,int)),this,
            SLOT(updateFluidXAttributeFromCellValue(int,int)));
  }
  else
  {
    m_Domain.setAttribute(m_ActiveClass.toStdString(),ID,Attr,Value);

    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::updateFluidXProcessOrder(int PcsOrd)
{
  int ID = ui->IDsListWidget->currentItem()->text().toInt();
  (&const_cast<openfluid::fluidx::SpatialUnitDescriptor&>(m_Domain.spatialUnit(m_ActiveClass.toStdString(),ID)))
      ->setProcessOrder(PcsOrd);

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT);
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::addEvent()
{
  QStringList UnitsIDsList = openfluid::tools::toQStringList(m_Domain.getIDsOfClass(m_ActiveClass.toStdString()));

  AddEventDialog AddEventDlg(m_ActiveClass,UnitsIDsList,
                             m_FluidxDesc.runConfiguration().getBeginDate(),
                             this);

  if (AddEventDlg.exec() == QDialog::Accepted)
  {
    openfluid::core::Event Ev(AddEventDlg.getDateTime());

    const openfluid::core::Event::EventInfosMap_t EvInfos = AddEventDlg.getInfos();

    openfluid::core::Event::EventInfosMap_t::const_iterator it;
    openfluid::core::Event::EventInfosMap_t::const_iterator itb = EvInfos.begin();
    openfluid::core::Event::EventInfosMap_t::const_iterator ite = EvInfos.end();

    for (it=itb;it!=ite;++it)
      Ev.addInfo((*it).first,(*it).second);

    m_Domain.addEvent(m_ActiveClass.toStdString(),
                      AddEventDlg.getUnitID(),
                      Ev);

    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALEVENTS);

    refreshClassEvents();
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::editEvent()
{
  int Row = ui->EventsTableWidget->currentRow();

  if (Row >= 0)
  {
    openfluid::fluidx::EventID_t OriginEventID =
        ui->EventsTableWidget->item(Row,0)->data(EventIDRole).value<openfluid::fluidx::EventID_t>();

    openfluid::fluidx::EventDescriptor* EvDesc = &(m_Domain.event(OriginEventID));

    if (EvDesc)
    {
      QStringList UnitsIDsList = openfluid::tools::toQStringList(m_Domain.getIDsOfClass(m_ActiveClass.toStdString()));

      openfluid::core::UnitID_t OriginUnitID = ui->EventsTableWidget->item(Row,1)->text().toInt();

      EditEventDialog EditEventDlg(m_ActiveClass,UnitsIDsList,
                                  ui->EventsTableWidget->item(Row,1)->text(),
                                  EvDesc->event().getDateTime(),
                                  EvDesc->event().getInfos(),
                                  this);


      if (EditEventDlg.exec() == QDialog::Accepted)
      {
        openfluid::core::UnitID_t FinalUnitID = EditEventDlg.getUnitID();

        openfluid::core::Event Ev(EditEventDlg.getDateTime());
        const openfluid::core::Event::EventInfosMap_t EvInfos = EditEventDlg.getInfos();

        openfluid::core::Event::EventInfosMap_t::const_iterator it;
        openfluid::core::Event::EventInfosMap_t::const_iterator itb = EvInfos.begin();
        openfluid::core::Event::EventInfosMap_t::const_iterator ite = EvInfos.end();

        for (it=itb;it!=ite;++it)
          Ev.addInfo((*it).first,(*it).second);


        if (OriginUnitID == FinalUnitID)
        {
          m_Domain.modifyEvent(OriginEventID,Ev);
        }
        else
        {
          m_Domain.deleteEvent(m_ActiveClass.toStdString(),OriginUnitID,OriginEventID);
          m_Domain.addEvent(m_ActiveClass.toStdString(),FinalUnitID,Ev);
        }

        emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALEVENTS);

        refreshClassEvents();
      }
    }
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::removeEvents()
{
  bool OK = true;

  QList<QTableWidgetItem *> SelItems = ui->EventsTableWidget->selectedItems();

  if (SelItems.size() > 0)
  {
    OK = (QMessageBox::question(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                tr("You are removing one or many spatial event(s).\n"
                                   "All associated informations will be lost.\n"
                                   "\n"
                                   "Proceed anyway?"),
                                QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);

    if (OK)
    {

      while(!SelItems.isEmpty())
      {
        openfluid::core::UnitID_t UnitID = ui->EventsTableWidget->item(SelItems[0]->row(),1)->text().toInt();

        openfluid::fluidx::EventID_t EventID =
            ui->EventsTableWidget->item(SelItems[0]->row(),0)->data(EventIDRole).value<openfluid::fluidx::EventID_t>();

        m_Domain.deleteEvent(m_ActiveClass.toStdString(),UnitID,EventID);

        ui->EventsTableWidget->removeRow(SelItems[0]->row());
        SelItems = ui->EventsTableWidget->selectedItems();
      }

      emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALEVENTS);

      refreshClassEvents();
    }
  }
}

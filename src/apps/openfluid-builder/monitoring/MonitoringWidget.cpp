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
  \file MonitoringWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/tools/QtHelpers.hpp>

#include "ui_WaresManagementWidget.h"
#include "ui_MonitoringWidget.h"
#include "ui_AddWareDialog.h"
#include "MonitoringWidget.hpp"
#include "ObserverWidget.hpp"
#include "AddObserverDialog.hpp"



MonitoringWidget::MonitoringWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WorkspaceWidget(Parent, AFXDesc), ui(new Ui::MonitoringWidget), m_Monitoring(AFXDesc.monitoring())
{
  ui->setupUi(this);

  mp_WaresManWidget = new WaresManagementWidget(this,false);
  layout()->addWidget(mp_WaresManWidget);

  mp_WaresManWidget->ui->AddWareFirstButton->setText(tr("Add observer"));

  connect(mp_WaresManWidget->ui->AddWareFirstButton,SIGNAL(clicked()),this,SLOT(addObserver()));

  refresh();

}


// =====================================================================
// =====================================================================


MonitoringWidget::~MonitoringWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void MonitoringWidget::addObserver()
{
  QStringList ObsList = openfluid::tools::toQStringList(m_Monitoring.getOrderedIDs());

  AddObserverDialog AddObsDlg(ObsList,this);

  // TODo duplicate observers may appears in list if a path is defined twice
  if (AddObsDlg.exec() == QDialog::Accepted)
  {
    openfluid::ware::WareID_t ID = AddObsDlg.getSelectedID().toStdString();

    m_Monitoring.addToObserverList(ID);

    ObserverWidget* ObsWidget = new ObserverWidget(this,&m_Monitoring.descriptor(ID),ID);

    connect(ObsWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
    connect(ObsWidget,SIGNAL(srcEditAsked(const QString&)),this,SLOT(notifySrcEditAsked(const QString&)));
    connect(ObsWidget,SIGNAL(upClicked(const QString&)),this,SLOT(moveModelItemUp(const QString&)));
    connect(ObsWidget,SIGNAL(downClicked(const QString&)),this,SLOT(moveModelItemDown(const QString&)));
    connect(ObsWidget,SIGNAL(removeClicked(const QString&)),this,SLOT(removeModelItem(const QString&)));

    // compute position in layout, taking into account the ending spacer
    int Position = mp_WaresManWidget->ui->WaresListAreaContents->layout()->count()-1;
    ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(Position,ObsWidget);

    mp_WaresManWidget->updateUpDownButtons();

    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MONITORING);
  }
}


// =====================================================================
// =====================================================================


void MonitoringWidget::moveModelItemUp(const QString& ID)
{
  int From = m_Monitoring.getFirstIndex(ID.toStdString());
  m_Monitoring.moveItemTowardsTheBeginning(ID.toStdString());
  int To = m_Monitoring.getFirstIndex(ID.toStdString());

  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  mp_WaresManWidget->updateUpDownButtons();

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MONITORING);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::moveModelItemDown(const QString& ID)
{
  int From = m_Monitoring.getFirstIndex(ID.toStdString());
  m_Monitoring.moveItemTowardsTheEnd(ID.toStdString());
  int To = m_Monitoring.getFirstIndex(ID.toStdString());


  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  mp_WaresManWidget->updateUpDownButtons();

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MONITORING);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::removeModelItem(const QString& ID)
{
  int Position = m_Monitoring.getFirstIndex(ID.toStdString());

  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(Position)->widget());
  W->deleteLater();

  m_Monitoring.removeFromObserverList(ID.toStdString());

  mp_WaresManWidget->updateUpDownButtons();

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MONITORING);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::refresh()
{
  const std::list<openfluid::fluidx::ObserverDescriptor*>& Items = m_Monitoring.items();

  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator itb = Items.begin();
  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator ite = Items.end();
  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator it;
  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator itl = Items.end().operator --();


  for (it = itb; it!= ite; ++it)
  {
    ObserverWidget* ObsWidget = new ObserverWidget(this,*it,(*it)->getID());
    mp_WaresManWidget->ui->WaresListAreaContents->layout()->addWidget(ObsWidget);
    if (it == itb) ObsWidget->setUpButtonEnabled(false);
    if (it == itl) ObsWidget->setDownButtonEnabled(false);

    connect(ObsWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
    connect(ObsWidget,SIGNAL(srcEditAsked(const QString&)),this,SLOT(notifySrcEditAsked(const QString&)));
    connect(ObsWidget,SIGNAL(upClicked(const QString&)),this,SLOT(moveModelItemUp(const QString&)));
    connect(ObsWidget,SIGNAL(downClicked(const QString&)),this,SLOT(moveModelItemDown(const QString&)));
    connect(ObsWidget,SIGNAL(removeClicked(const QString&)),this,SLOT(removeModelItem(const QString&)));

  }
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->addStretch();
}


// =====================================================================
// =====================================================================


void MonitoringWidget::dispatchChangesFromChildren()
{
  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MONITORING);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::notifySrcEditAsked(const QString& ID)
{
  emit srcEditAsked(ID,openfluid::ware::PluggableWare::OBSERVER);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::updateWares()
{
  int LastIndex = mp_WaresManWidget->ui->WaresListAreaContents->layout()->count()-1;

  for (int i=0;i<=LastIndex;i++)
  {
    WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->itemAt(i)->widget());
    if (W != NULL) W->updateWare();
  }

}


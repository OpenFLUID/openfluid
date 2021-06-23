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
  @file MonitoringWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/tools/DataHelpers.hpp>

#include "ui_WaresManagementWidget.h"
#include "ui_MonitoringWidget.h"
#include "ui_AddWareDialog.h"
#include "MonitoringWidget.hpp"
#include "ObserverWidget.hpp"
#include "AddObserverDialog.hpp"


MonitoringWidget::MonitoringWidget(QWidget* Parent, openfluid::fluidx::FluidXDescriptor& FXDesc):
  WorkspaceWidget(Parent, FXDesc), ui(new Ui::MonitoringWidget), m_Monitoring(FXDesc.monitoring())
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
  AddObserverDialog AddObsDlg(this);

  if (AddObsDlg.exec() == QDialog::Accepted)
  {
    openfluid::ware::WareID_t ID = AddObsDlg.getSelectedID().toStdString();

    openfluid::fluidx::ObserverDescriptor* ObsDesc = new openfluid::fluidx::ObserverDescriptor(ID);

    m_Monitoring.appendItem(ObsDesc);

    ObserverWidget* ObsWidget = new ObserverWidget(this,ObsDesc,ID,0);

    connect(ObsWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
    connect(ObsWidget,SIGNAL(srcEditAsked(const QString&)),this,SLOT(notifySrcEditAsked(const QString&)));
    connect(ObsWidget,SIGNAL(upClicked(const QString&,int)),this,SLOT(moveModelItemUp(const QString&,int)));
    connect(ObsWidget,SIGNAL(downClicked(const QString&,int)),this,SLOT(moveModelItemDown(const QString&,int)));
    connect(ObsWidget,SIGNAL(removeClicked(const QString&,int)),this,SLOT(removeModelItem(const QString&,int)));

    // compute position in layout, taking into account the ending spacer
    int Position = mp_WaresManWidget->ui->WaresListAreaContents->layout()->count()-1;
    ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(Position,ObsWidget);

    mp_WaresManWidget->updateIndexesAndButtons();

    emit changed(openfluid::builderext::FluidXUpdateFlags::Flag::FLUIDX_MONITORING);
  }
}


// =====================================================================
// =====================================================================


void MonitoringWidget::moveModelItemUp(const QString& /*ID*/, int CurrentIndex)
{
  if (CurrentIndex < 0)
  {
    return;
  }

  unsigned int From = CurrentIndex;
  unsigned int To = (From == 0) ? m_Monitoring.getItemsCount() - 1 : From - 1;

  m_Monitoring.moveItem(From, To);

  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  mp_WaresManWidget->updateIndexesAndButtons();

  emit changed(openfluid::builderext::FluidXUpdateFlags::Flag::FLUIDX_MONITORING);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::moveModelItemDown(const QString& /*ID*/, int CurrentIndex)
{
  if (CurrentIndex < 0)
  {
    return;
  }

  unsigned int From = CurrentIndex;
  unsigned int To = (From == (unsigned int)m_Monitoring.getItemsCount() - 1) ? 0 : From + 1;

  m_Monitoring.moveItem(From,To);


  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  mp_WaresManWidget->updateIndexesAndButtons();

  emit changed(openfluid::builderext::FluidXUpdateFlags::Flag::FLUIDX_MONITORING);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::removeModelItem(const QString& /*ID*/, int CurrentIndex)
{
  if (CurrentIndex < 0)
  {
    return;
  }

  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(CurrentIndex)->widget());
  W->deleteLater();

  m_Monitoring.removeItem(CurrentIndex);

  mp_WaresManWidget->updateIndexesAndButtons();

  emit changed(openfluid::builderext::FluidXUpdateFlags::Flag::FLUIDX_MONITORING);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::refresh()
{
  const std::list<openfluid::fluidx::ObserverDescriptor*>& Items = m_Monitoring.items();

  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator itb = Items.begin();
  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator ite = Items.end();
  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator it;
  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator itl = Items.end().operator--();


  for (it = itb; it!= ite; ++it)
  {
    ObserverWidget* ObsWidget = new ObserverWidget(this,*it,(*it)->getID(),0);
    mp_WaresManWidget->ui->WaresListAreaContents->layout()->addWidget(ObsWidget);
    if (it == itb)
    {
      ObsWidget->setUpButtonEnabled(false);
    }
    if (it == itl)
    {
      ObsWidget->setDownButtonEnabled(false);
    }

    connect(ObsWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
    connect(ObsWidget,SIGNAL(srcEditAsked(const QString&)),this,SLOT(notifySrcEditAsked(const QString&)));
    connect(ObsWidget,SIGNAL(upClicked(const QString&,int)),this,SLOT(moveModelItemUp(const QString&,int)));
    connect(ObsWidget,SIGNAL(downClicked(const QString&,int)),this,SLOT(moveModelItemDown(const QString&,int)));
    connect(ObsWidget,SIGNAL(removeClicked(const QString&,int)),this,SLOT(removeModelItem(const QString&,int)));

  }
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->addStretch();

  mp_WaresManWidget->updateIndexesAndButtons();
}


// =====================================================================
// =====================================================================


void MonitoringWidget::dispatchChangesFromChildren()
{
  emit changed(openfluid::builderext::FluidXUpdateFlags::Flag::FLUIDX_MONITORING);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::notifySrcEditAsked(const QString& ID)
{
  emit srcEditAsked(ID,openfluid::ware::WareType::OBSERVER,false);
}


// =====================================================================
// =====================================================================


void MonitoringWidget::prepareWaresUpdate()
{
  int LastIndex = mp_WaresManWidget->ui->WaresListAreaContents->layout()->count()-1;

  for (int i=0;i<=LastIndex;i++)
  {
    WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->itemAt(i)->widget());
    if (W != nullptr)
    {
      W->prepareWareUpdate();
    }
  }
}


// =====================================================================
// =====================================================================


void MonitoringWidget::updateWares()
{
  int LastIndex = mp_WaresManWidget->ui->WaresListAreaContents->layout()->count()-1;

  for (int i=0;i<=LastIndex;i++)
  {
    WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->itemAt(i)->widget());
    if (W != nullptr)
    {
      W->updateWare();
    }
  }

}


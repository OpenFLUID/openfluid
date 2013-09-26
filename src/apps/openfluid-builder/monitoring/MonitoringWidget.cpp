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
  \file MonitoringWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <iostream>
#include <QMessageBox>

#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>


#include "ui_WaresManagementWidget.h"
#include "ui_MonitoringWidget.h"
#include "MonitoringWidget.hpp"
#include "ObserverWidget.hpp"


MonitoringWidget::MonitoringWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WorkspaceWidget(Parent, AFXDesc), ui(new Ui::MonitoringWidget), m_Monitoring(AFXDesc.getMonitoring())
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
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
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

  emit changed();
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

  emit changed();
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

  emit changed();
}


// =====================================================================
// =====================================================================


void MonitoringWidget::refresh()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  const std::list<openfluid::fluidx::ObserverDescriptor*>& Items = m_Monitoring.getItems();

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
  emit changed();
}

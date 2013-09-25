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
  \file ModelWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <iostream>
#include <QMessageBox>

#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

#include <openfluid/machine/SimulatorSignatureRegistry.hpp>

#include "ui_WaresManagementWidget.h"
#include "ModelWidget.hpp"
#include "SimulatorWidget.hpp"
#include "GeneratorWidget.hpp"


ModelWidget::ModelWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WaresManagementWidget(Parent, AFXDesc,true,true), m_Model(AFXDesc.getModel())
{
  ui->WaresListLabel->setText(tr("Coupled model"));

  ui->AddWareFirstButton->setText(tr("Add simulator"));
  ui->AddWareSecondButton->setText(tr("Add generator"));

  connect(ui->AddWareFirstButton,SIGNAL(clicked()),this,SLOT(addSimulator()));
  connect(ui->AddWareSecondButton,SIGNAL(clicked()),this,SLOT(addGenerator()));
  connect(ui->AddGlobalParamButton,SIGNAL(clicked()),this,SLOT(addGlobalParam()));

  refresh();
}


// =====================================================================
// =====================================================================


ModelWidget::~ModelWidget()
{

}


// =====================================================================
// =====================================================================


void ModelWidget::addSimulator()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void ModelWidget::addGenerator()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void ModelWidget::addGlobalParam()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void ModelWidget::moveModelItemUp(const QString& ID)
{
  int Position = m_Model.getFirstItemIndex(ID.toStdString());

  if (Position < 0)
    return;

  unsigned int From = Position;
  unsigned int To = (From == 0) ? m_Model.getItemsCount() - 1 : From - 1;

  m_Model.moveItem(From, To);

  WareWidget* W = (WareWidget*)(ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  updateUpDownButtons();

  dispatchChangesFromChildren();
}


// =====================================================================
// =====================================================================


void ModelWidget::moveModelItemDown(const QString& ID)
{
  int Position = m_Model.getFirstItemIndex(ID.toStdString());

  if (Position < 0)
    return;

  unsigned int From = Position;
  unsigned int To = (From == m_Model.getItemsCount() - 1) ? 0 : From + 1;

  m_Model.moveItem(From, To);


  WareWidget* W = (WareWidget*)(ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  updateUpDownButtons();

  dispatchChangesFromChildren();

}


// =====================================================================
// =====================================================================


void ModelWidget::removeModelItem(const QString& ID)
{
  int Position = m_Model.getFirstItemIndex(ID.toStdString());

  if (Position < 0)
    return;

  WareWidget* W = (WareWidget*)(ui->WaresListAreaContents->layout()->takeAt(Position)->widget());
  W->deleteLater();

  m_Model.removeItem(Position);

  updateUpDownButtons();

  dispatchChangesFromChildren();
}


// =====================================================================
// =====================================================================


void ModelWidget::refresh()
{
  updateCoupledModel();

  updateGlobalParams();
}


// =====================================================================
// =====================================================================


void ModelWidget::updateGlobalParams()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}


// =====================================================================
// =====================================================================


void ModelWidget::updateCoupledModel()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items = m_Model.getItems();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itb = Items.begin();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator ite = Items.end();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itl = Items.end().operator --();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it;

  for (it = itb; it!= ite; ++it)
  {
    if ((*it)->getType() == openfluid::fluidx::WareDescriptor::PluggedSimulator)
    {
      SimulatorWidget* SimWidget = new SimulatorWidget(this,*it,m_Model.getID(*it));
      ui->WaresListAreaContents->layout()->addWidget(SimWidget);
      if (it == itb) SimWidget->setUpButtonEnabled(false);
      if (it == itl) SimWidget->setDownButtonEnabled(false);

      connect(SimWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
      connect(SimWidget,SIGNAL(upClicked(const QString&)),this,SLOT(moveModelItemUp(const QString&)));
      connect(SimWidget,SIGNAL(downClicked(const QString&)),this,SLOT(moveModelItemDown(const QString&)));
      connect(SimWidget,SIGNAL(removeClicked(const QString&)),this,SLOT(removeModelItem(const QString&)));
    }
    else if ((*it)->getType() == openfluid::fluidx::WareDescriptor::Generator)
    {
      // TODO see if a more elegant method is possible for generators signature
      // than passing signature instance to constructor

      GeneratorWidget* GenWidget = new GeneratorWidget(this,*it,
                                                       m_Model.getID(*it),
                                                       openfluid::machine::SimulatorSignatureRegistry::getInstance()->getSignatureItemInstance(*it));

      ui->WaresListAreaContents->layout()->addWidget(GenWidget);
      if (it == itb) GenWidget->setUpButtonEnabled(false);
      if (it == itl) GenWidget->setDownButtonEnabled(false);

      connect(GenWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
      connect(GenWidget,SIGNAL(upClicked(const QString&)),this,SLOT(moveModelItemUp(const QString&)));
      connect(GenWidget,SIGNAL(downClicked(const QString&)),this,SLOT(moveModelItemDown(const QString&)));
      connect(GenWidget,SIGNAL(removeClicked(const QString&)),this,SLOT(removeModelItem(const QString&)));

    }
  }
  ((QBoxLayout*)(ui->WaresListAreaContents->layout()))->addStretch();
}

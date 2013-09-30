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
#include "ui_ModelWidget.h"
#include "ModelWidget.hpp"
#include "SimulatorWidget.hpp"
#include "GeneratorWidget.hpp"


ModelWidget::ModelWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WorkspaceWidget(Parent, AFXDesc), ui(new Ui::ModelWidget), m_Model(AFXDesc.getModel())
{
  ui->setupUi(this);

  mp_WaresManWidget = new WaresManagementWidget(this,true);
  ui->ModelTabWidget->insertTab(0,mp_WaresManWidget,"Management");
  ui->ModelTabWidget->setCurrentIndex(0);

  ui->GraphicalView->setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);
  mp_ModelScene = new ModelScene(m_Model,this);
  ui->GraphicalView->setScene(mp_ModelScene);

  mp_ShowHideGlobalParamsLabel = new ActionLabel(tr("show"),ui->GlobalParamsWidget);
  ui->GlobalParamsTitleLayout->insertWidget(1,mp_ShowHideGlobalParamsLabel);
  ui->GlobalParamsManagementWidget->setVisible(false);

  ui->AddGlobalParamButton->setText("");
  ui->AddGlobalParamButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddGlobalParamButton->setIconSize(QSize(20,20));

  mp_WaresManWidget->ui->AddWareFirstButton->setText(tr("Add simulator"));
  mp_WaresManWidget->ui->AddWareSecondButton->setText(tr("Add generator"));

  connect(mp_ShowHideGlobalParamsLabel,SIGNAL(clicked()),this,SLOT(updateShowHideGlobalParams()));
  connect(ui->AddGlobalParamButton,SIGNAL(clicked()),this,SLOT(addGlobalParam()));

  connect(mp_WaresManWidget->ui->AddWareFirstButton,SIGNAL(clicked()),this,SLOT(addSimulator()));
  connect(mp_WaresManWidget->ui->AddWareSecondButton,SIGNAL(clicked()),this,SLOT(addGenerator()));

  refresh();
}


// =====================================================================
// =====================================================================


ModelWidget::~ModelWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void ModelWidget::updateShowHideGlobalParams()
{
  if (ui->GlobalParamsManagementWidget->isVisible())
  {
    ui->GlobalParamsManagementWidget->setVisible(false);
    mp_ShowHideGlobalParamsLabel->setText(tr("show"));
  }
  else
  {
    ui->GlobalParamsManagementWidget->setVisible(true);
    mp_ShowHideGlobalParamsLabel->setText(tr("hide"));
  }
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

  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  mp_WaresManWidget->updateUpDownButtons();

  emit changed();
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


  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  mp_WaresManWidget->updateUpDownButtons();

  emit changed();

}


// =====================================================================
// =====================================================================


void ModelWidget::removeModelItem(const QString& ID)
{
  int Position = m_Model.getFirstItemIndex(ID.toStdString());

  if (Position < 0)
    return;

  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(Position)->widget());
  W->deleteLater();

  m_Model.removeItem(Position);

  mp_WaresManWidget->updateUpDownButtons();

  emit changed();
}


// =====================================================================
// =====================================================================


void ModelWidget::refresh()
{
  updateCoupledModel();

  updateGlobalParams();

  mp_ModelScene->refresh();
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
      mp_WaresManWidget->ui->WaresListAreaContents->layout()->addWidget(SimWidget);
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

      mp_WaresManWidget->ui->WaresListAreaContents->layout()->addWidget(GenWidget);
      if (it == itb) GenWidget->setUpButtonEnabled(false);
      if (it == itl) GenWidget->setDownButtonEnabled(false);

      connect(GenWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
      connect(GenWidget,SIGNAL(upClicked(const QString&)),this,SLOT(moveModelItemUp(const QString&)));
      connect(GenWidget,SIGNAL(downClicked(const QString&)),this,SLOT(moveModelItemDown(const QString&)));
      connect(GenWidget,SIGNAL(removeClicked(const QString&)),this,SLOT(removeModelItem(const QString&)));

    }
  }
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->addStretch();
}


// =====================================================================
// =====================================================================


void ModelWidget::dispatchChangesFromChildren()
{
  mp_ModelScene->refresh();
  emit changed();
}

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
  @file ObserverWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/ObserverSignatureRegistry.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>

#include "builderconfig.hpp"

#include "ui_WareWidget.h"
#include "ObserverWidget.hpp"
#include "ParameterWidget.hpp"
#include "AddParamDialog.hpp"



ObserverWidget::ObserverWidget(QWidget* Parent,
                               openfluid::fluidx::ObserverDescriptor* Desc,
                               const openfluid::ware::WareID_t& ID,
                               int Index):
  ClickableWareWidget(Parent,ID,Desc->isEnabled(),BUILDER_OBSERVER_BGCOLOR,Index), mp_Desc(Desc)
{
  refresh();

  connect(ui->AddParamButton,SIGNAL(clicked()),this,SLOT(addParameterToList()));
}


// =====================================================================
// =====================================================================


ObserverWidget::~ObserverWidget()
{

}


// =====================================================================
// =====================================================================


void ObserverWidget::refresh()
{
  const openfluid::machine::ObserverSignatureInstance* Signature =
    openfluid::machine::ObserverSignatureRegistry::instance()->signature(m_ID);

  if (Signature != NULL)
  {
    setAvailableWare(true);
    ui->NameLabel->setText(QString::fromStdString(Signature->Signature->Name));
    ui->InfosSideWidget->update(Signature);

    updateParametersList();

    // TODO begin to be refactored, see also SimulatorWidget =========

    mp_ParamsWidget = NULL;

    if (Signature->WithParametersWidget)
    {
      mp_ParamsWidget = static_cast<openfluid::ui::ware::ParameterizationWidget*>(
          openfluid::machine::ObserverPluginsManager::instance()->getParameterizationWidget(Signature));
      mp_ParamsWidget->setParent(this);
      mp_ParamsWidget->linkParams(&(mp_Desc->parameters()));

      connect(mp_ParamsWidget,SIGNAL(changed()),this,SLOT(notifyChangedFromParameterizationWidget()));

      int Position = ui->ParameterizationStackWidget->addWidget(mp_ParamsWidget);

      mp_ParamsWidget->update();

      ui->ParameterizationStackWidget->setCurrentIndex(Position);
    }

    updateParameterizationSwitch();

    // end to be refactored =========

  }
  else
  {
    setAvailableWare(false);
  }

  updateWidgetBackground();
}


// =====================================================================
// =====================================================================


void ObserverWidget::setEnabledWare(bool Enabled)
{
  mp_Desc->setEnabled(Enabled);
  WareWidget::setEnabledWare(Enabled);
  emit changed();
}


// =====================================================================
// =====================================================================


void ObserverWidget::updateParametersList()
{
  clearParameterWidgets();

  openfluid::ware::WareParams_t DescParams = mp_Desc->getParameters();

  for (openfluid::ware::WareParams_t::iterator it = DescParams.begin();it != DescParams.end(); ++it)
  {
    ParameterWidget* ParamWidget =
        new ParameterWidget(this,
                            QString::fromStdString((*it).first),QString::fromStdString((*it).second),
                            QString::fromStdString(""),true);

    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),
            this, SLOT(updateParameterValue(const QString&,const QString&)));
    connect(ParamWidget,SIGNAL(removeClicked(const QString&)),
            this, SLOT(removeParameterFromList(const QString&)));


    ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()))->addWidget(ParamWidget);
  }
}


// =====================================================================
// =====================================================================


void ObserverWidget::addParameterToList()
{
  QStringList ExistPList;

  openfluid::ware::WareParams_t Params = mp_Desc->getParameters();

  for (openfluid::ware::WareParams_t::iterator it = Params.begin();it != Params.end(); ++it)
    ExistPList.append(QString::fromStdString((*it).first));

  // set existing parameters list as completion list
  // (for easy access to series of similar parameters)

  AddParamDialog AddPDlg(ExistPList,ExistPList,this);

  if (AddPDlg.exec() == QDialog::Accepted)
  {
    if (addParameterWidget(AddPDlg.getParamName(),AddPDlg.getParamValue()))
    {
      mp_Desc->setParameter(AddPDlg.getParamName().toStdString(),AddPDlg.getParamValue().toStdString());
      emit changed();
    }
  }
}


// =====================================================================
// =====================================================================



void ObserverWidget::updateParameterValue(const QString& Name, const QString& Value)
{
  mp_Desc->setParameter(Name.toStdString(),Value.toStdString());
  emit changed();
}


// =====================================================================
// =====================================================================


void ObserverWidget::removeParameterFromList(const QString& Name)
{
  if (removeParameterWidget(Name))
  {
    mp_Desc->eraseParameter(Name.toStdString());
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void ObserverWidget::prepareWareUpdate()
{
  if (mp_ParamsWidget)
  {
    ui->ParameterizationStackWidget->removeWidget(mp_ParamsWidget);
    delete mp_ParamsWidget;
    mp_ParamsWidget = NULL;
  }
}


// =====================================================================
// =====================================================================


void ObserverWidget::updateWare()
{
  refresh();
}

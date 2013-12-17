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
  \file GeneratorWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include "builderconfig.hpp"

#include "ui_WareWidget.h"
#include "GeneratorWidget.hpp"
#include "ParameterWidget.hpp"


GeneratorWidget::GeneratorWidget(QWidget* Parent,
                                 openfluid::fluidx::ModelItemDescriptor* Desc,
                                 const openfluid::ware::WareID_t& ID,
                                 openfluid::machine::ModelItemSignatureInstance* SignInstance):
  WareWidget(Parent,ID,Desc->isEnabled(),BUILDER_GENERATOR_BGCOLOR),mp_Desc(Desc), mp_SignInstance(SignInstance)
{
  ui->AddParamButton->setVisible(false);
  refresh();
}


// =====================================================================
// =====================================================================


GeneratorWidget::~GeneratorWidget()
{
}


// =====================================================================
// =====================================================================


void GeneratorWidget::updateParams()
{
  std::vector<openfluid::ware::SignatureHandledDataItem>* SignParams =
      &(mp_SignInstance->Signature->HandledData.SimulatorParams);

  openfluid::ware::WareParams_t DescParams = mp_Desc->getParameters();

  for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator it = SignParams->begin();
       it != SignParams->end(); ++it)
  {
    std::string ParamName = (*it).DataName;
    QString ParamValue = "";

    if (DescParams.find(ParamName) != DescParams.end())
      ParamValue = QString::fromStdString(DescParams[ParamName]);

    ParameterWidget* ParamWidget = new ParameterWidget(this,
                                                       QString::fromStdString(ParamName),ParamValue,
                                                       QString::fromStdString((*it).DataUnit));

    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),this, SLOT(updateParamValue(const QString&,const QString&)));

    ((QBoxLayout*)(ui->ParamsAreaContents->layout()))->addWidget(ParamWidget);
  }

  ((QBoxLayout*)(ui->ParamsAreaContents->layout()))->addStretch();

}


// =====================================================================
// =====================================================================


void GeneratorWidget::refresh()
{

  if (mp_SignInstance != NULL)
  {
    setAvailableWare(true);
    ui->NameLabel->setText(QString::fromStdString(mp_SignInstance->Signature->Name));

    // TODO add produced variable in signature
    mp_SignatureWidget->update(mp_SignInstance);

    updateParams();
  }
  else
  {
    setAvailableWare(false);
    ui->NameLabel->setText("");
  }

}


// =====================================================================
// =====================================================================


void GeneratorWidget::setEnabledWare(bool Enabled)
{
  mp_Desc->setEnabled(Enabled);
  WareWidget::setEnabledWare(Enabled);
  emit changed();
}


// =====================================================================
// =====================================================================


void GeneratorWidget::updateParamValue(const QString& Name, const QString& Value)
{
  mp_Desc->setParameter(Name.toStdString(),Value.toStdString());
  emit changed();
}

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
  @file GeneratorWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */



#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include "builderconfig.hpp"

#include "ui_WareWidget.h"
#include "GeneratorWidget.hpp"
#include "ParameterWidget.hpp"


GeneratorWidget::GeneratorWidget(QWidget* Parent,
                                 openfluid::fluidx::ModelItemDescriptor* Desc,
                                 const openfluid::ware::WareID_t& ID,
                                 int Index,
                                 const openfluid::machine::ModelItemSignatureInstance* SignInstance):
  WareWidget(Parent,ID,Desc->isEnabled(),BUILDER_GENERATOR_BGCOLOR, Index),mp_Desc(Desc), mp_SignInstance(SignInstance)
{
  ui->AddParamButton->setVisible(false);
  ui->ParameterizationSwitchLabel->setVisible(false);

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
  openfluid::ware::WareParams_t DescParams = mp_Desc->getParameters();

  // required parameters
  std::vector<openfluid::ware::SignatureDataItem>* RequiredParams =
      &(mp_SignInstance->Signature->HandledData.RequiredParams);

  for (auto it = RequiredParams->begin(); it != RequiredParams->end(); ++it)
  {
    std::string ParamName = (*it).DataName;
    QString ParamValue = "";

    if (DescParams.find(ParamName) != DescParams.end())
      ParamValue = QString::fromStdString(DescParams[ParamName]);

    ParameterWidget* ParamWidget = new ParameterWidget(this,
                                                       QString::fromStdString(ParamName),ParamValue,
                                                       QString::fromStdString((*it).DataUnit),
                                                       true,false);

    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),this,
            SLOT(updateParamValue(const QString&,const QString&)));

    ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()))->addWidget(ParamWidget);
  }


  // used parameters
  std::vector<openfluid::ware::SignatureDataItem>* UsedParams =
      &(mp_SignInstance->Signature->HandledData.UsedParams);

  for (auto it = UsedParams->begin(); it != UsedParams->end(); ++it)
  {
    std::string ParamName = (*it).DataName;
    QString ParamValue = "";

    if (DescParams.find(ParamName) != DescParams.end())
      ParamValue = QString::fromStdString(DescParams[ParamName]);

    ParameterWidget* ParamWidget = new ParameterWidget(this,
                                                       QString::fromStdString(ParamName),ParamValue,
                                                       QString::fromStdString((*it).DataUnit),
                                                       false,false);

    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),this,
            SLOT(updateParamValue(const QString&,const QString&)));

    ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()))->addWidget(ParamWidget);
  }
}


// =====================================================================
// =====================================================================


void GeneratorWidget::refresh()
{

  if (mp_SignInstance != nullptr)
  {
    setAvailableWare(true);
    ui->NameLabel->setText(QString::fromStdString(mp_SignInstance->Signature->Name));

    // TODO add produced variable in signature
    ui->InfosSideWidget->update(mp_SignInstance);

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

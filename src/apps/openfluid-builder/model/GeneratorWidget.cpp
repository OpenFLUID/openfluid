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
  \file GeneratorWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <openfluid/machine/SimulatorSignatureRegistry.hpp>


#include "ui_WareWidget.h"
#include "GeneratorWidget.hpp"
#include "ParameterWidget.hpp"


GeneratorWidget::GeneratorWidget(QWidget* Parent,
                                 openfluid::fluidx::ModelItemDescriptor* Desc,
                                 const openfluid::ware::WareID_t& ID,
                                 openfluid::machine::ModelItemSignatureInstance* SignInstance):
  WareWidget(Parent,ID,Desc->isEnabled(),"#FDFEDE"),mp_Desc(Desc), mp_SignInstance(SignInstance)
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
    mp_SignatureWidget->update(mp_SignInstance);

    updateParams();
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

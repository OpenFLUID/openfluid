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
  \file SimulatorWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include "builderconfig.hpp"

#include "ui_WareWidget.h"
#include "SimulatorWidget.hpp"
#include "ParameterWidget.hpp"
#include "AddParamDialog.hpp"



SimulatorWidget::SimulatorWidget(QWidget* Parent, openfluid::fluidx::ModelItemDescriptor* Desc,
                                 const openfluid::ware::WareID_t& ID):
  WareWidget(Parent,ID,Desc->isEnabled(),BUILDER_SIMULATOR_BGCOLOR), mp_Desc(Desc)
{
  refresh();

  connect(ui->AddParamButton,SIGNAL(clicked()),this,SLOT(addParam()));
}


// =====================================================================
// =====================================================================


SimulatorWidget::~SimulatorWidget()
{

}


// =====================================================================
// =====================================================================


void SimulatorWidget::updateParams(openfluid::machine::ModelItemSignatureInstance* Signature)
{
  std::vector<openfluid::ware::SignatureHandledDataItem>* SignParams =
      &(Signature->Signature->HandledData.SimulatorParams);

  openfluid::ware::WareParams_t DescParams = mp_Desc->getParameters();

  QStringList ParamsInSign;

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

    ParamsInSign << QString::fromStdString(ParamName);

    ((QBoxLayout*)(ui->ParamsAreaContents->layout()))->addWidget(ParamWidget);
  }


  for (openfluid::ware::WareParams_t::iterator it = DescParams.begin();it != DescParams.end(); ++it)
  {
    if (!ParamsInSign.contains(QString::fromStdString((*it).first)))
    {
      ParameterWidget* ParamWidget = new ParameterWidget(this,
                                                         QString::fromStdString((*it).first),QString::fromStdString((*it).second),
                                                         QString::fromStdString(""),true);

      connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),this, SLOT(updateParamValue(const QString&,const QString&)));
      connect(ParamWidget,SIGNAL(removeClicked(const QString&)),this, SLOT(removeParam(const QString&)));


      ((QBoxLayout*)(ui->ParamsAreaContents->layout()))->addWidget(ParamWidget);
    }
  }

  ((QBoxLayout*)(ui->ParamsAreaContents->layout()))->addStretch();
}


// =====================================================================
// =====================================================================


void SimulatorWidget::refresh()
{
  openfluid::machine::ModelItemSignatureInstance* Signature = NULL;

  Signature = openfluid::machine::SimulatorSignatureRegistry::getInstance()->getSignatureItemInstance(m_ID);

  if (Signature != NULL)
  {
    setAvailableWare(true);
    ui->NameLabel->setText(QString::fromStdString(Signature->Signature->Name));
    mp_SignatureWidget->update(Signature);

    updateParams(Signature);
  }
  else
  {
    setAvailableWare(false);
  }
}



// =====================================================================
// =====================================================================


void SimulatorWidget::setEnabledWare(bool Enabled)
{
  mp_Desc->setEnabled(Enabled);
  WareWidget::setEnabledWare(Enabled);
  emit changed();
}


// =====================================================================
// =====================================================================


void SimulatorWidget::addParam()
{
  QStringList ExistPList;

  openfluid::ware::WareParams_t Params = mp_Desc->getParameters();

  for (openfluid::ware::WareParams_t::iterator it = Params.begin();it != Params.end(); ++it)
    ExistPList.append(QString::fromStdString((*it).first));


  AddParamDialog AddPDlg(ExistPList,QStringList(),this);

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


void SimulatorWidget::updateParamValue(const QString& Name, const QString& Value)
{
  mp_Desc->setParameter(Name.toStdString(),Value.toStdString());
  emit changed();
}


// =====================================================================
// =====================================================================


void SimulatorWidget::removeParam(const QString& Name)
{
  if (removeParameterWidget(Name))
  {
    mp_Desc->eraseParameter(Name.toStdString());
    emit changed();
  }
}

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
  @file SimulatorWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>

#include "ui_WareWidget.h"
#include "SimulatorWidget.hpp"
#include "ParameterWidget.hpp"
#include "AddParamDialog.hpp"

#include "builderconfig.hpp"
#include "ProjectCentral.hpp"
#include "ExtensionsRegistry.hpp"
#include "WaresTranslationsRegistry.hpp"


SimulatorWidget::SimulatorWidget(QWidget* Parent, openfluid::fluidx::ModelItemDescriptor* Desc,
                                 const openfluid::ware::WareID_t& ID,
                                 int Index):
  ClickableWareWidget(Parent,ID,QString::fromStdString(ID),Desc->isEnabled(),BUILDER_SIMULATOR_BGCOLOR,Index),
  mp_Desc(Desc),m_IsTranslated(false)
{
  refresh();

  ui->GenerateSrcButton->setText("");
  ui->GenerateSrcButton->setIcon(QIcon(":/icons/ghost2sim.png"));
  ui->GenerateSrcButton->setIconSize(QSize(32,16));

  connect(ui->AddParamButton,SIGNAL(clicked()),this,SLOT(addParameterToList()));
  connect(ui->GenerateSrcButton,SIGNAL(clicked()),this,SLOT(notifySrcGenerateAsked()));
}


// =====================================================================
// =====================================================================


SimulatorWidget::~SimulatorWidget()
{

}


// =====================================================================
// =====================================================================


void SimulatorWidget::updateParametersListWithSignature(const openfluid::machine::ModelItemSignatureInstance* Signature)
{
  clearParameterWidgets();

  std::vector<openfluid::ware::SignatureDataItem>* UsedParams = &(Signature->Signature->HandledData.UsedParams);
  std::vector<openfluid::ware::SignatureDataItem>* RequiredParams = &(Signature->Signature->HandledData.RequiredParams);

  openfluid::ware::WareParams_t DescParams = mp_Desc->getParameters();

  QStringList ParamsInSign;

  // Required params

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

    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),
            this, SLOT(updateParameterValue(const QString&,const QString&)));

    ParamsInSign << QString::fromStdString(ParamName);

    ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()))->addWidget(ParamWidget);
  }


  // Used params

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

    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),
            this, SLOT(updateParameterValue(const QString&,const QString&)));

    ParamsInSign << QString::fromStdString(ParamName);

    ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()))->addWidget(ParamWidget);
  }


  // Other params not in signature

  for (openfluid::ware::WareParams_t::iterator it = DescParams.begin();it != DescParams.end(); ++it)
  {
    if (!ParamsInSign.contains(QString::fromStdString((*it).first)))
    {
      ParameterWidget* ParamWidget =
          new ParameterWidget(this,
                              QString::fromStdString((*it).first),QString::fromStdString((*it).second),
                              QString::fromStdString(""),
                              false,true);

      connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),
              this, SLOT(updateParameterValue(const QString&,const QString&)));
      connect(ParamWidget,SIGNAL(removeClicked(const QString&)),
              this, SLOT(removeParameterFromList(const QString&)));


      ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()))->addWidget(ParamWidget);
    }
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::updateParametersList()
{
  const openfluid::machine::ModelItemSignatureInstance* Signature =
    openfluid::machine::SimulatorSignatureRegistry::instance()->signature(m_ID);

  updateParametersListWithSignature(Signature);
}


// =====================================================================
// =====================================================================


void SimulatorWidget::refresh()
{
  const openfluid::machine::ModelItemSignatureInstance* Signature =
    openfluid::machine::SimulatorSignatureRegistry::instance()->signature(m_ID);

  m_Ghost = false;

  if (Signature != nullptr)
  {
    m_Ghost = Signature->Ghost;

    if (!m_IsTranslated)
    {
      WaresTranslationsRegistry::instance()->tryLoadWareTranslation(QString::fromStdString(Signature->FileFullPath));
      m_IsTranslated = true;
    }

    setAvailableWare(true);

    ui->NameLabel->setText(QString::fromStdString(Signature->Signature->Name));
    ui->InfosSideWidget->update(Signature);

    updateParametersListWithSignature(Signature);

    // TODO begin to be refactored, see also ObserverWidget =========

    mp_ParamsWidget = nullptr;

    if (ExtensionsRegistry::instance()->isParameterizationExtensionRegistered(Signature->LinkUID))
    {
      mp_ParamsWidget = static_cast<openfluid::builderext::PluggableParameterizationExtension*>(
          ExtensionsRegistry::instance()->instanciateParameterizationExtension(Signature->LinkUID));
      mp_ParamsWidget->setParent(this);
      mp_ParamsWidget->linkParams(&(mp_Desc->parameters()));
      mp_ParamsWidget->setFluidXDescriptor(&(ProjectCentral::instance()->advancedDescriptors()));

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

  ui->GenerateSrcButton->setVisible(m_Ghost);
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


void SimulatorWidget::addParameterToList()
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


void SimulatorWidget::updateParameterValue(const QString& Name, const QString& Value)
{
  mp_Desc->setParameter(Name.toStdString(),Value.toStdString());
  emit changed();
}


// =====================================================================
// =====================================================================


void SimulatorWidget::removeParameterFromList(const QString& Name)
{
  if (removeParameterWidget(Name))
  {
    mp_Desc->eraseParameter(Name.toStdString());
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::prepareWareUpdate()
{
  if (mp_ParamsWidget)
  {
    ui->ParameterizationStackWidget->removeWidget(mp_ParamsWidget);
    delete mp_ParamsWidget;
    mp_ParamsWidget = nullptr;
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::updateWare()
{
  refresh();
}


// =====================================================================
// =====================================================================


void SimulatorWidget::notifySrcGenerateAsked()
{
  if (m_Ghost)
    emit srcGenerateAsked(QString::fromStdString(m_ID));
}

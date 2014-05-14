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
  \file SimulationConfigurationWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include "ui_RunConfigurationWidget.h"
#include "RunConfigurationWidget.hpp"

#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>


RunConfigurationWidget::RunConfigurationWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WorkspaceWidget(Parent, AFXDesc), ui(new Ui::RunConfigurationWidget)
{
  ui->setupUi(this);

  ui->ConstraintComboBox->addItem(tr("None"));
  ui->ConstraintComboBox->addItem(tr("Checked - Simulators scheduling requests are checked to be equal to DeltaT"));
  ui->ConstraintComboBox->addItem(tr("Forced - Simulators scheduling requests are forced to DeltaT"));

  refresh();


  connect(ui->BeginDateTimeEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(updateBeginDateFXDesc(const QDateTime&)));
  connect(ui->EndDateTimeEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(updateEndDateFXDesc(const QDateTime&)));

  connect(ui->DeltaTSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateDeltaTFXDesc(int)));
  connect(ui->ConstraintComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateConstraintFXDesc(int)));

  connect(ui->MemoryGroupBox,SIGNAL(toggled(bool)),this,SLOT(updateMemoryFXDesc(bool)));
  connect(ui->MemoryStepsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateMemoryStepsFXDesc(int)));
}


// =====================================================================
// =====================================================================


RunConfigurationWidget::~RunConfigurationWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::refresh()
{
  ui->BeginDateTimeEdit->setDateTime(QDateTime::fromString(QString(m_AdvFluidxDesc.getRunDescriptor().getBeginDate().getAsString("%Y-%m-%d %H:%M:%S").c_str()),"yyyy-MM-dd HH:mm:ss"));
  ui->EndDateTimeEdit->setDateTime(QDateTime::fromString(QString(m_AdvFluidxDesc.getRunDescriptor().getEndDate().getAsString("%Y-%m-%d %H:%M:%S").c_str()),"yyyy-MM-dd HH:mm:ss"));

  ui->DeltaTSpinBox->setValue(m_AdvFluidxDesc.getRunDescriptor().getDeltaT());
  ui->ConstraintComboBox->setCurrentIndex(m_AdvFluidxDesc.getRunDescriptor().getSchedulingConstraint());

  ui->MemoryGroupBox->setChecked(m_AdvFluidxDesc.getRunDescriptor().isUserValuesBufferSize());
  if (ui->MemoryGroupBox->isEnabled()) ui->MemoryStepsSpinBox->setValue(m_AdvFluidxDesc.getRunDescriptor().getValuesBufferSize());
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateConstraintFXDesc(int Index)
{
  m_AdvFluidxDesc.getRunDescriptor().setSchedulingConstraint(openfluid::base::SimulationStatus::SchedulingConstraint(Index));
  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_RUNCONFIG);
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateDeltaTFXDesc(int Value)
{
  m_AdvFluidxDesc.getRunDescriptor().setDeltaT(Value);
  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_RUNCONFIG);
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateBeginDateFXDesc(const QDateTime& QDT)
{
  openfluid::core::DateTime DT;
  DT.setFromString(QDT.toString("yyyy-MM-dd HH:mm:ss").toStdString(),"%Y-%m-%d %H:%M:%S");

  m_AdvFluidxDesc.getRunDescriptor().setBeginDate(DT);
  if (ui->EndDateTimeEdit->dateTime() <= QDT)
  {
    ui->EndDateTimeEdit->setDateTime(QDT.addSecs(1));
  }

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_RUNCONFIG);
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateEndDateFXDesc(const QDateTime& QDT)
{
  openfluid::core::DateTime DT;
  DT.setFromString(QDT.toString("yyyy-MM-dd HH:mm:ss").toStdString(),"%Y-%m-%d %H:%M:%S");

  m_AdvFluidxDesc.getRunDescriptor().setEndDate(DT);
  if (ui->BeginDateTimeEdit->dateTime() >= QDT)
  {
    ui->BeginDateTimeEdit->setDateTime(QDT.addSecs(-1));
  }

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_RUNCONFIG);
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateMemoryFXDesc(bool On)
{
  if (On)
    m_AdvFluidxDesc.getRunDescriptor().setValuesBufferSize(ui->MemoryStepsSpinBox->value());
  else
    m_AdvFluidxDesc.getRunDescriptor().unsetUserValuesBufferSize();
  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_RUNCONFIG);
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateMemoryStepsFXDesc(int Value)
{
  m_AdvFluidxDesc.getRunDescriptor().setValuesBufferSize(Value);
  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_RUNCONFIG);
}



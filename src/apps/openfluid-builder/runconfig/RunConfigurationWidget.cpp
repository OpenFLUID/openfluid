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
  @file RunConfigurationWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/core/ValuesBufferProperties.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_RunConfigurationWidget.h"
#include "RunConfigurationWidget.hpp"


RunConfigurationWidget::RunConfigurationWidget(QWidget* Parent, openfluid::fluidx::FluidXDescriptor& FXDesc):
  WorkspaceWidget(Parent, FXDesc), ui(new Ui::RunConfigurationWidget)
{
  ui->setupUi(this);

  ui->ConstraintComboBox->addItem(tr("None"));
  ui->ConstraintComboBox->addItem(tr("Checked - Simulators scheduling requests are checked to be equal to DeltaT"));
  ui->ConstraintComboBox->addItem(tr("Forced - Simulators scheduling requests are forced to DeltaT"));

  ui->ResetThreadsButton->setText("");
  ui->ResetThreadsButton->setIcon(openfluid::ui::common::getIcon("refresh","/ui/common"));
  ui->ResetThreadsButton->setIconSize(QSize(16,16));

  ui->defaultRadioButton->setChecked(true);

  ui->ThreadsSpinBox->setMaximum(std::numeric_limits<int>::max());
  ui->MemoryStepsSpinBox->setMaximum(std::numeric_limits<int>::max());
  ui->MemoryStepsSpinBox->setValue(openfluid::core::ValuesBufferProperties::getBufferSize());

  refresh();


  connect(ui->PeriodWidget,SIGNAL(beginChanged(const QDateTime&)),
          this,SLOT(updateBeginDateFXDesc(const QDateTime&)));
  connect(ui->PeriodWidget,SIGNAL(endChanged(const QDateTime&)),
          this,SLOT(updateEndDateFXDesc(const QDateTime&)));

  connect(ui->DeltaTSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateDeltaTFXDesc(int)));
  connect(ui->ConstraintComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateConstraintFXDesc(int)));

  connect(ui->CustomRadioButton,SIGNAL(toggled(bool)),this,SLOT(updateMemoryFXDesc(bool)));
  connect(ui->MemoryStepsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateMemoryStepsFXDesc(int)));

  connect(ui->ClearOutputCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateClearOutput(bool)));
  connect(ui->ProfilingCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateProfiling(bool)));
  connect(ui->ThreadsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateMaxThreads(int)));
  connect(ui->ResetThreadsButton,SIGNAL(clicked()),this,SLOT(resetThreadsToIdeal()));
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
  openfluid::core::DateTime BeginDate = m_FluidxDesc.runConfiguration().getBeginDate();
  openfluid::core::DateTime EndDate = m_FluidxDesc.runConfiguration().getEndDate();
  int DeltaT = m_FluidxDesc.runConfiguration().getDeltaT();
  
  ui->PeriodWidget->setBeginDateTime(
      QDateTime::fromString(
          QString(BeginDate.getAsString("%Y-%m-%d %H:%M:%S").c_str()),
          "yyyy-MM-dd HH:mm:ss"));

  ui->PeriodWidget->setEndDateTime(
      QDateTime::fromString(
          QString(EndDate.getAsString("%Y-%m-%d %H:%M:%S").c_str()),
          "yyyy-MM-dd HH:mm:ss"));

  ui->DeltaTSpinBox->setValue(DeltaT);
  ui->ConstraintComboBox->setCurrentIndex(m_FluidxDesc.runConfiguration().getSchedulingConstraint());

  
  std::uint64_t Duration = openfluid::core::Duration_t(EndDate.diffInSeconds(BeginDate));
  std::size_t DefaultValuesBufferSize = openfluid::machine::Engine::computeValuesBuffersDefaultSize(Duration, DeltaT);
  ui->defaultRadioButton->setText(tr("Default: last %1 values").arg(DefaultValuesBufferSize));

  ui->CustomRadioButton->setChecked(m_FluidxDesc.runConfiguration().isUserValuesBufferSize());
  ui->MemoryStepsSpinBox->setValue(m_FluidxDesc.runConfiguration().getValuesBufferSize());
  ui->MemoryStepsSpinBox->setEnabled(ui->CustomRadioButton->isChecked());


  ui->ClearOutputCheckBox->setChecked(openfluid::base::RunContextManager::instance()
                                      ->getProjectConfigValue("builder.runconfig.options","clearoutdir").toBool());

  ui->ProfilingCheckBox->setChecked(openfluid::base::RunContextManager::instance()
                                      ->getProjectConfigValue("builder.runconfig.options","profiling").toBool());

  int MaxThreads = openfluid::base::RunContextManager::instance()
                    ->getProjectConfigValue("builder.runconfig.options","maxthreads").toInt();

  if (MaxThreads < 1)
  {
    resetThreadsToIdeal();
  }

  ui->ThreadsSpinBox->setValue(MaxThreads);

}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateConstraintFXDesc(int Index)
{
  m_FluidxDesc.runConfiguration().setSchedulingConstraint(
      openfluid::base::SimulationStatus::SchedulingConstraint(Index));
  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_RUNCONFIG);
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::runConfigChange()
{
  refresh();

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_RUNCONFIG);
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateDeltaTFXDesc(int Value)
{
  m_FluidxDesc.runConfiguration().setDeltaT(Value);
  runConfigChange();
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateBeginDateFXDesc(const QDateTime& QDT)
{
  openfluid::core::DateTime DT;
  DT.setFromString(QDT.toString("yyyy-MM-dd HH:mm:ss").toStdString(),"%Y-%m-%d %H:%M:%S");

  m_FluidxDesc.runConfiguration().setBeginDate(DT);
  runConfigChange();
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateEndDateFXDesc(const QDateTime& QDT)
{
  openfluid::core::DateTime DT;
  DT.setFromString(QDT.toString("yyyy-MM-dd HH:mm:ss").toStdString(),"%Y-%m-%d %H:%M:%S");

  m_FluidxDesc.runConfiguration().setEndDate(DT);
  runConfigChange();
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateMemoryFXDesc(bool On)
{
  if (On)
  {
    m_FluidxDesc.runConfiguration().setValuesBufferSize(ui->MemoryStepsSpinBox->value());
  }
  else
  {
    m_FluidxDesc.runConfiguration().unsetUserValuesBufferSize();
  }
  runConfigChange();
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateMemoryStepsFXDesc(int Value)
{
  m_FluidxDesc.runConfiguration().setValuesBufferSize(Value);
  runConfigChange();
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateClearOutput(bool On)
{
  openfluid::base::RunContextManager::instance()->setProjectConfigValue("builder.runconfig.options","clearoutdir",
                                                                         QVariant(On));
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateProfiling(bool On)
{
  openfluid::base::RunContextManager::instance()->setProjectConfigValue("builder.runconfig.options","profiling",
                                                                         QVariant(On));
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::updateMaxThreads(int Num)
{
  openfluid::base::RunContextManager::instance()->setProjectConfigValue("builder.runconfig.options","maxthreads",
                                                                         QVariant(Num));
}


// =====================================================================
// =====================================================================


void RunConfigurationWidget::resetThreadsToIdeal()
{
  int MaxThreads = openfluid::base::Environment::getIdealThreadCount();
  updateMaxThreads(MaxThreads);

  ui->ThreadsSpinBox->setValue(MaxThreads);
}

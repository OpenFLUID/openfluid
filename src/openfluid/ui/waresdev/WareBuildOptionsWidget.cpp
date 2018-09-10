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
  @file WareBuildOptionsWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ui/waresdev/WareBuildOptionsWidget.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_WareBuildOptionsWidget.h"


namespace openfluid { namespace ui { namespace waresdev {


WareBuildOptionsWidget::WareBuildOptionsWidget(QWidget* Parent):
  QWidget(Parent), ui(new Ui::WareBuildOptionsWidget)
{
  ui->setupUi(this);

  ui->ConfigureModeComboBox->addItems(QStringList() << "Release" << "Debug");
  ui->ConfigureModeComboBox->setCurrentIndex(0);

  ui->InstallCheckBox->setChecked(true);

  ui->ResetJobsButton->setText("");
  ui->ResetJobsButton->setIcon(openfluid::ui::common::getIcon("refresh","/ui/common"));
  ui->ResetJobsButton->setIconSize(QSize(16,16));

  ui->JobsCheckBox->setChecked(true);
  resetJobsToIdeal();

  connect(ui->InstallCheckBox,SIGNAL(toggled(bool)),this,SLOT(handleInstallChanged()));
  connect(ui->ConfigureModeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(handleConfigureModeChanged()));
  connect(ui->JobsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(handleBuildJobsChanged()));
  connect(ui->JobsCheckBox,SIGNAL(toggled(bool)),this,SLOT(handleBuildJobsChanged()));
  connect(ui->ResetJobsButton,SIGNAL(clicked()),this,SLOT(resetJobsToIdeal()));
}


// =====================================================================
// =====================================================================


WareBuildOptionsWidget::~WareBuildOptionsWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WareBuildOptionsWidget::handleInstallChanged()
{
  if (ui->InstallCheckBox->isChecked())
    emit buildModeChanged(openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_WITHINSTALL);
  else
    emit buildModeChanged(openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_NOINSTALL);
}


// =====================================================================
// =====================================================================


void WareBuildOptionsWidget::handleConfigureModeChanged()
{
  if (ui->ConfigureModeComboBox->currentIndex() == 0)
    emit configureModeChanged(openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_RELEASE);
  else
    emit configureModeChanged(openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_DEBUG);
}


// =====================================================================
// =====================================================================


void WareBuildOptionsWidget::handleBuildJobsChanged()
{
  if (ui->JobsCheckBox->isChecked())
  {
    ui->JobsSpinBox->setEnabled(true);
    emit buildJobsChanged(ui->JobsSpinBox->value());
  }
  else
  {
    ui->JobsSpinBox->setEnabled(false);
    emit buildJobsChanged(0);
  }
}


// =====================================================================
// =====================================================================


void WareBuildOptionsWidget::setBuildMode(openfluid::waresdev::WareSrcContainer::BuildMode Mode)
{
  ui->InstallCheckBox->setChecked(Mode == openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_WITHINSTALL);
}


// =====================================================================
// =====================================================================


void WareBuildOptionsWidget::setConfigureMode(openfluid::waresdev::WareSrcContainer::ConfigMode Mode)
{
  if (Mode == openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_RELEASE)
    ui->ConfigureModeComboBox->setCurrentIndex(0);
  else if (Mode == openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_DEBUG)
    ui->ConfigureModeComboBox->setCurrentIndex(1);
}


// =====================================================================
// =====================================================================


openfluid::waresdev::WareSrcContainer::BuildMode WareBuildOptionsWidget::getBuildMode() const
{
  if (ui->InstallCheckBox->isChecked())
    return openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_WITHINSTALL;

  return openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_NOINSTALL;
}


// =====================================================================
// =====================================================================


openfluid::waresdev::WareSrcContainer::ConfigMode WareBuildOptionsWidget::getConfigureMode() const
{
  if (ui->ConfigureModeComboBox->currentIndex() == 0)
    return openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_RELEASE;

  return openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_DEBUG;
}


// =====================================================================
// =====================================================================


unsigned int WareBuildOptionsWidget::getBuildJobs() const
{
  if (ui->JobsCheckBox->isChecked())
  {
    return ui->JobsSpinBox->value();
  }
  else
  {
    return 0;
  }
}


// =====================================================================
// =====================================================================


void WareBuildOptionsWidget::resetJobsToIdeal()
{
  ui->JobsSpinBox->setValue(openfluid::base::Environment::getIdealJobsCount());
}


} } } // namespaces


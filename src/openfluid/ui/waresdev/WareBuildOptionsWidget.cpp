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
  @author Armel THÖNI <armel.thoni@inrae.fr>
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

  ui->JobsSpinBox->setMaximum(std::numeric_limits<int>::max());

  ui->ResetJobsButton->setText("");
  ui->ResetJobsButton->setIcon(openfluid::ui::common::getIcon("refresh","/ui/common"));
  ui->ResetJobsButton->setIconSize(QSize(16,16));

  ui->JobsCheckBox->setChecked(true);

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


void WareBuildOptionsWidget::setBuildOptions(openfluid::waresdev::WareBuildOptions& BuildOptions)
{

  setConfigureMode(BuildOptions.getConfigMode());

  setBuildMode(BuildOptions.getBuildMode());

  setJobsNumber(BuildOptions.JobsNumber);
  ui->JobsCheckBox->setChecked(BuildOptions.IsMultipleJobs);
}


// =====================================================================
// =====================================================================


openfluid::waresdev::WareBuildOptions WareBuildOptionsWidget::getOptions()
{

  openfluid::waresdev::WareBuildOptions BuildOptions;
  BuildOptions.IsWithInstall = ui->InstallCheckBox->isChecked();
  BuildOptions.IsReleaseMode = ui->ConfigureModeComboBox->currentIndex() == 0;
  BuildOptions.IsMultipleJobs = ui->JobsCheckBox->isChecked();
  BuildOptions.JobsNumber = ui->JobsSpinBox->value();
  return BuildOptions;
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
  {
    ui->ConfigureModeComboBox->setCurrentIndex(0);
  }
  else if (Mode == openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_DEBUG)
  {
    ui->ConfigureModeComboBox->setCurrentIndex(1);
  }
}


// =====================================================================
// =====================================================================


void WareBuildOptionsWidget::setJobsNumber(int JobsNumber)
{
  ui->JobsSpinBox->setValue(JobsNumber);
}


// =====================================================================
// =====================================================================


void WareBuildOptionsWidget::resetJobsToIdeal()
{
  ui->JobsSpinBox->setValue(openfluid::base::Environment::getIdealJobsCount());
}


} } } // namespaces


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
  @file WareBuildStatusWidget.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "WareBuildStatusWidget.hpp"
#include "ui_WareBuildStatusWidget.h"


WareBuildStatusWidget::WareBuildStatusWidget(QWidget* Parent):
  QWidget(Parent), ui(new Ui::WareBuildStatusWidget)
{
  ui->setupUi(this);

  ui->buildOptionsButton->setText("");
  ui->buildOptionsButton->setIcon(openfluid::ui::common::getIcon("settings","/ui/common"));

  connect(ui->buildOptionsButton, SIGNAL(clicked()), this, SIGNAL(settingsButtonClicked()));
  connect(ui->jobStatus, SIGNAL(scrolled(bool)), this, SIGNAL(jobsScrolled(bool)));
}


// =====================================================================
// =====================================================================


WareBuildStatusWidget::~WareBuildStatusWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


openfluid::ui::common::ActionLabel* WareBuildStatusWidget::installLabel()
{
  return ui->buildStatus;
}


// =====================================================================
// =====================================================================


openfluid::ui::common::ActionLabel* WareBuildStatusWidget::configureModeLabel()
{
  return ui->configStatus;
}


// =====================================================================
// =====================================================================


openfluid::ui::common::ActionLabel* WareBuildStatusWidget::jobsLabel()
{
  return ui->jobStatus;
}

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
  @file WaresImportFilterWidget.cpp

  @author Armel THONI <armel.thoni@inrae.fr>
*/


#include <openfluid/ui/waresdev/WaresImportFilterWidget.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_WaresImportFilterWidget.h"


namespace openfluid { namespace ui { namespace waresdev {


WaresImportFilterWidget::WaresImportFilterWidget(QWidget* Parent):
  QWidget(Parent), ui(new Ui::WaresImportFilterWidget)
{
  ui->setupUi(this);

  connect(ui->nameFilterLineEdit,SIGNAL(textChanged(QString)),this,SLOT(handleFilterChange()));
  connect(ui->showAlreadyImportedCheckbox,SIGNAL(toggled(bool)),this,SLOT(handleFilterChange()));
  connect(ui->showUnauthorizedCheckbox,SIGNAL(toggled(bool)),this,SLOT(handleFilterChange()));
}


// =====================================================================
// =====================================================================


WaresImportFilterWidget::~WaresImportFilterWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WaresImportFilterWidget::handleFilterChange()
{
  emit filterChanged();
}


// =====================================================================
// =====================================================================


bool WaresImportFilterWidget::areAlreadyImportedWaresEnabled()
{
  return ui->showAlreadyImportedCheckbox->isChecked();
}


// =====================================================================
// =====================================================================


bool WaresImportFilterWidget::areUnauthorizedWaresEnabled()
{
  return ui->showUnauthorizedCheckbox->isChecked();
}


// =====================================================================
// =====================================================================


QString WaresImportFilterWidget::getFilteringText()
{
  return ui->nameFilterLineEdit->text();
}


// =====================================================================
// =====================================================================


void WaresImportFilterWidget::showAuthorizationFilterCheckbox(bool Showing)
{
  ui->showUnauthorizedCheckbox->setVisible(Showing);
}

} } } // namespaces

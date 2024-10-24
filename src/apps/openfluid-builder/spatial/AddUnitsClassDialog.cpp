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
  @file AddUnitsClassDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <QPushButton>

#include <openfluid/ui/config.hpp>
#include <openfluid/tools/IDHelpers.hpp>

#include "ui_AddUnitDialog.h"
#include "AddUnitsClassDialog.hpp"


AddUnitsClassDialog::AddUnitsClassDialog(const QStringList& ExistingClasses, QWidget* Parent):
  AddUnitDialog(Parent), m_ExistingClasses(ExistingClasses)
{
  updateDefaultMessage(tr("Add spatial units class"));

  ui->ClassLabel->setVisible(false);
  ui->AttributesWidget->setVisible(false);

  ui->ClassEdit->setPlaceholderText(getPlaceholderRequired());
  ui->UnitIDEdit->setText("1");

  ui->PcsOrderSpinBox->setMaximum(std::numeric_limits<int>::max());

  connect(ui->ClassEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->UnitIDEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));

  adjustSize();

  ui->ClassEdit->setFocus();

  checkGlobal();
}


// =====================================================================
// =====================================================================


AddUnitsClassDialog::~AddUnitsClassDialog()
{

}


// =====================================================================
// =====================================================================


void AddUnitsClassDialog::checkGlobal()
{
  if (ui->ClassEdit->text().isEmpty())
  {
    setMessage(tr("Units class name cannot be empty"));
  }
  else if (m_ExistingClasses.contains(ui->ClassEdit->text()))
  {
    setMessage(tr("Units class name already exists"));
  }
  else if (!openfluid::tools::isValidUnitsClassName(ui->ClassEdit->text().toStdString()))
  {
    setMessage(tr("'#', ':' and ';' are forbidden for units class name"));
  }
  else if (ui->UnitIDEdit->text().isEmpty())
  {
    setMessage(tr("Unit ID cannot be empty"));
  }
  else
  {
    setMessage();
  }
}


// =====================================================================
// =====================================================================


QString AddUnitsClassDialog::getClassName() const
{
  return ui->ClassEdit->text();
}


// =====================================================================
// =====================================================================


int AddUnitsClassDialog::getUnitID() const
{
  return ui->UnitIDEdit->text().toInt();
}


// =====================================================================
// =====================================================================


int AddUnitsClassDialog::getUnitPcsOrd() const
{
  return ui->PcsOrderSpinBox->value();
}

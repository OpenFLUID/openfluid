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
 @file WareshubIssueDialog.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <openfluid/ui/waresdev/WareshubIssueDialog.hpp>

#include "ui_WareshubIssueDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareshubIssueDialog::WareshubIssueDialog(const QStringList& IDs, QWidget* Parent, const Issue& I) :
    QDialog(Parent), ui(new Ui::WareshubIssueDialog), m_IDs(IDs)
{
  ui->setupUi(this);

  ui->TypeComboBox->addItems(m_Types);
  ui->StateComboBox->addItems(m_Statuses);
  ui->UrgencyComboBox->addItems(m_Urgencies);


  ui->IDLineEdit->setText(I.m_ID);
  ui->TitleLineEdit->setText(I.m_Title);
  ui->CreatorLineEdit->setText(I.m_Creator);
  ui->DateDateEdit->setDate(I.m_Date);

  int Index = ui->TypeComboBox->findText(I.m_Type, Qt::MatchFixedString);
  if (Index > -1)
    ui->TypeComboBox->setCurrentIndex(Index);

  Index = ui->StateComboBox->findText(I.m_State, Qt::MatchFixedString);
  if (Index > -1)
    ui->StateComboBox->setCurrentIndex(Index);

  ui->DescriptionTextEdit->setPlainText(I.m_Description);

  Index = ui->UrgencyComboBox->findText(I.m_Urgency, Qt::MatchFixedString);
  if (Index > -1)
    ui->UrgencyComboBox->setCurrentIndex(Index);
}


// =====================================================================
// =====================================================================


WareshubIssueDialog::~WareshubIssueDialog()
{

}


// =====================================================================
// =====================================================================


WareshubIssueDialog::Issue WareshubIssueDialog::getIssue()
{
  Issue I;

  I.m_ID = ui->IDLineEdit->text();
  I.m_Title = ui->TitleLineEdit->text();
  I.m_Creator = ui->CreatorLineEdit->text();
  I.m_Date = ui->DateDateEdit->date();
  I.m_Type = ui->TypeComboBox->currentText();
  I.m_State = ui->StateComboBox->currentText();
  I.m_Description = ui->DescriptionTextEdit->toPlainText();
  I.m_Urgency = ui->UrgencyComboBox->currentText();

  return I;
}


// =====================================================================
// =====================================================================


} } } // namespaces

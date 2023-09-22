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


 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/

#ifndef QT_VERSION_MAJOR
#pragma message "Qt version not found in source"
#else
#pragma message "Qt version found in source"
#endif


#include <QPushButton>
#include <openfluid/ui/common/WareshubIssueDialog.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_WareshubIssueDialog.h"


namespace openfluid { namespace ui { namespace common {


// =====================================================================
// =====================================================================


WareshubIssueDialog::WareshubIssueDialog(const QList<unsigned int>& IDs, QWidget* Parent, const Issue& I) :
    openfluid::ui::common::MessageDialog(Parent),
    ui(new Ui::WareshubIssueDialog), m_IDs(IDs)
{
  ui->setupUi(this);

  //ui->TypeComboBox->addItems(m_Types); //TODO enable this when type data rebuilt
  ui->StateComboBox->addItems(m_Statuses);

  if (I.m_ID == 0)
  {
    setupMessageUi(tr("New issue"));

    QList<unsigned int> SortedIntIDs;
    for (const auto& ID : IDs)
    {
      SortedIntIDs << ID;
    }
    if (SortedIntIDs.isEmpty())
    {
      ui->IDLineEdit->setText("1");
    }
    else
    {
      unsigned int Max = *std::max_element(SortedIntIDs.begin(), SortedIntIDs.end());
      ui->IDLineEdit->setText(QString("%1").arg(Max + 1));
    }
  }
  else
  {
    setupMessageUi(tr("Edit issue"));
    m_IDs.removeAll(I.m_ID);
    ui->IDLineEdit->setText(QString::number(I.m_ID));
  }
#if (QT_VERSION_MAJOR < 6)
  ui->IDLineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9a-zA-Z_-]*$"), this));
#else
  ui->IDLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9a-zA-Z_-]*$"), this));
#endif

  ui->TitleLineEdit->setText(I.m_Title);
  ui->TagsLineEdit->setText(I.m_Tags);
  ui->CreatorLineEdit->setText(I.m_Creator);
  ui->DateDateTimeEdit->setDateTime(I.m_DateCreation);

  int Index = ui->StateComboBox->findText(I.m_State, Qt::MatchFixedString);
  if (Index > -1)
  {
    ui->StateComboBox->setCurrentIndex(Index);
  }

  ui->DescriptionTextEdit->setPlainText(I.m_Description);

  connect(ui->IDLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(onChanged()));
  connect(ui->TitleLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(onChanged()));
  onChanged();
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

  I.m_ID = ui->IDLineEdit->text().toUInt();
  I.m_Title = ui->TitleLineEdit->text();
  I.m_Creator = ui->CreatorLineEdit->text();
  I.m_DateCreation = ui->DateDateTimeEdit->dateTime();
  I.m_State = ui->StateComboBox->currentText();
  I.m_Tags = ui->TagsLineEdit->text();
  I.m_Description = ui->DescriptionTextEdit->toPlainText();

  return I;
}


// =====================================================================
// =====================================================================


void WareshubIssueDialog::onChanged()
{
  unsigned int ID = ui->IDLineEdit->text().toUInt();
  if (m_IDs.contains(ID))
  {
    setMessage(tr("This ID already exists"));
  }  
  if (ui->TitleLineEdit->text().isEmpty())
  {
    setMessage(tr("Title cannot be empty"));
  }
  else
  {
    setMessage();
  }
}


} } } // namespaces

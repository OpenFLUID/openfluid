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
 @file FindReplaceDialog.cpp


 @author Aline LIBRES <aline.libres@gmail.com>
*/


#include <openfluid/ui/waresdev/FindReplaceDialog.hpp>

#include "ui_FindReplaceDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


FindReplaceDialog::FindReplaceDialog(QWidget* Parent) :
    QDialog(Parent), ui(new Ui::FindReplaceDialog)
{
  ui->setupUi(this);

  connect(ui->FindButton, SIGNAL(clicked()), this, SLOT(onFindClicked()));
  connect(ui->ReplaceButton, SIGNAL(clicked()), this, SLOT(onReplaceClicked()));
  connect(ui->ReplaceFindButton, SIGNAL(clicked()), this, SLOT(onReplaceFindClicked()));
  connect(ui->ReplaceAllButton, SIGNAL(clicked()), this, SLOT(onReplaceAllClicked()));

  connect(ui->FindEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));

  ui->FindButton->setEnabled(false);
  ui->ReplaceAllButton->setEnabled(false);
  ui->ReplaceFindButton->setEnabled(false);
  ui->ReplaceButton->setEnabled(false);
}


// =====================================================================
// =====================================================================


FindReplaceDialog::~FindReplaceDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::show(const QString& SelectedText)
{
  bool EmptySelection = SelectedText.isEmpty();

  ui->ReplaceFindButton->setEnabled(!EmptySelection);
  ui->ReplaceButton->setEnabled(!EmptySelection);

  if (!EmptySelection)
  {
    ui->FindEdit->setText(SelectedText);
  }

  QWidget::show();
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::setMessage(const QString& Message, bool TextFound)
{
  ui->MsgLabel->setText(Message);

  ui->ReplaceFindButton->setEnabled(TextFound);
  ui->ReplaceButton->setEnabled(TextFound);
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::onTextChanged(const QString& Text)
{
  bool IsEmpty = Text.isEmpty();

  ui->FindButton->setEnabled(!IsEmpty);
  ui->ReplaceAllButton->setEnabled(!IsEmpty);

  if (IsEmpty)
  {
    ui->ReplaceFindButton->setEnabled(false);
    ui->ReplaceButton->setEnabled(false);
  }
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::onFindClicked()
{
  emit findReplaceRequested(FindReplaceAction::FindOnly, ui->FindEdit->text(), "", getFindOptions());
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::onReplaceClicked()
{
  emit findReplaceRequested(FindReplaceAction::ReplaceOnly, ui->FindEdit->text(), ui->ReplaceEdit->text(),
                            getFindOptions());
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::onReplaceFindClicked()
{
  emit findReplaceRequested(FindReplaceAction::ReplaceFind, ui->FindEdit->text(), ui->ReplaceEdit->text(),
                            getFindOptions());
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::onReplaceAllClicked()
{
  emit findReplaceRequested(FindReplaceAction::ReplaceAll, ui->FindEdit->text(), ui->ReplaceEdit->text(),
                            getFindOptions());
}


// =====================================================================
// =====================================================================


QTextDocument::FindFlags FindReplaceDialog::getFindOptions()
{
  QTextDocument::FindFlags Options;

  if (ui->BackwardRadioButton->isChecked())
  {
    Options |= QTextDocument::FindBackward;
  }  

  if (ui->CaseSensitiveCheckBox->isChecked())
  {
    Options |= QTextDocument::FindCaseSensitively;
  }  

  if (ui->WholeWordCheckBox->isChecked())
  {
    Options |= QTextDocument::FindWholeWords;
  }  

  return Options;
}


// =====================================================================
// =====================================================================


} } } // namespaces

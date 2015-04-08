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
 @brief Implements ...

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

  connect(ui->Find_pushButton, SIGNAL(clicked()), this, SLOT(onFindClicked()));
  connect(ui->Replace_pushButton, SIGNAL(clicked()), this, SLOT(onReplaceClicked()));
  connect(ui->ReplaceFind_pushButton, SIGNAL(clicked()), this, SLOT(onReplaceFindClicked()));
  connect(ui->ReplaceAll_pushButton, SIGNAL(clicked()), this, SLOT(onReplaceAllClicked()));
}


// =====================================================================
// =====================================================================


FindReplaceDialog::~FindReplaceDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::show()
{
  ui->ReplaceFind_pushButton->setEnabled(false);
  ui->Replace_pushButton->setEnabled(false);

  QWidget::show();
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::setMessage(const QString& Message, bool EnableReplace)
{
  ui->Msg_label->setText(Message);

  ui->ReplaceFind_pushButton->setEnabled(EnableReplace);
  ui->Replace_pushButton->setEnabled(EnableReplace);
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::onFindClicked()
{
  emit findReplaceRequested(FindOnly, ui->Find_lineEdit->text(), "", getFindOptions());
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::onReplaceClicked()
{
  emit findReplaceRequested(ReplaceOnly, ui->Find_lineEdit->text(), ui->Replace_lineEdit->text(), getFindOptions());
}

// =====================================================================
// =====================================================================


void FindReplaceDialog::onReplaceFindClicked()
{
  emit findReplaceRequested(ReplaceFind, ui->Find_lineEdit->text(), ui->Replace_lineEdit->text(), getFindOptions());
}


// =====================================================================
// =====================================================================


void FindReplaceDialog::onReplaceAllClicked()
{
  emit findReplaceRequested(ReplaceAll, ui->Find_lineEdit->text(), ui->Replace_lineEdit->text(), getFindOptions());
}


// =====================================================================
// =====================================================================


QTextDocument::FindFlags FindReplaceDialog::getFindOptions()
{
  QTextDocument::FindFlags Options;

  if (ui->Backward_radioButton->isChecked())
    Options |= QTextDocument::FindBackward;

  if (ui->CaseSensitive_checkBox->isChecked())
    Options |= QTextDocument::FindCaseSensitively;

  if (ui->WholeWord_checkBox->isChecked())
    Options |= QTextDocument::FindWholeWords;

  return Options;
}


// =====================================================================
// =====================================================================


} } }  // namespaces

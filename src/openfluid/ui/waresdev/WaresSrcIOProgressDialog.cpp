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
 @file WaresSrcIOProgressDialog.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include "ui_WaresSrcIOProgressDialog.h"

#include "WaresSrcIOProgressDialog.hpp"


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WaresSrcIOProgressDialog::WaresSrcIOProgressDialog(const QString& Description, QWidget* Parent) :
    QDialog(Parent), ui(new Ui::WaresSrcIOProgressDialog)
{
  ui->setupUi(this);

  ui->DescriptionLabel->setText(Description);

  ui->buttonBox->setEnabled(false);
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::writeInfo(const QString& Message)
{
  ui->textEdit->append(Message);

  ui->textEdit->ensureCursorVisible();
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::writeError(const QString& Message)
{
  QTextCursor Cursor = ui->textEdit->textCursor();

  QTextCharFormat OriginalFormat = Cursor.charFormat();

  QTextCharFormat RedFormat;
  RedFormat.setForeground(QColor("red"));
  Cursor.setCharFormat(RedFormat);

  Cursor.insertText(Message);

  Cursor.setCharFormat(OriginalFormat);


  ui->textEdit->ensureCursorVisible();
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::finish()
{
  ui->buttonBox->setEnabled(true);
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::progress(int Value)
{
  ui->progressBar->setValue(Value);
}


// =====================================================================
// =====================================================================


} } } //namespaces

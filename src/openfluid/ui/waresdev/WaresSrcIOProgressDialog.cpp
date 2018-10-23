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


#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>

#include "ui_WaresSrcIOProgressDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WaresSrcIOProgressDialog::WaresSrcIOProgressDialog(const QString& Description, bool IsMaxUndefined, QWidget* Parent) :
    QDialog(Parent), ui(new Ui::WaresSrcIOProgressDialog)
{
  ui->setupUi(this);

  ui->DescriptionLabel->setText(Description);

  ui->buttonBox->setEnabled(false);

  if (IsMaxUndefined)
    ui->progressBar->setMaximum(0);
}


// =====================================================================
// =====================================================================


WaresSrcIOProgressDialog::~WaresSrcIOProgressDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::writeInfo(const QString& Message)
{
  write(Message);
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::writeError(const QString& Message)
{
  write(Message, QColor("red"));
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::writeSuccess(const QString& Message)
{
  write(Message, QColor("green"));
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::write(const QString& Message, const QColor& Color)
{
  ui->textEdit->append("\n");

  if (!Color.isValid())
    ui->textEdit->append(Message);
  else
  {
    QTextCursor Cursor = ui->textEdit->textCursor();

    QTextCharFormat OriginalFormat = Cursor.charFormat();

    QTextCharFormat ColoredFormat;
    ColoredFormat.setForeground(Color);
    Cursor.setCharFormat(ColoredFormat);

    Cursor.insertText(Message);

    Cursor.setCharFormat(OriginalFormat);
  }

  ui->textEdit->ensureCursorVisible();
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::finish(bool Ok, const QString& Message)
{
  if (Ok)
  {
    writeSuccess(Message);
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(accept()));
  }
  else
  {
    writeError(Message);
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(reject()));
  }

  ui->buttonBox->setEnabled(true);
  ui->progressBar->setMaximum(100);
  progress(100);
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::finishAndQuit(bool Ok, const QString& Message)
{
  if (Ok)
  {
    writeSuccess(Message);
    accept();
  }
  else
    finish(Ok, Message);
}


// =====================================================================
// =====================================================================


void WaresSrcIOProgressDialog::progress(int Value)
{
  ui->progressBar->setValue(Value);
}


} } } //namespaces

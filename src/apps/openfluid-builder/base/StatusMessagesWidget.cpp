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
  @file StatusMessagesWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include "ui_StatusMessagesWidget.h"
#include "StatusMessagesWidget.hpp"


StatusMessagesWidget::StatusMessagesWidget(const QString& Category, QWidget* Parent):
  QWidget(Parent), ui(new Ui::StatusMessagesWidget)
{
  ui->setupUi(this);

  ui->CategoryLabel->setStyleSheet("color: white; text-decoration: underline;");
  ui->MessagesLabel->setStyleSheet("color: white;");

  ui->CategoryLabel->setText(Category);

  QFont TmpFont = ui->MessagesLabel->font();
  TmpFont.setPointSize(TmpFont.pointSize()-1);
  ui->MessagesLabel->setFont(TmpFont);

  setVisible(false);
}


// =====================================================================
// =====================================================================


StatusMessagesWidget::~StatusMessagesWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void StatusMessagesWidget::clear()
{
  setVisible(false);
  ui->MessagesLabel->setText("");
}


// =====================================================================
// =====================================================================


void StatusMessagesWidget::addMessage(const QString& Msg)
{
  setVisible(true);

  ui->MessagesLabel->setText(ui->MessagesLabel->text()+"- "+Msg+"\n");
}


// =====================================================================
// =====================================================================


void StatusMessagesWidget::setStatusLevel(ProjectStatusLevel Level)
{
  if (Level == PRJ_WARNING)
    ui->StatusIconLabel->setPixmap(QPixmap(":/images/check-warn.png"));
  else if (Level == PRJ_ERROR)
    ui->StatusIconLabel->setPixmap(QPixmap(":/images/check-error.png"));
  else if (Level == PRJ_DISABLED)
      ui->StatusIconLabel->setPixmap(QPixmap(":/images/check-disabled.png"));
}

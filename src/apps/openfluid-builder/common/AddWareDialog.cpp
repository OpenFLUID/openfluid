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
  @file AddWareDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QListWidgetItem>

#include "ui_AddWareDialog.h"
#include "AddWareDialog.hpp"
#include "SignatureWidget.hpp"


AddWareDialog::AddWareDialog(QWidget* Parent):
  openfluid::ui::common::MessageDialog(Parent),ui(new Ui::AddWareDialog)
{
  ui->setupUi(this);

  setupMessageUi("");

  ui->WaresListWidget->setIconSize(QSize(24,12));

  ui->WareSignatureWidget->mute();
}


// =====================================================================
// =====================================================================


AddWareDialog::~AddWareDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void AddWareDialog::labelizeItems(bool NameFirst)
{
  int ItemsCount = ui->WaresListWidget->count();

  for (int i = 0; i< ItemsCount; i++)
  {
    QListWidgetItem* Item = ui->WaresListWidget->item(i);
    QStringList WareInfos = Item->data(Qt::UserRole).toStringList();

    if (NameFirst && !WareInfos.at(1).isEmpty())
    {
      Item->setText(WareInfos.at(1));
    }
    else
    {
      Item->setText(WareInfos.at(0));
    }
  }

}


// =====================================================================
// =====================================================================


QString AddWareDialog::getSelectedID() const
{
  QListWidgetItem* Item = ui->WaresListWidget->currentItem();

  if (Item != 0)
    return Item->data(Qt::UserRole).toStringList().at(0);
  else
    return "";
}

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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include "ui_AddWareDialog.h"
#include "AddWareDialog.hpp"
#include "SignatureWidget.hpp"

#include <QListWidgetItem>


AddWareDialog::AddWareDialog(QWidget* Parent):
  openfluid::ui::common::OpenFLUIDDialog(Parent),ui(new Ui::AddWareDialog)
{
  ui->setupUi(this);

  mp_SignWidget = new SignatureWidget(this);
  mp_SignWidget->setVisible(false);

  ui->MainHorizontalLayout->addWidget(mp_SignWidget,0);

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


AddWareDialog::~AddWareDialog()
{
  mp_SignWidget->deleteLater();
  delete ui;
}


// =====================================================================
// =====================================================================


QString AddWareDialog::getSelectedID() const
{
  QListWidgetItem* Item = ui->WaresListWidget->currentItem();

  if (Item != 0)
    return Item->text();
  else
    return "";
}

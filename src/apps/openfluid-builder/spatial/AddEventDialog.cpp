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
  @file AddEventDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QPushButton>

#include <openfluid/ui/config.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_EditEventDialog.h"
#include "AddEventDialog.hpp"


AddEventDialog::AddEventDialog(const QString& ClassName, const QStringList& IDsList,
                               const openfluid::core::DateTime& CurrentDateTime,
                               QWidget* Parent):
  openfluid::ui::common::MessageDialog(Parent),ui(new Ui::EditEventDialog)
{
  ui->setupUi(this);

  setupMessageUi(tr("Add of a new event"));

  ui->AddInfoButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddInfoButton->setIconSize(QSize(20,20));

  ui->RemoveInfoButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveInfoButton->setIconSize(QSize(20,20));

  ui->UnitsClassLabel->setText(ClassName);

  ui->UnitIDComboBox->addItems(IDsList);

  ui->DateTimeEdit->setDateTime(
      QDateTime::fromString(QString(CurrentDateTime.getAsString("%Y-%m-%d %H:%M:%S").c_str()),"yyyy-MM-dd HH:mm:ss"));

  connect(ui->AddInfoButton,SIGNAL(clicked()),this,SLOT(addInfoLine()));
  connect(ui->RemoveInfoButton,SIGNAL(clicked()),this,SLOT(removeInfoLine()));

  connect(ui->InfosTableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(checkGlobal()));

  checkGlobal();
}


// =====================================================================
// =====================================================================


AddEventDialog::~AddEventDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void AddEventDialog::checkGlobal()
{

  QStringList KeysList;
  bool KeysAreUnique = true;

  int i=0;
  while (i<ui->InfosTableWidget->rowCount() && KeysAreUnique)
  {
    QString CurrentKey = ui->InfosTableWidget->item(i,0)->text();

    if (KeysList.contains(CurrentKey))
      KeysAreUnique = false;
    else
      KeysList.append(CurrentKey);

    i++;
  }

  if (!KeysAreUnique)
    setMessage(tr("Informations keys are not unique"));
  else
    setMessage();
}


// =====================================================================
// =====================================================================


void AddEventDialog::addInfoLine()
{
  int RowCount = ui->InfosTableWidget->rowCount();

  ui->InfosTableWidget->setRowCount(RowCount+1);

  ui->InfosTableWidget->setItem(RowCount,0,new QTableWidgetItem("newkey"));
  ui->InfosTableWidget->setItem(RowCount,1,new QTableWidgetItem("newvalue"));

  checkGlobal();
}


// =====================================================================
// =====================================================================


void AddEventDialog::removeInfoLine()
{
  int Row = ui->InfosTableWidget->currentRow();

  if (Row >= 0)
  {
    ui->InfosTableWidget->removeRow(Row);
    checkGlobal();
  }
}


// =====================================================================
// =====================================================================


openfluid::core::Event::EventInfosMap_t AddEventDialog::getInfos() const
{
  openfluid::core::Event::EventInfosMap_t Infos;

  int i=0;
  while (i<ui->InfosTableWidget->rowCount())
  {
    Infos[ui->InfosTableWidget->item(i,0)->text().toStdString()] =
        ui->InfosTableWidget->item(i,1)->text().toStdString();
   i++;
  }

  return Infos;
}


// =====================================================================
// =====================================================================


openfluid::core::DateTime AddEventDialog::getDateTime() const
{
  openfluid::core::DateTime DT;

  DT.setFromString(ui->DateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString(),"%Y-%m-%d %H:%M:%S");

  return DT;
}


// =====================================================================
// =====================================================================


openfluid::core::UnitID_t AddEventDialog::getUnitID() const
{
  return ui->UnitIDComboBox->currentText().toInt();
}


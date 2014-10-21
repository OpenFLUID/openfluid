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
  \file AddEventDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_EditEventDialog.h"
#include "AddEventDialog.hpp"
#include <openfluid/ui/config.hpp>

#include <QPushButton>


AddEventDialog::AddEventDialog(const QString& ClassName, const QStringList& IDsList,
                               const openfluid::core::DateTime& CurrentDateTime,
                               QWidget* Parent):
  openfluid::ui::common::OpenFLUIDDialog(Parent),ui(new Ui::EditEventDialog),m_DefaultMsg(tr("Add of a new event"))
{
  ui->setupUi(this);

  ui->AddInfoButton->setIcon(QIcon(":/ui/common/icons/add.png"));
  ui->AddInfoButton->setIconSize(QSize(20,20));

  ui->RemoveInfoButton->setIcon(QIcon(":/ui/common/icons/remove.png"));
  ui->RemoveInfoButton->setIconSize(QSize(20,20));

  ui->UnitsClassLabel->setText(ClassName);

  ui->UnitIDComboBox->addItems(IDsList);

  ui->DateTimeEdit->setDateTime(QDateTime::fromString(QString(CurrentDateTime.getAsString("%Y-%m-%d %H:%M:%S").c_str()),"yyyy-MM-dd HH:mm:ss"));

  connect(ui->AddInfoButton,SIGNAL(clicked()),this,SLOT(addInfoLine()));
  connect(ui->RemoveInfoButton,SIGNAL(clicked()),this,SLOT(removeInfoLine()));

  connect(ui->InfosTableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(checkGlobal()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

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


void AddEventDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(m_DefaultMsg);
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(openfluid::ui::config::DIALOGBANNER_WARNBGCOLOR));
    ui->MessageLabel->setText(Msg);
  }
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


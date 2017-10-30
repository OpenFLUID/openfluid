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
  @file AddUnitToClassDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */



#include "ui_AddUnitDialog.h"
#include "AddUnitToClassDialog.hpp"

#include <openfluid/ui/config.hpp>

#include <QPushButton>


AddUnitToClassDialog::AddUnitToClassDialog(const QString& ClassName,
                                           const QStringList& AttrsNames,
                                           const QStringList& UnitsIDs,
                                           QWidget* Parent):
  AddUnitDialog(Parent), m_AttrsNames(AttrsNames), m_UnitsIDs(UnitsIDs)
{
  ui->ClassEdit->setVisible(false);

  ui->ClassLabel->setText(ClassName);

  ui->AttrsTableWidget->setColumnCount(AttrsNames.size());
  ui->AttrsTableWidget->setHorizontalHeaderLabels(AttrsNames);
  ui->AttrsTableWidget->setRowCount(1);

  for (int i=0; i<AttrsNames.size();i++)
    ui->AttrsTableWidget->setItem(0,i,new QTableWidgetItem("0"));

  connect(ui->ClassEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->UnitIDEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->AttrsTableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(checkAttribute(int,int)));

  adjustSize();

  checkGlobal();
}


// =====================================================================
// =====================================================================


AddUnitToClassDialog::~AddUnitToClassDialog()
{

}


// =====================================================================
// =====================================================================


void AddUnitToClassDialog::checkGlobal()
{
  if (ui->UnitIDEdit->text().isEmpty())
    setMessage(tr("Unit ID cannot be empty"));
  else if (m_UnitsIDs.contains(ui->UnitIDEdit->text()))
    setMessage(tr("Unit ID already exists"));
  else
  {
    bool OK = true;
    for (int i=0; i<m_AttrsNames.size();i++)
      OK = OK && !(ui->AttrsTableWidget->item(0,i)->text().isEmpty());

    if (!OK)
      setMessage(tr("Wrong attribute(s) value(s)"));
    else
      setMessage();
  }

}


// =====================================================================
// =====================================================================


void AddUnitToClassDialog::checkAttribute(int /*Row*/,int /*Column*/)
{
  checkGlobal();
}


// =====================================================================
// =====================================================================


void AddUnitToClassDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Add spatial units class"));
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


QString AddUnitToClassDialog::getClassName() const
{
  return ui->ClassEdit->text();
}


// =====================================================================
// =====================================================================


int AddUnitToClassDialog::getUnitID() const
{
  return ui->UnitIDEdit->text().toInt();
}


// =====================================================================
// =====================================================================


int AddUnitToClassDialog::getUnitPcsOrd() const
{
  return ui->PcsOrderSpinBox->value();
}


// =====================================================================
// =====================================================================


QMap<QString,QString> AddUnitToClassDialog::getAttributes() const
{
  QMap<QString,QString> Attrs;

  for (int i=0; i<m_AttrsNames.size();i++)
    Attrs[ui->AttrsTableWidget->horizontalHeaderItem(i)->text()] = ui->AttrsTableWidget->item(0,i)->text();

  return Attrs;
}

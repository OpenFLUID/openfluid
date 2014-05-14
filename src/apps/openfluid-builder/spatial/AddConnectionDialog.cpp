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
  \file AddConnectionDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_AddConnectionDialog.h"
#include "AddConnectionDialog.hpp"
#include "AppTools.hpp"
#include "builderconfig.hpp"

#include <QPushButton>


AddConnectionDialog::AddConnectionDialog(const QString& SrcClass, const QString& SrcID,
                                         const openfluid::fluidx::AdvancedDomainDescriptor* Domain,
                                         QWidget* Parent):
  OpenFLUIDDialog(Parent),ui(new Ui::AddConnectionDialog), mp_Domain(Domain)
{
  ui->setupUi(this);

  ui->SrcClassLabel->setText(SrcClass);
  ui->SrcIDLabel->setText(SrcID);

  ui->ConnectionComboBox->addItem(tr("to"),BUILDER_CONNCODE_TO);
  ui->ConnectionComboBox->addItem(tr("from"),BUILDER_CONNCODE_FROM);
  ui->ConnectionComboBox->addItem(tr("as parent of"),BUILDER_CONNCODE_PARENTOF);
  ui->ConnectionComboBox->addItem(tr("as child of"),BUILDER_CONNCODE_CHILDOF);

  QStringList Classes = StringSetToQStringList(mp_Domain->getClassNames());

  foreach(QString ClassName, Classes)
    m_ClassID[ClassName] = QStringList();

  ui->DestClassComboBox->addItems(Classes);

  updateDestIDs();

  connect(ui->DestClassComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateDestIDs()));
  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  checkGlobal();

  adjustSize();
}


// =====================================================================
// =====================================================================


AddConnectionDialog::~AddConnectionDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void AddConnectionDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Add connection between spatial units"));
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_WARNBGCOLOR));
    ui->MessageLabel->setText(Msg);
  }
}


// =====================================================================
// =====================================================================


void AddConnectionDialog::checkGlobal()
{
  if (ui->DestClassComboBox->currentText().isEmpty())
    setMessage(tr("Destination unit class cannot be empty"));
  else if (ui->DestIDComboBox->currentText().isEmpty())
    setMessage(tr("Destination unit ID cannot be empty"));
  else
    setMessage();
}


// =====================================================================
// =====================================================================


void AddConnectionDialog::updateDestIDs()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  QString ClassName = ui->DestClassComboBox->currentText();
  if (m_ClassID[ClassName].isEmpty())
    m_ClassID[ClassName] = IntSetToQStringList(mp_Domain->getIDsOfClass(ClassName.toStdString()));

  ui->DestIDComboBox->clear();
  ui->DestIDComboBox->addItems(m_ClassID[ClassName]);
  ui->DestIDComboBox->setCurrentIndex(0);

  checkGlobal();

  QApplication::restoreOverrideCursor();
}


// =====================================================================
// =====================================================================


ConnectionCode AddConnectionDialog::getConnectionCode() const
{
  return static_cast<ConnectionCode>(ui->ConnectionComboBox->itemData(ui->ConnectionComboBox->currentIndex()).toInt());
}


// =====================================================================
// =====================================================================


QString AddConnectionDialog::getDestinationClass() const
{
  return ui->DestClassComboBox->currentText();
}


// =====================================================================
// =====================================================================


QString AddConnectionDialog::getDestinationID() const
{
  return ui->DestIDComboBox->currentText();
}

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
  @file EditAttributeNameDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPushButton>

#include <openfluid/ui/config.hpp>

#include "ui_EditAttributeNameDialog.h"
#include "EditAttributeNameDialog.hpp"


EditAttributeNameDialog::EditAttributeNameDialog(EditMode Mode,
                                                 const QStringList& AttrsList,
                                                 QWidget* Parent):
  openfluid::ui::common::MessageDialog(Parent),ui(new Ui::EditAttributeNameDialog),
  m_Mode(Mode), m_AttrsNames(AttrsList)
{
  ui->setupUi(this);

  if (m_Mode == EDIT_ADD)
  {
    ui->NewNameLabel->setText(tr("Attribute name:"));
    ui->DefaultValueLabel->setText(tr("Default value:"));
    setupMessageUi(tr("Add new attribute"));
    connect(ui->NewNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
    connect(ui->DefaultValueEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  }
  else if (m_Mode == EDIT_RENAME)
  {
    ui->OriginalNameLabel->setText(tr("Attribute to rename:"));
    ui->OriginalNameComboBox->addItems(AttrsList);
    ui->NewNameLabel->setText(tr("New attribute name:"));
    setupMessageUi(tr("Rename attribute"));
    connect(ui->OriginalNameComboBox,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(checkGlobal()));
    connect(ui->NewNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  }
  else if (m_Mode == EDIT_REMOVE)
  {
    ui->OriginalNameLabel->setText(tr("Attribute to remove:"));
    ui->OriginalNameComboBox->addItems(AttrsList);
    setupMessageUi(tr("Remove attribute"));
  }

  // "required" placeholder
  QString PlaceholderStr = getPlaceholderRequired();
  ui->NewNameEdit->setPlaceholderText(PlaceholderStr);
  ui->DefaultValueEdit->setPlaceholderText(PlaceholderStr);

  ui->OriginalNameWidget->setVisible(m_Mode == EDIT_REMOVE ||
                                     m_Mode == EDIT_RENAME);

  ui->NewNameWidget->setVisible(m_Mode == EDIT_RENAME ||
                                m_Mode == EDIT_ADD);

  ui->DefaultValueWidget->setVisible(m_Mode == EDIT_ADD);

  if (m_Mode == EDIT_ADD || m_Mode == EDIT_RENAME)
  {
    ui->NewNameEdit->setFocus();
  }

  adjustSize();

  checkGlobal();
}


// =====================================================================
// =====================================================================


EditAttributeNameDialog::~EditAttributeNameDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditAttributeNameDialog::checkGlobal()
{
  if (m_Mode == EDIT_ADD)
  {
    if (m_AttrsNames.contains(ui->NewNameEdit->text()))
    {
      setMessage(tr("New attribute name already exists"));
    }
    else if (ui->NewNameEdit->text().isEmpty())
    {
      setMessage(tr("New attribute name is empty"));
    }
    else if (ui->DefaultValueEdit->text().isEmpty())
    {
      setMessage(tr("Default attribute value is empty"));
    }
    else
    {
      setMessage();
    }
  }
  else if (m_Mode == EDIT_RENAME)
  {
    if (m_AttrsNames.contains(ui->NewNameEdit->text()))
    {
      setMessage(tr("New attribute name already exists"));
    }
    else if (ui->NewNameEdit->text().isEmpty())
    {
      setMessage(tr("New attribute name is empty"));
    }
    else
    {
      setMessage();
    }
  }
  else if (m_Mode == EDIT_REMOVE)
  {
    setMessage();
  }
}


// =====================================================================
// =====================================================================


QString EditAttributeNameDialog::getSelectedOriginalName() const
{
  return ui->OriginalNameComboBox->currentText();
}


// =====================================================================
// =====================================================================


QString EditAttributeNameDialog::getNewName() const
{
  return ui->NewNameEdit->text();
}


// =====================================================================
// =====================================================================


QString EditAttributeNameDialog::getDefaultValue() const
{
  return ui->DefaultValueEdit->text();
}

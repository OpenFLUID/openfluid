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
  @file EditSignatureDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPushButton>

#include <openfluid/ui/common/EditSignatureDialog.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_EditSignatureDialog.h"


namespace openfluid { namespace ui { namespace common {


EditSignatureDialog::EditSignatureDialog(QWidget* Parent):
    MessageDialog(Parent), ui(new Ui::EditSignatureDialog)
{
  ui->setupUi(this);

  setupMessageUi(tr("Edit signature"));
}


// =====================================================================
// =====================================================================


EditSignatureDialog::~EditSignatureDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditSignatureDialog::initialize(const QStringList& ExistingIDs)
{
  m_ExistingIDs = ExistingIDs;

  checkGlobally();

  connect(ui->SignatureWidget,SIGNAL(changed()),this,SLOT(checkGlobally()));
}


// =====================================================================
// =====================================================================


void EditSignatureDialog::checkGlobally()
{
  if (!ui->SignatureWidget->isValidID())
  {
    setMessage(tr("ID is not valid"));
  }
  else if (m_ExistingIDs.contains(ui->SignatureWidget->getEditedID()))
  {
   setMessage(tr("ID already exists"));
  }  
  else
  {
    setMessage();
  }
}


// =====================================================================
// =====================================================================


void EditSignatureDialog::initialize(const openfluid::ware::SimulatorSignature& Signature,
                                     const QStringList& ExistingIDs)
{
  ui->SignatureWidget->initialize(Signature);

  EditSignatureDialog::initialize(ExistingIDs);
}


// =====================================================================
// =====================================================================


openfluid::ware::SimulatorSignature EditSignatureDialog::getSignature() const
{
  return ui->SignatureWidget->getSignature();
}


} } } // namespaces


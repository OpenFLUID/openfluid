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
  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <QPushButton>

#include <openfluid/ui/common/EditSignatureDialog.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/common/SignatureDataEditDefs.hpp>
#include <openfluid/utils/InternalLogger.hpp>

#include "ui_EditSignatureDialog.h"


namespace openfluid { namespace ui { namespace common {


EditSignatureDialog::EditSignatureDialog(QWidget* Parent):
    MessageDialog(Parent), ui(new Ui::EditSignatureDialog)
{
  ui->setupUi(this);
  
  setupMessageUi(tr("Edit ware information"));
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

  connect(ui->SignatureWidget,SIGNAL(dataTableChanged()), this, SLOT(checkDataTable()));
}


// =====================================================================
// =====================================================================


void EditSignatureDialog::checkGlobally()
{
  if (ui->SignatureWidget->isEmptyID())
  {
    setMessage(QApplication::translate("openfluid::ui::config", openfluid::ui::config::WAREID_MESSAGE_EMPTY));
  }
  else if (m_ExistingIDs.contains(ui->SignatureWidget->getEditedID()))
  {
   setMessage(QApplication::translate("openfluid::ui::config", openfluid::ui::config::WAREID_MESSAGE_EXISTING));
  }  
  else
  {
    setMessage();
  }
}


// =====================================================================
// =====================================================================


void EditSignatureDialog::checkDataTable()
{
  QString NameHeader = QApplication::translate("openfluid::ui::common", DataTablesHeader::NAME);
  QString UnitsClassHeader = QApplication::translate("openfluid::ui::common", DataTablesHeader::UNITS_CLASS);

  try
  {
    if (ui->SignatureWidget->areAllCellsEmpty(NameHeader, DataTableType::VARIABLES))
    {
      setMessage(tr("Empty name in variables table"));
    } 
    else if (ui->SignatureWidget->areAllCellsEmpty(NameHeader, DataTableType::ATTRIBUTES))
    {
      setMessage(tr("Empty name in attributes table"));
    }  
    else if(ui->SignatureWidget->areAllCellsEmpty(UnitsClassHeader, DataTableType::VARIABLES))
    {
      setMessage(tr("Empty units class in variables table"));
    }
    else if(ui->SignatureWidget->areAllCellsEmpty(UnitsClassHeader, DataTableType::ATTRIBUTES))
    {
      setMessage(tr("Empty units class in attributes table"));
    }
    else if (!ui->SignatureWidget->areAllCellsValid(NameHeader, DataTableType::VARIABLES))
    {
      setMessage(tr("Wrong name format in variables table"));
    }
    else if (!ui->SignatureWidget->areAllCellsValid(NameHeader, DataTableType::ATTRIBUTES))
    {
      setMessage(tr("Wrong name format in attributes table"));
    }  
    else if(!ui->SignatureWidget->areAllCellsValid(UnitsClassHeader, DataTableType::VARIABLES))
    {
      setMessage(tr("Wrong units class format in variables table"));
    }
    else if(!ui->SignatureWidget->areAllCellsValid(UnitsClassHeader, DataTableType::ATTRIBUTES))
    {
      setMessage(tr("Wrong units class format in attributes table"));
    }
    else
    {
      setMessage();
    }
  }
  catch(const openfluid::base::FrameworkException& E)
  {
    openfluid::utils::log::warning("Signature edit", E.what());
    setMessage();
  }
}


// =====================================================================
// =====================================================================


void EditSignatureDialog::initialize(const openfluid::ware::SimulatorSignature& Signature,
                                     const QStringList& ExistingIDs)
{
  ui->SignatureWidget->initializeSimulator(Signature);

  EditSignatureDialog::initialize(ExistingIDs);
}


// =====================================================================
// =====================================================================


void EditSignatureDialog::initialize(const QString& SignaturePath,
                                     const QStringList& ExistingIDs)
{

  ui->SignatureWidget->initialize(SignaturePath);

  EditSignatureDialog::initialize(ExistingIDs);
}


// =====================================================================
// =====================================================================


openfluid::ware::SimulatorSignature EditSignatureDialog::getSignature() const
{
  return ui->SignatureWidget->getSignature();
}


// =====================================================================
// =====================================================================


bool EditSignatureDialog::exportSignature(QString Path) const
{
  return ui->SignatureWidget->exportSignature(Path);
}


// =====================================================================
// =====================================================================


bool EditSignatureDialog::askConfigure() const
{
  return ui->ReconfigureWareCheckBox->isChecked();
}


} } } // namespaces


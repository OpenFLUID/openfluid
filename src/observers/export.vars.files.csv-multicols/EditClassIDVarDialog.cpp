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
  @file EditClassIDVarDialog.cpp

  @author Armel THONI <armel.thoni@inra.fr>
*/


#include <QPushButton>

#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/ware/WareParamsTree.hpp>

#include "EditClassIDVarDialog.hpp"
#include "ui_EditClassIDVarDialog.h"


EditClassIDVarDialog::EditClassIDVarDialog(const QStringList& ClassNames, QWidget* Parent) :
  openfluid::ui::common::MessageDialog(Parent),
  ui(new Ui::EditClassIDVarDialog)
{
  ui->setupUi(this);
  setupMessageUi("Edit set element");
  ui->UnitsClassListWidget->addItems(ClassNames);

  ui->AllUnitsRadioButton->setChecked(true);
  ui->AllVariablesRadioButton->setChecked(true);
  ui->AllClassesRadioButton->setChecked(true);
  
  connect(ui->SelectedUnitsClassesRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->UnitsClassListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(checkGlobal()));
  connect(ui->SelectedUnitsRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->SelectedUnitsTextEdit,SIGNAL(textChanged()),this,SLOT(checkGlobal()));
  connect(ui->SelectedVariablesRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->SelectedVariablesTextEdit,SIGNAL(textChanged()),this,SLOT(checkGlobal()));
  connect(ui->CustomPrecisionCheckBox,SIGNAL(toggled(bool)),ui->PrecisionSpinBox,SLOT(setEnabled(bool)));
  
  checkGlobal();
}


// =====================================================================
// =====================================================================


EditClassIDVarDialog::~EditClassIDVarDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditClassIDVarDialog::checkGlobal()
{
  setMessage("");
  bool ValidLists = true;
  
  bool SeveralClasses = false;
  if (ui->SelectedUnitsClassesRadioButton->isChecked())
  {
    if (ui->UnitsClassListWidget->selectedItems().size() > 1)
    {
      SeveralClasses = true;
    }
  }
  else
  {
    SeveralClasses = true;
  }
  
  ui->UnitsClassListWidget->setEnabled(ui->SelectedUnitsClassesRadioButton->isChecked());
  ui->SelectedUnitsTextEdit->setEnabled(ui->SelectedUnitsRadioButton->isChecked());
  ui->SelectedVariablesTextEdit->setEnabled(ui->SelectedVariablesRadioButton->isChecked());
  
  if (SeveralClasses)
  {
    ui->groupBox_SpatialUnits->setEnabled(false);
    ui->groupBox_Variables->setEnabled(false);
    ui->AllUnitsRadioButton->setChecked(true);
    ui->AllVariablesRadioButton->setChecked(true);
  }
  else
  {
    ui->groupBox_SpatialUnits->setEnabled(true);
    ui->groupBox_Variables->setEnabled(true);

    if (ui->SelectedUnitsRadioButton->isChecked())
    {
      QRegExp Exp("^[0-9]+([-;][0-9]+)*$");
        
      std::string UnitsIDsStr = ui->SelectedUnitsTextEdit->toPlainText().trimmed().toStdString();
      std::vector<std::string> ContiguousIDs = openfluid::tools::splitString(UnitsIDsStr, ";"); 
      for (std::string ContiguousID : ContiguousIDs)
      {
        std::vector<std::string> IDLimits = openfluid::tools::splitString(ContiguousID, "-");
        if (IDLimits.size() > 2)
        {
          setMessage(tr("Too many dashes in units list"));
        }
      }
        
      if (ui->SelectedUnitsTextEdit->toPlainText().isEmpty())
      {
        setMessage(tr("Empty units list"));
      }
      else if (!Exp.exactMatch(ui->SelectedUnitsTextEdit->toPlainText()))
      {
        setMessage(tr("Invalid characters or syntax in units list"));
      }
    }

    if (ValidLists && ui->SelectedVariablesRadioButton->isChecked())
    {
      QStringList VarNames = ui->SelectedVariablesTextEdit->toPlainText().split(";");

      for (auto& Name : VarNames)
      {
        if (!openfluid::tools::isValidVariableName(Name.toStdString()))
        {
          setMessage(tr("Invalid variable name"));
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


void EditClassIDVarDialog::initialize(const QString& UnitsClasses, const QString& UnitsIDs, const QString& Variables, 
                                   const bool HasPrecision, const std::size_t Precision)
{
  if (UnitsClasses == "*")
  {
    ui->AllClassesRadioButton->setChecked(true);
  }
  else
  {
    ui->SelectedUnitsClassesRadioButton->setChecked(true);
    QList<QListWidgetItem *> UnitsClassesLines = ui->UnitsClassListWidget->findItems(UnitsClasses, Qt::MatchExactly);
    for (auto ClassLine : UnitsClassesLines)
    {
      ClassLine->setSelected(true);
    }
  }

  if (UnitsIDs == "*")
  {
    ui->AllUnitsRadioButton->setChecked(true);
  }
  else
  {
    ui->SelectedUnitsRadioButton->setChecked(true);
    ui->SelectedUnitsTextEdit->setPlainText(UnitsIDs);
  }

  if (Variables == "*")
  {
    ui->AllVariablesRadioButton->setChecked(true);
  }
  else
  {
    ui->SelectedVariablesRadioButton->setChecked(true);
    ui->SelectedVariablesTextEdit->setPlainText(Variables);
  }
  
  if (HasPrecision)
  {
    ui->CustomPrecisionCheckBox->setChecked(true);
    ui->PrecisionSpinBox->setValue(Precision);
  }
  else
  {
    ui->CustomPrecisionCheckBox->setChecked(false);
  }
}


// =====================================================================
// =====================================================================


std::vector<ClassIDVar> EditClassIDVarDialog::getClassIDVarSettings()
{
  std::vector<ClassIDVar> Triplets;
  std::vector<std::string> UnitsClasses;
  if (ui->AllClassesRadioButton->isChecked())
  {
    UnitsClasses.push_back("*");
  }
  else
  {
    QList<QListWidgetItem *> SelectedClasses = ui->UnitsClassListWidget->selectedItems();
    for (auto ClassItem : SelectedClasses)
    {
      UnitsClasses.push_back(ClassItem->text().toStdString());
    }
  }
  
  std::vector<std::string> UnitsIDs;
  if (ui->AllUnitsRadioButton->isChecked())
  {
    UnitsIDs.push_back("*");
  }
  else
  {
    std::string UnitsIDsStr = ui->SelectedUnitsTextEdit->toPlainText().trimmed().toStdString();
    std::vector<std::string> ContiguousIDs = openfluid::tools::splitString(UnitsIDsStr, ";"); 
    for (std::string ContiguousID : ContiguousIDs)
    {
      std::vector<std::string> IDLimits = openfluid::tools::splitString(ContiguousID, "-");
      if (IDLimits.size() == 1)
      {
        UnitsIDs.push_back(IDLimits[0]);
      }
      else if (IDLimits.size() == 2)
      {
        for (int i=std::stoi(IDLimits[0]) ; i<std::stoi(IDLimits[1])+1 ; i++)
        {
          UnitsIDs.push_back(std::to_string(i));
        }
      }
    }
  }

  std::vector<std::string> Vars;
  if (ui->AllVariablesRadioButton->isChecked())
  {
    Vars.push_back("*");
  }
  else
  {
    std::string VarsStr = ui->SelectedVariablesTextEdit->toPlainText().trimmed().toStdString();
    Vars = openfluid::tools::splitString(VarsStr, ";");
  }
  
  bool HasPrecision = false;
  int Precision = ui->PrecisionSpinBox->value();
  if (ui->CustomPrecisionCheckBox->isChecked())
  {
    HasPrecision = true;
  }
  
  // Generate all triplets
  for (const auto& Class : UnitsClasses)
  {
    for (const auto& Unit : UnitsIDs)
    {
      for (const auto& Var : Vars)
      {
        ClassIDVar CurrentTriplet;
        CurrentTriplet.UnitsClassesStr = Class;
        CurrentTriplet.UnitsIDsStr = Unit;
        CurrentTriplet.VariablesStr = Var;
        CurrentTriplet.HasPrecision = HasPrecision;
        CurrentTriplet.Precision = Precision;
        
        Triplets.push_back(CurrentTriplet);
      }
    }
  }
  
  return Triplets;
}

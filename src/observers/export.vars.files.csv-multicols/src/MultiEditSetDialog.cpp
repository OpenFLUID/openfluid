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
  @file MultiEditSetDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <QPushButton>

#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_MultiEditSetDialog.h"
#include "MultiEditSetDialog.hpp"
#include "EditClassIDVarDialog.hpp"


MultiEditSetDialog::MultiEditSetDialog(const QStringList& SetNames,
                                       const QStringList& FormatNames, const QStringList& ClassNames,
                                       QWidget* Parent):
  openfluid::ui::common::OpenFLUIDDialog(Parent),
  ui(new Ui::MultiEditSetDialog), m_ExistingSetsNames(SetNames), m_ClassNames(ClassNames)
{
  ui->setupUi(this);
  setAutoFillBackground(true);

  ui->AddTripletButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddTripletButton->setIconSize(QSize(16,16));

  ui->EditTripletButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->EditTripletButton->setIconSize(QSize(16,16));

  ui->RemoveTripletButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveTripletButton->setIconSize(QSize(16,16));
  

  ui->FormatComboBox->addItems(FormatNames);
  ui->SetNameEdit->setPlaceholderText(getPlaceholderRequired());

  connect(ui->SetNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->AddTripletButton,SIGNAL(clicked()),this,SLOT(addTriplet()));
  connect(ui->EditTripletButton,SIGNAL(clicked()),this,SLOT(editTriplet()));
  connect(ui->TripletsTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editTriplet()));
  connect(ui->RemoveTripletButton,SIGNAL(clicked()),this,SLOT(removeTriplet()));
  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
  connect(ui->TripletsTableWidget,SIGNAL(itemSelectionChanged()),this,SLOT(checkGlobal()));

  checkGlobal();
}


// =====================================================================
// =====================================================================


MultiEditSetDialog::~MultiEditSetDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void MultiEditSetDialog::checkGlobal()
{
  bool ValidTitle = !ui->SetNameEdit->text().isEmpty();
  bool EmptyList;
  ui->EditTripletButton->setEnabled(false);
  ui->RemoveTripletButton->setEnabled(false);
  
  if (m_TripletList.size()>0)
  {
    size_t SelectedNumber = selectedTripletsRows().size();
    if (SelectedNumber > 0)
    {
      ui->RemoveTripletButton->setEnabled(true);
    }
    if (SelectedNumber == 1)
    {
      ui->EditTripletButton->setEnabled(true);
    }
    EmptyList = false;
  }
  else
  {
    EmptyList = true;
  }
  
  if (!ValidTitle || EmptyList)
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
}


void MultiEditSetDialog::refreshTable()
{
  QBrush WarnBrush(QColor("#FFC65E"));
  std::size_t RowCount = 0;
  ui->TripletsTableWidget->setRowCount(RowCount);
  for (auto& Triplet : m_TripletList)
  {
    RowCount++;
    ui->TripletsTableWidget->setRowCount(RowCount);

    QTableWidgetItem *TableItem = new QTableWidgetItem(QString::fromStdString(Triplet.UnitsClassesStr));
    ui->TripletsTableWidget->setItem(RowCount-1, 0, TableItem);
    if (Triplet.UnitsIDsStr.empty())
    {
      TableItem->setBackground(WarnBrush);
    }

    TableItem = new QTableWidgetItem(QString::fromStdString(Triplet.UnitsIDsStr));
    ui->TripletsTableWidget->setItem(RowCount-1, 1, TableItem);
    if (Triplet.UnitsIDsStr.empty())
    {
      TableItem->setBackground(WarnBrush);
    }

    TableItem = new QTableWidgetItem(QString::fromStdString(Triplet.VariablesStr));
    ui->TripletsTableWidget->setItem(RowCount-1, 2, TableItem);
    if (Triplet.VariablesStr.empty())
    {
      TableItem->setBackground(WarnBrush);
    }

    if (Triplet.HasPrecision)
    {
      TableItem = new QTableWidgetItem(QString::number(Triplet.Precision));
    }
    else
    {
      TableItem = new QTableWidgetItem("");   
    }
    ui->TripletsTableWidget->setItem(RowCount-1, 3, TableItem);
  }
  checkGlobal();
}


// =====================================================================
// =====================================================================


std::vector<size_t> MultiEditSetDialog::selectedTripletsRows()
{
  QList<QTableWidgetSelectionRange> Ranges = ui->TripletsTableWidget->selectedRanges();
  std::vector<size_t> Positions;
  for (QTableWidgetSelectionRange& Range : Ranges)
  {
    for (int Row = Range.topRow();Row < Range.bottomRow()+1;Row++)
    {
      Positions.push_back(Row);
    }
  }
  return Positions;
}


// =====================================================================
// =====================================================================


void MultiEditSetDialog::initialize(const QString& Name, const QString& Format,
                               const QString& Selection)
{
  ui->SetNameEdit->setText(Name);
  ui->FormatComboBox->setCurrentIndex(ui->FormatComboBox->findText(Format));
  
  m_TripletList = stringSelectionToClassIDVarList(Selection.toStdString(), 
                                                  std::numeric_limits<unsigned int>::quiet_NaN(), 
                                                  "auto");
  
  refreshTable();
  
  checkGlobal();
}


// =====================================================================
// =====================================================================


void MultiEditSetDialog::addTriplet()
{

  EditClassIDVarDialog AddDlg(m_ClassNames, this);

  if (AddDlg.exec() == QDialog::Accepted)
  {
    std::vector<openfluid::tools::ClassIDVarPrecision> Triplets = AddDlg.getClassIDVarSettings();
    m_TripletList.insert(m_TripletList.end(), Triplets.begin(), Triplets.end());
    
    refreshTable();
  }
}


// =====================================================================
// =====================================================================


void MultiEditSetDialog::editTriplet()
{
  std::size_t CurrentRow = ui->TripletsTableWidget->currentRow();

  QString UnitsClasses, UnitsIDs, Variables;
  bool HasPrecision;
  std::size_t Precision;
  UnitsClasses = ui->TripletsTableWidget->item(CurrentRow,0)->text();
  UnitsIDs = ui->TripletsTableWidget->item(CurrentRow,1)->text();
  Variables = ui->TripletsTableWidget->item(CurrentRow,2)->text();
    
  QString PrecisionStr = ui->TripletsTableWidget->item(CurrentRow,3)->text();
  if (PrecisionStr == "")
  {
    HasPrecision = false;
    Precision = 0;
  }
  else
  {
    HasPrecision = true;
    Precision = std::stoi(PrecisionStr.toStdString());
  }
    

  EditClassIDVarDialog EditDlg(m_ClassNames, this);

  EditDlg.initialize(UnitsClasses, UnitsIDs, Variables, HasPrecision, Precision);


  if (EditDlg.exec() == QDialog::Accepted)
  {
    if (CurrentRow < m_TripletList.size())
    {
      m_TripletList.erase(m_TripletList.begin()+CurrentRow);
      std::vector<openfluid::tools::ClassIDVarPrecision> Triplets = EditDlg.getClassIDVarSettings();
      m_TripletList.insert(m_TripletList.begin()+CurrentRow, Triplets.begin(), Triplets.end());
      
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                "row greater than triplet list size");
    }
    refreshTable();
  }
}


// =====================================================================
// =====================================================================


void MultiEditSetDialog::removeTriplet()
{
  std::vector<size_t> InvertedPositions = selectedTripletsRows();
  std::sort(InvertedPositions.begin(), InvertedPositions.end(), std::greater<size_t>()); 
      
  bool NeedTableRefresh = false;
  for (size_t Row : InvertedPositions)
  {

      if (Row < m_TripletList.size())
      {
        m_TripletList.erase(m_TripletList.begin()+Row);
         NeedTableRefresh = true;
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                  "row number greater than triplet list size.");
      }
  }
  if (NeedTableRefresh)
  {
    refreshTable();
  }
}


// =====================================================================
// =====================================================================


openfluid::ware::WareParams_t MultiEditSetDialog::getMultiSetParams()
{
  std::string ParamsRoot = "set."+ui->SetNameEdit->text().toStdString()+".";

  std::string FormatStr = ui->FormatComboBox->currentText().toStdString();

  std::string SelectionStr = "";
  std::size_t TripletNumber = 0;
  for (auto& Triplet : m_TripletList)
  {
    SelectionStr += Triplet.getClassIDVarString(true);
    if (TripletNumber < m_TripletList.size()-1)
    {
      SelectionStr += ";";
    }
    TripletNumber++;
  }
  return openfluid::ware::WareParams_t({{ParamsRoot+"format",FormatStr},
                                        {ParamsRoot+"selection",SelectionStr}});
}


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
  @file SignatureDataEditorWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <QComboBox>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/core/Value.hpp>

#include "ui_SignatureDataEditorWidget.h"
#include "SignatureDataEditorWidget.hpp"


namespace openfluid { namespace ui { namespace common {


SignatureDataEditorWidget::SignatureDataEditorWidget(QWidget* Parent):
  QWidget(Parent), ui(new Ui::SignatureDataEditorWidget)
{
  ui->setupUi(this);

  m_DataIDHeader = QApplication::translate("openfluid::ui::common", DataTablesHeader::NAME);
  m_UnitsClassHeader = QApplication::translate("openfluid::ui::common", DataTablesHeader::UNITS_CLASS);
  m_IOConditionHeader = QApplication::translate("openfluid::ui::common", DataTablesHeader::IO_CONDITION);
  m_TypeHeader = QApplication::translate("openfluid::ui::common", DataTablesHeader::TYPE);
  m_DescriptionHeader = QApplication::translate("openfluid::ui::common", DataTablesHeader::DESCRIPTION);
  m_SIUnitHeader = QApplication::translate("openfluid::ui::common", DataTablesHeader::SIUNIT);

  m_ColHeadersValuesMap = {{m_DataIDHeader, 0},
                           {m_UnitsClassHeader, 1},
                           {m_IOConditionHeader, 2},
                           {m_TypeHeader, 3},
                           {m_DescriptionHeader, 4},
                           {m_SIUnitHeader, 5}};

  ui->AddButton->setText("");
  ui->AddButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddButton->setIconSize(QSize(20,20));

  ui->RemoveButton->setText("");
  ui->RemoveButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveButton->setIconSize(QSize(20,20));

  connect(ui->AddButton,SIGNAL(clicked()),this,SLOT(addDataLine()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removeDataLine()));
  connect(ui->DataTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(onItemChanged(QTableWidgetItem*)));
}


// =====================================================================
// =====================================================================


SignatureDataEditorWidget::~SignatureDataEditorWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


const QTableWidget* SignatureDataEditorWidget::dataTableWidget() const
{
  return ui->DataTableWidget;
}


// =====================================================================
// =====================================================================


void SignatureDataEditorWidget::initialize(const QList<DataColumns>& Columns)
{
  m_Columns = Columns;
  int StretchedCol = Columns.size()-1;
  int i = 0;

  QStringList ColHeaders;

  for (auto& Col : m_Columns)
  {
    if (Col == DataColumns::DATAID)
    {
      ColHeaders << m_DataIDHeader;
      m_AvailableHeaders.insert(m_DataIDHeader);
    }
    else if (Col == DataColumns::UNITSCLASS)
    {
      ColHeaders << m_UnitsClassHeader;
      m_AvailableHeaders.insert(m_UnitsClassHeader);
    }
    else if (Col == DataColumns::ROCONDITION || 
             Col == DataColumns::RWCONDITION || 
             Col == DataColumns::RWUCONDITION)
    {
      ColHeaders << m_IOConditionHeader;
      m_AvailableHeaders.insert(m_IOConditionHeader);
    }
    else if (Col == DataColumns::DATATYPE)
    {
      ColHeaders << m_TypeHeader;
      m_AvailableHeaders.insert(m_TypeHeader);
    }
    else if (Col == DataColumns::DESCRIPTION)
    {
      ColHeaders << m_DescriptionHeader;
      m_AvailableHeaders.insert(m_DescriptionHeader);
      StretchedCol = i;
    }
    else if (Col == DataColumns::SIUNIT)
    {
      ColHeaders << m_SIUnitHeader;
      m_AvailableHeaders.insert(m_SIUnitHeader);
    }

    i++;
  }

  ui->DataTableWidget->setColumnCount(m_Columns.size());
  ui->DataTableWidget->setHorizontalHeaderLabels(ColHeaders);

  ui->DataTableWidget->horizontalHeader()->setSectionResizeMode(StretchedCol,QHeaderView::Stretch);

}


// =====================================================================
// =====================================================================


bool SignatureDataEditorWidget::areAllCellsValid(const QString& Header, 
                                                 const DataTableType& HandledDataType)
{
  if(!m_ColHeadersValuesMap.contains(Header))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Header " + Header.toStdString() + 
                                              " not found for the SignatureDataEditorWidget");
  }

  for (int Row = 0; Row < ui->DataTableWidget->rowCount(); ++Row) 
  {
    QTableWidgetItem* Item = ui->DataTableWidget->item(Row, m_ColHeadersValuesMap[Header]);
    if(Header == m_UnitsClassHeader)
    {
      if(!openfluid::tools::isValidUnitsClassName(Item->text().toStdString()))
      {
        return false;
      }
    }
    else if(Header == m_DataIDHeader)
    {
      if(HandledDataType == DataTableType::VARIABLES)
      {
        if(!openfluid::tools::isValidVariableName(Item->text().toStdString()))
        {
          return false;
        }
      }
      else if(HandledDataType == DataTableType::ATTRIBUTES)
      {
        if(!openfluid::tools::isValidAttributeName(Item->text().toStdString()))
        {
          return false;
        }
      }
    }
  }
  return true;
}


// =====================================================================
// =====================================================================


void SignatureDataEditorWidget::onItemChanged(QTableWidgetItem*)
{
  emit dataTableChanged();
}


// =====================================================================
// =====================================================================


void SignatureDataEditorWidget::addDataLine(const QMap<int,QVariant>& DataLine)
{
  disconnect(ui->DataTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onItemChanged(QTableWidgetItem*)));

  int RowCount = ui->DataTableWidget->rowCount();
  ui->DataTableWidget->setRowCount(RowCount+1);
  int i = 0;

  for (auto& Col : m_Columns)
  {
    if (Col == DataColumns::DATAID)
    {
      QString TmpStr = "name";
      if (DataLine.contains(static_cast<int>(DataColumns::DATAID)))
      {
        TmpStr = DataLine.value(static_cast<int>(DataColumns::DATAID)).toString();
      }
      ui->DataTableWidget->setItem(RowCount,i,new QTableWidgetItem(TmpStr));
    }
    else if (Col == DataColumns::UNITSCLASS)
    {
      QString TmpStr = "CLASS";
      if (DataLine.contains(static_cast<int>(DataColumns::UNITSCLASS)))
      {
        TmpStr = DataLine.value(static_cast<int>(DataColumns::UNITSCLASS)).toString();
      }
      ui->DataTableWidget->setItem(RowCount,i,new QTableWidgetItem(TmpStr));
    }
    else if (Col == DataColumns::DESCRIPTION)
    {
      QString TmpStr = "";
      if (DataLine.contains(static_cast<int>(DataColumns::DESCRIPTION)))
      {
        TmpStr = DataLine.value(static_cast<int>(DataColumns::DESCRIPTION)).toString();
      }
      ui->DataTableWidget->setItem(RowCount,i,new QTableWidgetItem(TmpStr));
    }
    else if (Col == DataColumns::SIUNIT)
    {
      QString TmpStr = "";
      if (DataLine.contains(static_cast<int>(DataColumns::SIUNIT)))
      {
        TmpStr = DataLine.value(static_cast<int>(DataColumns::SIUNIT)).toString();
      }
      ui->DataTableWidget->setItem(RowCount,i,new QTableWidgetItem(TmpStr));
    }
    else if (Col == DataColumns::ROCONDITION)
    {
      QComboBox* Combo = new QComboBox();
      Combo->addItem(tr("Used"),static_cast<int>(DataConditionsIndices::USED));
      Combo->addItem(tr("Required"),static_cast<int>(DataConditionsIndices::REQUIRED));
      if (DataLine.contains(static_cast<int>(DataColumns::ROCONDITION)))
      {
        Combo->setCurrentIndex(DataLine.value(static_cast<int>(DataColumns::ROCONDITION)).toInt());
      }
      ui->DataTableWidget->setCellWidget(RowCount,i,Combo);
    }
    else if (Col == DataColumns::RWCONDITION)
    {
      QComboBox* Combo = new QComboBox();
      Combo->addItem(tr("Used"),static_cast<int>(DataConditionsIndices::USED));
      Combo->addItem(tr("Required"),static_cast<int>(DataConditionsIndices::REQUIRED));
      Combo->addItem(tr("Produced"),static_cast<int>(DataConditionsIndices::PRODUCED));
      if (DataLine.contains(static_cast<int>(DataColumns::RWCONDITION)))
      {
        Combo->setCurrentIndex(DataLine.value(static_cast<int>(DataColumns::RWCONDITION)).toInt());
      }
      ui->DataTableWidget->setCellWidget(RowCount,i,Combo);
    }
    else if (Col == DataColumns::RWUCONDITION)
    {
      QComboBox* Combo = new QComboBox();
      Combo->addItem(tr("Used"),static_cast<int>(DataConditionsIndices::USED));
      Combo->addItem(tr("Required"),static_cast<int>(DataConditionsIndices::REQUIRED));
      Combo->addItem(tr("Produced"),static_cast<int>(DataConditionsIndices::PRODUCED));
      Combo->addItem(tr("Updated"),static_cast<int>(DataConditionsIndices::UPDATED));
      if (DataLine.contains(static_cast<int>(DataColumns::RWUCONDITION)))
      {
        Combo->setCurrentIndex(DataLine.value(static_cast<int>(DataColumns::RWUCONDITION)).toInt());
      }
      ui->DataTableWidget->setCellWidget(RowCount,i,Combo);
    }
    else if (Col == DataColumns::DATATYPE)
    {
      QComboBox* Combo = new QComboBox();
      for (int TypeInt = openfluid::core::Value::NONE; TypeInt <= openfluid::core::Value::NULLL; TypeInt++)
      {
        Combo->addItem(QString::fromStdString(
          openfluid::core::Value::getStringFromValueType(static_cast<openfluid::core::Value::Type>(TypeInt))),TypeInt);
      }
      if (DataLine.contains(static_cast<int>(DataColumns::DATATYPE)))
      {
        Combo->setCurrentIndex(DataLine.value(static_cast<int>(DataColumns::DATATYPE)).toInt());
      }
      ui->DataTableWidget->setCellWidget(RowCount,i,Combo);
    }
    i++;
  }
  connect(ui->DataTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onItemChanged(QTableWidgetItem*)));
}


// =====================================================================
// =====================================================================


void SignatureDataEditorWidget::addDataLine()
{
  addDataLine({});

  QModelIndex Index = ui->DataTableWidget->model()->index(ui->DataTableWidget->rowCount()-1,0,QModelIndex());
  ui->DataTableWidget->edit(Index);
}


// =====================================================================
// =====================================================================


void SignatureDataEditorWidget::removeDataLine()
{
  int Row = ui->DataTableWidget->currentRow();

  if (Row >= 0)
  {
    ui->DataTableWidget->removeRow(Row);
    emit dataTableChanged();
  }
}


} } } // namespaces


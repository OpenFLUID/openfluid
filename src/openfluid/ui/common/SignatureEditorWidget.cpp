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
  @file SignatureEditorWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ui/common/SignatureEditorWidget.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/tools/IDHelpers.hpp>

#include "ui_SignatureEditorWidget.h"


namespace openfluid { namespace ui { namespace common {


SignatureEditorWidget::SignatureEditorWidget(QWidget* Parent):
  QTabWidget(Parent), ui(new Ui::SignatureEditorWidget), m_StaticID(false)
{
  ui->setupUi(this);

  setCurrentIndex(0);
  ui->IDLabel->setVisible(false);

  ui->AddAuthorButton->setText("");
  ui->AddAuthorButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddAuthorButton->setIconSize(QSize(20,20));

  ui->RemoveAuthorButton->setText("");
  ui->RemoveAuthorButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveAuthorButton->setIconSize(QSize(20,20));

  ui->FixedSchedValueEdit->setValidator(new QIntValidator(ui->FixedSchedValueEdit));
  ui->RangeSchedMinValueEdit->setValidator(new QIntValidator(ui->RangeSchedMinValueEdit));
  ui->RangeSchedMaxValueEdit->setValidator(new QIntValidator(ui->RangeSchedMaxValueEdit));

  connect(ui->AddAuthorButton,SIGNAL(clicked()),this,SLOT(addAuthorLine()));
  connect(ui->RemoveAuthorButton,SIGNAL(clicked()),this,SLOT(removeAuthorLine()));

}


// =====================================================================
// =====================================================================


SignatureEditorWidget::~SignatureEditorWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::notifyChanged()
{
  emit changed();
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeID(const QString& ID)
{
  if (ID.isEmpty())
   {
     ui->IDLabel->setVisible(false);
     ui->IDEdit->setVisible(true);
     ui->IDEdit->setText("");
     ui->IDEdit->setPlaceholderText(QApplication::translate("openfluid::ui::config",
                                                            openfluid::ui::config::PLACEHOLDER_REQUIRED));
     m_StaticID = false;
     connect(ui->IDEdit,SIGNAL(textEdited(const QString&)),this,SLOT(notifyChanged()));
   }
   else
   {
     ui->IDLabel->setVisible(true);
     ui->IDEdit->setVisible(false);
     ui->IDLabel->setText(ID);
     m_StaticID = true;
   }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeCommon(const openfluid::ware::WareSignature* Signature)
{
  int i;

  // infos

  initializeID(QString::fromStdString(Signature->ID));

  ui->NameEdit->setText(QString::fromStdString(Signature->Name));
  ui->DescriptionEdit->setText(QString::fromStdString(Signature->Description));
  ui->VersionEdit->setText(QString::fromStdString(Signature->Version));

  ui->AuthorsTableWidget->setRowCount(Signature->Authors.size());
  i = 0;
  for (auto& Author : Signature->Authors)
  {
    ui->AuthorsTableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Author.first)));
    ui->AuthorsTableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(Author.second)));
    i++;
  }

  ui->StatusComboBox->setCurrentIndex(Signature->Status);
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeParametersUIFromSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  QMap<int,QVariant> DataMap;

  ui->ParametersDataWidget->initialize({SignatureDataEditorWidget::DATACOL_DATAID,
                                        SignatureDataEditorWidget::DATACOL_ROCONDITION,
                                        SignatureDataEditorWidget::DATACOL_DESCRIPTION,
                                        SignatureDataEditorWidget::DATACOL_SIUNIT});

  DataMap.insert(SignatureDataEditorWidget::DATACOL_ROCONDITION,
                 SignatureDataEditorWidget::DATACOND_REQUIRED);

  for (auto& Item : Signature.HandledData.RequiredParams)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item.DataName));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_SIUNIT,QString::fromStdString(Item.DataUnit));
    ui->ParametersDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(SignatureDataEditorWidget::DATACOL_ROCONDITION,
                 SignatureDataEditorWidget::DATACOND_USED);

  for (auto& Item : Signature.HandledData.UsedParams)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item.DataName));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_SIUNIT,QString::fromStdString(Item.DataUnit));
    ui->ParametersDataWidget->addDataLine(DataMap);
  }

}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeExtrafilesUIFromSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  QMap<int,QVariant> DataMap;

  ui->ExtraFilesDataWidget->initialize({SignatureDataEditorWidget::DATACOL_DATAID,
                                        SignatureDataEditorWidget::DATACOL_ROCONDITION});

  DataMap.insert(SignatureDataEditorWidget::DATACOL_ROCONDITION,
                 SignatureDataEditorWidget::DATACOND_REQUIRED);

  for (auto& Item : Signature.HandledData.RequiredExtraFiles)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item));
    ui->ExtraFilesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(SignatureDataEditorWidget::DATACOL_ROCONDITION,
                 SignatureDataEditorWidget::DATACOND_USED);

  for (auto& Item : Signature.HandledData.UsedExtraFiles)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item));
    ui->ExtraFilesDataWidget->addDataLine(DataMap);
  }

}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeAttributesUIFromSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  QMap<int,QVariant> DataMap;

  ui->AttributesDataWidget->initialize({SignatureDataEditorWidget::DATACOL_DATAID,
                                        SignatureDataEditorWidget::DATACOL_UNITSCLASS,
                                        SignatureDataEditorWidget::DATACOL_RWCONDITION,
                                        SignatureDataEditorWidget::DATACOL_DESCRIPTION,
                                        SignatureDataEditorWidget::DATACOL_SIUNIT});

  DataMap.insert(SignatureDataEditorWidget::DATACOL_RWCONDITION,
                 SignatureDataEditorWidget::DATACOND_REQUIRED);

  for (auto& Item : Signature.HandledData.RequiredAttribute)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item.DataName));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_UNITSCLASS,QString::fromStdString(Item.UnitsClass));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_SIUNIT,QString::fromStdString(Item.DataUnit));
    ui->AttributesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(SignatureDataEditorWidget::DATACOL_RWCONDITION,
                 SignatureDataEditorWidget::DATACOND_USED);

  for (auto& Item : Signature.HandledData.UsedAttribute)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item.DataName));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_UNITSCLASS,QString::fromStdString(Item.UnitsClass));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_SIUNIT,QString::fromStdString(Item.DataUnit));
    ui->AttributesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(SignatureDataEditorWidget::DATACOL_RWCONDITION,
                 SignatureDataEditorWidget::DATACOND_PRODUCED);

  for (auto& Item : Signature.HandledData.ProducedAttribute)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item.DataName));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_UNITSCLASS,QString::fromStdString(Item.UnitsClass));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_SIUNIT,QString::fromStdString(Item.DataUnit));
    ui->AttributesDataWidget->addDataLine(DataMap);
  }

}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeEventsUIFromSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  QMap<int,QVariant> DataMap;

  ui->EventsDataWidget->initialize({SignatureDataEditorWidget::DATACOL_UNITSCLASS});

  for (auto& Item : Signature.HandledData.UsedEventsOnUnits)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_UNITSCLASS,QString::fromStdString(Item));
    ui->EventsDataWidget->addDataLine(DataMap);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeVariablesUIFromSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  QMap<int,QVariant> DataMap;

  ui->VariablesDataWidget->initialize({SignatureDataEditorWidget::DATACOL_DATAID,
                                       SignatureDataEditorWidget::DATACOL_UNITSCLASS,
                                       SignatureDataEditorWidget::DATACOL_RWUCONDITION,
                                       SignatureDataEditorWidget::DATACOL_DATATYPE,
                                       SignatureDataEditorWidget::DATACOL_DESCRIPTION,
                                       SignatureDataEditorWidget::DATACOL_SIUNIT});

  DataMap.insert(SignatureDataEditorWidget::DATACOL_RWUCONDITION,
                 SignatureDataEditorWidget::DATACOND_REQUIRED);

  for (auto& Item : Signature.HandledData.RequiredVars)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item.DataName));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_UNITSCLASS,QString::fromStdString(Item.UnitsClass));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATATYPE,Item.DataType);
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_SIUNIT,QString::fromStdString(Item.DataUnit));
    ui->VariablesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(SignatureDataEditorWidget::DATACOL_RWUCONDITION,
                 SignatureDataEditorWidget::DATACOND_USED);

  for (auto& Item : Signature.HandledData.UsedVars)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item.DataName));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_UNITSCLASS,QString::fromStdString(Item.UnitsClass));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATATYPE,Item.DataType);
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_SIUNIT,QString::fromStdString(Item.DataUnit));
    ui->VariablesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(SignatureDataEditorWidget::DATACOL_RWUCONDITION,
                 SignatureDataEditorWidget::DATACOND_PRODUCED);

  for (auto& Item : Signature.HandledData.ProducedVars)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item.DataName));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_UNITSCLASS,QString::fromStdString(Item.UnitsClass));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATATYPE,Item.DataType);
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_SIUNIT,QString::fromStdString(Item.DataUnit));
    ui->VariablesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(SignatureDataEditorWidget::DATACOL_RWUCONDITION,
                 SignatureDataEditorWidget::DATACOND_UPDATED);

  for (auto& Item : Signature.HandledData.UpdatedVars)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATAID,QString::fromStdString(Item.DataName));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_UNITSCLASS,QString::fromStdString(Item.UnitsClass));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DATATYPE,Item.DataType);
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_SIUNIT,QString::fromStdString(Item.DataUnit));
    ui->VariablesDataWidget->addDataLine(DataMap);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeDynamicsUIFromSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  QMap<int,QVariant> DataMap;

  ui->UndefSchedRadioButton->setChecked(true);

  ui->SpatialDynDataWidget->initialize({SignatureDataEditorWidget::DATACOL_UNITSCLASS,
                                        SignatureDataEditorWidget::DATACOL_DESCRIPTION});


  if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::DEFAULT)
  {
    ui->DefaultSchedRadioButton->setChecked(true);
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::FIXED)
  {
    ui->FixedSchedRadioButton->setChecked(true);
    ui->FixedSchedValueEdit->setText(QString("%1").arg(Signature.TimeScheduling.Min));
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::RANGE)
  {
    ui->RangeSchedRadioButton->setChecked(true);
    ui->RangeSchedMinValueEdit->setText(QString("%1").arg(Signature.TimeScheduling.Min));
    ui->RangeSchedMaxValueEdit->setText(QString("%1").arg(Signature.TimeScheduling.Max));
  }


  ui->SpatialDynDescEdit->setText(QString::fromStdString(Signature.HandledUnitsGraph.UpdatedUnitsGraph));

  for (auto& Item : Signature.HandledUnitsGraph.UpdatedUnitsClass)
  {
    DataMap.insert(SignatureDataEditorWidget::DATACOL_UNITSCLASS,QString::fromStdString(Item.UnitsClass));
    DataMap.insert(SignatureDataEditorWidget::DATACOL_DESCRIPTION,QString::fromStdString(Item.Description));
    ui->SpatialDynDataWidget->addDataLine(DataMap);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initialize(const openfluid::ware::SimulatorSignature& Signature)
{
  initializeCommon(&Signature);


  ui->DomainEdit->setText(QString::fromStdString(Signature.Domain));
  ui->ProcessEdit->setText(QString::fromStdString(Signature.Process));
  ui->MethodEdit->setText(QString::fromStdString(Signature.Method));


  initializeParametersUIFromSignature(Signature);

  initializeExtrafilesUIFromSignature(Signature);

  initializeAttributesUIFromSignature(Signature);

  initializeEventsUIFromSignature(Signature);

  initializeVariablesUIFromSignature(Signature);

  initializeDynamicsUIFromSignature(Signature);

}


// =====================================================================
// =====================================================================


std::string extractTableFieldToString(const QTableWidget* TableW, int Row, int Column)
{
  const QTableWidgetItem* TableWItem = nullptr;

  TableWItem = TableW->item(Row,Column);
  if (TableWItem)
  {
    return TableWItem->text().toStdString();
  }

  return "";
}


// =====================================================================
// =====================================================================


SignatureDataEditorWidget::DataConditionsIndices extractTableComboToCondition(const QTableWidget* TableW,
                                                                              int Row, int Column)
{
  QComboBox* CondCombo = static_cast<QComboBox*>(TableW->cellWidget(Row,Column));

  if (CondCombo)
  {
    return static_cast<SignatureDataEditorWidget::DataConditionsIndices>
      (CondCombo->itemData(CondCombo->currentIndex()).toInt());
  }

  return SignatureDataEditorWidget::DATACOND_UNKNOWN;
}


// =====================================================================
// =====================================================================


openfluid::core::Value::Type extractTableComboToDataType(const QTableWidget* TableW,
                                                         int Row, int Column)
{
  QComboBox* TypeCombo = static_cast<QComboBox*>(TableW->cellWidget(Row,Column));

  if (TypeCombo)
  {
    return static_cast<openfluid::core::Value::Type>
      (TypeCombo->itemData(TypeCombo->currentIndex()).toInt());
  }

  return openfluid::core::Value::NONE;
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromParametersUI(openfluid::ware::SimulatorSignature& Signature) const
{
  const QTableWidget* DataTableW = ui->ParametersDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::ware::SignatureDataItem Item;

    Item.DataName = extractTableFieldToString(DataTableW,i,0);
    Item.Description = extractTableFieldToString(DataTableW,i,2);
    Item.DataUnit = extractTableFieldToString(DataTableW,i,3);

    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,1);

    if (CondIndex >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DATACOND_USED)
      {
        Signature.HandledData.UsedParams.push_back(Item);
      }
      else
      {
        Signature.HandledData.RequiredParams.push_back(Item);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromExtrafilesUI(openfluid::ware::SimulatorSignature& Signature) const
{
  const QTableWidget* DataTableW = ui->ExtraFilesDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    std::string Filename;

    Filename = extractTableFieldToString(DataTableW,i,0);

    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,1);

    if (CondIndex >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DATACOND_USED)
      {
        Signature.HandledData.UsedExtraFiles.push_back(Filename);
      }
      else
      {
        Signature.HandledData.RequiredExtraFiles.push_back(Filename);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromAttributesUI(openfluid::ware::SimulatorSignature& Signature) const
{
  const QTableWidget* DataTableW = ui->AttributesDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::ware::SignatureSpatialDataItem Item;

    Item.DataName = extractTableFieldToString(DataTableW,i,0);
    Item.UnitsClass = extractTableFieldToString(DataTableW,i,1);
    Item.Description = extractTableFieldToString(DataTableW,i,3);
    Item.DataUnit = extractTableFieldToString(DataTableW,i,4);


    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,2);

    if (CondIndex >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DATACOND_USED)
      {
        Signature.HandledData.UsedAttribute.push_back(Item);
      }
      else if (CondIndex == SignatureDataEditorWidget::DATACOND_REQUIRED)
      {
        Signature.HandledData.RequiredAttribute.push_back(Item);
      }
      else
      {
        Signature.HandledData.ProducedAttribute.push_back(Item);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromEventsUI(openfluid::ware::SimulatorSignature& Signature) const
{
  const QTableWidget* DataTableW = ui->EventsDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::core::UnitsClass_t Item = extractTableFieldToString(DataTableW,i,0);
    Signature.HandledData.UsedEventsOnUnits.push_back(Item);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromVariablesUI(openfluid::ware::SimulatorSignature& Signature) const
{
  const QTableWidget* DataTableW = ui->VariablesDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::ware::SignatureTypedSpatialDataItem Item;

    Item.DataName = extractTableFieldToString(DataTableW,i,0);
    Item.UnitsClass = extractTableFieldToString(DataTableW,i,1);
    Item.Description = extractTableFieldToString(DataTableW,i,4);
    Item.DataUnit = extractTableFieldToString(DataTableW,i,5);

    Item.DataType = extractTableComboToDataType(DataTableW,i,3);

    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,2);

    if (CondIndex >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DATACOND_USED)
      {
        Signature.HandledData.UsedVars.push_back(Item);
      }
      else if (CondIndex == SignatureDataEditorWidget::DATACOND_REQUIRED)
      {
        Signature.HandledData.RequiredVars.push_back(Item);
      }
      else if (CondIndex == SignatureDataEditorWidget::DATACOND_PRODUCED)
      {
        Signature.HandledData.ProducedVars.push_back(Item);
      }
      else
      {
        Signature.HandledData.UpdatedVars.push_back(Item);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromDynamicsUI(openfluid::ware::SimulatorSignature& Signature) const
{
  Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::UNDEFINED;

  if (ui->DefaultSchedRadioButton->isChecked())
  {
    Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::DEFAULT;
  }
  else if (ui->FixedSchedRadioButton->isChecked())
  {
    Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::FIXED;
    openfluid::core::Duration_t Dt = ui->FixedSchedValueEdit->text().toLongLong();
    Signature.TimeScheduling.Min = Dt;
    Signature.TimeScheduling.Max = Dt;
  }
  else if (ui->RangeSchedRadioButton->isChecked())
  {
    Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::RANGE;
    openfluid::core::Duration_t DtMin = ui->RangeSchedMinValueEdit->text().toLongLong();
    openfluid::core::Duration_t DtMax = ui->RangeSchedMaxValueEdit->text().toLongLong();
    Signature.TimeScheduling.Min = DtMin;
    Signature.TimeScheduling.Max = DtMax;
  }


  Signature.HandledUnitsGraph.UpdatedUnitsGraph = ui->SpatialDynDescEdit->toPlainText().toStdString();

  const QTableWidget* DataTableW = ui->SpatialDynDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::ware::SignatureUnitsClassItem Item;
    Item.UnitsClass = extractTableFieldToString(DataTableW,i,0);
    Item.Description = extractTableFieldToString(DataTableW,i,1);
    Signature.HandledUnitsGraph.UpdatedUnitsClass.push_back(Item);
  }
}


// =====================================================================
// =====================================================================


openfluid::ware::SimulatorSignature SignatureEditorWidget::getSignature() const
{
  openfluid::ware::SimulatorSignature Signature;

  if (m_StaticID)
  {
    Signature.ID = ui->IDLabel->text().toStdString();
  }
  else
  {
    Signature.ID = ui->IDEdit->text().toStdString();
  }

  Signature.Name = ui->NameEdit->text().toStdString();
  Signature.Description = ui->DescriptionEdit->toPlainText().toStdString();

  for (int i=0; i < ui->AuthorsTableWidget->rowCount() ; i++)
  {
    Signature.Authors.push_back(std::make_pair(ui->AuthorsTableWidget->item(i,0)->text().toStdString(),
                                               ui->AuthorsTableWidget->item(i,1)->text().toStdString()));
  }

  Signature.Version = ui->VersionEdit->text().toStdString();
  Signature.Status = openfluid::ware::EXPERIMENTAL;  // TODO change Status to enum class
  if (ui->StatusComboBox->currentIndex() == 1)
  {
    Signature.Status = openfluid::ware::BETA;
  }
  else if (ui->StatusComboBox->currentIndex() == 2)
  {
    Signature.Status = openfluid::ware::STABLE;
  }  


  Signature.Domain = ui->DomainEdit->text().toStdString();
  Signature.Process = ui->ProcessEdit->text().toStdString();
  Signature.Method = ui->MethodEdit->text().toStdString();


  updateSignatureFromParametersUI(Signature);

  updateSignatureFromExtrafilesUI(Signature);

  updateSignatureFromAttributesUI(Signature);

  updateSignatureFromEventsUI(Signature);

  updateSignatureFromVariablesUI(Signature);

  updateSignatureFromDynamicsUI(Signature);


  return Signature;
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::addAuthorLine()
{
  int RowCount = ui->AuthorsTableWidget->rowCount();

  ui->AuthorsTableWidget->setRowCount(RowCount+1);

  ui->AuthorsTableWidget->setItem(RowCount,0,new QTableWidgetItem("name"));
  ui->AuthorsTableWidget->setItem(RowCount,1,new QTableWidgetItem("email"));
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::removeAuthorLine()
{
  int Row = ui->AuthorsTableWidget->currentRow();

  if (Row >= 0)
  {
    ui->AuthorsTableWidget->removeRow(Row);
  }
}


// =====================================================================
// =====================================================================


bool SignatureEditorWidget::isValidID() const
{
  if (m_StaticID)
  {
    return true;
  }

  return openfluid::tools::isValidWareID(ui->IDEdit->text().toStdString());
}


// =====================================================================
// =====================================================================


QString SignatureEditorWidget::getEditedID() const
{
  return ui->IDEdit->text();
}


} } } // namespaces

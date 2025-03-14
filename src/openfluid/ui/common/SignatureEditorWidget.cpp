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
  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <vector>

#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/ui/common/SignatureEditorWidget.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/waresdev/ObserverSignatureSerializer.hpp>
#include <openfluid/waresdev/BuilderextSignatureSerializer.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/config.hpp>

#include "ui_SignatureEditorWidget.h"


namespace openfluid { namespace ui { namespace common {


SignatureEditorWidget::SignatureEditorWidget(QWidget* Parent):
  QTabWidget(Parent), ui(new Ui::SignatureEditorWidget), m_StaticID(false)
{
  ui->setupUi(this);

  mp_IssuesManager = ui->IssuesManagerWidget;

  setCurrentIndex(0);
  ui->IDLabel->setVisible(false);

  for (auto* Button : std::vector<QPushButton*>({ui->AddAuthorButton, ui->AddContactButton, ui->AddDependencyButton}))
  {
    Button->setText("");
    Button->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
    Button->setIconSize(QSize(20,20));
  }

  for (auto* Button : std::vector<QPushButton*>({ui->RemoveAuthorButton, ui->RemoveContactButton, 
                                                 ui->RemoveDependencyButton}))
  {
    Button->setText("");
    Button->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
    Button->setIconSize(QSize(20,20));
  }

  ui->FixedSchedValueEdit->setValidator(new QIntValidator(ui->FixedSchedValueEdit));
  ui->RangeSchedMinValueEdit->setValidator(new QIntValidator(ui->RangeSchedMinValueEdit));
  ui->RangeSchedMaxValueEdit->setValidator(new QIntValidator(ui->RangeSchedMaxValueEdit));

  connect(ui->AddAuthorButton,SIGNAL(clicked()),this,SLOT(addAuthorLine()));
  connect(ui->RemoveAuthorButton,SIGNAL(clicked()),this,SLOT(removeAuthorLine()));

  connect(ui->AddContactButton,SIGNAL(clicked()),this,SLOT(addContactLine()));
  connect(ui->RemoveContactButton,SIGNAL(clicked()),this,SLOT(removeContactLine()));

  connect(ui->AddDependencyButton,SIGNAL(clicked()),this,SLOT(addDependencyLine()));
  connect(ui->RemoveDependencyButton,SIGNAL(clicked()),this,SLOT(removeDependencyLine()));

  connect(ui->VariablesDataWidget,SIGNAL(dataTableChanged()), this, SIGNAL(dataTableChanged()));
  connect(ui->AttributesDataWidget,SIGNAL(dataTableChanged()), this, SIGNAL(dataTableChanged()));

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


bool SignatureEditorWidget::areAllCellsValid(const QString& Header, const DataTableType& HandledDataType)
{
  if(HandledDataType == DataTableType::VARIABLES)
  {
    return ui->VariablesDataWidget->areAllCellsValid(Header, HandledDataType);
  }
  if(HandledDataType == DataTableType::ATTRIBUTES)
  {
    return ui->AttributesDataWidget->areAllCellsValid(Header, HandledDataType);
  }

  return true;
}


// =====================================================================
// =====================================================================


bool SignatureEditorWidget::areAllCellsEmpty(const QString& Header, const DataTableType& HandledDataType)
{
  if(HandledDataType == DataTableType::VARIABLES)
  {
    return ui->VariablesDataWidget->areAllCellsEmpty(Header);
  }
  if(HandledDataType == DataTableType::ATTRIBUTES)
  {
    return ui->AttributesDataWidget->areAllCellsEmpty(Header);
  }

  return true;
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
    QString IDTooltip;
    ui->IDEdit->setValidator(
#if (QT_VERSION_MAJOR < 6)
      new QRegExpValidator(openfluid::ui::common::getWareIdRegExp(IDTooltip), this));
#else
      new QRegularExpressionValidator(openfluid::ui::common::getWareIdRegExp(IDTooltip), this));
#endif
    ui->IDEdit->setToolTip(IDTooltip);
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

  ui->LicenseEdit->setText(QString::fromStdString(Signature->License));

  ui->ContactsTableWidget->setRowCount(Signature->Contacts.size());
  i = 0;
  for (auto& Contact : Signature->Contacts)
  {
    ui->ContactsTableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Contact.first)));
    ui->ContactsTableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(Contact.second)));
    i++;
  }

  ui->TagsEdit->setText(QString::fromStdString(openfluid::tools::join(Signature->Tags, 
                                                                      openfluid::config::CHAR_TAG_SEPARATOR)));

  ui->DependenciesTableWidget->setRowCount(Signature->Dependencies.size());
  i = 0;
  for (auto& Dependency : Signature->Dependencies)
  {
    ui->DependenciesTableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Dependency.first)));
    ui->DependenciesTableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(Dependency.second)));
    i++;
  }

  mp_IssuesManager->loadContent(Signature->Issues);

}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeParametersUIFromSignature(const openfluid::ware::DataWareSignature& Signature)
{
    QMap<int,QVariant> DataMap;

  ui->ParametersDataWidget->initialize({SignatureDataEditorWidget::DataColumns::DATAID,
                                        SignatureDataEditorWidget::DataColumns::ROCONDITION,
                                        SignatureDataEditorWidget::DataColumns::DESCRIPTION,
                                        SignatureDataEditorWidget::DataColumns::SIUNIT});

  DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::ROCONDITION),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::REQUIRED));

  for (auto& Item : Signature.HandledData.RequiredParams)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item.Name));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::SIUNIT),
                   QString::fromStdString(Item.SIUnit));
    ui->ParametersDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::ROCONDITION),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::USED));

  for (auto& Item : Signature.HandledData.UsedParams)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item.Name));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::SIUNIT),
                   QString::fromStdString(Item.SIUnit));
    ui->ParametersDataWidget->addDataLine(DataMap);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeExtrafilesUIFromSignature(const openfluid::ware::DataWareSignature& Signature)
{
  QMap<int,QVariant> DataMap;

  ui->ExtraFilesDataWidget->initialize({SignatureDataEditorWidget::DataColumns::DATAID,
                                        SignatureDataEditorWidget::DataColumns::ROCONDITION});

  DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::ROCONDITION),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::REQUIRED));

  for (auto& Item : Signature.HandledData.RequiredExtraFiles)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item));
    ui->ExtraFilesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::ROCONDITION),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::USED));

  for (auto& Item : Signature.HandledData.UsedExtraFiles)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item));
    ui->ExtraFilesDataWidget->addDataLine(DataMap);
  }

}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeInputAttributesUIFromSignature(
                              const openfluid::ware::DataWareSignature& Signature, bool IsRW)
{
  SignatureDataEditorWidget::DataColumns WriteState = SignatureDataEditorWidget::DataColumns::ROCONDITION;
  if (IsRW)
  {
    WriteState = SignatureDataEditorWidget::DataColumns::RWCONDITION;
  }

  QMap<int,QVariant> DataMap;

  ui->AttributesDataWidget->initialize({SignatureDataEditorWidget::DataColumns::DATAID,
                                        SignatureDataEditorWidget::DataColumns::UNITSCLASS,
                                        WriteState,
                                        SignatureDataEditorWidget::DataColumns::DESCRIPTION,
                                        SignatureDataEditorWidget::DataColumns::SIUNIT});

  DataMap.insert(static_cast<int>(WriteState),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::REQUIRED));

  for (auto& Item : Signature.HandledData.RequiredAttributes)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item.Name));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::UNITSCLASS),
                   QString::fromStdString(Item.UnitsClass));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::SIUNIT),
                   QString::fromStdString(Item.SIUnit));
    ui->AttributesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(static_cast<int>(WriteState),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::USED));

  for (auto& Item : Signature.HandledData.UsedAttributes)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item.Name));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::UNITSCLASS),
                   QString::fromStdString(Item.UnitsClass));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::SIUNIT),
                   QString::fromStdString(Item.SIUnit));
    ui->AttributesDataWidget->addDataLine(DataMap);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeAttributesUIFromSignature(const openfluid::ware::SimulatorSignature& Signature, 
                                                                bool IsRW)
{
  SignatureDataEditorWidget::DataColumns WriteState = SignatureDataEditorWidget::DataColumns::ROCONDITION;
  if (IsRW)
  {
    WriteState = SignatureDataEditorWidget::DataColumns::RWCONDITION;
  }

  initializeInputAttributesUIFromSignature(Signature, IsRW);
  
  QMap<int,QVariant> DataMap;
  
  DataMap.insert(static_cast<int>(WriteState),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::PRODUCED));

  for (auto& Item : Signature.SimulatorHandledData.ProducedAttributes)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item.Name));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::UNITSCLASS),
                   QString::fromStdString(Item.UnitsClass));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::SIUNIT),
                   QString::fromStdString(Item.SIUnit));
    ui->AttributesDataWidget->addDataLine(DataMap);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeEventsUIFromSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  QMap<int,QVariant> DataMap;

  ui->EventsDataWidget->initialize({SignatureDataEditorWidget::DataColumns::UNITSCLASS});

  for (auto& Item : Signature.SimulatorHandledData.UsedEventsOnUnits)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::UNITSCLASS),
                   QString::fromStdString(Item));
    ui->EventsDataWidget->addDataLine(DataMap);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeInputVariablesUIFromSignature(const openfluid::ware::DataWareSignature& Signature,
                                                                    bool IsRW)
{
  QMap<int,QVariant> DataMap;

  SignatureDataEditorWidget::DataColumns WriteState = SignatureDataEditorWidget::DataColumns::ROCONDITION;
  if (IsRW)
  {
    WriteState = SignatureDataEditorWidget::DataColumns::RWUCONDITION;
  }

  ui->VariablesDataWidget->initialize({SignatureDataEditorWidget::DataColumns::DATAID,
                                       SignatureDataEditorWidget::DataColumns::UNITSCLASS,
                                       WriteState,
                                       SignatureDataEditorWidget::DataColumns::DATATYPE,
                                       SignatureDataEditorWidget::DataColumns::DESCRIPTION,
                                       SignatureDataEditorWidget::DataColumns::SIUNIT});

  DataMap.insert(static_cast<int>(WriteState),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::REQUIRED));

  for (auto& Item : Signature.HandledData.RequiredVars)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item.Name));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::UNITSCLASS),
                   QString::fromStdString(Item.UnitsClass));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATATYPE),
                   static_cast<int>(Item.DataType));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::SIUNIT),
                   QString::fromStdString(Item.SIUnit));
    ui->VariablesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(static_cast<int>(WriteState),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::USED));

  for (auto& Item : Signature.HandledData.UsedVars)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item.Name));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::UNITSCLASS),
                   QString::fromStdString(Item.UnitsClass));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATATYPE),
                   static_cast<int>(Item.DataType));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::SIUNIT),
                   QString::fromStdString(Item.SIUnit));
    ui->VariablesDataWidget->addDataLine(DataMap);
  }

}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeVariablesUIFromSignature(const openfluid::ware::SimulatorSignature& Signature, 
                                                               bool IsRW)
{
  initializeInputVariablesUIFromSignature(Signature, IsRW);

  SignatureDataEditorWidget::DataColumns WriteState = SignatureDataEditorWidget::DataColumns::ROCONDITION;
  if (IsRW)
  {
    WriteState = SignatureDataEditorWidget::DataColumns::RWUCONDITION;
  }
  
  QMap<int,QVariant> DataMap;

  DataMap.insert(static_cast<int>(WriteState),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::PRODUCED));

  for (auto& Item : Signature.SimulatorHandledData.ProducedVars)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item.Name));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::UNITSCLASS),
                   QString::fromStdString(Item.UnitsClass));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATATYPE),
                   static_cast<int>(Item.DataType));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::SIUNIT),
                   QString::fromStdString(Item.SIUnit));
    ui->VariablesDataWidget->addDataLine(DataMap);
  }

  DataMap.insert(static_cast<int>(WriteState),
                 static_cast<int>(SignatureDataEditorWidget::DataConditionsIndices::UPDATED));

  for (auto& Item : Signature.SimulatorHandledData.UpdatedVars)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATAID),
                   QString::fromStdString(Item.Name));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::UNITSCLASS),
                   QString::fromStdString(Item.UnitsClass));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DATATYPE),
                   static_cast<int>(Item.DataType));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::SIUNIT),
                   QString::fromStdString(Item.SIUnit));
    ui->VariablesDataWidget->addDataLine(DataMap);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeDynamicsUIFromSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  QMap<int,QVariant> DataMap;

  ui->UndefSchedRadioButton->setChecked(true);

  ui->SpatialDynDataWidget->initialize({SignatureDataEditorWidget::DataColumns::UNITSCLASS,
                                        SignatureDataEditorWidget::DataColumns::DESCRIPTION});


  if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT)
  {
    ui->DefaultSchedRadioButton->setChecked(true);
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED)
  {
    ui->FixedSchedRadioButton->setChecked(true);
    ui->FixedSchedValueEdit->setText(QString("%1").arg(Signature.TimeScheduling.Min));
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
  {
    ui->RangeSchedRadioButton->setChecked(true);
    ui->RangeSchedMinValueEdit->setText(QString("%1").arg(Signature.TimeScheduling.Min));
    ui->RangeSchedMaxValueEdit->setText(QString("%1").arg(Signature.TimeScheduling.Max));
  }


  ui->SpatialDynDescEdit->setText(QString::fromStdString(Signature.HandledUnitsGraph.UpdatedUnitsGraph));

  for (auto& Item : Signature.HandledUnitsGraph.UpdatedUnitsClasses)
  {
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::UNITSCLASS),
                   QString::fromStdString(Item.UnitsClass));
    DataMap.insert(static_cast<int>(SignatureDataEditorWidget::DataColumns::DESCRIPTION),
                   QString::fromStdString(Item.Description));
    ui->SpatialDynDataWidget->addDataLine(DataMap);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeIntegrationUIFromSignature(
  const openfluid::builderext::BuilderExtensionSignature& Signature)
{
  ui->BextSetupWidget->initializeFromSignature(Signature);
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeSimulator(const openfluid::ware::SimulatorSignature& Signature)
{
  initializeCommon(&Signature);

  initializeParametersUIFromSignature(Signature);

  initializeExtrafilesUIFromSignature(Signature);

  initializeAttributesUIFromSignature(Signature);

  initializeEventsUIFromSignature(Signature);

  initializeVariablesUIFromSignature(Signature);

  initializeDynamicsUIFromSignature(Signature);

}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeObserver(const openfluid::ware::ObserverSignature& Signature)
{
  initializeCommon(&Signature);

  initializeParametersUIFromSignature(Signature);

  initializeExtrafilesUIFromSignature(Signature);

  initializeInputAttributesUIFromSignature(Signature, false);

  initializeInputVariablesUIFromSignature(Signature, false);
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initializeBuilderext(const openfluid::builderext::BuilderExtensionSignature& Signature)
{
  initializeCommon(&Signature);

  initializeIntegrationUIFromSignature(Signature);
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::initialize(const QString& SignaturePath)
{
  openfluid::ware::WareType Type = openfluid::waresdev::detectWareType(SignaturePath.toStdString());
  

  if (Type == openfluid::ware::WareType::SIMULATOR)
  {
    removeTab(5);//BuilderextTab
    auto Signature = openfluid::waresdev::SimulatorSignatureSerializer().readFromJSONFile(SignaturePath.toStdString());

    initializeSimulator(Signature);
  }
  else 
  {
    if (Type == openfluid::ware::WareType::OBSERVER)
    {
      removeTab(4);//DynamicsTab
      removeTab(4);//BuilderextTab
      auto Signature = openfluid::waresdev::ObserverSignatureSerializer().readFromJSONFile(SignaturePath.toStdString());
      //ui->ExtraFilesGroupBox->setVisible(false);
      initializeObserver(Signature);
    }
    else if (Type == openfluid::ware::WareType::BUILDEREXT)
    {
      removeTab(1);//ParametersTab
      removeTab(1);//AttributesTab
      removeTab(1);//VariablesTab
      removeTab(1);//DynamicsTab
      auto Signature = openfluid::waresdev::BuilderextSignatureSerializer().readFromJSONFile(
        SignaturePath.toStdString());

      initializeBuilderext(Signature);
    }
  }
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

  return SignatureDataEditorWidget::DataConditionsIndices::UNKNOWN;
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


void SignatureEditorWidget::updateSignatureFromParametersUI(openfluid::ware::DataWareSignature& Signature) const
{
  const QTableWidget* DataTableW = ui->ParametersDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::ware::SignatureDataItem Item;

    Item.Name = extractTableFieldToString(DataTableW,i,0);
    Item.Description = extractTableFieldToString(DataTableW,i,2);
    Item.SIUnit = extractTableFieldToString(DataTableW,i,3);

    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,1);

    if (static_cast<int>(CondIndex) >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DataConditionsIndices::USED)
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


void SignatureEditorWidget::updateSignatureFromExtrafilesUI(openfluid::ware::DataWareSignature& Signature) const
{
  const QTableWidget* DataTableW = ui->ExtraFilesDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    std::string Filename;

    Filename = extractTableFieldToString(DataTableW,i,0);

    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,1);

    if (static_cast<int>(CondIndex) >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DataConditionsIndices::USED)
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


void SignatureEditorWidget::updateSignatureFromReadAttributesUI(openfluid::ware::DataWareSignature& Signature) const
{
  const QTableWidget* DataTableW = ui->AttributesDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::ware::SignatureSpatialDataItem Item;

    Item.Name = extractTableFieldToString(DataTableW,i,0);
    Item.UnitsClass = extractTableFieldToString(DataTableW,i,1);
    Item.Description = extractTableFieldToString(DataTableW,i,3);
    Item.SIUnit = extractTableFieldToString(DataTableW,i,4);


    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,2);

    if (static_cast<int>(CondIndex) >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DataConditionsIndices::USED)
      {
        Signature.HandledData.UsedAttributes.push_back(Item);
      }
      else if (CondIndex == SignatureDataEditorWidget::DataConditionsIndices::REQUIRED)
      {
        Signature.HandledData.RequiredAttributes.push_back(Item);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromAttributesUI(openfluid::ware::SimulatorSignature& Signature) const
{
  updateSignatureFromReadAttributesUI(Signature);
  const QTableWidget* DataTableW = ui->AttributesDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::ware::SignatureSpatialDataItem Item;

    Item.Name = extractTableFieldToString(DataTableW,i,0);
    Item.UnitsClass = extractTableFieldToString(DataTableW,i,1);
    Item.Description = extractTableFieldToString(DataTableW,i,3);
    Item.SIUnit = extractTableFieldToString(DataTableW,i,4);


    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,2);

    if (static_cast<int>(CondIndex) >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DataConditionsIndices::PRODUCED)
      {
        Signature.SimulatorHandledData.ProducedAttributes.push_back(Item);
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
    Signature.SimulatorHandledData.UsedEventsOnUnits.push_back(Item);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromReadVariablesUI(openfluid::ware::DataWareSignature& Signature) const
{
  const QTableWidget* DataTableW = ui->VariablesDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::ware::SignatureSpatialDataItem Item;

    Item.Name = extractTableFieldToString(DataTableW,i,0);
    Item.UnitsClass = extractTableFieldToString(DataTableW,i,1);
    Item.Description = extractTableFieldToString(DataTableW,i,4);
    Item.SIUnit = extractTableFieldToString(DataTableW,i,5);

    Item.DataType = extractTableComboToDataType(DataTableW,i,3);

    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,2);

    if (static_cast<int>(CondIndex) >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DataConditionsIndices::USED)
      {
        Signature.HandledData.UsedVars.push_back(Item);
      }
      else if (CondIndex == SignatureDataEditorWidget::DataConditionsIndices::REQUIRED)
      {
        Signature.HandledData.RequiredVars.push_back(Item);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromVariablesUI(openfluid::ware::SimulatorSignature& Signature) const
{
  updateSignatureFromReadVariablesUI(Signature);

  const QTableWidget* DataTableW = ui->VariablesDataWidget->dataTableWidget();

  for (int i=0; i < DataTableW->rowCount();i++)
  {
    openfluid::ware::SignatureSpatialDataItem Item;

    Item.Name = extractTableFieldToString(DataTableW,i,0);
    Item.UnitsClass = extractTableFieldToString(DataTableW,i,1);
    Item.Description = extractTableFieldToString(DataTableW,i,4);
    Item.SIUnit = extractTableFieldToString(DataTableW,i,5);

    Item.DataType = extractTableComboToDataType(DataTableW,i,3);

    SignatureDataEditorWidget::DataConditionsIndices CondIndex = extractTableComboToCondition(DataTableW,i,2);

    if (static_cast<int>(CondIndex) >= 0)
    {
      if (CondIndex == SignatureDataEditorWidget::DataConditionsIndices::PRODUCED)
      {
        Signature.SimulatorHandledData.ProducedVars.push_back(Item);
      }
      else if (CondIndex == SignatureDataEditorWidget::DataConditionsIndices::UPDATED)
      {
        Signature.SimulatorHandledData.UpdatedVars.push_back(Item);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromDynamicsUI(openfluid::ware::SimulatorSignature& Signature) const
{
  Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::SchedulingType::UNDEFINED;

  if (ui->DefaultSchedRadioButton->isChecked())
  {
    Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT;
  }
  else if (ui->FixedSchedRadioButton->isChecked())
  {
    Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED;
    openfluid::core::Duration_t Dt = ui->FixedSchedValueEdit->text().toLongLong();
    Signature.TimeScheduling.Min = Dt;
    Signature.TimeScheduling.Max = Dt;
  }
  else if (ui->RangeSchedRadioButton->isChecked())
  {
    Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE;
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
    Signature.HandledUnitsGraph.UpdatedUnitsClasses.push_back(Item);
  }
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromCommonsUI(openfluid::ware::WareSignature& Signature) const
{
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


  for (int i=0; i < ui->ContactsTableWidget->rowCount() ; i++)
  {
    Signature.Contacts.push_back(std::make_pair(ui->ContactsTableWidget->item(i,0)->text().toStdString(),
                                               ui->ContactsTableWidget->item(i,1)->text().toStdString()));
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

  Signature.License = ui->LicenseEdit->text().toStdString();

  Signature.Tags = openfluid::tools::split(ui->TagsEdit->text().toStdString(), openfluid::config::CHAR_TAG_SEPARATOR);

  for (int i=0; i < ui->DependenciesTableWidget->rowCount() ; i++)
  {
    Signature.Dependencies[ui->DependenciesTableWidget->item(i,0)->text().toStdString()] = 
                                               ui->DependenciesTableWidget->item(i,1)->text().toStdString();
    
  }
  
  Signature.Issues = mp_IssuesManager->getIssues();
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::updateSignatureFromIntegrationUI(
  openfluid::builderext::BuilderExtensionSignature& Signature) const
{
  Signature.Mode = ui->BextSetupWidget->getBuilderextMode();
  Signature.Category = ui->BextSetupWidget->getBuilderextCategory();
  Signature.MenuText = ui->BextSetupWidget->getBuilderextMenuText().toStdString();
}


// =====================================================================
// =====================================================================


openfluid::ware::SimulatorSignature SignatureEditorWidget::getSignature() const
{

  openfluid::ware::SimulatorSignature Signature;

  updateSignatureFromCommonsUI(Signature);

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


bool SignatureEditorWidget::exportSignature(const QString& SignaturePath) const
{
  openfluid::ware::WareType Type = openfluid::waresdev::detectWareType(SignaturePath.toStdString());
 
  if (Type == openfluid::ware::WareType::SIMULATOR)
  {
    auto Signature = getSignature();
    openfluid::waresdev::SimulatorSignatureSerializer().writeToJSONFile(Signature,SignaturePath.toStdString());
    return true;
  }
  else if (Type == openfluid::ware::WareType::OBSERVER)
  {
    openfluid::ware::ObserverSignature Signature;
    updateSignatureFromCommonsUI(Signature);
    updateSignatureFromParametersUI(Signature);
    updateSignatureFromExtrafilesUI(Signature);
    updateSignatureFromReadAttributesUI(Signature);
    updateSignatureFromReadVariablesUI(Signature);
    openfluid::waresdev::ObserverSignatureSerializer().writeToJSONFile(Signature,SignaturePath.toStdString());
    return true;
  }
  else if (Type == openfluid::ware::WareType::BUILDEREXT)
  {
    openfluid::builderext::BuilderExtensionSignature Signature;
    updateSignatureFromCommonsUI(Signature);
    updateSignatureFromIntegrationUI(Signature);
    openfluid::waresdev::BuilderextSignatureSerializer().writeToJSONFile(Signature,SignaturePath.toStdString());
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::removeLine(QTableWidget* Table)
{
  int Row = Table->currentRow();

  if (Row >= 0)
  {
    Table->removeRow(Row);
  }
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
  removeLine(ui->AuthorsTableWidget);
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::addContactLine()
{
  int RowCount = ui->ContactsTableWidget->rowCount();

  ui->ContactsTableWidget->setRowCount(RowCount+1);

  ui->ContactsTableWidget->setItem(RowCount,0,new QTableWidgetItem("name"));
  ui->ContactsTableWidget->setItem(RowCount,1,new QTableWidgetItem("email"));
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::removeContactLine()
{
  removeLine(ui->ContactsTableWidget);
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::addDependencyLine()
{
  int RowCount = ui->DependenciesTableWidget->rowCount();

  ui->DependenciesTableWidget->setRowCount(RowCount+1);

  ui->DependenciesTableWidget->setItem(RowCount,0,new QTableWidgetItem("name"));
  ui->DependenciesTableWidget->setItem(RowCount,1,new QTableWidgetItem("*"));
}


// =====================================================================
// =====================================================================


void SignatureEditorWidget::removeDependencyLine()
{
  removeLine(ui->DependenciesTableWidget);
}


// =====================================================================
// =====================================================================


bool SignatureEditorWidget::isEmptyID() const
{
  return (getEditedID().isEmpty() && !m_StaticID);
}


// =====================================================================
// =====================================================================


QString SignatureEditorWidget::getEditedID() const
{
  return ui->IDEdit->text();
}


} } } // namespaces

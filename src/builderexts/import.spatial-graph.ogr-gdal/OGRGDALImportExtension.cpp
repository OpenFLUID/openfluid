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
  \file OGRGDALImportExtension.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_OGRGDALDialog.h"
#include "OGRGDALImportExtension.hpp"
#include "FileSourceAddDialog.hpp"
#include "WFSSourceAddDialog.hpp"

#include <QMessageBox>


// =====================================================================
// =====================================================================


BEGIN_BUILDEREXT_SIGNATURE("import.spatial-graph.ogr-gdal", openfluid::builderext::TYPE_MODAL)

  DECLARE_CATEGORY(openfluid::builderext::CAT_SPATIAL)
  DECLARE_MENUTEXT("Spatial data import (OGR/GDAL)")

END_BUILDEREXT_SIGNATURE


// =====================================================================
// =====================================================================


OGRGDALImportExtension::OGRGDALImportExtension() :
  openfluid::builderext::PluggableModalExtension(), ui(new Ui::OGRGDALDialog),
  m_CurrentSrcIndex(-1)
{
  Q_INIT_RESOURCE(ogrgdalimport);

  ui->setupUi(this);

  setWindowTitle("Spatial data import (OGR/GDAL)");

  ui->AddFileButton->setIcon(QIcon(":/add.png"));
  ui->AddFileButton->setIconSize(QSize(16,16));

  ui->AddWFSButton->setIcon(QIcon(":/add.png"));
  ui->AddWFSButton->setIconSize(QSize(16,16));

  ui->RemoveButton->setIcon(QIcon(":/remove.png"));
  ui->RemoveButton->setIconSize(QSize(16,16));

  ui->ConfigTabWidget->setEnabled(false);
  ui->ConfigTabWidget->setCurrentIndex(0);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  ui->SourcesTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#else
  ui->SourcesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif

  connect(ui->AddFileButton,SIGNAL(clicked()),this,SLOT(addFileSource()));
  connect(ui->AddWFSButton,SIGNAL(clicked()),this,SLOT(addWFSSource()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removeSource()));

  connect(ui->PrecheckButton,SIGNAL(clicked()),this,SLOT(runPrecheck()));

  connect(ui->SourcesTableWidget,SIGNAL(itemSelectionChanged()),this,SLOT(updateConfigTabWidget()));


  connect(ui->UnitsClassLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateUnitsClassInfos()));

  connect(ui->UnitsIDsComboBox,SIGNAL(activated(int)),this,SLOT(updateUnitsIDsInfos()));
  connect(ui->PcsOrdComboBox,SIGNAL(activated(int)),this,SLOT(updateUnitsPcsOrdInfos()));

  connect(ui->ToConnectComboBox,SIGNAL(activated(int)),this,SLOT(updateUnitsToConnInfos()));
  connect(ui->ChildofConnectComboBox,SIGNAL(activated(int)),this,SLOT(updateUnitsChildofConnInfos()));

  connect(ui->AttributesListWidget,SIGNAL(itemChanged(QListWidgetItem*)),
          this,SLOT(updateImportedFieldsInfos(QListWidgetItem*)));

  connect(ui->ComputeAreaCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateIsAreaComputeInfos()));
  connect(ui->ComputeAreaLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateAreaComputeAttrInfos()));

  connect(ui->ComputeLengthCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateIsLengthComputeInfos()));
  connect(ui->ComputeLengthLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateLengthComputeAttrInfos()));

  connect(ui->ComputeXCentroidCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateIsXCentroidComputeInfos()));
  connect(ui->ComputeXCentroidLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateXCentroidComputeAttrInfos()));

  connect(ui->ComputeYCentroidCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateIsYCentroidComputeInfos()));
  connect(ui->ComputeYCentroidLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateYCentroidComputeAttrInfos()));

  connect(ui->ComputeZCentroidCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateIsZCentroidComputeInfos()));
  connect(ui->ComputeZCentroidLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateZCentroidComputeAttrInfos()));


  ui->ButtonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
  connect(ui->ButtonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(processButtonBoxClicked(QAbstractButton*)));

  ui->SourcesTableWidget->setFocus();
}


// =====================================================================
// =====================================================================


OGRGDALImportExtension::~OGRGDALImportExtension()
{
  delete ui;
}


// =====================================================================
// =====================================================================


bool OGRGDALImportExtension::initialize()
{
  std::string DirStr;
  OPENFLUID_GetRunEnvironment("dir.input",DirStr);

  m_InputDir = QString::fromStdString(DirStr);

  return true;
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::update(openfluid::builderext::FluidXUpdateFlags::Flags /*UpdateFlags*/)
{

}


// =====================================================================
// =====================================================================

void OGRGDALImportExtension::addSource(const SourceInfos& SrcInfos)
{
  m_SourcesInfos.append(SrcInfos);

  ui->SourcesTableWidget->setRowCount(ui->SourcesTableWidget->rowCount()+1);

  int RowIndex = ui->SourcesTableWidget->rowCount()-1;

  ui->SourcesTableWidget->setItem(RowIndex,1,new QTableWidgetItem(SrcInfos.SourceURI));
  ui->SourcesTableWidget->setItem(RowIndex,2,new QTableWidgetItem(SrcInfos.LayerName));
  ui->SourcesTableWidget->setItem(RowIndex,3,new QTableWidgetItem(QString(OGRGeometryTypeToName(SrcInfos.SourceGeomType))));

  ui->SourcesTableWidget->selectRow(RowIndex);

  ui->SourcesTableWidget->setFocus();

  updateConfigTabWidget();
}


// =====================================================================
// =====================================================================



void OGRGDALImportExtension::addFileSource()
{
  FileSourceAddDialog AddDlg(m_InputDir,this);

  if (AddDlg.exec() == QDialog::Accepted)
  {
    addSource(AddDlg.getSourceInfos());
  }
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::addWFSSource()
{
  WFSSourceAddDialog AddDlg(this);

  if (AddDlg.exec() == QDialog::Accepted)
  {
    addSource(AddDlg.getSourceInfos());
  }
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::removeSource()
{
  if (ui->SourcesTableWidget->currentRow() >= 0)
  {
    if (QMessageBox::question(QApplication::activeWindow(),
                              "Spatial data import (OGR/GDAL)",
                              tr("You are about to remove the selected source to import.\n"
                                  "Import configuration for this source will be lost.\n\n"
                                  "Proceed anyway?"),
                                  QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {
      m_SourcesInfos.removeAt(ui->SourcesTableWidget->currentRow());
      ui->SourcesTableWidget->removeRow(ui->SourcesTableWidget->currentRow());
    }
  }
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::processButtonBoxClicked(QAbstractButton* Button)
{
  QDialogButtonBox::StandardButton StdButton = ui->ButtonBox->standardButton(Button);

  if (StdButton == QDialogButtonBox::Cancel)
    cancelImport();
  else if(StdButton == QDialogButtonBox::Apply)
    proceedToImport();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::proceedToImport()
{
  accept();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::cancelImport()
{
  if (m_SourcesInfos.size() == 0)
    reject();
  else
  {
    if (QMessageBox::question(QApplication::activeWindow(),
                              "Spatial data import (OGR/GDAL)",
                              tr("You have at least one source configured for import.\n"
                                  "This configuration(s) will be lost.\n\n"
                                  "Quit the spatial import anyway?"),
                                  QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {
      reject();
    }
  }
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::closeEvent(QCloseEvent* Event)
{
  Event->ignore();
  cancelImport();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateConfigTabWidget()
{
  std::cout << __PRETTY_FUNCTION__ << ", line " << __LINE__ << std::endl;

  if (ui->SourcesTableWidget->currentRow() >= m_SourcesInfos.size())
  {
    if (m_SourcesInfos.size() == 0)
    {
      ui->SourcesTableWidget->selectRow(-1);
    }
    else
    {
      ui->SourcesTableWidget->selectRow(0);
    }
    return;
  }

  m_CurrentSrcIndex = ui->SourcesTableWidget->currentRow();

  ui->UnitsClassLineEdit->clear();
  ui->UnitsIDsComboBox->clear();
  ui->PcsOrdComboBox->clear();
  ui->ToConnectComboBox->clear();
  ui->ChildofConnectComboBox->clear();
  ui->AttributesListWidget->clear();


  std::cout << m_SourcesInfos.size() << " > " << m_CurrentSrcIndex << std::endl;


  if (m_CurrentSrcIndex >=0)
  {
    ui->ConfigTabWidget->setEnabled(true);

    // Units class
    ui->UnitsClassLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].UnitsClass);


    // Units IDs field
    ui->UnitsIDsComboBox->addItems(m_SourcesInfos[m_CurrentSrcIndex].AvailableFields);

    if (m_SourcesInfos[m_CurrentSrcIndex].UnitsIDsField.isEmpty())
    {
      ui->UnitsIDsComboBox->setCurrentIndex(ui->UnitsIDsComboBox->findText("OFLD_ID"));
    }
    else
    {
      ui->UnitsIDsComboBox->setCurrentIndex(ui->UnitsIDsComboBox->findText(m_SourcesInfos[m_CurrentSrcIndex].UnitsIDsField));
    }


    // Units process order field
    ui->PcsOrdComboBox->addItems(m_SourcesInfos[m_CurrentSrcIndex].AvailableFields);

    if (m_SourcesInfos[m_CurrentSrcIndex].UnitsPcsOrdField.isEmpty())
    {
      ui->PcsOrdComboBox->setCurrentIndex(ui->PcsOrdComboBox->findText("OFLD_PSORD"));
    }
    else
    {
      ui->PcsOrdComboBox->setCurrentIndex(ui->PcsOrdComboBox->findText(m_SourcesInfos[m_CurrentSrcIndex].UnitsPcsOrdField));
    }


    // Units "to" connections field
    ui->ToConnectComboBox->addItems(m_SourcesInfos[m_CurrentSrcIndex].AvailableFields);

    if (m_SourcesInfos[m_CurrentSrcIndex].ToConnectionsField.isEmpty())
    {
      ui->ToConnectComboBox->setCurrentIndex(ui->ToConnectComboBox->findText("OFLD_TO"));
    }
    else
    {
      ui->ToConnectComboBox->setCurrentIndex(ui->ToConnectComboBox->findText(m_SourcesInfos[m_CurrentSrcIndex].ToConnectionsField));
    }


    // Units "childof" connections field
    ui->ChildofConnectComboBox->addItems(m_SourcesInfos[m_CurrentSrcIndex].AvailableFields);

    if (m_SourcesInfos[m_CurrentSrcIndex].ChildofConnectionsField.isEmpty())
    {
      ui->ChildofConnectComboBox->setCurrentIndex(ui->ChildofConnectComboBox->findText("OFLD_CHILD"));
    }
    else
    {
      ui->ChildofConnectComboBox->setCurrentIndex(ui->ChildofConnectComboBox->findText(m_SourcesInfos[m_CurrentSrcIndex].ChildofConnectionsField));
    }


    // Fields to import as attributes
    for (int i=0; i< m_SourcesInfos[m_CurrentSrcIndex].AvailableFields.size();i++)
    {
      QListWidgetItem* Item = new QListWidgetItem(m_SourcesInfos[m_CurrentSrcIndex].AvailableFields[i]);
      if (m_SourcesInfos[m_CurrentSrcIndex].ImportedFields.contains(m_SourcesInfos[m_CurrentSrcIndex].AvailableFields[i]))
        Item->setCheckState(Qt::Checked);
      else
        Item->setCheckState(Qt::Unchecked);
      ui->AttributesListWidget->addItem(Item);
    }


    // Area attribute to compute
    ui->ComputeAreaCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsAreaCompute);
    ui->ComputeAreaLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].AreaComputeAttribute);

    // Length attribute to compute
    ui->ComputeLengthCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsLengthCompute);
    ui->ComputeLengthLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].LengthComputeAttribute);

    // XCentroid attribute to compute
    ui->ComputeXCentroidCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsXCentroidCompute);
    ui->ComputeXCentroidLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].XCentroidComputeAttribute);

    // YCentroid attribute to compute
    ui->ComputeYCentroidCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsYCentroidCompute);
    ui->ComputeYCentroidLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].YCentroidComputeAttribute);

    // ZCentroid attribute to compute
    ui->ComputeZCentroidCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsZCentroidCompute);
    ui->ComputeZCentroidLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].ZCentroidComputeAttribute);


    // Import to datastore
    // TODO

    for (int i=0;i<m_SourcesInfos.size();i++)
    {
      std::cout << m_SourcesInfos[i].AreaComputeAttribute.toStdString() << std::endl;
    }
  }
  else
  {
    ui->ConfigTabWidget->setEnabled(false);
  }

  std::cout << __PRETTY_FUNCTION__ << ", line " << __LINE__ << std::endl;
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::runPrecheck()
{
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateUnitsClassInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].UnitsClass = ui->UnitsClassLineEdit->text();
  ui->SourcesTableWidget->setItem(m_CurrentSrcIndex,0,new QTableWidgetItem(m_SourcesInfos[m_CurrentSrcIndex].UnitsClass));
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateUnitsIDsInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].UnitsIDsField = ui->UnitsIDsComboBox->currentText();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateUnitsPcsOrdInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].UnitsPcsOrdField = ui->PcsOrdComboBox->currentText();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateUnitsToConnInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].ToConnectionsField = ui->ToConnectComboBox->currentText();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateUnitsChildofConnInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].ChildofConnectionsField = ui->ChildofConnectComboBox->currentText();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateImportedFieldsInfos(QListWidgetItem* Item)
{
  if (Item->checkState() == Qt::Unchecked)
    m_SourcesInfos[m_CurrentSrcIndex].ImportedFields.removeAll(Item->text());

  if (Item->checkState() == Qt::Checked)
    m_SourcesInfos[m_CurrentSrcIndex].ImportedFields.append(Item->text());
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateIsAreaComputeInfos()
{
  ui->ComputeAreaLineEdit->setEnabled(ui->ComputeAreaCheckBox->isChecked());
  m_SourcesInfos[m_CurrentSrcIndex].IsAreaCompute = ui->ComputeAreaCheckBox->isChecked();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateAreaComputeAttrInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].AreaComputeAttribute = ui->ComputeAreaLineEdit->text();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateIsLengthComputeInfos()
{
  ui->ComputeLengthLineEdit->setEnabled(ui->ComputeLengthCheckBox->isChecked());
  m_SourcesInfos[m_CurrentSrcIndex].IsLengthCompute = ui->ComputeLengthCheckBox->isChecked();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateLengthComputeAttrInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].LengthComputeAttribute = ui->ComputeLengthLineEdit->text();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateIsXCentroidComputeInfos()
{
  ui->ComputeXCentroidLineEdit->setEnabled(ui->ComputeXCentroidCheckBox->isChecked());
  m_SourcesInfos[m_CurrentSrcIndex].IsXCentroidCompute = ui->ComputeXCentroidCheckBox->isChecked();
}

// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateXCentroidComputeAttrInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].XCentroidComputeAttribute = ui->ComputeXCentroidLineEdit->text();
}

// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateIsYCentroidComputeInfos()
{
  ui->ComputeYCentroidLineEdit->setEnabled(ui->ComputeYCentroidCheckBox->isChecked());
  m_SourcesInfos[m_CurrentSrcIndex].IsYCentroidCompute = ui->ComputeYCentroidCheckBox->isChecked();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateYCentroidComputeAttrInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].YCentroidComputeAttribute = ui->ComputeYCentroidLineEdit->text();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateIsZCentroidComputeInfos()
{
  ui->ComputeZCentroidLineEdit->setEnabled(ui->ComputeZCentroidCheckBox->isChecked());
  m_SourcesInfos[m_CurrentSrcIndex].IsZCentroidCompute = ui->ComputeZCentroidCheckBox->isChecked();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateZCentroidComputeAttrInfos()
{
  m_SourcesInfos[m_CurrentSrcIndex].ZCentroidComputeAttribute = ui->ComputeZCentroidLineEdit->text();
}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(OGRGDALImportExtension)

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
#include "PrecheckWorker.hpp"
#include "ImportWorker.hpp"

#include <QMessageBox>
#include <QThread>


// =====================================================================
// =====================================================================


BEGIN_BUILDEREXT_SIGNATURE("import.spatial-graph.ogr-gdal", openfluid::builderext::TYPE_MODAL)

  DECLARE_CATEGORY(openfluid::builderext::CAT_SPATIAL)
  DECLARE_MENUTEXT("Spatial data import (OGR/GDAL)")

END_BUILDEREXT_SIGNATURE


// =====================================================================
// =====================================================================

/* TODO check why the following message appears on Mac OSX:
modalSession has been exited prematurely - check for a reentrant call to endModalSession
*/

OGRGDALImportExtension::OGRGDALImportExtension() :
  openfluid::builderext::PluggableModalExtension(), ui(new Ui::OGRGDALDialog),
  m_CurrentSrcIndex(-1), mp_PrecheckImportDlg(NULL)
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

  ui->PrecheckButton->setEnabled(false);

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

  connect(ui->SourcesTableWidget,SIGNAL(itemSelectionChanged()),this,SLOT(updateUI()));


  connect(ui->UnitsClassLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateUnitsClassInfos()));

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

  connect(ui->DatasetImportCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateIsDatasetImportInfos()));
  connect(ui->DatasetImportLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateIsDatasetImportInfos()));
  connect(ui->DatastoreIDCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateIsDatasetImportInfos()));
  connect(ui->DatastoreIDLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateIsDatasetImportInfos()));

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

  OPENFLUID_GetRunEnvironment("dir.temp",DirStr);
  m_TempDir = QString::fromStdString(DirStr);


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

  updateUI();
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
  WFSSourceAddDialog AddDlg(m_TempDir,this);

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

  if (mp_PrecheckImportDlg != NULL)
    delete mp_PrecheckImportDlg;

  mp_PrecheckImportDlg = new PrecheckImportDialog(OGRGDALEXT_PRECHECK_STEPS+m_SourcesInfos.size()+2,this);

  mp_PrecheckImportDlg->open();


  QThread* WThread = new QThread;
  ImportWorker* Worker = new ImportWorker(m_SourcesInfos,mp_AdvancedDesc,m_InputDir);
  Worker->moveToThread(WThread);

  connect(Worker, SIGNAL(stepEntered(QString)), mp_PrecheckImportDlg, SLOT(handleStepEntered(QString)));
  connect(Worker, SIGNAL(stepCompleted(int,QString)), mp_PrecheckImportDlg, SLOT(handleStepCompleted(int,QString)));
  connect(Worker, SIGNAL(completed(QString)), mp_PrecheckImportDlg, SLOT(handleCompleted(QString)));
  connect(Worker, SIGNAL(finished()), mp_PrecheckImportDlg, SLOT(handleFinished()));

  // for automatic closing on successful import
  connect(Worker, SIGNAL(closeRequired()), mp_PrecheckImportDlg, SLOT(handleCloseRequired()));
  connect(mp_PrecheckImportDlg, SIGNAL(closeRequired()), this, SLOT(handleCloseRequired()));

  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));

  WThread->start();
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


void OGRGDALImportExtension::updateUI()
{
  ui->PrecheckButton->setEnabled(!m_SourcesInfos.isEmpty());
  ui->ButtonBox->button(QDialogButtonBox::Apply)->setEnabled(!m_SourcesInfos.isEmpty());

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
  ui->PcsOrdComboBox->clear();
  ui->ToConnectComboBox->clear();
  ui->ChildofConnectComboBox->clear();
  ui->AttributesListWidget->clear();


  if (m_CurrentSrcIndex >=0)
  {
    ui->ConfigTabWidget->setEnabled(true);

    // Units class
    ui->UnitsClassLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].UnitsClass);


    // Units process order field
    ui->PcsOrdComboBox->addItem("");
    ui->PcsOrdComboBox->addItems(m_SourcesInfos[m_CurrentSrcIndex].AvailableFields);
    ui->PcsOrdComboBox->setCurrentIndex(ui->PcsOrdComboBox->findText(m_SourcesInfos[m_CurrentSrcIndex].UnitsPcsOrdField));


    // Units "to" connections field
    ui->ToConnectComboBox->addItem("");
    ui->ToConnectComboBox->addItems(m_SourcesInfos[m_CurrentSrcIndex].AvailableFields);
    ui->ToConnectComboBox->setCurrentIndex(ui->ToConnectComboBox->findText(m_SourcesInfos[m_CurrentSrcIndex].ToConnectionsField));


    // Units "childof" connections field
    ui->ChildofConnectComboBox->addItem("");
    ui->ChildofConnectComboBox->addItems(m_SourcesInfos[m_CurrentSrcIndex].AvailableFields);
    ui->ChildofConnectComboBox->setCurrentIndex(ui->ChildofConnectComboBox->findText(m_SourcesInfos[m_CurrentSrcIndex].ChildofConnectionsField));


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


    // Area attribute to compute, enabled for 2D+ geometries only
    if (m_SourcesInfos[m_CurrentSrcIndex].getGeometryDimension() >= 2)
    {
      ui->ComputeAreaCheckBox->setEnabled(true);
      ui->ComputeAreaCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsAreaCompute);
      ui->ComputeAreaLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].AreaComputeAttribute);
    }
    else
    {
      ui->ComputeAreaCheckBox->setEnabled(false);
      ui->ComputeAreaLineEdit->setEnabled(false);
      ui->ComputeAreaCheckBox->setChecked(false);
    }

    // Length attribute to compute, enabled for 1D geometries only
    if (m_SourcesInfos[m_CurrentSrcIndex].getGeometryDimension() == 1)
    {
      ui->ComputeLengthCheckBox->setEnabled(true);
      ui->ComputeLengthCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsLengthCompute);
      ui->ComputeLengthLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].LengthComputeAttribute);
    }
    else
    {
      ui->ComputeLengthCheckBox->setEnabled(false);
      ui->ComputeLengthLineEdit->setEnabled(false);
      ui->ComputeLengthCheckBox->setChecked(false);
    }

    // XCentroid attribute to compute
    ui->ComputeXCentroidCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsXCentroidCompute);
    ui->ComputeXCentroidLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].XCentroidComputeAttribute);

    // YCentroid attribute to compute
    ui->ComputeYCentroidCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsYCentroidCompute);
    ui->ComputeYCentroidLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].YCentroidComputeAttribute);

    // ZCentroid attribute to compute
    ui->ComputeZCentroidCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsZCentroidCompute);
    ui->ComputeZCentroidLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].ZCentroidComputeAttribute);


    // Import to files and datastore
    ui->DatasetImportLineEdit->setEnabled(true);
    ui->DatastoreIDCheckBox->setEnabled(true);
    ui->DatastoreIDLineEdit->setEnabled(true);

    if (m_SourcesInfos[m_CurrentSrcIndex].IsAlreadyInDataset)
    {
      ui->InDatasetLabel->setVisible(true);
      ui->DatasetImportCheckBox->setVisible(false);
      ui->DatasetImportLineEdit->setVisible(false);
    }
    else
    {
      ui->InDatasetLabel->setVisible(false);
      ui->DatasetImportCheckBox->setVisible(true);
      ui->DatasetImportLineEdit->setVisible(true);
    }

    ui->DatasetImportLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].RelativeDatasetPath);
    ui->DatastoreIDLineEdit->setText(m_SourcesInfos[m_CurrentSrcIndex].DatastoreID);
    ui->DatasetImportCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsDatasetImport);
    ui->DatastoreIDCheckBox->setChecked(m_SourcesInfos[m_CurrentSrcIndex].IsDatastore);

    ui->DatasetImportLineEdit->setEnabled(m_SourcesInfos[m_CurrentSrcIndex].IsDatasetImport);
    ui->DatastoreIDCheckBox->setEnabled(m_SourcesInfos[m_CurrentSrcIndex].IsDatasetImport ||
                                        m_SourcesInfos[m_CurrentSrcIndex].IsAlreadyInDataset);
    ui->DatastoreIDLineEdit->setEnabled((m_SourcesInfos[m_CurrentSrcIndex].IsDatasetImport ||
                                         m_SourcesInfos[m_CurrentSrcIndex].IsAlreadyInDataset) &&
                                        m_SourcesInfos[m_CurrentSrcIndex].IsDatastore);

  }
  else
  {
    ui->ConfigTabWidget->setEnabled(false);
  }
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::runPrecheck()
{
  if (mp_PrecheckImportDlg != NULL)
    delete mp_PrecheckImportDlg;

  mp_PrecheckImportDlg = new PrecheckImportDialog(OGRGDALEXT_PRECHECK_STEPS,this);

  mp_PrecheckImportDlg->open();


  QThread* WThread = new QThread;
  PrecheckWorker* Worker = new PrecheckWorker(m_SourcesInfos,mp_AdvancedDesc);
  Worker->moveToThread(WThread);


  connect(Worker, SIGNAL(stepEntered(QString)), mp_PrecheckImportDlg, SLOT(handleStepEntered(QString)));
  connect(Worker, SIGNAL(stepCompleted(int,QString)), mp_PrecheckImportDlg, SLOT(handleStepCompleted(int,QString)));
  connect(Worker, SIGNAL(completed(QString)), mp_PrecheckImportDlg, SLOT(handleCompleted(QString)));
  connect(Worker, SIGNAL(finished()), mp_PrecheckImportDlg, SLOT(handleFinished()));

  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));

  WThread->start();
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


void OGRGDALImportExtension::updateIsDatasetImportInfos()
{
  if (!(ui->DatasetImportCheckBox->isChecked() ||
      m_SourcesInfos[m_CurrentSrcIndex].IsAlreadyInDataset))
  {
    ui->DatastoreIDCheckBox->setChecked(false);
  }

  m_SourcesInfos[m_CurrentSrcIndex].IsDatasetImport = ui->DatasetImportCheckBox->isChecked();
  m_SourcesInfos[m_CurrentSrcIndex].RelativeDatasetPath = ui->DatasetImportLineEdit->text();

  m_SourcesInfos[m_CurrentSrcIndex].IsDatastore = ui->DatastoreIDCheckBox->isChecked();
  m_SourcesInfos[m_CurrentSrcIndex].DatastoreID = ui->DatastoreIDLineEdit->text();

  ui->DatasetImportLineEdit->setEnabled(ui->DatasetImportCheckBox->isChecked());
  ui->DatastoreIDCheckBox->setEnabled(m_SourcesInfos[m_CurrentSrcIndex].IsAlreadyInDataset ||
                                      ui->DatasetImportCheckBox->isChecked());
  ui->DatastoreIDLineEdit->setEnabled(ui->DatastoreIDCheckBox->isChecked());
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::handleCloseRequired()
{
  openfluid::builderext::FluidXUpdateFlags::Flags ChangesFlags =
      openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT |
      openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS;

  bool DatastoreChanged = false;

  for (int i=0;i<m_SourcesInfos.size();i++)
    DatastoreChanged = DatastoreChanged || m_SourcesInfos[i].IsDatastore;

  if (DatastoreChanged)
    ChangesFlags = ChangesFlags | openfluid::builderext::FluidXUpdateFlags::FLUIDX_DATASTORE;

  emit fluidxChanged(ChangesFlags);

  accept();
}


// =====================================================================
// =====================================================================



DEFINE_BUILDEREXT_CLASS(OGRGDALImportExtension)

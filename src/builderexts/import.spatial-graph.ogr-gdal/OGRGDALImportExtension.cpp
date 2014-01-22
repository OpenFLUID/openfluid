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
  openfluid::builderext::PluggableModalExtension(), ui(new Ui::OGRGDALDialog)
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

  connect(ui->CheckButton,SIGNAL(clicked()),this,SLOT(runConsistencyCheck()));

  connect(ui->SourcesTableWidget,SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),
          this,SLOT(updateConfigTabWidget()));

  ui->ButtonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
  connect(ui->ButtonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(processButtonBoxClicked(QAbstractButton*)));
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
                              "OpenFLUID-Builder",
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
  reject();
}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::updateConfigTabWidget()
{

}


// =====================================================================
// =====================================================================


void OGRGDALImportExtension::runConsistencyCheck()
{
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


DEFINE_BUILDEREXT_CLASS(OGRGDALImportExtension)

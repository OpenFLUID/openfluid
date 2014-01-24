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
  \file FileSourceAddDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_SourceAddDialog.h"
#include "FileSourceAddDialog.hpp"


#include <QFileDialog>
#include <QMessageBox>

#include "ogrsf_frmts.h"

#include <iostream>


FileSourceAddDialog::FileSourceAddDialog(const QString& InputDir, QWidget* Parent):
  SourceAddDialog(Parent), m_InputDir(InputDir)
{
  setWindowTitle("Spatial data import (OGR/GDAL) - Add file source");

  ui->URLConnectWidget->setVisible(false);

  ui->FileLineEdit->setText(m_CurrentSourceURI);

  connect(ui->BrowseButton,SIGNAL(clicked()),this,SLOT(selectFile()));
}


// =====================================================================
// =====================================================================


FileSourceAddDialog::~FileSourceAddDialog()
{

}


// =====================================================================
// =====================================================================


void FileSourceAddDialog::selectFile()
{
  m_CurrentSourceURI = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"),
                                                    m_InputDir,
                                                    tr("All vector files (*.shp *.mif);;ESRI Shapefiles (*.shp);;Mapinfo (*.mif)"));

  if (!m_CurrentSourceURI.isEmpty())
  {
    ui->LayersTableWidget->setRowCount(0);
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    openDataSource();

    if (mp_DataSource == NULL)
    {
      ui->LayersTableWidget->selectRow(0);

      QMessageBox::critical(this,tr("File error"),tr("Error opening file\n")+m_CurrentSourceURI+tr("\n\nAborting."));
    }
  }

  ui->FileLineEdit->setText(m_CurrentSourceURI);
}


// =====================================================================
// =====================================================================


void FileSourceAddDialog::prepareToImport()
{
  std::cout << __PRETTY_FUNCTION__ << ", line " << __LINE__ << std::endl;

  m_SrcInfos.SourceURI = m_CurrentSourceURI;
}


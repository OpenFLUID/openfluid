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
  \file WFSSourceAddDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_SourceAddDialog.h"
#include "WFSSourceAddDialog.hpp"

#include <QPushButton>
#include <QDir>
#include <QFile>


WFSSourceAddDialog::WFSSourceAddDialog(const QString& TempDir, QWidget* Parent):
  SourceAddDialog(Parent), m_TempDir(TempDir)
{
  setWindowTitle("Spatial data import (OGR/GDAL) - Add WFS source");

  ui->FileSelectWidget->setVisible(false);

  connect(ui->ConnectButton,SIGNAL(clicked()),this,SLOT(connectToWFS()));
}


// =====================================================================
// =====================================================================


WFSSourceAddDialog::~WFSSourceAddDialog()
{

}


// =====================================================================
// =====================================================================


void WFSSourceAddDialog::connectToWFS()
{
  ui->URLLineEdit->setText(ui->URLLineEdit->text().trimmed());

  if (!ui->URLLineEdit->text().isEmpty())
  {
    m_CurrentSourceURI = "WFS:"+ui->URLLineEdit->text();
    openDataSource();
  }
}


// =====================================================================
// =====================================================================


void WFSSourceAddDialog::updateAfterOpen()
{

}


// =====================================================================
// =====================================================================


bool WFSSourceAddDialog::prepareToImport()
{

  OGRSFDriver *CopyDriver;

  // determine the exact URI for the selected layer to import
  QString LayerURI = m_CurrentSourceURI.section("?",0,0) +
                     "?request=GetFeature&typeName=" +
                     QString(mp_DataSource->GetLayer(ui->LayersTableWidget->currentRow())->GetName());


  // prepare temp directory for cached layer file
  QString TempCacheDir = m_TempDir+"/import.spatial-graph.ogr-gdal";
  if (!QDir().mkpath(TempCacheDir)) return false;


  OGRRegisterAll();

  OGRDataSource* LayerDS = OGRSFDriverRegistrar::Open(LayerURI.toStdString().c_str(), FALSE );

  if (LayerDS)
  {
    m_SrcInfos.SourceURI = m_CurrentSourceURI;
    QString CachedURI;

    // Determine the file name for local layer cache
    unsigned int DirNameIncr = 0;
    do
    {
      DirNameIncr++;
      CachedURI = QString(TempCacheDir+"/layer_cache_%1.geojson").arg(DirNameIncr);
    }
    while (QFile::exists(CachedURI));


    // copy layer in cache using geoJSON format
    CopyDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("GeoJSON");

    OGRDataSource *CopiedDS;
    CopiedDS = CopyDriver->CopyDataSource(LayerDS,CachedURI.toStdString().c_str());
    OGRDataSource::DestroyDataSource(LayerDS);

    if (CopiedDS)
    {
      OGRDataSource::DestroyDataSource(CopiedDS);
      m_SrcInfos.CachedSourceURI = CachedURI;
    }
  }

  return !m_SrcInfos.CachedSourceURI.isEmpty();
}


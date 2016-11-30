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
  @file WFSSourceAddDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <QPushButton>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include <openfluid/utils/GDALCompatibility.hpp>

#include "ui_SourceAddDialog.h"
#include "WFSSourceAddDialog.hpp"


WFSSourceAddDialog::WFSSourceAddDialog(const QString& TempDir, QWidget* Parent):
  SourceAddDialog(Parent), m_TempDir(TempDir)
{
  setWindowTitle("Spatial data import (OGR/GDAL) - Add WFS source");

  ui->FileSelectWidget->setVisible(false);
  ui->AuthWidget->setVisible(true);
  ui->AuthCheckBox->setChecked(false);
  ui->AuthWidget->setEnabled(false);

  connect(ui->AuthCheckBox,SIGNAL(toggled(bool)),ui->AuthWidget,SLOT(setEnabled(bool)));

  connect(ui->ConnectButton,SIGNAL(clicked()),this,SLOT(connectToWFS()));
}


// =====================================================================
// =====================================================================


WFSSourceAddDialog::~WFSSourceAddDialog()
{

}


// =====================================================================
// =====================================================================


void WFSSourceAddDialog::generateXMLConfigFile(const QString& URL)
{

  QString ConfigFilePath = getXMLConfigFilePath();

  if (!QDir().mkpath(QFileInfo(ConfigFilePath).absolutePath()))
    return;

  QFile ConfigFile(ConfigFilePath);
  if (!ConfigFile.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream Out(&ConfigFile);

  Out << "<OGRWFSDataSource>\n";
  Out << "  <URL>" << URL << "</URL>\n";

  if (!m_CurrentUsername.isEmpty())
    Out << "  <UserPwd>" << m_CurrentUsername << ":" << m_CurrentPassword << "</UserPwd>\n";
  Out << "</OGRWFSDataSource>\n";

}


// =====================================================================
// =====================================================================


QString WFSSourceAddDialog::getXMLConfigFilePath()
{
  return m_TempDir+"/import.spatial-graph.ogr-gdal/wfsconfig.xml";
}


// =====================================================================
// =====================================================================


void WFSSourceAddDialog::connectToWFS()
{
  ui->URLLineEdit->setText(ui->URLLineEdit->text().trimmed());

  if (!ui->URLLineEdit->text().isEmpty())
  {
    m_CurrentUsername = "";
    m_CurrentPassword = "";
    m_CurrentSourceURI = getXMLConfigFilePath();
    m_RealSourceURI = ui->URLLineEdit->text();
    m_CurrentSourceDisplay = m_RealSourceURI;

    if (ui->AuthCheckBox->isChecked() && !ui->UsernameLineEdit->text().isEmpty())
    {
      m_CurrentUsername = ui->UsernameLineEdit->text();
      m_CurrentPassword = ui->PasswordLineEdit->text();
    }

    generateXMLConfigFile(m_RealSourceURI);

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
  QString ConfigFilePath = getXMLConfigFilePath();
  GDALDriver_COMPAT* CopyDriver;

  // determine the exact URI for the selected layer to import
  QString LayerURI = m_RealSourceURI.section("?",0,0) +
                     "?request=GetFeature&amp;typeName=" +
                     QString(mp_DataSource->GetLayer(ui->LayersTableWidget->currentRow())->GetName());


  // prepare temp directory for cached layer file
  QString TempCacheDir = m_TempDir+"/import.spatial-graph.ogr-gdal";
  if (!QDir().mkpath(TempCacheDir))
    return false;

  generateXMLConfigFile(LayerURI);

  GDALAllRegister_COMPAT();

  GDALDataset_COMPAT* LayerDS = GDALOpenRO_COMPAT(ConfigFilePath.toStdString().c_str());

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
    CopyDriver = GDALGetDriverByName_COMPAT("GeoJSON");

    GDALDataset_COMPAT *CopiedDS;
    CopiedDS = GDALCopy_COMPAT(CopyDriver,LayerDS,CachedURI.toStdString().c_str());
    GDALClose_COMPAT(LayerDS);

    if (CopiedDS)
    {
      GDALClose_COMPAT(CopiedDS);
      m_SrcInfos.CachedSourceURI = CachedURI;
    }
  }

  m_SrcInfos.SourceDisplay = m_CurrentSourceDisplay;

  return !m_SrcInfos.CachedSourceURI.isEmpty();
}


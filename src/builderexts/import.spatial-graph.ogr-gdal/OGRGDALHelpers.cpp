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
  \file OGRGDALHelpers.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "OGRGDALHelpers.hpp"

#include <QStringList>


const QMap<QString,QString> OGRGDALHelpers::m_FileExtsDriversMap =
    OGRGDALHelpers::initOGRGDALDriversMap();


// =====================================================================
// =====================================================================


QMap<QString,QString> OGRGDALHelpers::initOGRGDALDriversMap()
{
  QMap<QString,QString> Map;

  Map.insert("shp","ESRI Shapefile");
  Map.insert("geojson","GeoJSON");

  return Map;
}


// =====================================================================
// =====================================================================


QString OGRGDALHelpers::getDriverFromFileExt(const QString& FileExt)
{
  if (isFileExt(FileExt))
  {
    return m_FileExtsDriversMap[FileExt];
  }

  return "";
}


// =====================================================================
// =====================================================================



bool OGRGDALHelpers::isFileExt(const QString& FileExt)
{
  return m_FileExtsDriversMap.contains(FileExt);
}


// =====================================================================
// =====================================================================


bool OGRGDALHelpers::convertFieldToAttribute(const OGRFeature* Feature,
                                             const int FieldIndex,
                                             QString& Attr)
{
  if (Feature == NULL) return false;
  if (FieldIndex < 0) return false;

  const OGRFieldDefn* FieldDef = const_cast<OGRFeature*>(Feature)->GetFieldDefnRef(FieldIndex);

  if (FieldDef == NULL) return false;

  const OGRFieldType FieldType = const_cast<OGRFieldDefn*>(FieldDef)->GetType();

  if (FieldType == OFTInteger)
  {
    Attr = QString::number(const_cast<OGRFeature*>(Feature)->GetFieldAsInteger(FieldIndex));
    return true;
  }
  else if (FieldType == OFTReal)
  {
    Attr = QString::number(const_cast<OGRFeature*>(Feature)->GetFieldAsDouble(FieldIndex));
    return true;
  }
  else if (FieldType == OFTString)
  {
    Attr = QString(const_cast<OGRFeature*>(Feature)->GetFieldAsString(FieldIndex));
    return true;
  }
  else
    return false;
}


// =====================================================================
// =====================================================================


bool OGRGDALHelpers::convertConnectionsStringToList(const QString& ConnStr,
                                                    QList<SourceConnection>& ConnList)
{
  ConnList.clear();

  QStringList UnitsList = ConnStr.split(";",QString::SkipEmptyParts);

  if (!UnitsList.isEmpty())
  {
    for (int i=0; i< UnitsList.size(); i++)
    {
      QStringList ClassIDPair = UnitsList[i].split("#",QString::SkipEmptyParts);

      if (ClassIDPair.size() != 2) return false;

      ClassIDPair[0] = ClassIDPair[0].trimmed();
      ClassIDPair[1] = ClassIDPair[1].trimmed();

      bool OK = false;
      int ID = ClassIDPair[1].toInt(&OK);

      if (!OK) return false;

      ConnList.append(SourceConnection(ClassIDPair[0],ID));
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


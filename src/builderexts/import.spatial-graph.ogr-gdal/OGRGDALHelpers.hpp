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
  @file OGRGDALHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OGRGDALHELPERS_HPP__
#define __OGRGDALHELPERS_HPP__


#include <QMap>
#include <QString>

#include <ogrsf_frmts.h>

#include "SourceData.hpp"


class OGRGDALHelpers
{

  private:

    static const QMap<QString,QString> m_FileExtsDriversMap;

    static QMap<QString,QString> initOGRGDALDriversMap();


  public:

    static QString getDriverFromFileExt(const QString& FileExt);

    static bool isFileExt(const QString& FileExt);

    static bool convertFieldToAttribute(const OGRFeature* Feature, const int FieldIndex,
                                        QString& Attr);

    static bool convertConnectionsStringToList(const QString& ConnStr,
                                               QList<SourceConnection>& ConnList);

};


#endif /* __OGRGDALHELPERS_HPP__ */


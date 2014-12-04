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
  \file QtHelpers.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __QTHELPERS_HPP__
#define __QTHELPERS_HPP__


#include <openfluid/tools/GDALHelpers.hpp>
#include <openfluid/core/DateTime.hpp>

#include <string>
#include <list>
#include <vector>
#include <set>

#include <QVariant>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QColor>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {

/**
  Transforms the given string to a string compatible with the INI files managed with QSettings.
  @param[in] Str the string to transform
  @return The transformed string
*/
QString DLLEXPORT toIniCompatible(const std::string& Str);


/**
  Transforms the given variable from an INI file managed with QSettings to a string
  @param[in] Var the variable to transform
  @return The variable transformed into a string
*/
std::string DLLEXPORT fromIniCompatible(const QVariant& Var);


/**
  Transforms a QStringList into a std::list of std::string
  @param[in] StrList the QStringList to transform
  @return The QStringList transformed into a std::list of std::string
*/
std::list<std::string> DLLEXPORT toStdStringList(const QStringList& StrList);


/**
  Transforms an OpenFLUID DateTime to a QDateTime
  @param[in] DT The OpenFLUID DateTime to transform
  @return The DT transformed into QDateTime
*/
QDateTime DLLEXPORT toQDateTime(openfluid::core::DateTime DT);


/**
  Transforms a std::vector of std::string into a QStringList
  @param[in] StrVect the std::vector of std::string to transform
  @return The StrVect transformed into a QStringList
*/
QStringList DLLEXPORT toQStringList(const std::vector<std::string>& StrVect);


/**
  Transforms a std::set of std::string into a QStringList
  @param[in] StrSet the std::set of std::string to transform
  @return The StrSet transformed into a QStringList
*/
QStringList DLLEXPORT toQStringList(const std::set<std::string>& StrSet);


/**
  Transforms a std::list of std::string into a QStringList
  @param[in] StrList the std::list of std::string to transform
  @return The StrList transformed into a QStringList
*/
QStringList DLLEXPORT toQStringList(const std::list<std::string>& StrList);


/**
  Transforms a std::set of int into a QStringList
  @param[in] IntSet the std::set of int to transform
  @return The intSet transformed into a QStringList
*/
QStringList DLLEXPORT toQStringList(const std::set<int>& IntSet);


/**
  Returns the available GDAL/OGR formats as a QFileDialog formats string.
  The string is built according to GDAL/OGR available drivers.
  The OGRRegisterAll() and/or GDALAllRegister() instruction must be called before using this function.
  @param[in] Drivers The available extensions list
  @param[in] AllFormatsLabel the label displayed for all formats
  @return the format string (e.g. "All vector files (*.shp *.mif *.tab);;ESRI Shapefiles (*.shp);;Mapinfo (*.mif *.tab)")
*/
QString DLLEXPORT getOGRGDALFormatsForQFileDialogs(const GDALDriversFilesExts_t& Drivers,
                                                   const QString& AllFormatsLabel);

/**
  Escapes characters in the given string to be compatible with the XML format.
  @param[in] Str the string to escape
  @return The escaped string
*/
QString DLLEXPORT escapeXMLEntities(const QString& Str);


} } // namespaces


#endif /* __QTHELPERS_HPP__ */

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
  @file QtHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_TOOLS_QTHELPERS_HPP__
#define __OPENFLUID_TOOLS_QTHELPERS_HPP__


#include <string>
#include <list>
#include <vector>
#include <set>

#include <QVariant>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QColor>

#include <openfluid/core/DateTime.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


/**
  Transforms the given string to a string compatible with the INI files managed with QSettings.
  @param[in] Str the string to transform
  @return The transformed string
*/
QString OPENFLUID_API toIniCompatible(const std::string& Str);


/**
  Transforms the given variable from an INI file managed with QSettings to a string
  @param[in] Var the variable to transform
  @return The variable transformed into a string
*/
std::string OPENFLUID_API fromIniCompatible(const QVariant& Var);


/**
  Transforms a QStringList into a std::list of std::string
  @param[in] StrList the QStringList to transform
  @return The QStringList transformed into a std::list of std::string
*/
std::list<std::string> OPENFLUID_API toStdStringList(const QStringList& StrList);


/**
  Transforms a QStringList into a std::vector of std::string
  @param[in] StrList the QStringList to transform
  @return The QStringList transformed into a std::vector of std::string
*/
std::vector<std::string> toStdStringVector(const QStringList& StrList);


/**
  Transforms an OpenFLUID DateTime to a QDateTime
  @param[in] DT The OpenFLUID DateTime to transform
  @return The DT transformed into QDateTime
*/
QDateTime OPENFLUID_API toQDateTime(const openfluid::core::DateTime& DT);


/**
  Transforms a std::vector of std::string into a QStringList
  @param[in] StrVect the std::vector of std::string to transform
  @return The StrVect transformed into a QStringList
*/
QStringList OPENFLUID_API toQStringList(const std::vector<std::string>& StrVect);


/**
  Transforms a std::set of std::string into a QStringList
  @param[in] StrSet the std::set of std::string to transform
  @return The StrSet transformed into a QStringList
*/
QStringList OPENFLUID_API toQStringList(const std::set<std::string>& StrSet);


/**
  Transforms a std::list of std::string into a QStringList
  @param[in] StrList the std::list of std::string to transform
  @return The StrList transformed into a QStringList
*/
QStringList OPENFLUID_API toQStringList(const std::list<std::string>& StrList);


/**
  Transforms a std::set of int into a QStringList
  @param[in] IntSet the std::set of int to transform
  @return The intSet transformed into a QStringList
*/
QStringList OPENFLUID_API toQStringList(const std::set<int>& IntSet);


/**
  Escapes characters in the given string to be compatible with the XML format.
  @param[in] Str the string to escape
  @return The escaped string
*/
QString OPENFLUID_API escapeXMLEntities(const QString& Str);


/**
  Decodes XML entities to characters
  @param[in] Str the string to decode
  @return The decoded string
*/
QString OPENFLUID_API decodeXMLEntities(const QString& Str);


} } // namespaces


#endif /* __OPENFLUID_TOOLS_QTHELPERS_HPP__ */

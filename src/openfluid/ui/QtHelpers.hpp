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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UI_QTHELPERS_HPP__
#define __OPENFLUID_UI_QTHELPERS_HPP__


#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <regex>

#include <QVariant>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QPoint>
#include <QSize>

#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/core/DateTime.hpp>


namespace openfluid { namespace ui {


/**
  Transforms the given string to a string compatible with the INI files managed with QSettings.
  @param[in] Str the string to transform
  @return The transformed string
*/
inline QString toIniCompatible(const std::string& Str)
{
  return (QString::fromStdString(Str));
}


// =====================================================================
// =====================================================================


/**
  Transforms the given variable from an INI file managed with QSettings to a string
  @param[in] Var the variable to transform
  @return The variable transformed into a string
*/
inline std::string fromIniCompatible(const QVariant& Var)
{
  if (Var.type() == QVariant::StringList)
  {
    return Var.toStringList().join(", ").toStdString();
  }
  else
  {
    return Var.toString().toStdString();
  }
}


// =====================================================================
// =====================================================================


/**
  Transforms a QStringList into a std::list of std::string
  @param[in] StrList the QStringList to transform
  @return The QStringList transformed into a std::list of std::string
*/
inline std::list<std::string> toStdStringList(const QStringList& StrList)
{
  std::list<std::string> TmpList;

  for (const auto& QStr : StrList)
  {
    TmpList.push_back(QStr.toStdString());
  }

  return TmpList;
}


// =====================================================================
// =====================================================================


/**
  Transforms a QStringList into a std::vector of std::string
  @param[in] StrList the QStringList to transform
  @return The QStringList transformed into a std::vector of std::string
*/
inline std::vector<std::string> toStdStringVector(const QStringList& StrList)
{
  std::vector<std::string> TmpVector;

  for (const auto& QStr : StrList)
  {
    TmpVector.push_back(QStr.toStdString());
  }

  return TmpVector;
}


// =====================================================================
// =====================================================================


/**
  Transforms a std::map of QString,QString into a std::map of std::string,std::string
  @param[in] StrMap the map to transform
  @return The std::map of QString,QString>transformed into a std::map of std::string,std::string
*/
inline std::map<std::string,std::string> toStdStringMap(const std::map<QString,QString>& StrMap)
{
  std::map<std::string,std::string> TmpMap;

  for (const auto& QStrStr : StrMap)
  {
    TmpMap[QStrStr.first.toStdString()] = QStrStr.second.toStdString();
  }

  return TmpMap;
}


// =====================================================================
// =====================================================================


/**
  Transforms an OpenFLUID DateTime to a QDateTime
  @param[in] DT The OpenFLUID DateTime to transform
  @return The DT transformed into QDateTime
*/
inline QDateTime toQDateTime(const openfluid::core::DateTime& DT)
{
  QDate D(DT.getYear(),DT.getMonth(),DT.getDay());
  QTime T(DT.getHour(),DT.getMinute(),DT.getSecond());

  return QDateTime(D,T,Qt::UTC);
}


// =====================================================================
// =====================================================================


/**
  Transforms a std::vector of std::string into a QStringList
  @param[in] StrVect the std::vector of std::string to transform
  @return The StrVect transformed into a QStringList
*/
inline QStringList toQStringList(const std::vector<std::string>& StrVect)
{
  QStringList QSL;

  for (const auto& Str : StrVect)
  {
    QSL.append(QString::fromStdString(Str));
  }

  return QSL;
}


// =====================================================================
// =====================================================================


/**
  Transforms a std::set of std::string into a QStringList
  @param[in] StrSet the std::set of std::string to transform
  @return The StrSet transformed into a QStringList
*/
inline QStringList toQStringList(const std::set<std::string>& StrSet)
{
  QStringList QSL;

  for (const auto& Str : StrSet)
  {
    QSL.append(QString::fromStdString(Str));
  }

  return QSL;
}


// =====================================================================
// =====================================================================


/**
  Transforms a std::list of std::string into a QStringList
  @param[in] StrList the std::list of std::string to transform
  @return The StrList transformed into a QStringList
*/
inline QStringList toQStringList(const std::list<std::string>& StrList)
{
  QStringList QSL;

  for (const auto& Str : StrList)
  {
    QSL.append(QString::fromStdString(Str));
  }

  return QSL;
}


// =====================================================================
// =====================================================================


/**
  Transforms a std::set of int into a QStringList
  @param[in] IntSet the std::set of int to transform
  @return The intSet transformed into a QStringList
*/
inline QStringList toQStringList(const std::set<int>& IntSet)
{
  QStringList QSL;

  for (const auto& Int : IntSet)
  {
    QSL.append(QString("%1").arg(Int));
  }

  return QSL;
}


// =====================================================================
// =====================================================================


/**
  Converts arguments string to list. It first splits the string using spaces that are not inclosed in quotes,
  then remove enclosing quotes for each argument if any.
  @param[in] ArgsStr the argument string to convert
  @return The list of arguments
*/
inline QStringList convertArgsStringToList(const QString& ArgsStr)
{
  // converting string args as a list, without splitting spaces in quotes
  QStringList ArgsList = ArgsStr.split(QRegExp("\\s(?=(?:\"[^\"]*\"|[^\"])*$)"),QString::SkipEmptyParts);
  
  // removing of quotes
  for (auto& Arg : ArgsList)
  {
    if (Arg.startsWith("\"") && Arg.endsWith("\""))
    {
      Arg.remove(0,1);
      Arg.remove(Arg.size()-1,1);
    }
  }

  return ArgsList;
}


// =====================================================================
// =====================================================================


/**
  Escapes characters in the given string to be compatible with the XML format.
  @param[in] Str the string to escape
  @return The escaped string
*/
inline QString escapeXMLEntities(const QString& Str)
{
  QString EscapedStr = Str;
  EscapedStr.replace("&","&amp;").replace(">","&gt;").replace("<","&lt;").replace("\"","&quot;").replace("'","&apos;");
  return EscapedStr;
}


// =====================================================================
// =====================================================================


/**
  Decodes XML entities to characters
  @param[in] Str the string to decode
  @return The decoded string
*/
inline QString decodeXMLEntities(const QString& Str)
{
  QString DecodedStr = Str;
  DecodedStr.replace("&amp;","&").replace("&gt;",">").replace("&lt;","<").replace("&quot;","\"");
  return DecodedStr;
}


// =====================================================================
// =====================================================================


/**
  Converts a \@Point formatted string to a QPoint
  @param[in] Str the \@Point formatted string (e.g. "\@Point(-127 53)")
  @return the QPoint object
*/
inline QPoint toQPoint(const std::string& Str)
{
  auto Coords = openfluid::tools::fromGeometryString(Str,"Point");
  return QPoint(Coords.first,Coords.second);
}


// =====================================================================
// =====================================================================


/**
  Converts a QPoint to a \@Point formatted string
  @param[in] Point the QPoint object to convert 
  @return the \@Point formatted string (e.g. "\@Point(-127 53)")
*/
inline std::string fromQPoint(const QPoint& Point)
{
  if (!Point.isNull())
  {
    return openfluid::tools::toGeometryString("Point",Point.x(),Point.y());
  }

  return "";
}


// =====================================================================
// =====================================================================


/**
  Converts a \@Size formatted string to a QSize
  @param[in] Str the \@Size formatted string (e.g. "\@Size(1920 1080)")
  @return the QSize object
*/
inline QSize toQSize(const std::string& Str)
{
  auto Size = openfluid::tools::fromGeometryString(Str,"Size");
  return QSize(Size.first,Size.second);
}


// =====================================================================
// =====================================================================


/**
  Converts a QSize to a \@Size formatted string
  @param[in] Size the Size object to convert 
  @return the \@Size formatted string (e.g. "\@Size(1920 1080)")
*/
inline std::string fromQSize(const QSize& Size)
{
  if (!Size.isNull())
  {
    return openfluid::tools::toGeometryString("Size",Size.width(),Size.height());
  }

  return "";
}

} } // namespaces


#endif /* __OPENFLUID_UI_QTHELPERS_HPP__ */

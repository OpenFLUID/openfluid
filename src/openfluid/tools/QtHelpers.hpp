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


#ifndef __OPENFLUID_TOOLS_QTHELPERS_HPP__
#define __OPENFLUID_TOOLS_QTHELPERS_HPP__


#include <string>
#include <list>
#include <vector>
#include <set>
#include <regex>

#include <QVariant>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QPoint>

#include <openfluid/core/DateTime.hpp>


namespace openfluid { namespace tools {


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

  for (int i=0; i<StrList.size(); i++)
  {
    TmpList.push_back(StrList[i].toStdString());
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

  for (int i=0; i<StrList.size(); i++)
  {
    TmpVector.push_back(StrList[i].toStdString());
  }

  return TmpVector;
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
  for (unsigned int i=0; i<StrVect.size();++i)
  {
    QSL.append(QString(StrVect[i].c_str()));
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

  std::set<std::string>::const_iterator it;
  std::set<std::string>::const_iterator itb = StrSet.begin();
  std::set<std::string>::const_iterator ite = StrSet.end();

  for (it=itb;it!= ite;++it)
  {
    QSL.append(QString((*it).c_str()));
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

  std::list<std::string>::const_iterator it;
  std::list<std::string>::const_iterator itb = StrList.begin();
  std::list<std::string>::const_iterator ite = StrList.end();

  for (it=itb;it!= ite;++it)
  {
    QSL.append(QString((*it).c_str()));
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

  std::set<int>::const_iterator it;
  std::set<int>::const_iterator itb = IntSet.begin();
  std::set<int>::const_iterator ite = IntSet.end();

  for (it=itb;it!= ite;++it)
  {
    QSL.append(QString("%1").arg(*it));
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
  QPoint Point;
  std::regex PointRegex("@Point\\(([\\-\\d]+) ([\\-\\d]+)\\)");
  std::smatch Match;

  if (std::regex_match(Str, Match, PointRegex))
  {
    if (Match.size() == 3)
    {
      Point.setX(std::stoi(Match.str(1)));
      Point.setY(std::stoi(Match.str(2)));
    }
  }

  return Point;
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
    return "@Point("+std::to_string(Point.x())+" "+std::to_string(Point.y())+")";
  }

  return "";
}


} } // namespaces


#endif /* __OPENFLUID_TOOLS_QTHELPERS_HPP__ */

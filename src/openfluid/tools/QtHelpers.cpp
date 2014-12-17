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
  \file QtHelpers.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/tools/QtHelpers.hpp>


namespace openfluid { namespace tools {



QString toIniCompatible(const std::string& Str)
{
  return (QString::fromStdString(Str));
}


// =====================================================================
// =====================================================================


std::string fromIniCompatible(const QVariant& Var)
{
  if (Var.type() == QVariant::StringList)
    return Var.toStringList().join(", ").toStdString();
  else return Var.toString().toStdString();
}


// =====================================================================
// =====================================================================


std::list<std::string> toStdStringList(const QStringList& StrList)
{
  std::list<std::string> TmpList;

  for (int i=0; i<StrList.size(); i++)
    TmpList.push_back(StrList[i].toStdString());

  return TmpList;
}


// =====================================================================
// =====================================================================


QDateTime toQDateTime(openfluid::core::DateTime DT)
{
  QDate D(DT.getYear(),DT.getMonth(),DT.getDay());
  QTime T(DT.getHour(),DT.getMinute(),DT.getSecond());

  return QDateTime(D,T,Qt::UTC);
}


// =====================================================================
// =====================================================================


QStringList toQStringList(const std::vector<std::string>& StrVect)
{
  QStringList QSL;
  for (unsigned int i=0; i<StrVect.size();++i)
    QSL.append(QString(StrVect[i].c_str()));

  return QSL;
}


// =====================================================================
// =====================================================================


QStringList toQStringList(const std::set<std::string>& StrSet)
{
  QStringList QSL;

  std::set<std::string>::const_iterator it;
  std::set<std::string>::const_iterator itb = StrSet.begin();
  std::set<std::string>::const_iterator ite = StrSet.end();

  for (it=itb;it!= ite;++it)
    QSL.append(QString((*it).c_str()));

  return QSL;
}


// =====================================================================
// =====================================================================


QStringList toQStringList(const std::list<std::string>& StrList)
{
  QStringList QSL;

  std::list<std::string>::const_iterator it;
  std::list<std::string>::const_iterator itb = StrList.begin();
  std::list<std::string>::const_iterator ite = StrList.end();

  for (it=itb;it!= ite;++it)
    QSL.append(QString((*it).c_str()));

  return QSL;
}


// =====================================================================
// =====================================================================


QStringList toQStringList(const std::set<int>& IntSet)
{
  QStringList QSL;

  std::set<int>::const_iterator it;
  std::set<int>::const_iterator itb = IntSet.begin();
  std::set<int>::const_iterator ite = IntSet.end();

  for (it=itb;it!= ite;++it)
    QSL.append(QString("%1").arg(*it));

  return QSL;
}


// =====================================================================
// =====================================================================


QString getOGRGDALFormatsForQFileDialogs(const GDALDriversFilesExts_t& Drivers,
                                         const QString& AllFormatsLabel)
{
  QStringList AllFormats;
  QStringList DetailedFormats;

  GDALDriversFilesExts_t::const_iterator it;
  GDALDriversFilesExts_t::const_iterator bit = Drivers.begin();
  GDALDriversFilesExts_t::const_iterator eit = Drivers.end();

  for (it=bit; it!= eit; ++it)
  {
    QString CurrentFormatsStr;

    for (unsigned int i =0; i< (*it).second.FilesExts.size(); i++)
    {
      if (i!=0) CurrentFormatsStr += " ";
      CurrentFormatsStr += "*." + QString::fromStdString((*it).second.FilesExts.at(i));
    }

    DetailedFormats.append(QString::fromStdString((*it).second.Label) + "(" + CurrentFormatsStr + ")");
    AllFormats.append(CurrentFormatsStr);
  }

  if (!AllFormats.isEmpty())
  {
    QString AllFormatsStr;

    AllFormatsStr = AllFormatsLabel +" (" + AllFormats.join(" ")+")";
    DetailedFormats.prepend(AllFormatsStr);
  }

  return DetailedFormats.join(";;");
}


// =====================================================================
// =====================================================================


QString escapeXMLEntities(const QString& Str)
{
  QString EscapedStr = Str;
  EscapedStr.replace("&","&amp;").replace(">","&gt;").replace("<","&lt;").replace("\"","&quot;");
  return EscapedStr;
}




} } // namespaces

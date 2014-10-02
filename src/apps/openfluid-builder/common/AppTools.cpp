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
  \file AppTools.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/base/ProjectManager.hpp>

#include "AppTools.hpp"
#include <QDir>


QDateTime convertToQDateTime(openfluid::core::DateTime DT)
{
  QDate D(DT.getYear(),DT.getMonth(),DT.getDay());
  QTime T(DT.getHour(),DT.getMinute(),DT.getSecond());

  return QDateTime(D,T,Qt::UTC);
}


// =====================================================================
// =====================================================================


QStringList StringVectorToQStringList(const std::vector<std::string>& StrVect)
{
  QStringList QSL;
  for (unsigned int i=0; i<StrVect.size();++i)
    QSL.append(QString(StrVect[i].c_str()));

  return QSL;
}


// =====================================================================
// =====================================================================


QStringList StringSetToQStringList(const std::set<std::string>& StrSet)
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


QStringList StringListToQStringList(const std::list<std::string>& StrSet)
{
  QStringList QSL;

  std::list<std::string>::const_iterator it;
  std::list<std::string>::const_iterator itb = StrSet.begin();
  std::list<std::string>::const_iterator ite = StrSet.end();

  for (it=itb;it!= ite;++it)
    QSL.append(QString((*it).c_str()));

  return QSL;
}


// =====================================================================
// =====================================================================


QStringList IntSetToQStringList(const std::set<int>& IntSet)
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


QString getProjectInfosAsHTML(const QString& ProjectPath, bool IncludeFullPath)
{
  QString InfosStr;
  std::string Name, Description, Authors, CreationDate, LastModDate;

  if (openfluid::base::ProjectManager::getProjectInfos(ProjectPath.toStdString(),
                                                       Name, Description, Authors, CreationDate, LastModDate))
  {
    openfluid::core::DateTime TmpDate;
    TmpDate.setFromString(CreationDate,"%Y%m%dT%H%M%S");
    CreationDate = TmpDate.getAsString("%Y-%m-%d, %H:%M:%S");
    TmpDate.setFromString(LastModDate,"%Y%m%dT%H%M%S");
    LastModDate = TmpDate.getAsString("%Y-%m-%d, %H:%M:%S");

    InfosStr += "<table><tr><td valign='middle' width='74px' style='padding: 5px;'><IMG STYLE='vertical-align:middle;' SRC=':/icons/openfluid_icon.png' /></td>"
                "<td valign='middle' style='padding: 5px;'><i>Project:</i><br>"
                "<b><big>"+QString(Name.c_str())+"</big></b></td>"
                "</tr></table><hr/>";
    InfosStr += "<i>Description:</i><br>"+QString(Description.c_str())+"<hr/>";
    InfosStr += "<i>Authors:</i><br>"+QString(Authors.c_str())+"<hr/>";
    InfosStr += "<i>Creation:</i><br>"+QString(CreationDate.c_str())+"<hr/>";
    InfosStr += "<i>Last modification:</i><br>"+QString(LastModDate.c_str());
    if (IncludeFullPath)
      InfosStr += "<hr/><i>Project location:</i><br>"+QDir::toNativeSeparators(ProjectPath);
  }

  return InfosStr;
}


// =====================================================================
// =====================================================================


QColor getRandomColor()
{
  return QColor(qrand() % 256,qrand() % 256,qrand() % 256);
}


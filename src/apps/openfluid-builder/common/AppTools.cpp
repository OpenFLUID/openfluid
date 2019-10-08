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
  @file AppTools.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <QProcess>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/config.hpp>

#include "AppTools.hpp"


QString getProjectInfosAsHTML(const QString& ProjectPath, bool IncludeFullPath)
{
  QString InfosStr;
  std::string Name, Description, Authors, CreationDate, LastModDate;

  if (openfluid::base::RunContextManager::getProjectInfos(ProjectPath.toStdString(),
                                                       Name, Description, Authors, CreationDate, LastModDate))
  {
    openfluid::core::DateTime TmpDate;
    TmpDate.setFromString(CreationDate,"%Y%m%dT%H%M%S");
    CreationDate = TmpDate.getAsString("%Y-%m-%d, %H:%M:%S");
    TmpDate.setFromString(LastModDate,"%Y%m%dT%H%M%S");
    LastModDate = TmpDate.getAsString("%Y-%m-%d, %H:%M:%S");

    InfosStr += "<table><tr><td valign='middle' width='74px' style='padding: 5px;'>"
                "<img style='vertical-align:middle;' src=':/builder/icons/openfluid_icon.png' width='64'/></td>"
                "<td valign='middle' style='padding: 5px;'><i>Project:</i><br>"
                "<b><big>"+QString(Name.c_str())+"</big></b></td>"
                "</tr></table><hr/>";
    InfosStr += "<i>Description:</i><br>"+QString(Description.c_str())+"<hr/>";
    InfosStr += "<i>Authors:</i><br>"+QString(Authors.c_str())+"<hr/>";
    InfosStr += "<i>Creation:</i><br>"+QString(CreationDate.c_str())+"<hr/>";
    InfosStr += "<i>Last modification:</i><br>"+QString(LastModDate.c_str());
    if (IncludeFullPath)
    {
      InfosStr += "<hr/><i>Project location:</i><br>"+QDir::toNativeSeparators(ProjectPath);
    }
  }

  return InfosStr;
}


// =====================================================================
// =====================================================================


void launchDevStudio()
{
  QString ExeName = QString::fromStdString(openfluid::config::DEVSTUDIO_APP);

#if defined OPENFLUID_OS_WINDOWS
  ExeName += ".exe";
#endif

#if defined OPENFLUID_OS_MAC
  ExeName += ".app";
#endif

  QString ExePath = QApplication::applicationDirPath()+"/"+ExeName;

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#if defined OPENFLUID_OS_MAC
  bool ExecOK = QProcess::startDetached("open",QStringList("/Applications/"+ExeName));
#else
  bool ExecOK = QProcess::startDetached(ExePath);
#endif


  QApplication::restoreOverrideCursor();

  if (!ExecOK)
  {
    QMessageBox::critical(QApplication::activeWindow(),"OpenFLUID-Builder",
                          QObject::tr("OpenFLUID-DevStudio could not be launched"),
                          QMessageBox::Close);
  }
}

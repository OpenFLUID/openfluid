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
  @file UIHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UICOMMON_UIHELPERS_HPP__
#define __OPENFLUID_UICOMMON_UIHELPERS_HPP__


#include <QColor>
#include <QRegExp>
#include <QLineEdit>
#include <QIcon>
#include <QApplication>
#include <QFile>


namespace openfluid { namespace ui { namespace common {


inline QColor getRandomColor()
{
  return QColor(qrand() % 256,qrand() % 256,qrand() % 256);
}


// =====================================================================
// =====================================================================


inline void fixLineEdit(QLineEdit* LineEdit,QRegExp SearchRegExp = QRegExp("[^\\w]"),QString NewStr = "_")
{
  int CPos = LineEdit->cursorPosition();
  LineEdit->setText(LineEdit->text().replace(SearchRegExp,NewStr));
  LineEdit->setCursorPosition(CPos);
}


// =====================================================================
// =====================================================================


inline QIcon getIcon(const QString& IconName,const QString& ResourcePath,bool IsLight = false)
{
  QString IconSuffix = "dark";

  if (IsLight)
  {
    IconSuffix = "light";
  }

  QIcon TmpIcon(QString(":%1/icons/%2_%3.png").arg(ResourcePath).arg(IconName).arg(IconSuffix));

  if (IsLight)
  {
    TmpIcon.addPixmap(QPixmap(QString(":%1/icons/%2_grayed.png").arg(ResourcePath).arg(IconName)),QIcon::Disabled);
  }

  return TmpIcon;
}


// =====================================================================
// =====================================================================


inline QPixmap getImage(const QString& ImageName,const QString& ResourcePath,bool AutoHiDPI = true)
{
  QString TmpPath = QString(":%1/images/%2.png").arg(ResourcePath).arg(ImageName);

  if (AutoHiDPI && qApp->devicePixelRatio() >= 2.0)
  {
    QString TmpHiDPIPath = QString(":%1/images/%2@2x.png").arg(ResourcePath).arg(ImageName);

    if (QFile::exists(TmpHiDPIPath))
    {
      TmpPath = TmpHiDPIPath;
    }
  }

  return QPixmap(TmpPath);
}


} } }  // namespaces


#endif /* __OPENFLUID_UICOMMON_UIHELPERS_HPP__ */

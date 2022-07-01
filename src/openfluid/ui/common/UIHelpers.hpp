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


#include <cmath>
#include <random>

#include <QColor>
#include <QRegExp>
#include <QLineEdit>
#include <QIcon>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace ui { namespace common {


inline QColor getRandomColor()
{
  static std::random_device RD;
  static std::mt19937 G(RD());
  static std::uniform_int_distribution<> Distri255(0, 255);
  
  return QColor(Distri255(G),Distri255(G),Distri255(G));
}


// =====================================================================
// =====================================================================


inline double computeLuminance(const QColor& Color)
{
  // Reference: https://www.w3.org/WAI/GL/wiki/Relative_luminance
  auto adjustColor = [](const double ChannelValue)
  {
    double RelValue = ChannelValue/255;
    if (RelValue <= 0.03928)
    {
       return RelValue/12.92;
    }
    else
    {
      // low-gamma adjust coefficient
      return std::pow((RelValue + 0.055) / 1.055, 2.4);
    }
  };
  return adjustColor(Color.red())*0.2126 + adjustColor(Color.green())*0.7152 + adjustColor(Color.blue())*0.0722;
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


inline bool isHiDPIDevice()
{
  return (qApp->devicePixelRatio() >= 2.0);
}


// =====================================================================
// =====================================================================


inline QIcon getIcon(const QString& IconName,const QString& ResourcePath,
                     bool IsLight = false,bool AutoHiDPI = true)
{
  QString IconSuffix = "dark";

  if (IsLight)
  {
    IconSuffix = "light";
  }

  QString TmpPath = QString(":%1/icons/%2_%3.png").arg(ResourcePath).arg(IconName).arg(IconSuffix);

  if (AutoHiDPI && isHiDPIDevice())
  {
    QString TmpHiDPIPath = QString(":%1/icons/%2_%3@2x.png").arg(ResourcePath).arg(IconName).arg(IconSuffix);

    if (QFile::exists(TmpHiDPIPath))
    {
      TmpPath = TmpHiDPIPath;
    }
  }

  QIcon TmpIcon(TmpPath);

  if (IsLight)
  {
    QString TmpLightPath = QString(":%1/icons/%2_grayed.png").arg(ResourcePath).arg(IconName);

    if (AutoHiDPI && isHiDPIDevice())
    {
      QString TmpLightHiDPIPath = QString(":%1/icons/%2_grayed@2x.png").arg(ResourcePath).arg(IconName);

      if (QFile::exists(TmpLightHiDPIPath))
      {
        TmpLightPath = TmpLightHiDPIPath;
      }
    }
    TmpIcon.addPixmap(QPixmap(TmpLightPath),QIcon::Disabled);
  }

  return TmpIcon;
}


// =====================================================================
// =====================================================================


inline QPixmap getImage(const QString& ImageName,const QString& ResourcePath,bool AutoHiDPI = true)
{
  QString TmpPath = QString(":%1/images/%2.png").arg(ResourcePath).arg(ImageName);

  if (AutoHiDPI && isHiDPIDevice())
  {
    QString TmpHiDPIPath = QString(":%1/images/%2@2x.png").arg(ResourcePath).arg(ImageName);

    if (QFile::exists(TmpHiDPIPath))
    {
      TmpPath = TmpHiDPIPath;
    }
  }

  return QPixmap(TmpPath);
}


// =====================================================================
// =====================================================================


inline void createNewFile(QWidget* Parent, const QString& Path)
{
  if (Path.isEmpty())
  {
    return;
  }

  QString FileToCreate = QFileDialog::getSaveFileName(Parent,
                                                      QApplication::translate("openfluid::ui::common","Create file"),
                                                      Path,
                                                      QApplication::translate("openfluid::ui::common","All files"));

  if (!FileToCreate.isEmpty())
  {
    auto NewFilePath = openfluid::tools::Path(FileToCreate.toStdString());
    
    if (NewFilePath.isDirectory())
    {
      QMessageBox::critical(Parent,QApplication::translate("openfluid::ui::common","Error creating file"),
                           QApplication::translate("openfluid::ui::common","\"%1\" is an existing directory")
                             .arg(QString::fromStdString(NewFilePath.toNative())));
    }
    else
    {
      NewFilePath.makeFile();
    }
    
  }  
}


} } }  // namespaces


#endif /* __OPENFLUID_UICOMMON_UIHELPERS_HPP__ */

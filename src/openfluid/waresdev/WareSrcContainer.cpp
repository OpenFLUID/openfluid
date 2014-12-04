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
 \file WareSrcContainer.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/waresdev/WareSrcContainer.hpp>

#include <QDir>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/config.hpp>

namespace openfluid { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcContainer::WareSrcContainer(const QString& AbsolutePath,
                                   WareSrcManager::WareType Type,
                                   const QString& WareName) :
    m_AbsolutePath(AbsolutePath), m_AbsoluteCMakeConfigPath(""), m_AbsoluteMainCppPath(
        "")
{
  QDir Dir(AbsolutePath);

  QString CMakeFilePath = Dir.absoluteFilePath(
      QString::fromStdString(openfluid::config::WARESDEV_CMAKE_USERFILE));

  if (QFile::exists(CMakeFilePath))
  {
    m_AbsoluteCMakeConfigPath = CMakeFilePath;

    QFile File(m_AbsoluteCMakeConfigPath);
    if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
      throw openfluid::base::FrameworkException(
          "WareSrcContainer constructor",
          QString("Cannot open file %1").arg(m_AbsoluteCMakeConfigPath)
              .toStdString());

    QString MainCppFilename = searchMainCppFileName(File.readAll());

    if (!MainCppFilename.isEmpty())
    {
      QString MainCppFilePath = Dir.absoluteFilePath(MainCppFilename);

      if (QFile::exists(MainCppFilePath))
        m_AbsoluteMainCppPath = MainCppFilePath;
    }
  }

}


// =====================================================================
// =====================================================================


WareSrcContainer::~WareSrcContainer()
{

}


// =====================================================================
// =====================================================================


QString WareSrcContainer::searchMainCppFileName(const QString& CMakeFileContent)
{
  QStringList Lines = CMakeFileContent.split('\n');

  QRegExp RE(
      QString("^\\s*SET\\s*\\((?:%1|%2|%3)\\s+(\\w+\\.cpp).*\\).*").arg(
          QString::fromStdString(openfluid::config::WARESDEV_CMAKE_SIMCPPVAR))
          .arg(
          QString::fromStdString(openfluid::config::WARESDEV_CMAKE_OBSCPPVAR))
          .arg(
          QString::fromStdString(
              openfluid::config::WARESDEV_CMAKE_BEXTCPPVAR)));

  foreach(QString L,Lines){
  if (RE.indexIn(L) > -1)
  return RE.cap(1);
}

  return "";
}


// =====================================================================
// =====================================================================


QStringList WareSrcContainer::getDefaultFiles()
{
  QStringList L;

  if (!m_AbsoluteCMakeConfigPath.isEmpty())
    L << m_AbsoluteCMakeConfigPath;

  if (!m_AbsoluteMainCppPath.isEmpty())
    L << m_AbsoluteMainCppPath;
  else
  {
    QDir Dir(m_AbsolutePath);

    QStringList NameFilters;
    NameFilters << "*.cpp";

    QString FirstCpp =
        Dir.entryList(NameFilters, QDir::Files, QDir::Name).value(0, "");

    if (!FirstCpp.isEmpty())
      L << Dir.absoluteFilePath(FirstCpp);
  }

  return L;
}


// =====================================================================
// =====================================================================


QString WareSrcContainer::getMainCppFile()
{
  return m_AbsoluteMainCppPath;
}


// =====================================================================
// =====================================================================


} } // namespaces

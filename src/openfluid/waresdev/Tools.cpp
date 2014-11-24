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
 \file Tools.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/waresdev/Tools.hpp>

#include <QString>
#include <QDir>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/config.hpp>

namespace openfluid { namespace waresdev {


// =====================================================================
// =====================================================================


QString Tools::getCurrentSrcDir(SrcType Type)
{
  QString SrcTypeSubDir;

  switch (Type)
  {
    case SRCTYPE_SIMULATOR:
      SrcTypeSubDir = QString::fromStdString(
          openfluid::config::SIMULATORS_PLUGINS_SUBDIR);
      break;
    case SRCTYPE_OBSERVER:
      SrcTypeSubDir = QString::fromStdString(
          openfluid::config::OBSERVERS_PLUGINS_SUBDIR);
      break;
    case SRCTYPE_BUILDEREXT:
      SrcTypeSubDir = "builderexts";
      break;
    default:
      throw openfluid::base::FrameworkException(
          "waresdev::Tools::getCurrentDir", "wrong source type");
      break;
  }

  QString Path = QString("%1/%2/%3").arg(
      openfluid::base::PreferencesManager::getInstance()->getWorkspacePath())
      .arg(QString::fromStdString(openfluid::config::WARESDEV_SUBDIR)).arg(
      SrcTypeSubDir);

  if (!QDir(Path).mkpath(Path))
    throw openfluid::base::FrameworkException(
        "waresdev::Tools::getCurrentSrcDir",
        QString("unable to open or create %1 directory").arg(Path).toStdString());

  return Path;
}


// =====================================================================
// =====================================================================


} } // namespaces

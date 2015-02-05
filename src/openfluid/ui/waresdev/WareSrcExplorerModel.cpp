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
 @file WareSrcExplorerModel.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <openfluid/ui/waresdev/WareSrcExplorerModel.hpp>

#include <QFont>

#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcExplorerModel::WareSrcExplorerModel(const QString& Path) :
    QFileSystemModel(), m_DirPath(Path)
{
  m_DefaultIcons["*.c++;*.cxx;*.cpp;*.cc;*.C;*.moc;*.c"] = ":/ui/common/icons/cpp.png";
  m_DefaultIcons["*.h;*.hh;*.H;*.h++;*.hxx;*.hpp;*.hcc"] = ":/ui/common/icons/hpp.png";
  m_DefaultIcons["CMake.in.config"] = ":/ui/common/icons/cmake-config.png";
  m_DefaultIcons["CMakeLists.txt;*.cmake"] = ":/ui/common/icons/cmakelists.png";
  m_DefaultIcons["*.f;*.F;*.for;*.FOR;*.f90;*.F90;*.fpp;*.FPP;*.f95;*.F95"] = ":/ui/common/icons/fortran.png";
  m_DefaultIcons["*.ui"] = ":/ui/common/icons/qt-ui.png";
  m_DefaultIcons["wareshub.json"] = ":/ui/common/icons/wareshub.png";

  m_UserIcons = openfluid::ui::waresdev::WareSrcFiletypeManager::instance()->getIconsByFileExtensionList();

  mp_Manager = openfluid::waresdev::WareSrcManager::instance();

  connect(this, SIGNAL(directoryLoaded(const QString&)), this, SLOT(onDirectoryLoaded(const QString&)));

  setRootPath(m_DirPath);
}


// =====================================================================
// =====================================================================


void WareSrcExplorerModel::onDirectoryLoaded(const QString& Path)
{
  QModelIndex Parent = index(Path);

  for (int Row = 0; Row < rowCount(Parent); ++Row)
  {
    QString ChildPath = filePath(index(Row, 0, Parent));

    m_PathInfos[ChildPath] = mp_Manager->getPathInfo(ChildPath);
  }

}


// =====================================================================
// =====================================================================


QVariant WareSrcExplorerModel::data(const QModelIndex& Index, int Role) const
{
  if (!Index.isValid())
    return QFileSystemModel::data(Index, Role);

  if (Role == Qt::ForegroundRole && fileName(Index).startsWith("_build"))
    return QVariant(QColor("#A3A3A3"));

  if (Role == QFileSystemModel::FileIconRole)
  {
    if (!isDir(Index))
    {
      for (QMap<QString, QString>::const_iterator it = m_UserIcons.begin(); it != m_UserIcons.end(); ++it)
      {
        if (QDir::match(it.key(), fileName(Index)))
          return QIcon(it.value());
      }

      for (QMap<QString, QString>::const_iterator it = m_DefaultIcons.begin(); it != m_DefaultIcons.end(); ++it)
      {
        if (QDir::match(it.key(), fileName(Index)))
          return QIcon(it.value());
      }

      return QIcon(":/ui/common/icons/notype.png");
    }
    else if (m_PathInfos.value(filePath(Index)).m_isAWare)
      return QIcon(":/ui/common/icons/waredir.png");
  }

  if (Role == Qt::FontRole && !isDir(Index) && fileInfo(Index).suffix() == "cpp")
  {
    openfluid::waresdev::WareSrcManager::PathInfo PInfo = m_PathInfos.value(filePath(Index));

    if (openfluid::waresdev::WareSrcContainer(
          PInfo.m_AbsolutePathOfWare, PInfo.m_WareType, PInfo.m_WareName).getMainCppFile() == filePath(Index))
    {
      QFont Font;
      Font.setBold(true);
      return Font;
    }

  }

  return QFileSystemModel::data(Index, Role);
}


// =====================================================================
// =====================================================================


} } }  // namespaces

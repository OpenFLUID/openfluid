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
 @file WareSrcManager.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_WARESDEV_WARESRCMANAGER_HPP__
#define __OPENFLUID_WARESDEV_WARESRCMANAGER_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/PluggableWare.hpp>

#include <QString>
#include <QMap>


namespace openfluid { namespace waresdev {

class OPENFLUID_API WareSrcManager
{
  public:

    struct PathInfo
    {
        QString m_AbsolutePath;
        bool m_IsInCurrentWorkspace;
        openfluid::ware::WareType m_WareType;
        QString m_WareName;
        bool m_isAWare;
        bool m_isAWareFile;
        QString m_RelativePathToWareDir;
        QString m_AbsolutePathOfWare;
        QString m_FileName;


        PathInfo() :
          m_AbsolutePath(""), m_IsInCurrentWorkspace(false), m_WareType(openfluid::ware::WareType::UNDEFINED),
          m_WareName(""),
          m_isAWare(false), m_isAWareFile(false), m_RelativePathToWareDir(""), m_AbsolutePathOfWare(""),
          m_FileName("")
        {
        }
    };


  private:

    static WareSrcManager* mp_Instance;

    QString m_WaresdevDirName;

    QString m_WorkspacePath;

    QString m_WaresdevPath;

    /**
     * Absolute paths of each ware type sub-directory in the current workspace
     */
    QMap<openfluid::ware::WareType, QString> m_WareTypePathByWareType;

    WareSrcManager();

    ~WareSrcManager();


  public:

    static WareSrcManager* instance();

    /**
     * Set default internal paths (workspace and waresdev sub dirs) and create them if they don't exist.
     * @param NewAbsoluteWorkspacePath
     * @throws openfluid::base::FrameworkException if a path can not be created
     */
    void switchWorkspace(const QString& NewAbsoluteWorkspacePath);

    QString getWareTypePath(openfluid::ware::WareType WareSrcType);

    QString getWarePath(const QString& WareID, openfluid::ware::WareType OFWareType, QString& ErrMsg);

    /**
     * Make Path absolute then splits it to retrieve PathInfo attributes.
     * @param Path
     * @return A new PathInfo
     */
    PathInfo getPathInfo(const QString& Path);

    QString getWaresdevPath() const;

    QString getWorkspacePath() const;
};

} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCMANAGER_HPP__ */

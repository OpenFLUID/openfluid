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
 @file PreferencesManager.hpp

 @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
 @author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __OPENFLUID_BASE_PREFERENCESMANAGER_HPP__
#define __OPENFLUID_BASE_PREFERENCESMANAGER_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>

#include <map>
#include <vector>


#include <QSettings>


namespace openfluid { namespace base {


// =====================================================================
// =====================================================================


class OPENFLUID_API PreferencesManager
{

  private:

    static PreferencesManager* mp_Instance;

    static QString m_FileName;

    QSettings* mp_ConfFile;

    PreferencesManager();

    void setDefaultValues();

    void setExtraPaths(const QString& Key, const QStringList& Paths);

    void addExtraPath(const QString& Key, const QString& Path);

    void removeExtraPath(const QString& Key, const QString& Path);

    QStringList getExtraPaths(const QString& Key);

    static QString guessLang();


  public:

    static const int RecentProjectsLimit;

    class RecentProject_t
    {
      public:
        QString Name;
        QString Path;
    };

    typedef std::vector<RecentProject_t> RecentProjectsList_t;

    typedef std::map<QString, QString> MarketPlaces_t;

    static PreferencesManager* instance();

    ~PreferencesManager();

    /* Used only if we want to set another file name for the conf file
     * instead of the default one (for tests eg.)
     * To be set before the first call of instance().
     */
    static void setFileName(const QString& AbsoluteFileName);

    QString getFileName();

    bool isValidKey(const QString& Group, const QString& Key);


    void setLang(const QString& Lang);

    QString getLang();

    static QStringList getAvailableLangs();

    static bool isAvailableLang(const QString& Lang);

    void setRecentMax(int RecentMax);

    int getRecentMax();

    bool addRecentProject(const QString& ProjectName, const QString& ProjectPath = "");

    RecentProjectsList_t getRecentProjects();

    void clearRecentProjects();

    void adaptRecentProjects();


    void setWorkspacesPaths(const QStringList& Paths);

    QStringList getWorkspacesPaths();

    QString getWorkspacePath();

    QString getProjectsPath();


    void setExtraSimulatorsPaths(const QStringList& Paths);

    void addExtraSimulatorsPath(const QString& Path);

    void removeExtraSimulatorsPath(const QString& Path);

    QStringList getExtraSimulatorsPaths();


    void setExtraExtensionsPaths(const QStringList& Paths);

    void addExtraExtensionsPath(const QString& Path);

    void removeExtraExtensionsPath(const QString& Path);

    QStringList getExtraExtensionsPaths();


    void setExtraObserversPaths(const QStringList& Paths);

    void addExtraObserversPath(const QString& Path);

    void removeExtraObserversPath(const QString& Path);

    QStringList getExtraObserversPaths();


    void setDeltaT(openfluid::core::Duration_t DeltaT);

    openfluid::core::Duration_t getDeltaT();


    void setBegin(const QString& Begin);

    QString getBegin();

    void setEnd(const QString& End);

    QString getEnd();


    bool addMarketplace(const QString& PlaceName, const QString& PlaceUrl);

    void removeMarketplace(const QString& PlaceName);

    MarketPlaces_t getMarketplaces();


    bool isExtensionValueExist(const QString& PluginName, const QString& Key);

    QString getExtensionValue(const QString& PluginName, const QString& Key);

    void setExtensionValue(const QString& PluginName, const QString& Key, const QString& Value);


    Qt::DockWidgetArea getDockPosition();

    void setDockPosition(Qt::DockWidgetArea Position);

    Qt::ToolBarArea getToolBarPosition();

    void setToolBarPosition(Qt::ToolBarArea Position);


    bool isItemRemovalConfirm();

    void setItemRemovalConfirm(bool Confirm);

    bool isParamRemovalConfirm();

    void setParamRemovalConfirm(bool Confirm);

    bool isWaresWatchersActive();

    void setWaresWatchersActive(bool Active);

    bool isAutomaticSaveBeforeRun();

    void setAutomaticSaveBeforeRun(bool AutoSave);

    bool isSpatialUnitsRemovalConfirm();

    void setSpatialUnitsRemovalConfirm(bool Confirm);

    bool isSpatialConnsRemovalConfirm();

    void setSpatialConnsRemovalConfirm(bool Confirm);

    bool isSpatialAttrsRemovalConfirm();

    void setSpatialAttrsRemovalConfirm(bool Confirm);


    bool isAutomaticSaveBeforeBuild();

    void setAutomaticSaveBeforeBuild(bool AutoSave);
};

} } //namespaces

#endif /* __OPENFLUID_BASE_PREFERENCESMANAGER_HPP__ */

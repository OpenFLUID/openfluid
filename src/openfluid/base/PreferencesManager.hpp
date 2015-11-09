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


#include <map>
#include <vector>

#include <QSettings>
#include <QStringList>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/utils/SingletonMacros.hpp>


namespace openfluid { namespace base {


// =====================================================================
// =====================================================================


class OPENFLUID_API PreferencesManager
{

  OPENFLUID_SINGLETON_DEFINITION(PreferencesManager)

  private:

    static QString m_FileName;

    QSettings* mp_ConfFile;

    PreferencesManager();

    ~PreferencesManager();

    void setDefaultValues();

    void setBuilderExtraPaths(const QString& Key, const QStringList& Paths);

    void addBuilderExtraPath(const QString& Key, const QString& Path);

    void removeBuilderExtraPath(const QString& Key, const QString& Path);

    QStringList getBuilderExtraPaths(const QString& Key);

    static QString guessLang();


  public:

    static const int RecentProjectsLimit;

    class RecentProject_t
    {
      public:
        QString Name;
        QString Path;
    };

    struct SyntaxHighlightingRule_t
    {
        QString m_Color;
        QStringList m_Decoration;

        SyntaxHighlightingRule_t(const QString& Color,const QStringList& Decoration)
        : m_Color(Color),m_Decoration(Decoration) {}
    };

    typedef std::vector<RecentProject_t> RecentProjectsList_t;

    typedef QMap<QString,SyntaxHighlightingRule_t> SyntaxHighlightingRules_t;

    typedef std::map<QString, QString> MarketPlaces_t;

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

    void setBuilderRecentMax(int RecentMax);

    int getBuilderRecentMax();

    bool addBuilderRecentProject(const QString& ProjectName, const QString& ProjectPath = "");

    RecentProjectsList_t getBuilderRecentProjects();

    void clearBuilderRecentProjects();

    void adaptBuilderRecentProjects();


    void setBuilderWorkspacesPaths(const QStringList& Paths);

    QStringList getBuilderWorkspacesPaths();

    QString getBuilderWorkspacePath();

    QString getBuilderProjectsPath();


    void setBuilderExtraSimulatorsPaths(const QStringList& Paths);

    void addBuilderExtraSimulatorsPath(const QString& Path);

    void removeBuilderExtraSimulatorsPath(const QString& Path);

    QStringList getBuilderExtraSimulatorsPaths();


    void setBuilderExtraExtensionsPaths(const QStringList& Paths);

    void addBuilderExtraExtensionsPath(const QString& Path);

    void removeBuilderExtraExtensionsPath(const QString& Path);

    QStringList getBuilderExtraExtensionsPaths();


    void setBuilderExtraObserversPaths(const QStringList& Paths);

    void addBuilderExtraObserversPath(const QString& Path);

    void removeBuilderExtraObserversPath(const QString& Path);

    QStringList getBuilderExtraObserversPaths();


    void setBuilderDeltaT(openfluid::core::Duration_t DeltaT);

    openfluid::core::Duration_t getBuilderDeltaT();


    void setBuilderBegin(const QString& Begin);

    QString getBuilderBegin();

    void setBuilderEnd(const QString& End);

    QString getBuilderEnd();


    bool addMarketplace(const QString& PlaceName, const QString& PlaceUrl);

    void removeMarketplace(const QString& PlaceName);

    MarketPlaces_t getMarketplaces();


    bool isBuilderExtensionValueExist(const QString& PluginName, const QString& Key);

    QString getBuilderExtensionValue(const QString& PluginName, const QString& Key);

    void setBuilderExtensionValue(const QString& PluginName, const QString& Key, const QString& Value);


    Qt::DockWidgetArea getBuilderDockPosition();

    void setBuilderDockPosition(Qt::DockWidgetArea Position);

    Qt::ToolBarArea getBuilderToolBarPosition();

    void setBuilderToolBarPosition(Qt::ToolBarArea Position);


    bool isBuilderItemRemovalConfirm();

    void setBuilderItemRemovalConfirm(bool Confirm);

    bool isBuilderParamRemovalConfirm();

    void setBuilderParamRemovalConfirm(bool Confirm);

    bool isBuilderWaresWatchersActive();

    void setBuilderWaresWatchersActive(bool Active);

    bool isBuilderAutomaticSaveBeforeRun();

    void setBuilderAutomaticSaveBeforeRun(bool AutoSave);

    bool isBuilderSpatialUnitsRemovalConfirm();

    void setBuilderSpatialUnitsRemovalConfirm(bool Confirm);

    bool isBuilderSpatialConnsRemovalConfirm();

    void setBuilderSpatialConnsRemovalConfirm(bool Confirm);

    bool isBuilderSpatialAttrsRemovalConfirm();

    void setBuilderSpatialAttrsRemovalConfirm(bool Confirm);


    bool isWaresdevAutomaticSaveBeforeBuild();

    void setWaresdevAutomaticSaveBeforeBuild(bool AutoSave);


    void setWaresdevTextEditorDefaults(bool ForceReset);

    bool isWaresdevSyntaxHighlightingEnabled();

    void setWaresdevSyntaxHighlightingEnabled(bool Enabled);

    SyntaxHighlightingRules_t getWaresdevSyntaxHighlightingRules();

    void setWaresdevSyntaxHighlightingRules(const SyntaxHighlightingRules_t& Rules);

    bool isWaresdevCurrentlineHighlightingEnabled();

    void setWaresdevCurrentlineHighlightingEnabled(bool Enabled);

    QString getWaresdevCurrentlineColor();

    void setWaresdevCurrentlineColor(const QString& Color);

    QString getWaresdevFontName();

    void setWaresdevFontName(const QString& FontName);

    bool isWaresdevLineWrappingEnabled();

    void setWaresdevLineWrappingEnabled(bool Enabled);

    int getWaresdevIndentSpaceNb();

    void setWaresdevIndentSpaceNb(int SpaceNumber);

    QString getWaresdevConfigEnv(const QString& Name);

    void setWaresdevConfigEnv(const QString& Name,const QString& Value);

    QString getWaresdevConfigOptions();

    void setWaresdevConfigOptions(const QString& Options);

    QString getWaresdevConfigGenerator();

    void setWaresdevConfigGenerator(const QString& Generator);

    QString getWaresdevBuildEnv(const QString& Name);

    void setWaresdevBuildEnv(const QString& Name,const QString& Value);

    bool isWaresdevShowCommandEnv(const QString& Name);

    void setWaresdevShowCommandEnv(const QString& Name, bool Enabled);

    bool isWaresdevSslNoVerify();

    void setWaresdevSslNoVerify(bool NoVerify);

    QString getWaresdevImportWaresHubLastUrl();

    void setWaresdevImportWaresHubLastUrl(const QString& Url);

    QString getWaresdevImportWaresHubLastUsername();

    void setWaresdevImportWaresHubLastUsername(const QString& Username);
};

} } //namespaces

#endif /* __OPENFLUID_BASE_PREFERENCESMANAGER_HPP__ */

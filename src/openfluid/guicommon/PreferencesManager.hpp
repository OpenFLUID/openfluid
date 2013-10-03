/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file PreferencesManager.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __PREFERENCESMANAGER_HPP__
#define __PREFERENCESMANAGER_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>

#include <map>
#include <vector>


#include <QSettings>


namespace openfluid { namespace guicommon {


// =====================================================================
// =====================================================================


class DLLEXPORT PreferencesManager
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

    static PreferencesManager* getInstance();

    ~PreferencesManager();

    /* Used only if we want to set another file name for the conf file
     * instead of the default one (for tests eg.)
     * To be set before the first call of getInstance().
     */
    static void setFileName(const QString& AbsoluteFileName);

    QString getFileName();

    bool isValidKey(const QString& Group, const QString& Key);


    void setLang(const QString& Lang);

    QString getLang();

    static QStringList getAvailableLangs();

    static bool isAvailableLang(const QString& Lang);

    void setRecentMax(unsigned int RecentMax);

    unsigned int getRecentMax();

    bool addRecentProject(const QString& ProjectName, const QString& ProjectPath = "");

    RecentProjectsList_t getRecentProjects();

    void clearRecentProjects();

    void adaptRecentProjects();


    void setWorkdir(const QString& Workdir);

    QString getWorkdir();


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


    bool getItemRemovalConfirm();

    void setItemRemovalConfirm(bool Confirm);

    bool getParamRemovalConfirm();

    void setParamRemovalConfirm(bool Confirm);

    bool getWaresWatcher();

    void setWaresWatcher(bool Active);

};

} } //namespaces

#endif /* __PREFERENCESMANAGER_HPP__ */

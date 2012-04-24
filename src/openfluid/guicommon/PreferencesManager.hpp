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

#include <map>
#include <iostream>
#include <glibmm/keyfile.h>

namespace openfluid { namespace guicommon {

// =====================================================================
// =====================================================================


class DLLEXPORT PreferencesManager
{
  private:

    static PreferencesManager* mp_Instance;

    static std::string m_FileName;

    Glib::KeyFile* mp_KFile;

    PreferencesManager();

    void setDefaultValues();

    void loadKeyFile();

  public:

    typedef std::map<std::string, std::string> MarketPlaces_t;

    static PreferencesManager* getInstance();

    ~PreferencesManager();

    bool save();

    /* Used only if we want to set another file name for the conf file
     * instead of the default one (for tests eg.)
     * To be set before the first call of getInstance().
     */
    static void setFileName(Glib::ustring AbsoluteFileName);
    std::string getFileName();

    bool isValidKey(std::string Group, std::string Key);

    void setLang(Glib::ustring Lang);
    Glib::ustring getLang();

    void setRecentMax(unsigned int RecentMax);
    int getRecentMax();

    bool
    addRecentProject(std::string ProjectPath, std::string ProjectName = "");
    void clearRecentProjects();
    std::vector<std::pair<std::string, std::string> > getRecentProjects();

    void setWorkdir(Glib::ustring Workdir);
    Glib::ustring getWorkdir();

    void setExtraPlugPaths(std::vector<Glib::ustring> ExtraPlugPaths);
    void addExtraPlugPath(Glib::ustring Path);
    void removeExtraPlugPath(Glib::ustring Path);
    std::vector<std::string> getExtraPlugPaths();

    void setExtraExtensionPaths(std::vector<Glib::ustring> ExtraExtPaths);
    void addExtraExtensionPath(Glib::ustring Path);
    void removeExtraExtensionPath(Glib::ustring Path);
    std::vector<std::string> getExtraExtensionPaths();

    void setDeltaT(unsigned int DeltaT);
    int getDeltaT();

    void setBegin(std::string Begin);
    std::string getBegin();

    void setEnd(std::string End);
    std::string getEnd();

    void setOutFilesBufferInKB(unsigned int Buffer);
    int getOutFilesBufferInKB();

    bool
    addMarketplace(Glib::ustring PlaceName, Glib::ustring PlaceUrl);
    void removeMarketplace(Glib::ustring PlaceName);
    MarketPlaces_t getMarketplaces();

    bool isPluginValueExist(std::string PluginName, std::string Key);
    std::string getPluginValue(std::string PluginName, std::string Key);
    void setPluginValue(std::string PluginName, std::string Key,
        std::string Value);

};

} } //namespaces

#endif /* __PREFERENCESMANAGER_HPP__ */

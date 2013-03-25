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

#ifndef BUILDERAPPCOORDINATOR_HPP_
#define BUILDERAPPCOORDINATOR_HPP_

#include <sigc++/sigc++.h>

#include "BuilderAppActions.hpp"

namespace openfluid {
namespace guicommon {
class BuilderModule;
}
}

class BuilderAppWindow;
class BuilderAppState;
class EngineProjectNewDialog;
class EngineProjectOpenDialog;
class PreferencesDialog;

class BuilderAppCoordinator: public sigc::trackable
{
  private:
    BuilderAppWindow& m_MainWindow;
    BuilderAppActions m_Actions;
    openfluid::guicommon::BuilderModule* mp_CurrentModule;
    BuilderAppState* mp_CurrentState;
    BuilderAppState* mp_HomeState;
    BuilderAppState* mp_ProjectState;

    EngineProjectNewDialog* mp_NewProjectDialog;
    EngineProjectOpenDialog* mp_OpenProjectDialog;

    PreferencesDialog* mp_PreferencesDialog;

    bool m_HasToBeSaved;

    void unsetCurrentModule();

    void setCurrentModule(openfluid::guicommon::BuilderModule* Module);

    void whenNewProjectAsked();

    void whenOpenProjectAsked();

    void whenCloseProjectAsked();

    void whenQuitAsked();

    bool whenMainWindowCloseAsked(GdkEventAny* Event);

    void whenRunAsked();

    void whenMarketAsked();

    void whenDemosAsked();

    void whenPreferencesAsked();

    void whenAboutAsked();

    void whenOnlineAsked(const std::string& URL);

    void whenSaveAsked();

    void whenSaveAsAsked();

    void whenPropertiesAsked();

    void whenMapViewAsked();

    void whenRefreshAsked();

    void whenExtensionAsked(const std::string& ExtensionID);

    void onCheckHappened(bool IsCheckOk);

    void onChangeHappened();

    void onSaveHappened();

  public:

    BuilderAppCoordinator(BuilderAppWindow& MainWindow,
        BuilderAppActions& Actions);

    ~BuilderAppCoordinator();

    void setState(BuilderAppState& State);

    BuilderAppState* getHomeState();

    BuilderAppState* getProjectState();

    void setHomeModule();

    void setProjectModule(std::string ProjectFolder);

    openfluid::guicommon::BuilderModule* getCurrentModule();

    void quitApp();

    bool showCloseProjectDialog();

    bool showQuitAppDialog();

    void openProject(std::string ProjectPath = "");

    void createProject();

    void closeProject();

    PreferencesDialog* getPreferencesDialog();

    void updateMainWindowInformation();

    void updateRecentsList();

    void configExtensionsMenus();

};

#endif /* BUILDERAPPCOORDINATOR_HPP_ */

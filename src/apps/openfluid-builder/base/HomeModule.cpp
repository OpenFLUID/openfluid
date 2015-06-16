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
  @file HomeModule.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/ui/market/MarketClientAssistant.hpp>
#include <openfluid/ui/common/PreferencesDialog.hpp>

#include "AppTools.hpp"
#include "HomeModule.hpp"
#include "ExtensionPluginsManager.hpp"

#include <QApplication>


HomeModule::HomeModule(const AppActions* Actions):
AbstractModule(), mp_Widget(NULL), mp_Actions(Actions)
{

}


// =====================================================================
// =====================================================================


HomeModule::~HomeModule()
{

}

// =====================================================================
// =====================================================================


QWidget* HomeModule::mainWidgetRebuilt(QWidget* Parent)
{
  if (mp_Widget != NULL)
    delete mp_Widget;

  mp_Widget = new HomeWidget(Parent,mp_Actions);

  return mp_Widget;
}


// =====================================================================
// =====================================================================


bool HomeModule::whenQuitAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


bool HomeModule::whenNewAsked()
{
  return true;
}

// =====================================================================
// =====================================================================


bool HomeModule::whenOpenAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


bool HomeModule::whenReloadAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


void HomeModule::whenSaveAsked()
{

}


// =====================================================================
// =====================================================================


bool HomeModule::whenSaveAsAsked()
{
  return false;
}


// =====================================================================
// =====================================================================


void HomeModule::whenPropertiesAsked()
{

}


// =====================================================================
// =====================================================================


bool HomeModule::whenCloseAsked()
{
  return false;
}


// =====================================================================
// =====================================================================


void HomeModule::whenPreferencesAsked()
{
  openfluid::ui::common::PreferencesDialog PrefsDlg(QApplication::activeWindow(),true);
  PrefsDlg.initializeBuilderPrefs(
      openfluid::tools::toQStringList(ExtensionPluginsManager::instance()->getPluginsStandardSearchPaths()));
  PrefsDlg.exec();
}


// =====================================================================
// =====================================================================


void HomeModule::whenRecentProjectsActionsChanged()
{
  mp_Widget->refreshRecentProjects();
}


// =====================================================================
// =====================================================================


void HomeModule::whenRunAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModule::whenExtensionAsked(const QString& /*ID*/)
{

}


// =====================================================================
// =====================================================================


void HomeModule::whenMarketAsked()
{
#if defined(ENABLE_MARKET_INTEGRATION)
  openfluid::ui::market::MarketClientAssistant MarketAssistant(QApplication::activeWindow());
  MarketAssistant.exec();
#endif
}


// =====================================================================
// =====================================================================


void HomeModule::whenWaresRefreshAsked()
{

}


// =====================================================================
// =====================================================================


bool HomeModule::whenOpenExampleAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


void HomeModule::whenNewSimulatorSrcAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModule::whenNewGhostSimulatorAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModule::whenOpenSimulatorSrcAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModule::whenNewObserverSrcAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModule::whenOpenObserverSrcAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModule::whenLaunchDevStudioAsked()
{
  launchDevStudio();
}


// =====================================================================
// =====================================================================


void HomeModule::whenSrcEditAsked(const QString& /*ID*/,openfluid::ware::PluggableWare::WareType /*WType*/,
                                  bool /*Ghost*/)
{

}

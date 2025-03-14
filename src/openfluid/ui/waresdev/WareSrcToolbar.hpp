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
 @file WareSrcToolbar.hpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WARESRCTOOLBAR_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCTOOLBAR_HPP__


#include <QToolBar>
#include <QAction>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/ui/waresdev/WareBuildOptionsWidget.hpp>
#include <openfluid/ui/waresdev/WareSrcActionsCollection.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WareSrcToolbar: public QToolBar
{
  Q_OBJECT

  private:

    bool m_IsIncluded;
    openfluid::ui::waresdev::WareSrcActionsCollection* mp_ActionsCollection;

    std::list<openfluid::base::PreferencesManager::ExternalTool_t> m_ExternalTools;
    
    QMap<QString, QAction*> m_ExternalToolsActions;


  public:

    WareSrcToolbar(bool IsIncluded, 
                   openfluid::ui::waresdev::WareSrcActionsCollection* ActionsCollection, 
                   QWidget* Parent = nullptr);

    virtual ~WareSrcToolbar();

    void createActions();

    const QMap<QString, QAction*> externalToolsActions();

};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESRCTOOLBAR_HPP__ */

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
  \file HomeModule.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_HOMEMODULE_HPP__
#define __OPENFLUID_BUILDERAPP_HOMEMODULE_HPP__

#include "AbstractModule.hpp"
#include "HomeWidget.hpp"

class HomeModule : public AbstractModule
{
  Q_OBJECT;

  private:

     HomeWidget* mp_Widget;

     const AppActions* mp_Actions;

  public:

    HomeModule(const AppActions* Actions);

    ~HomeModule();

    QWidget* getMainWidget(QWidget* Parent);

    QWidget* getDockWidget(QWidget* /*Parent*/)
    { return NULL; }

    bool whenQuitAsked();

    bool whenNewAsked();

    bool whenOpenAsked();

    bool whenReloadAsked();

    void whenSaveAsked();

    bool whenSaveAsAsked();

    void whenPropertiesAsked();

    bool whenCloseAsked();

    void whenPreferencesAsked();

    void whenRecentProjectsActionsChanged();

    void whenRunAsked();

    void whenExtensionAsked(const QString& ID);

    void whenMarketAsked();

    void whenWaresRefreshAsked();

    bool whenOpenExampleAsked();
};


#endif /* __OPENFLUID_BUILDERAPP_HOMEMODULE_HPP__ */

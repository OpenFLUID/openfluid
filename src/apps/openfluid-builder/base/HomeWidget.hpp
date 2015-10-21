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
  @file HomeWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_HOMEWIDGET_HPP__
#define __OPENFLUID_BUILDERAPP_HOMEWIDGET_HPP__

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <openfluid/ui/common/ClickableLabel.hpp>

#include "AbstractMainWidget.hpp"


class AppActions;


// =====================================================================
// =====================================================================


class RecentProjectLabel : public openfluid::ui::common::ClickableLabel
{
  Q_OBJECT;

  protected:

    void enterEvent(QEvent* Event);

    void leaveEvent(QEvent* Event);

  public:

    RecentProjectLabel(const QString& Text, QWidget* Parent = nullptr);

    virtual ~RecentProjectLabel()
    { }

};


// =====================================================================
// =====================================================================


namespace Ui
{
  class HomeWidget;
}


class HomeWidget : public AbstractMainWidget
{
  Q_OBJECT;

  private:

    Ui::HomeWidget* ui;

    QVBoxLayout* mp_RecentsLayout;

    QLabel* mp_RecentProjectsLabel;

    const AppActions* mp_Actions;

    QPushButton* createButton(const QAction* Action, const QString& Text);

  public:

    HomeWidget(QWidget* Parent, const AppActions* Actions);

    ~HomeWidget();

    void refreshRecentProjects();
};


#endif /* __OPENFLUID_BUILDERAPP_HOMEWIDGET_HPP__ */

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
  @file WorkspaceDevActionsWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QVBoxLayout>
#include <QHBoxLayout>

#include <openfluid/ui/waresdev/WorkspaceDevActionsWidget.hpp>
#include <openfluid/ui/common/TagLabel.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevActionsWidget::WorkspaceDevActionsWidget(const WorkspaceDevDashboardTypes::ActionsByRows& Actions,
                                                     QWidget* Parent) :
    QWidget(Parent)
{

  QVBoxLayout* MainLayout = new QVBoxLayout;

  for (const auto& Row : Actions)
  {
    QHBoxLayout* RowLayout = new QHBoxLayout;
    for (const auto& Item : Row)
    {
      openfluid::ui::common::TagLabel* Tag = new openfluid::ui::common::TagLabel(Item.second,this);
      Tag->setOutlined(true);
      m_ActionsMap[Item.first] = Tag;
      RowLayout->addWidget(Tag);
    }
    RowLayout->addStretch(1);

    MainLayout->addLayout(RowLayout);
  }

  setLayout(MainLayout);
}


// =====================================================================
// =====================================================================


WorkspaceDevActionsWidget::~WorkspaceDevActionsWidget()
{

}


// =====================================================================
// =====================================================================


void WorkspaceDevActionsWidget::setState(const QString& ActionName, bool IsOK)
{
  auto it = m_ActionsMap.find(ActionName);

  if (it != m_ActionsMap.end())
  {
    openfluid::ui::common::TagLabel* Tag = (*it).second;

    Tag->setTextColor("white");
    Tag->setOutlined(false);

    if (IsOK)
    {
      Tag->setBackgroundColor("green");
    }
    else
    {
      Tag->setBackgroundColor("red");
    }
  }
}


} } } // namespaces

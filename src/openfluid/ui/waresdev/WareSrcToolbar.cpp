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
 \file WareSrcToolbar.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>

#include <QMenu>
#include <QToolButton>
#include <openfluid/ui/waresdev/WareSrcActions.hpp>

namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcToolbar::WareSrcToolbar(bool IsIncluded, QWidget* Parent) :
    QToolBar(Parent)
{
  if (!IsIncluded)
  {
    setIconSize(QSize(32, 32));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  }

  WareSrcActions* Actions =
      openfluid::ui::waresdev::WareSrcActions::getInstance();
  addAction(Actions->getAction("NewFile"));
  addAction(Actions->getAction("OpenFile"));
  addAction(Actions->getAction("SaveFile"));
  addAction(Actions->getAction("SaveAsFile"));

  if (IsIncluded)
  {
    QWidget* Spacer = new QWidget();
    Spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addWidget(Spacer);

    QMenu* Menu = new QMenu();
    QMenu* SubMenu = Menu->addMenu(tr("File"));
    SubMenu->addAction(Actions->getAction("NewFile"));
    SubMenu->addAction(Actions->getAction("OpenFile"));
    SubMenu->addAction(Actions->getAction("SaveFile"));
    SubMenu->addAction(Actions->getAction("SaveAsFile"));
    SubMenu->addAction(Actions->getAction("CloseFile"));
    SubMenu->addAction(Actions->getAction("DeleteFile"));
    QToolButton* MenuButton = new QToolButton(this);
    MenuButton->setText(tr("Menu"));
    MenuButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    MenuButton->setPopupMode(QToolButton::InstantPopup);
    MenuButton->setMenu(Menu);
    addWidget(MenuButton);
  }
}


// =====================================================================
// =====================================================================


WareSrcToolbar::~WareSrcToolbar()
{

}


// =====================================================================
// =====================================================================

} } }  // namespaces

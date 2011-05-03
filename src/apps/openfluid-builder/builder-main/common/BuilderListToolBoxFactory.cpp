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
 \file BuilderListToolBoxFactory.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderListToolBoxFactory.hpp"

#include <glibmm/i18n.h>


// =====================================================================
// =====================================================================


BuilderListToolBox* BuilderListToolBoxFactory::createModelStructureToolBox()
{
  BuilderListToolBox* ToolBox = new BuilderListToolBoxImpl();
  ToolBox->setAllCommandVisible();
  ToolBox->setEditCommandVisible(false);
  ToolBox->setAddCommandTooltipText(_("Append a function to the Model"));
  ToolBox->setRemoveCommandTooltipText(_(
      "Remove the selected function from the Model"));
  ToolBox->setUpCommandTooltipText(_(
      "Move the selected function toward the begin of the Model"));
  ToolBox->setDownCommandTooltipText(_(
      "Move the selected function toward the end of the Model"));
  ToolBox->setAllCommandAvailable(false);
  return ToolBox;
}


// =====================================================================
// =====================================================================


BuilderListToolBox* BuilderListToolBoxFactory::createDomainStructureToolBox()
{
  BuilderListToolBox* ToolBox = new BuilderListToolBoxImpl();
  ToolBox->setAllCommandVisible();
  ToolBox->setUpCommandVisible(false);
  ToolBox->setDownCommandVisible(false);
  ToolBox->setAddCommandTooltipText(_("Add a Unit to the Domain"));
  ToolBox->setRemoveCommandTooltipText(_("Delete the selected Unit"));
  ToolBox->setEditCommandTooltipText(_("Edit the selected Unit"));
  return ToolBox;
}


// =====================================================================
// =====================================================================


BuilderListToolBox* BuilderListToolBoxFactory::createDomainUnitRelationToolBox(
    std::string RelationName)
{
  BuilderListToolBox* ToolBox = new BuilderListToolBoxImpl();
  ToolBox->setAllCommandVisible();
  ToolBox->setUpCommandVisible(false);
  ToolBox->setDownCommandVisible(false);
  ToolBox->setEditCommandVisible(false);
  ToolBox->setAddCommandTooltipText(Glib::ustring::compose(_(
      "Add a %1 Unit to the Unit"), RelationName));
  ToolBox->setRemoveCommandTooltipText(Glib::ustring::compose(_(
      "Remove the selected %1 Unit"), RelationName));
  return ToolBox;
}


// =====================================================================
// =====================================================================


BuilderListToolBox* BuilderListToolBoxFactory::createDomainIDataToolBox()
{
  BuilderListToolBox* ToolBox = new BuilderListToolBoxImpl();
  ToolBox->setAllCommandVisible();
  ToolBox->setUpCommandVisible(false);
  ToolBox->setDownCommandVisible(false);
  ToolBox->setEditCommandVisible(false);
  ToolBox->setAddCommandTooltipText(_(
      "Add an inputdata for the current unit class"));
  ToolBox->setRemoveCommandTooltipText(_(
      "Remove an inputdata from the current unit class"));
  return ToolBox;
}


// =====================================================================
// =====================================================================


BuilderListToolBox* BuilderListToolBoxFactory::createDomainEventsToolBox()
{
  BuilderListToolBox* ToolBox = new BuilderListToolBoxImpl();
  ToolBox->setAllCommandVisible();
  ToolBox->setUpCommandVisible(false);
  ToolBox->setDownCommandVisible(false);
  ToolBox->setEditCommandVisible(false);
  ToolBox->setAddCommandTooltipText(_(
      "Add an event for the current unit class"));
  ToolBox->setRemoveCommandTooltipText(_(
      "Remove an event from current unit class"));
  return ToolBox;
}


// =====================================================================
// =====================================================================


BuilderListToolBox* BuilderListToolBoxFactory::createSimulOutFilesToolBox()
{
  BuilderListToolBox* ToolBox = new BuilderListToolBoxImpl();
  ToolBox->setAllCommandVisible();
  ToolBox->setUpCommandVisible(false);
  ToolBox->setDownCommandVisible(false);
  ToolBox->setEditCommandTooltipText(_("Edit the selected File Format"));
  ToolBox->setAddCommandTooltipText(_("Add a File Format"));
  ToolBox->setRemoveCommandTooltipText(_("Remove the selected File Format"));
  return ToolBox;
}


// =====================================================================
// =====================================================================


BuilderListToolBox* BuilderListToolBoxFactory::createSimulOutSetsToolBox()
{
  BuilderListToolBox* ToolBox = new BuilderListToolBoxImpl();
  ToolBox->setAllCommandVisible();
  ToolBox->setUpCommandVisible(false);
  ToolBox->setDownCommandVisible(false);
  ToolBox->setEditCommandTooltipText(_("Edit the selected Set"));
  ToolBox->setAddCommandTooltipText(_("Add a Set"));
  ToolBox->setRemoveCommandTooltipText(_("Remove the selected Set"));
  return ToolBox;
}

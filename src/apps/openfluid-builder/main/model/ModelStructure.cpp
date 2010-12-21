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
  \file ModelStructure.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <iostream>

#include <glibmm/i18n.h>

#include <boost/foreach.hpp>

#include "ModelStructure.hpp"


// =====================================================================
// =====================================================================


ModelStructure::ModelStructure(Glib::RefPtr<Gtk::Builder> GladeBuilder, openfluid::machine::ModelInstance & ModelInstance)
: mp_Builder(GladeBuilder)
{
  mp_ViewList = new ModelStructure_VList(this, GladeBuilder);

  mp_ViewTools = new ModelStructure_VTools(this, GladeBuilder);

  mp_ViewNotebook = new ModelStructure_VNotebook(this, GladeBuilder);

  mp_GlobalParams = new ModelGlobalParams(GladeBuilder);

  initItems(ModelInstance);
}


// =====================================================================
// =====================================================================


ModelStructure::~ModelStructure()
{
  delete mp_ViewList;
  delete mp_ViewTools;
  delete mp_ViewNotebook;

  BOOST_FOREACH(ModelStructureItem * i, m_StructureItemsList)
    delete i;

  delete mp_GlobalParams;
}


// =====================================================================
// =====================================================================


void ModelStructure::initItems(openfluid::machine::ModelInstance & ModelInstance)
{
  std::list<openfluid::machine::ModelItemInstance*> IInstances = ModelInstance.getItems();

  for(std::list<openfluid::machine::ModelItemInstance*>::iterator it=IInstances.begin() ; it!=IInstances.end() ; ++it)
  {
    openfluid::machine::ModelItemInstance * IInstance = *it;

    ModelStructureItem * Item = new ModelStructureItem(this, mp_GlobalParams, mp_Builder, *IInstance);

    addItem(Item,-1);
  }
}


// =====================================================================
// =====================================================================


void ModelStructure::addItem(ModelStructureItem * StructureItem, int Position)
{
  // add in ModelDescriptor
  addModelItemDescriptor(StructureItem->getItemDescriptor(), Position);

  // add in StructureItemsList
  StructureItemsList_t::iterator it = m_StructureItemsList.begin();
  std::advance(it,Position);
  m_StructureItemsList.insert(it,StructureItem);

  // add in Views
  mp_ViewList->addRow(StructureItem->getId(),Position+1,Position);
  mp_ViewList->updateIndexes();
  mp_ViewNotebook->addPage(StructureItem->getId(),StructureItem->asWidget(),Position);
}


// =====================================================================
// =====================================================================


void ModelStructure::moveItem(int PositionFrom, int PositionTo)
{
  // move in ModelDescriptor
  moveModelItemDescriptor(PositionFrom, PositionTo);

  // move in StructureItemsList
  StructureItemsList_t::iterator itFrom = m_StructureItemsList.begin();
  std::advance(itFrom,PositionFrom);

  StructureItemsList_t::iterator itTo;
  if(isEndPosition(PositionTo))
    itTo = m_StructureItemsList.end();
  else
  {
    itTo = m_StructureItemsList.begin();
    std::advance(itTo,PositionTo);
  }

  m_StructureItemsList.splice(itTo,m_StructureItemsList,itFrom);

  // move in Views
  mp_ViewList->moveRow(PositionFrom,PositionTo);
  mp_ViewList->updateIndexes();
  mp_ViewNotebook->movePage(PositionFrom,PositionTo);
}


// =====================================================================
// =====================================================================


void ModelStructure::removeItem(int Position)
{
  if(!isAnExistingPosition(Position))
  {
    throw openfluid::base::OFException("openfluid Builder","ModelStructure::removeItem","trying to remove an item from an invalid position");
    return;
  }

  // remove from Model Descriptor
  removeModelItemDescriptor(Position);

  // remove from StructureItemsList
  StructureItemsList_t::iterator it = m_StructureItemsList.begin();
  std::advance(it,Position);
  ModelStructureItem * StructureItem = *it;
  m_StructureItemsList.erase(it);

  //remove from Views
  mp_ViewList->removeRow(Position);
  mp_ViewList->updateIndexes();
  mp_ViewNotebook->removePage(Position);

  delete StructureItem;
}


// =====================================================================
// =====================================================================


void ModelStructure::addModelItemDescriptor(openfluid::base::ModelItemDescriptor * ItemDescriptor, int Position)
{
  appendItem(ItemDescriptor);

  moveModelItemDescriptor(getItems().size()-1,Position);
}


// =====================================================================
// =====================================================================


void ModelStructure::moveModelItemDescriptor(int PositionFrom, int PositionTo)
{
  if(!isAnExistingPosition(PositionFrom))
  {
    throw openfluid::base::OFException("openfluid Builder","ModelStructure::moveModelItemDescriptor","trying to move an item from an invalid position");
    return;
  }

  ModelDescription_t & Items = getItems();

  ModelDescription_t::iterator itFrom = Items.begin();
  std::advance(itFrom,PositionFrom);

  ModelDescription_t::iterator itTo;


  if(isEndPosition(PositionTo))
  {
    itTo = Items.end();
  }
  else
  {
    itTo = Items.begin();
    std::advance(itTo,PositionTo);
  }


  Items.splice(itTo,Items,itFrom);
}


// =====================================================================
// =====================================================================


void ModelStructure::removeModelItemDescriptor(int Position)
{
  ModelDescription_t::iterator it = getItems().begin();

  std::advance(it,Position);

  getItems().erase(it);
}


// =====================================================================
// =====================================================================


bool ModelStructure::isAnExistingPosition(int Position)
{
  return (Position > -1 && Position < (int)getItems().size());
}


// =====================================================================
// =====================================================================


bool ModelStructure::isEndPosition(int Position)
{
  return (Position < 0 || Position > (int)getItems().size()-1);
}


// =====================================================================
// =====================================================================


void ModelStructure::VList_itemAdded(std::string Data, int Position)
{
  // TODO: manage adding of Generator

  ModelStructureItem * StructureItem = new ModelStructureItem(this, mp_GlobalParams, mp_Builder, Data);

  addItem(StructureItem, Position);

}


// =====================================================================
// =====================================================================


void ModelStructure::VList_itemMoved(int PositionFrom, int PositionTo)
{
  if(!isAnExistingPosition(PositionFrom))
  {
    throw openfluid::base::OFException("openfluid Builder","ModelStructure::PList_dragDataMoved","trying to move an item from an invalid position");
    return;
  }

  moveItem(PositionFrom,PositionTo);
}


// =====================================================================
// =====================================================================


void ModelStructure::VTools_itemRemoved(int Position)
{
  removeItem(Position);
}


// =====================================================================
// =====================================================================


openfluid::base::ModelDescriptor * ModelStructure::getModelDescriptor()
{
  return (openfluid::base::ModelDescriptor *)this;
}

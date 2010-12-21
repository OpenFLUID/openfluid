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
  \file ModelStructureItem.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <iostream>

#include <glibmm/i18n.h>
#include <boost/foreach.hpp>

#include "BuilderAvailableFunctions.hpp"
#include "ModelStructure.hpp"
#include "ModelStructureItem.hpp"


// =====================================================================
// =====================================================================


ModelStructureItem::ModelStructureItem(ModelStructure * Structure, ModelGlobalParams * GlobalParams, Glib::RefPtr<Gtk::Builder> GladeBuilder, Glib::ustring Id, openfluid::base::ModelItemDescriptor::ModelItemType Type)
: mp_Structure(Structure), mp_GlobalParams(GlobalParams), mp_Builder(GladeBuilder)
{
  mp_View = new ModelStructureItem_V(this,GladeBuilder);

  if(Type == openfluid::base::ModelItemDescriptor::PluggedFunction)
  {
    mp_ItemDescriptor = new openfluid::base::FunctionDescriptor(Id);

    m_Id = Id;

    initParams(Id);
  }

  else if(Type == openfluid::base::ModelItemDescriptor::Generator)
  {
    mp_ItemDescriptor = new openfluid::base::GeneratorDescriptor(); // TODO: getGenerator infos

    m_Id = _("(Generator)");
  }

  else
    throw openfluid::base::OFException("ModelStructureItem constructor","NoModelItemType");

}

ModelStructureItem::ModelStructureItem(ModelStructure * Structure, ModelGlobalParams * GlobalParams, Glib::RefPtr<Gtk::Builder> GladeBuilder, openfluid::machine::ModelItemInstance & Instance)
: mp_Structure(Structure), mp_GlobalParams(GlobalParams)
{
  mp_View = new ModelStructureItem_V(this,GladeBuilder);

  openfluid::base::FuncID_t Id = Instance.Signature->ID;

  if(Instance.ItemType == openfluid::base::ModelItemDescriptor::PluggedFunction)
  {
    mp_ItemDescriptor = new openfluid::base::FunctionDescriptor(Id);

    m_Id = Id;

    initParams(Id, &Instance.Params);

  }

  else if(Instance.ItemType == openfluid::base::ModelItemDescriptor::Generator)
  {
    mp_ItemDescriptor = new openfluid::base::GeneratorDescriptor(); // TODO: getGenerator infos

    m_Id = _("(Generator)");
  }

  else
    throw openfluid::base::OFException("ModelStructureItem constructor","NoModelItemType");

}
// =====================================================================
// =====================================================================


ModelStructureItem::~ModelStructureItem()
{
  delete mp_View;

  BOOST_FOREACH(ParamsMap_t::value_type i, m_ParamsMap)
    delete i.second;
}


// =====================================================================
// =====================================================================


void ModelStructureItem::initParams(Glib::ustring FunctionId, openfluid::core::FuncParamsMap_t * ParamsValues)
{
  if(openfluid::machine::ModelItemInstance * IInstance = BuilderAvailableFunctions::getInstance()->getPluggableFunction(FunctionId))
  {
    std::vector<openfluid::base::SignatureHandledDataItem> ParamsDef = IInstance->Signature->HandledData.FunctionParams;

    for (unsigned int i=0 ; i<ParamsDef.size(); i++)
    {
      openfluid::base::SignatureHandledDataItem ParamDef = ParamsDef[i];

      openfluid::core::FuncParamValue_t Value = "";

      if(ParamsValues)
        Value = (*ParamsValues)[ParamDef.DataName];

      ModelGlobalParam * GlobalParam = mp_GlobalParams->getGlobalParam(ParamDef);

      ModelParam * Param = new ModelParam(this,ParamDef,GlobalParam,Value);

      // init in ModelStructureItem
      m_ParamsMap[ParamDef.DataName] = Param;

      // init in View
      mp_View->addParamWidgetRow(Param->asWidgetVector());
    }
  }
}


// =====================================================================
// =====================================================================


openfluid::base::FuncID_t ModelStructureItem::getId()
{
  return m_Id;
}


// =====================================================================
// =====================================================================


openfluid::core::FuncParamValue_t ModelStructureItem::getParamValue(openfluid::core::FuncParamKey_t Key)
{
  return mp_ItemDescriptor->getParameters()[Key];
}


// =====================================================================
// =====================================================================


openfluid::base::ModelItemDescriptor * ModelStructureItem::getItemDescriptor()
{
  return mp_ItemDescriptor;
}


// =====================================================================
// =====================================================================


Gtk::Widget * ModelStructureItem::asWidget()
{
  return mp_View;
}


// =====================================================================
// =====================================================================


void ModelStructureItem::setParamValue(openfluid::core::FuncParamKey_t Key, openfluid::core::FuncParamValue_t Value)
{
  mp_ItemDescriptor->setParameter(Key,Value);
}

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
  \file ModelGlobalParam.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <iostream>

#include <glibmm/i18n.h>

#include <boost/foreach.hpp>

#include "ModelGlobalParam.hpp"


// =====================================================================
// =====================================================================


ModelGlobalParam::ModelGlobalParam(ModelGlobalParams * GlobalParams, openfluid::base::SignatureHandledDataItem Signature, Glib::ustring GlobalValue )
: mp_GlobalParams(GlobalParams), m_Signature(Signature),
  m_GlobalValue(GlobalValue), m_GlobalValueActivated(false)
{
  mp_View = new ModelGlobalParam_V(this);

  updateLinkedParams();

  updateView();
}


// =====================================================================
// =====================================================================


ModelGlobalParam::~ModelGlobalParam()
{
  delete mp_View;
}


// =====================================================================
// =====================================================================


Glib::ustring ModelGlobalParam::getValue()
{
  return m_GlobalValue;
}


// =====================================================================
// =====================================================================


Glib::ustring ModelGlobalParam::getKey()
{
  return m_Signature.DataName;
}


// =====================================================================
// =====================================================================


Glib::ustring ModelGlobalParam::getUnit()
{
  return m_Signature.DataUnit;
}


// =====================================================================
// =====================================================================


bool ModelGlobalParam::isActivated()
{
  return m_GlobalValueActivated;
}


// =====================================================================
// =====================================================================


std::vector<Gtk::Widget *> ModelGlobalParam::asWidgetVector()
{
  return mp_View->asWidgetVector();
}


// =====================================================================
// =====================================================================


void ModelGlobalParam::linkParam(ModelParam * Param)
{
  m_ParamList.push_back(Param);
}


// =====================================================================
// =====================================================================


void ModelGlobalParam::unlinkParam(ModelParam * Param)
{
  m_ParamList.remove(Param);

  if(m_ParamList.empty())
    mp_GlobalParams->deleteGlobalParam(this);
}


// =====================================================================
// =====================================================================


void ModelGlobalParam::C_activated()
{
  m_GlobalValueActivated = true;

  updateLinkedParams();

  updateView();
}


// =====================================================================
// =====================================================================


void ModelGlobalParam::V_unactivated()
{

  m_GlobalValueActivated = false;
  m_GlobalValue = "";

  mp_GlobalParams->C_globalParamUnactivated(this);

  updateLinkedParams();
}


// =====================================================================
// =====================================================================


void ModelGlobalParam::V_globalValueChanged(std::string GlobalValue)
{
  m_GlobalValue = GlobalValue;

  updateLinkedParams();
}


// =====================================================================
// =====================================================================


void ModelGlobalParam::updateLinkedParams()
{
  BOOST_FOREACH(ModelParam * i, m_ParamList)
      i->update();
}


// =====================================================================
// =====================================================================


void ModelGlobalParam::updateView()
{
  mp_View->setLabel(getKey());

  mp_View->setUnit(getUnit());

  mp_View->setValue(m_GlobalValue);
}

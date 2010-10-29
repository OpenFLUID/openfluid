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
  \file ModelGlobalParams.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <iostream>

#include <glibmm/i18n.h>

#include "ModelGlobalParams.hpp"


// =====================================================================
// =====================================================================


ModelGlobalParams::ModelGlobalParams(Glib::RefPtr<Gtk::Builder> GladeBuilder)
{
  Gtk::Button * p_ButtonAddGlobal;

  GladeBuilder->get_widget("ComboGlobal",mp_Combo);
  GladeBuilder->get_widget("ButtonAddGlobal",p_ButtonAddGlobal);
  GladeBuilder->get_widget("ComboGlobal",mp_Combo);
  GladeBuilder->get_widget("TableGlobal",mp_Table);

  p_ButtonAddGlobal->signal_clicked().connect(
      sigc::mem_fun(*this,&ModelGlobalParams::onButtonAddClicked));

  mp_TreeModelParams = Gtk::TreeStore::create(m_GlobalAvailColumns);

  mp_TreeModelParams->set_sort_column(m_GlobalAvailColumns.m_Id,Gtk::SORT_ASCENDING);


  mp_TreeModelFilterCombo = Gtk::TreeModelFilter::create(mp_TreeModelParams);

  mp_TreeModelFilterCombo->set_visible_column(m_GlobalAvailColumns.m_IsNotUsed);


  mp_Combo->set_model(mp_TreeModelFilterCombo);

  mp_Combo->pack_start(m_GlobalAvailColumns.m_Id);
  mp_Combo->pack_start(m_GlobalAvailColumns.m_Unit);

}


// =====================================================================
// =====================================================================


ModelGlobalParams::~ModelGlobalParams()
{

}


// =====================================================================
// =====================================================================


void ModelGlobalParams::addParamRequest(Glib::ustring ParamId, Glib::ustring ParamUnit, Gtk::Label * GlobalLabel)
{
  Gtk::TreeIter ItParam = getParamIter(ParamId,ParamUnit);

  Gtk::TreeRow RowNewParam;

  if(ItParam == mp_TreeModelParams->children().end()) // avail param doesn't exist, we create it
  {
    RowNewParam = *(mp_TreeModelParams->append());

    RowNewParam[m_GlobalAvailColumns.m_Id] = ParamId;
    RowNewParam[m_GlobalAvailColumns.m_Unit] = ParamUnit;
    RowNewParam[m_GlobalAvailColumns.m_IsNotUsed] = true;
    RowNewParam[m_GlobalAvailColumns.m_Value] = "";
  }
  else // avail param exists, we get it
  {
    RowNewParam = *ItParam;

    if(RowNewParam[m_GlobalAvailColumns.m_IsNotUsed] == false) // param is used
      setGlobalLabelValue(GlobalLabel, RowNewParam[m_GlobalAvailColumns.m_Value]);
  }

  std::vector<Gtk::Label *> Labels = RowNewParam[m_GlobalAvailColumns.m_Labels];

  Labels.push_back(GlobalLabel);

  RowNewParam[m_GlobalAvailColumns.m_Labels] = Labels;

  updateComboState();

}


// =====================================================================
// =====================================================================


void ModelGlobalParams::removeParamRequest(std::vector<Gtk::Label *> GlobalLabelsIn)
{
  Gtk::TreeModel::Children Params = mp_TreeModelParams->children();

  Gtk::TreeModel::Children::iterator ItParam;

  std::vector<Gtk::Label *>::iterator ItLabelExisting;

  std::vector<Gtk::Label *> GlobalLabelsExisting;

  std::vector<Gtk::TreeRowReference> ParamsToDel;

  for(ItParam=Params.begin() ; ItParam!=Params.end() ; ++ItParam)
  {
    Gtk::TreeRow RowParam = *ItParam;

    GlobalLabelsExisting = RowParam[m_GlobalAvailColumns.m_Labels];

    for(unsigned int i=0 ; i<GlobalLabelsIn.size() ; i++)
    {
      ItLabelExisting = std::find(GlobalLabelsExisting.begin(), GlobalLabelsExisting.end(), GlobalLabelsIn[i]);

      if(ItLabelExisting != GlobalLabelsExisting.end())
      {
        GlobalLabelsIn[i]->set_text("");
        GlobalLabelsExisting.erase(ItLabelExisting);
      }
    }

    if(GlobalLabelsExisting.empty())
    {
      ParamsToDel.push_back(Gtk::TreeRowReference(mp_TreeModelParams,mp_TreeModelParams->get_path(ItParam)));
    }
    else
      RowParam[m_GlobalAvailColumns.m_Labels] = GlobalLabelsExisting;
  }

  removeGlobalParams(ParamsToDel);

  updateComboState();

  tempCheckModel();

}


// =====================================================================
// =====================================================================


Glib::ustring ModelGlobalParams::getGlobalParamValue(Glib::ustring ParamId/*, Glib::ustring ParamUnit*/)
{
  Glib::ustring Value = "";

  Gtk::TreeIter Iter = getParamIter(ParamId,"");

  if(Iter != mp_TreeModelParams->children().end())
  {
    Value = Iter->get_value(m_GlobalAvailColumns.m_Value);
  }

  return Value;
}


// =====================================================================
// =====================================================================


Gtk::TreeIter ModelGlobalParams::getParamIter(Glib::ustring ParamId, Glib::ustring ParamUnit)
{
  Gtk::TreeModel::Children Params = mp_TreeModelParams->children();

  Gtk::TreeModel::Children::iterator It;

  for(It=Params.begin() ; It!=Params.end() ; ++It)
  {
    Gtk::TreeRow RowParam = *It;

    if(ParamUnit == "" && RowParam[m_GlobalAvailColumns.m_Id] == ParamId)
    {
      return It;
    }
    else if(RowParam[m_GlobalAvailColumns.m_Id] == ParamId
        && RowParam[m_GlobalAvailColumns.m_Unit] == ParamUnit)
    {
      return It;
    }
  }

  return Params.end();
}


// =====================================================================
// =====================================================================


void ModelGlobalParams::setGlobalParamUsed(Gtk::TreeRowReference RefRow)
{
  Gtk::TreeIter Iter = mp_TreeModelParams->get_iter(RefRow.get_path());

  // Remove Param from Combo visible list
  Iter->set_value(m_GlobalAvailColumns.m_IsNotUsed,false);

  updateComboState();

  // Create Table Row
  createTableRowWidgets(RefRow);

  // Update Param tab
  std::vector<Gtk::Label *> Labels = Iter->get_value(m_GlobalAvailColumns.m_Labels);

  for(unsigned int i=0 ; i<Labels.size() ; i++)
  {
    setGlobalLabelValue(Labels[i],"");
  }

}


// =====================================================================
// =====================================================================


void ModelGlobalParams::setGlobalParamUnused(Gtk::TreeRowReference RefRow)
{
  Gtk::TreeIter Iter = mp_TreeModelParams->get_iter(RefRow.get_path());

  // Add Param to Combo visible list
  Iter->set_value(m_GlobalAvailColumns.m_IsNotUsed,true);

  updateComboState();

  // Remove Table Row
  removeTableRowWidgets(Iter);

  // Update param tab
  std::vector<Gtk::Label *> Labels = Iter->get_value(m_GlobalAvailColumns.m_Labels);

  for(unsigned int i=0 ; i<Labels.size() ; i++)
  {
    Labels[i]->set_text(_(""));
  }

}


// =====================================================================
// =====================================================================


void ModelGlobalParams::removeGlobalParams(std::vector<Gtk::TreeRowReference> ParamsToDel)
{
  for(unsigned int i=0 ; i<ParamsToDel.size() ; i++)
  {
    Gtk::TreeIter ParamIter = mp_TreeModelParams->get_iter(ParamsToDel[i].get_path());

    removeTableRowWidgets(ParamIter);

    mp_TreeModelParams->erase(ParamIter);
  }
}


// =====================================================================
// =====================================================================


void ModelGlobalParams::createTableRowWidgets(Gtk::TreeRowReference RefParam)
{
  Gtk::TreeIter IterBase = mp_TreeModelParams->get_iter(RefParam.get_path());

  Gtk::TreeModel::Row Row = *IterBase;


  // Create widgets
  Gtk::Button * ButtonSuppr = Gtk::manage(new Gtk::Button());


  Gtk::Label * Label = Gtk::manage(new Gtk::Label(Row[m_GlobalAvailColumns.m_Id],Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));

  Gtk::Entry * Entry = Gtk::manage(new Gtk::Entry());

  Gtk::Label * Unit = Gtk::manage(new Gtk::Label(Row[m_GlobalAvailColumns.m_Unit],Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));

  Gtk::Image * Image = Gtk::manage(new Gtk::Image(Gtk::Stock::DELETE, Gtk::ICON_SIZE_BUTTON));
  Image->show();
  ButtonSuppr->add(*Image);

  std::vector<Gtk::Widget *> TableRowWidgets;
  TableRowWidgets.push_back(ButtonSuppr);
  TableRowWidgets.push_back(Label);
  TableRowWidgets.push_back(Entry);
  TableRowWidgets.push_back(Unit);

  Row[m_GlobalAvailColumns.m_TableRowWidgets] = TableRowWidgets;

  ButtonSuppr->signal_clicked().connect(
      sigc::bind<Gtk::TreeRowReference>(
          sigc::mem_fun(*this,&ModelGlobalParams::onButtonSupprClicked),
          RefParam));

  Entry->signal_focus_out_event().connect(
      sigc::bind<Gtk::Entry *, Gtk::TreeRowReference>(
          sigc::mem_fun(*this,&ModelGlobalParams::onEntryFocusOut),
          Entry, RefParam));

  ButtonSuppr->set_visible(true);
  Label->set_visible(true);
  Entry->set_visible(true);
  Unit->set_visible(true);


  /* Attach table row widgets to the bottom of the table
   * (top-attach of new row widgets is bottom-attach of the bottom widget of the table)
   */

  Gtk::Table_Helpers::TableList List = mp_Table->children();
  Gtk::Table_Helpers::TableList::iterator IteTable;

  int top = 0;

  if(!List.empty())
    top = List.begin()->get_bottom_attach(); // first widget in TableList is the last attached

  for(unsigned int left=0 ; left<TableRowWidgets.size() ; left++)
  {
    mp_Table->attach(*TableRowWidgets[left],left,left+1,top,top+1,Gtk::FILL,Gtk::FILL,5,5);
  }

}


// =====================================================================
// =====================================================================


void ModelGlobalParams::removeTableRowWidgets(Gtk::TreeModel::iterator & Iter)
{
  std::vector<Gtk::Widget *> RowWidgets = Iter->get_value(m_GlobalAvailColumns.m_TableRowWidgets);

  for(unsigned int i=0 ; i<RowWidgets.size() ; i++)
  {
    mp_Table->remove(*RowWidgets[i]);
  }
}


// =====================================================================
// =====================================================================


void ModelGlobalParams::updateComboState()
{
  mp_TreeModelFilterCombo->refilter();

  if(!mp_TreeModelFilterCombo->children().empty())
  {
    mp_Combo->set_active(0);
    mp_Combo->set_sensitive(true);
  }
  else
    mp_Combo->set_sensitive(false);

  tempCheckModel();
}


// =====================================================================
// =====================================================================


void ModelGlobalParams::setGlobalLabelValue(Gtk::Label * GlobalLabel, Glib::ustring GlobalValue)
{
    GlobalLabel->set_text(Glib::ustring::compose(_("(global : \"%1\")"),GlobalValue));
}

// =====================================================================
// =====================================================================


void ModelGlobalParams::onButtonAddClicked()
{
  Gtk::TreeModel::iterator It = mp_Combo->get_active();

  if(It)
  {
    Gtk::TreeIter ItBase = mp_TreeModelFilterCombo->convert_iter_to_child_iter(It);

    Gtk::TreeRowReference RefBase = Gtk::TreeRowReference(mp_TreeModelParams,mp_TreeModelParams->get_path(ItBase));

    setGlobalParamUsed(RefBase);
  }
}


// =====================================================================
// =====================================================================


void ModelGlobalParams::onButtonSupprClicked(Gtk::TreeRowReference RowRef)
{
  setGlobalParamUnused(RowRef);
}


// =====================================================================
// =====================================================================


bool ModelGlobalParams::onEntryFocusOut(GdkEventFocus * /*Event*/, Gtk::Entry * Entry, Gtk::TreeRowReference RefRow)
{
  Gtk::TreeIter Iter = mp_TreeModelParams->get_iter(RefRow.get_path());

  Gtk::TreeRow Row = *Iter;

  if(Row[m_GlobalAvailColumns.m_Value] != Entry->get_text())
  {
    Row[m_GlobalAvailColumns.m_Value] = Entry->get_text();

    std::vector<Gtk::Label *> Labels = Row[m_GlobalAvailColumns.m_Labels];

    for(unsigned int i=0 ; i<Labels.size() ; i++)
    {
      setGlobalLabelValue(Labels[i],Entry->get_text());
    }
  }

  tempCheckModel();

  return true;
}


// =====================================================================
// =====================================================================


void ModelGlobalParams::tempCheckModel()
{
//  std::cout << "-------- checking GlobalParamsTree : " << std::endl;
//
//  Gtk::TreeModel::Children Params = mp_TreeModelParams->children();
//
//  Gtk::TreeModel::Children::iterator It;
//
//  for(It=Params.begin() ; It!=Params.end() ; ++It)
//  {
//    Gtk::TreeRow RowParam = *It;
//
//    std::cout << "- " << RowParam[m_GlobalAvailColumns.m_Id] << " " << RowParam[m_GlobalAvailColumns.m_Unit];
//
//    std::cout << " isnotused :" << RowParam[m_GlobalAvailColumns.m_IsNotUsed];
//
//    std::vector<Gtk::Label *> Labels = RowParam[m_GlobalAvailColumns.m_Labels];
//
//    std::cout << " nb labels :" << Labels.size() << std::endl;
//  }

}


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
  \file ModelAvailFct.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <glibmm/i18n.h>
#include <boost/filesystem.hpp>

#include <openfluid/machine.hpp>
#include <openfluid/base.hpp>

#include "builderconfig.hpp"

#include "ModelAvailFct.hpp"


// =====================================================================
// =====================================================================


ModelAvailFct::ModelAvailFct(Glib::RefPtr<Gtk::Builder> GladeBuilder)
{
  GladeBuilder->get_widget("TreeViewAvailFct",mp_TreeViewAvailFct);
  GladeBuilder->get_widget("TreeViewFctVars",mp_TreeViewFctVars);

  GladeBuilder->get_widget("LabelAvailFctId",mp_LabelAvailFctId);
  GladeBuilder->get_widget("LabelAvailFctName",mp_LabelAvailFctName);
  GladeBuilder->get_widget("LabelAvailFctPath",mp_LabelAvailFctPath);
  GladeBuilder->get_widget("LabelAvailFctDescription",mp_LabelAvailFctDescription);
  GladeBuilder->get_widget("LabelAvailFctVersion",mp_LabelAvailFctVersion);
  GladeBuilder->get_widget("LabelAvailFctStatus",mp_LabelAvailFctStatus);
  GladeBuilder->get_widget("LabelAvailFctDomain",mp_LabelAvailFctDomain);
  GladeBuilder->get_widget("LabelAvailFctProcess",mp_LabelAvailFctProcess);
  GladeBuilder->get_widget("LabelAvailFctMethod",mp_LabelAvailFctMethod);
  GladeBuilder->get_widget("LabelAvailFctAuthorName",mp_LabelAvailFctAuthorName);
  GladeBuilder->get_widget("LabelAvailFctAuthorEmail",mp_LabelAvailFctAuthorEmail);

  GladeBuilder->get_widget("TableAvailFctParams",mp_TableAvailFctParams);

  Glib::RefPtr<Gdk::Pixbuf> IconRed = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"red.svg"),8,-1,true);
  Glib::RefPtr<Gdk::Pixbuf> IconOrange = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"orange.svg"),8,-1,true);
  Glib::RefPtr<Gdk::Pixbuf> IconGreen = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"green.svg"),8,-1,true);

  mp_LabelAvailFctParamsEmpty = new Gtk::Label(_("(none)"));

  //Create the Tree model
  mp_TreeModelAvailFct = Gtk::TreeStore::create(m_Columns);

  // Add rows to the tree model
      // Simulation functions rows
  Gtk::TreeModel::Row SimulFctRow = *(mp_TreeModelAvailFct->append());

  SimulFctRow[m_Columns.m_FunctionId] = _("Simulation Functions");

  openfluid::machine::ArrayOfModelItemInstance PlugContainers = openfluid::machine::PluginManager::getInstance()->getAvailableFunctions();

  for (unsigned int i=0;i<PlugContainers.size();i++)
  {
    if (PlugContainers[i]->SDKCompatible && PlugContainers[i]->Signature!=NULL)
    {
      Gtk::TreeModel::Row Row = *(mp_TreeModelAvailFct->append(SimulFctRow.children()));

      Row[m_Columns.m_FunctionId] = PlugContainers[i]->Signature->ID;
      Row[m_Columns.m_FunctionName] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Name,("(unknown)"));
      Row[m_Columns.m_FunctionPath] = ((boost::filesystem::path)PlugContainers[i]->Filename).parent_path().string();

      Row[m_Columns.m_FunctionDescription] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Description,("(none)"));
      Row[m_Columns.m_FunctionVersion] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Version,("(unknown)"));

      Row[m_Columns.m_FunctionDomain] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Domain,("(unknown)"));
      Row[m_Columns.m_FunctionProcess] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Process,("(unknown)"));
      Row[m_Columns.m_FunctionMethod] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Method,("(unknown)"));
      Row[m_Columns.m_FunctionAuthorName] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Author,("(unknown)"));
      Row[m_Columns.m_FunctionAuthorEmail] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->AuthorEmail,("(unknown)"));

      switch(PlugContainers[i]->Signature->Status)
      {
        case openfluid::base::EXPERIMENTAL:
          Row[m_Columns.m_FunctionStatusStr] = "Experimental";
          Row[m_Columns.m_FunctionStatusIcon] = IconRed;
        break;

        case openfluid::base::BETA:
          Row[m_Columns.m_FunctionStatusStr] = "Beta";
          Row[m_Columns.m_FunctionStatusIcon] = IconOrange;
        break;

        case openfluid::base::STABLE:
          Row[m_Columns.m_FunctionStatusStr] = "Stable";
          Row[m_Columns.m_FunctionStatusIcon] = IconGreen;
        break;

        default:
          Row[m_Columns.m_FunctionStatusStr] = "(unknown)";
      }

      Row[m_Columns.m_FunctionParams] = PlugContainers[i]->Signature->HandledData.FunctionParams;
      Row[m_Columns.m_FunctionProducedVars] = PlugContainers[i]->Signature->HandledData.ProducedVars;
      Row[m_Columns.m_FunctionUpdatedVars] = PlugContainers[i]->Signature->HandledData.UpdatedVars;
      Row[m_Columns.m_FunctionRequiredVars] = PlugContainers[i]->Signature->HandledData.RequiredVars;
      Row[m_Columns.m_FunctionUsedVars] = PlugContainers[i]->Signature->HandledData.UsedVars;
      Row[m_Columns.m_FunctionRequiredPrevVars] = PlugContainers[i]->Signature->HandledData.RequiredPrevVars;
      Row[m_Columns.m_FunctionUsedPrevVars] = PlugContainers[i]->Signature->HandledData.UsedPrevVars;
    }
  }
      // Generators rows
  Gtk::TreeModel::Row GeneratorFctRow = *(mp_TreeModelAvailFct->append());

  GeneratorFctRow[m_Columns.m_FunctionId] = _("Generators");

  Gtk::TreeModel::Row Row;

  Row = *(mp_TreeModelAvailFct->append(GeneratorFctRow.children()));
  Row[m_Columns.m_FunctionId] = "Fixed Generator";

  Row = *(mp_TreeModelAvailFct->append(GeneratorFctRow.children()));
  Row[m_Columns.m_FunctionId] = "Random Generator";

  Row = *(mp_TreeModelAvailFct->append(GeneratorFctRow.children()));
  Row[m_Columns.m_FunctionId] = "Interp Generator";


  // Attach the tree model to the tree view
  mp_TreeViewAvailFct->set_model(mp_TreeModelAvailFct);

  // Add columns to the tree view
  Gtk::TreeView::Column * ColumnId = Gtk::manage(new Gtk::TreeView::Column("Id"));

  ColumnId->pack_start(m_Columns.m_FunctionStatusIcon,false);
  ColumnId->pack_start(m_Columns.m_FunctionId);

  mp_TreeViewAvailFct->append_column(*ColumnId);

  mp_TreeViewAvailFct->append_column("Domain",m_Columns.m_FunctionDomain);

  mp_TreeViewAvailFct->get_column(0)->set_sort_column(m_Columns.m_FunctionId);
  mp_TreeViewAvailFct->get_column(1)->set_sort_column(m_Columns.m_FunctionDomain);

  mp_TreeViewAvailFct->get_column(0)->set_resizable();
  mp_TreeViewAvailFct->get_column(1)->set_resizable();

  mp_TreeViewAvailFct->get_column(0)->set_reorderable();
  mp_TreeViewAvailFct->get_column(1)->set_reorderable();


  Glib::RefPtr<Gtk::TreeSelection> TreeSelection = mp_TreeViewAvailFct->get_selection();

  // Only "leaf" rows can be selected
  TreeSelection->set_select_function(sigc::mem_fun(*this, &ModelAvailFct::isFunctionRowSelectable));

  TreeSelection->signal_changed().connect(sigc::mem_fun(*this, &ModelAvailFct::onAvalaibleFunctionSelected));

  mp_TreeViewAvailFct->expand_all();


  // Select first available function
  Gtk::TreeModel::iterator sub = mp_TreeModelAvailFct->children()[0]->children()[0];
  if(sub)
    TreeSelection->select(sub);

}


// =====================================================================
// =====================================================================


ModelAvailFct::~ModelAvailFct()
{
  delete mp_LabelAvailFctParamsEmpty; // not managed
}


// =====================================================================
// =====================================================================


void ModelAvailFct::onAvalaibleFunctionSelected()
{
  Gtk::TreeModel::iterator iter = mp_TreeViewAvailFct->get_selection()->get_selected();

  if(iter)
  {
    Gtk::TreeModel::Row row = *iter;

    // Update selected function Signature

    mp_LabelAvailFctId->set_text(row[m_Columns.m_FunctionId]);
    mp_LabelAvailFctName->set_text(row[m_Columns.m_FunctionName]);
    mp_LabelAvailFctPath->set_text(row[m_Columns.m_FunctionPath]);
    mp_LabelAvailFctDescription->set_text(row[m_Columns.m_FunctionDescription]);

    mp_LabelAvailFctVersion->set_text(row[m_Columns.m_FunctionVersion]);
    mp_LabelAvailFctStatus->set_text(row[m_Columns.m_FunctionStatusStr]);

    mp_LabelAvailFctDomain->set_text(row[m_Columns.m_FunctionDomain]);
    mp_LabelAvailFctProcess->set_text(row[m_Columns.m_FunctionProcess]);
    mp_LabelAvailFctMethod->set_text(row[m_Columns.m_FunctionMethod]);
    mp_LabelAvailFctAuthorName->set_text(row[m_Columns.m_FunctionAuthorName]);
    mp_LabelAvailFctAuthorEmail->set_text(row[m_Columns.m_FunctionAuthorEmail]);


    // Update selected function Params

    Gtk::Container * ContainerAvailFctParams = mp_TableAvailFctParams->get_parent();

    ContainerAvailFctParams->remove(*mp_TableAvailFctParams);

    delete mp_TableAvailFctParams;

    std::vector<openfluid::base::SignatureHandledDataItem> Params = row[m_Columns.m_FunctionParams];

    if(Params.empty())
    {
      mp_TableAvailFctParams = Gtk::manage(new Gtk::Table(1,1));

      mp_TableAvailFctParams->attach(*mp_LabelAvailFctParamsEmpty,0,1,0,1);
    }
    else
    {
      mp_TableAvailFctParams = Gtk::manage(new Gtk::Table(Params.size()*2+1,2, false));
      mp_TableAvailFctParams->set_border_width(3);
      mp_TableAvailFctParams->set_col_spacings(3);

      mp_TableAvailFctParams->attach(*Gtk::manage(new Gtk::HSeparator()),0,2,0,1,Gtk::FILL|Gtk::EXPAND,Gtk::FILL);

      for(unsigned int i=0 ; i<Params.size() ; i++)
      {
        Glib::ustring LabelParamStr = Glib::ustring::compose("%1 (%2) :",Params[i].DataName, Params[i].DataUnit);

        Gtk::Label * LabelParam = Gtk::manage(new Gtk::Label(LabelParamStr,0,0));
        Gtk::Label * LabelParamDesc = Gtk::manage(new Gtk::Label(Params[i].Description,0,0));

        mp_TableAvailFctParams->attach(*LabelParam,0,1,i*2+1,i*2+2,Gtk::FILL|Gtk::EXPAND,Gtk::FILL);
        mp_TableAvailFctParams->attach(*LabelParamDesc,1,2,i*2+1,i*2+2,Gtk::FILL|Gtk::EXPAND,Gtk::FILL);
        mp_TableAvailFctParams->attach(*Gtk::manage(new Gtk::HSeparator()),0,2,i*2+2,i*2+3,Gtk::FILL|Gtk::EXPAND,Gtk::FILL);
      }
    }

    ContainerAvailFctParams->add(*mp_TableAvailFctParams);

    ContainerAvailFctParams->show_all_children();


    // Update selection function Variables

      //Create the Tree model
    Glib::RefPtr<Gtk::TreeStore> TreeModelFctVars = Gtk::TreeStore::create(m_ColumnsVar);

      // Add rows to the tree model
    createFctVarsTree(TreeModelFctVars,_("Produced"),row[m_Columns.m_FunctionProducedVars]);
    createFctVarsTree(TreeModelFctVars,_("Updated"),row[m_Columns.m_FunctionUpdatedVars]);
    createFctVarsTree(TreeModelFctVars,_("Required"),row[m_Columns.m_FunctionRequiredVars]);
    createFctVarsTree(TreeModelFctVars,_("Used"),row[m_Columns.m_FunctionUsedVars]);
    createFctVarsTree(TreeModelFctVars,_("Required t-1"),row[m_Columns.m_FunctionRequiredPrevVars]);
    createFctVarsTree(TreeModelFctVars,_("Used t-1"),row[m_Columns.m_FunctionUsedPrevVars]);

      // Attach the tree model to the tree view
    mp_TreeViewFctVars->unset_model();
    mp_TreeViewFctVars->set_model(TreeModelFctVars);

      // Add columns to the tree view
    mp_TreeViewFctVars->remove_all_columns();
    mp_TreeViewFctVars->append_column(_("Name"),m_ColumnsVar.m_FctVarName);
    mp_TreeViewFctVars->append_column(_("Unit Class"),m_ColumnsVar.m_FctVarUnitClass);
    mp_TreeViewFctVars->append_column(_("Unit"),m_ColumnsVar.m_FctVarUnit);
    mp_TreeViewFctVars->append_column(_("Description"),m_ColumnsVar.m_FctVarDesc);

    mp_TreeViewFctVars->get_column(0)->set_sort_column(m_ColumnsVar.m_FctVarName);
    mp_TreeViewFctVars->get_column(1)->set_sort_column(m_ColumnsVar.m_FctVarUnitClass);

    mp_TreeViewFctVars->get_column(0)->set_resizable();
    mp_TreeViewFctVars->get_column(1)->set_resizable();
    mp_TreeViewFctVars->get_column(2)->set_resizable();
    mp_TreeViewFctVars->get_column(3)->set_resizable();

    mp_TreeViewFctVars->get_column(0)->set_reorderable();
    mp_TreeViewFctVars->get_column(1)->set_reorderable();
    mp_TreeViewFctVars->get_column(2)->set_reorderable();
    mp_TreeViewFctVars->get_column(3)->set_reorderable();

    mp_TreeViewFctVars->expand_all();
  }

}


// =====================================================================
// =====================================================================


bool ModelAvailFct::isFunctionRowSelectable(const Glib::RefPtr<Gtk::TreeModel>& model, const Gtk::TreeModel::Path& path, bool /*path_currently_selected*/)
{
  const Gtk::TreeModel::iterator it = model->get_iter(path);

  return it->children().empty(); // only allow leaf nodes to be selected
}


// =====================================================================
// =====================================================================


void ModelAvailFct::createFctVarsTree(Glib::RefPtr<Gtk::TreeStore> TreeModel, Glib::ustring Title, std::vector<openfluid::base::SignatureHandledDataItem> Vars)
{
  if(!Vars.empty())
  {
    Gtk::TreeModel::Row TopRow = *(TreeModel->append());

    TopRow[m_ColumnsVar.m_FctVarName] = Title;

    for (unsigned int i=0 ; i<Vars.size() ; i++)
    {
      Gtk::TreeModel::Row Row = *(TreeModel->append(TopRow.children()));

      Row[m_ColumnsVar.m_FctVarName] = Vars[i].DataName;
      Row[m_ColumnsVar.m_FctVarUnit] = Vars[i].DataUnit;
      Row[m_ColumnsVar.m_FctVarUnitClass] = Vars[i].UnitClass;
      Row[m_ColumnsVar.m_FctVarDesc] = Vars[i].Description;
    }
  }
}

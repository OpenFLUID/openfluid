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

#include "ModelAvailFct.hpp"


// =====================================================================
// =====================================================================


ModelAvailFct::ModelAvailFct(Glib::RefPtr<Gtk::Builder> GladeBuilder)
{
  GladeBuilder->get_widget("TreeViewAvailFct",mp_TreeViewAvailFct);
  GladeBuilder->get_widget("NotebookAvailFct",mp_NotebookAvailFct);

  GladeBuilder->get_widget("LabelAvailFctId",mp_LabelAvailFctId);
  GladeBuilder->get_widget("LabelAvailFctName",mp_LabelAvailFctName);
  GladeBuilder->get_widget("LabelAvailFctDescription",mp_LabelAvailFctDescription);
  GladeBuilder->get_widget("LabelAvailFctVersion",mp_LabelAvailFctVersion);
  GladeBuilder->get_widget("LabelAvailFctSDKVersion",mp_LabelAvailFctSDKVersion);
  GladeBuilder->get_widget("LabelAvailFctStatus",mp_LabelAvailFctStatus);
  GladeBuilder->get_widget("LabelAvailFctDomain",mp_LabelAvailFctDomain);
  GladeBuilder->get_widget("LabelAvailFctProcess",mp_LabelAvailFctProcess);
  GladeBuilder->get_widget("LabelAvailFctMethod",mp_LabelAvailFctMethod);
  GladeBuilder->get_widget("LabelAvailFctAuthorName",mp_LabelAvailFctAuthorName);
  GladeBuilder->get_widget("LabelAvailFctAuthorEmail",mp_LabelAvailFctAuthorEmail);


  //Create the Tree model
  mp_TreeModelAvailFct = Gtk::TreeStore::create(m_Columns);

  // Add rows to the tree model
  Gtk::TreeModel::Row SimulFctRow = *(mp_TreeModelAvailFct->append());
  SimulFctRow[m_Columns.m_FunctionType] = _("Simulation Functions");

  openfluid::machine::ArrayOfModelItemInstance PlugContainers = openfluid::machine::PluginManager::getInstance()->getAvailableFunctions();

  for (unsigned int i=0;i<PlugContainers.size();i++)
  {
    if (PlugContainers[i]->SDKCompatible && PlugContainers[i]->Signature!=NULL)
    {
      boost::filesystem::path TmpPath(PlugContainers[i]->Filename);
      TmpPath = boost::filesystem::change_extension(TmpPath,"");

      Glib::ustring StatusStr="";
      switch(PlugContainers[i]->Signature->Status)
      {
        case openfluid::base::EXPERIMENTAL: StatusStr = "Experimental";
        break;
        case openfluid::base::BETA: StatusStr = "Beta";
        break;
        case openfluid::base::STABLE: StatusStr = "Stable";
        break;
      }

      Gtk::TreeModel::Row Row = *(mp_TreeModelAvailFct->append(SimulFctRow.children()));
      Row[m_Columns.m_FunctionType] = "";
      Row[m_Columns.m_FunctionId] = openfluid::tools::ReplaceEmptyString(TmpPath.leaf(),"(unknown simulation function)");
      Row[m_Columns.m_FunctionName] = PlugContainers[i]->Signature->Name;
      Row[m_Columns.m_FunctionDescription] = PlugContainers[i]->Signature->Description;
      Row[m_Columns.m_FunctionVersion] = PlugContainers[i]->Signature->Version;
      Row[m_Columns.m_FunctionSDKVersion] = PlugContainers[i]->Signature->SDKVersion;
      Row[m_Columns.m_FunctionStatus] = StatusStr;
      Row[m_Columns.m_FunctionDomain] = PlugContainers[i]->Signature->Domain;
      Row[m_Columns.m_FunctionProcess] = PlugContainers[i]->Signature->Process;
      Row[m_Columns.m_FunctionMethod] = PlugContainers[i]->Signature->Method;
      Row[m_Columns.m_FunctionAuthorName] = PlugContainers[i]->Signature->Author;
      Row[m_Columns.m_FunctionAuthorEmail] = PlugContainers[i]->Signature->AuthorEmail;

      openfluid::core::FuncParamsMap_t Params = PlugContainers[i]->Params;
      openfluid::core::FuncParamsMap_t::const_iterator ParamIt;
      for (ParamIt = Params.begin() ; ParamIt != Params.end() ; ++ ParamIt)
      {

        std::cout << ParamIt->first << std::endl;
      }
    }
  }

  // Add rows to the tree model, by Domain
  //  typedef std::map<Glib::ustring, openfluid::machine::ArrayOfModelItemInstance> PlugByDomain_t;
  //  PlugByDomain_t PlugByDomain;

  //  openfluid::machine::ArrayOfModelItemInstance PlugContainers = openfluid::machine::PluginManager::getInstance()->getAvailableFunctions();
  //
  //  for (unsigned int i=0;i<PlugContainers.size();i++)
  //  {
  //    if (PlugContainers[i]->SDKCompatible && PlugContainers[i]->Signature!=NULL)
  //      PlugByDomain[PlugContainers[i]->Signature->Domain].push_back(PlugContainers[i]);
  //  }
  //
  //  for (PlugByDomain_t::const_iterator it = PlugByDomain.begin() ; it != PlugByDomain.end() ; ++it)
  //  {
  //    Gtk::TreeModel::Row DomainRow = *(mp_TreeModelAvailFct->append(SimulFctRow.children()));
  //    DomainRow[m_Columns.m_FunctionType] = "";
  //    DomainRow[m_Columns.m_FunctionDomain] = it->first;
  //    DomainRow[m_Columns.m_FunctionId] = "";
  //    DomainRow[m_Columns.m_FunctionName] = "";
  //
  //    for(unsigned int i=0 ; i<it->second.size() ; i++)
  //    {
  //      Gtk::TreeModel::Row Row = *(mp_TreeModelAvailFct->append(DomainRow.children()));
  //
  //      boost::filesystem::path TmpPath(it->second[i]->Filename);
  //      TmpPath = boost::filesystem::change_extension(TmpPath,"");
  //
  //      Row[m_Columns.m_FunctionType] = "";
  //      Row[m_Columns.m_FunctionDomain] = "";
  //      Row[m_Columns.m_FunctionId] = openfluid::tools::ReplaceEmptyString(TmpPath.leaf(),"(unknown simulation function)");
  //      Row[m_Columns.m_FunctionName] = it->second[i]->Signature->Name;
  //    }
  //  }


  // Attach the tree model to the tree view
  mp_TreeViewAvailFct->set_model(mp_TreeModelAvailFct);

  // Add columns to the tree view
  typedef std::pair<Glib::ustring,Gtk::TreeModelColumn<Glib::ustring> > ColumnPair_t;
  std::vector<ColumnPair_t> Columns;
  Columns.push_back(std::make_pair("Type",m_Columns.m_FunctionType));
  Columns.push_back(std::make_pair("Id",m_Columns.m_FunctionId));
  Columns.push_back(std::make_pair("Domain",m_Columns.m_FunctionDomain));

  for (unsigned int i=0 ; i < Columns.size() ; i++)
  {
    mp_TreeViewAvailFct->append_column(Columns[i].first, Columns[i].second);
    Gtk::TreeView::Column * Column = mp_TreeViewAvailFct->get_column(i);
    Column->set_sort_column(Columns[i].second);
    Column->set_resizable();
  }

  Glib::RefPtr<Gtk::TreeSelection> TreeSelection = mp_TreeViewAvailFct->get_selection();

  // Only "leaf" rows can be selected
  TreeSelection->set_select_function(sigc::mem_fun(*this, &ModelAvailFct::isFunctionRowSelectable));

  TreeSelection->signal_changed().connect(sigc::mem_fun(*this, &ModelAvailFct::onAvalaibleFunctionSelected));

  mp_TreeViewAvailFct->expand_all();

  mp_TreeModelAvailFct->set_sort_column(m_Columns.m_FunctionId,Gtk::SORT_ASCENDING);

  // Select first available function
  Gtk::TreeModel::iterator sub = mp_TreeModelAvailFct->children()[0]->children()[0];
  if(sub)
    TreeSelection->select(sub);

}


// =====================================================================
// =====================================================================


ModelAvailFct::~ModelAvailFct()
{

}


// =====================================================================
// =====================================================================


void ModelAvailFct::onAvalaibleFunctionSelected()
{
  Gtk::TreeModel::iterator iter = mp_TreeViewAvailFct->get_selection()->get_selected();

  if(iter)
  {
    Gtk::TreeModel::Row row = *iter;

    mp_LabelAvailFctId->set_text(row[m_Columns.m_FunctionId]);
    mp_LabelAvailFctName->set_text(row[m_Columns.m_FunctionName]);
    mp_LabelAvailFctDescription->set_text(row[m_Columns.m_FunctionDescription]);

    mp_LabelAvailFctVersion->set_text(row[m_Columns.m_FunctionVersion]);
    mp_LabelAvailFctSDKVersion->set_text(row[m_Columns.m_FunctionSDKVersion]);
    mp_LabelAvailFctStatus->set_text(row[m_Columns.m_FunctionStatus]);

    mp_LabelAvailFctDomain->set_text(row[m_Columns.m_FunctionDomain]);
    mp_LabelAvailFctProcess->set_text(row[m_Columns.m_FunctionProcess]);
    mp_LabelAvailFctMethod->set_text(row[m_Columns.m_FunctionMethod]);
    mp_LabelAvailFctAuthorName->set_text(row[m_Columns.m_FunctionAuthorName]);
    mp_LabelAvailFctAuthorEmail->set_text(row[m_Columns.m_FunctionAuthorEmail]);
  }

}


// =====================================================================
// =====================================================================


bool ModelAvailFct::isFunctionRowSelectable(const Glib::RefPtr<Gtk::TreeModel>& model, const Gtk::TreeModel::Path& path, bool /*path_currently_selected*/)
{
  const Gtk::TreeModel::iterator it = model->get_iter(path);

  return it->children().empty(); // only allow leaf nodes to be selected
}

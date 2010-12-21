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
  \file SimulationOutput.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <iostream>

#include <glibmm/i18n.h>

#include "SimulationOutput.hpp"


// =====================================================================
// =====================================================================


SimulationOutput::SimulationOutput(Glib::RefPtr<Gtk::Builder> GladeBuilder, openfluid::base::OutputDescriptor & OutputDesc,
    ItemsStrByClassMap_t UnitsMap, ItemsStrByClassMap_t VarsMap)
: m_OutputDesc(OutputDesc)
{
  GladeBuilder->get_widget("TreeViewOutput",mp_TreeViewOutput);

  createComboFixedTreeModels();

  m_refUnitsModel = createComboByClassTreeModel(UnitsMap);
  m_refVarsModel = createComboByClassTreeModel(VarsMap);

  mp_MainTreeModel = createMainTreeModel();

  mp_TreeViewOutput->set_model(mp_MainTreeModel);

  initTreeView();
}


// =====================================================================
// =====================================================================


SimulationOutput::~SimulationOutput()
{

}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeStore> SimulationOutput::createMainTreeModel()
{
  Glib::RefPtr<Gtk::TreeStore> MainTreeModel = Gtk::TreeStore::create(m_OutputColumns);


  int FilesSize = m_OutputDesc.getFileSets().size();

  for(int i=0 ; i<FilesSize ; i++)
  {
    Gtk::TreeRow RowFile = *(MainTreeModel->append());

    openfluid::base::OutputFilesDescriptor OutputFileDesc = m_OutputDesc.getFileSets()[i];

    RowFile[m_OutputColumns.m_FileDesc] = &(m_OutputDesc.getFileSets()[i]);
    RowFile[m_OutputColumns.m_IsAFile] = true;
    RowFile[m_OutputColumns.m_IsASet] = false;

    RowFile[m_OutputColumns.m_FormatName] = Glib::ustring::compose("Format_%1",i+1);

    std::string Col = OutputFileDesc.getColSeparator();
    std::string::size_type Pos;
    while((Pos = Col.find(" ")) != std::string::npos)
      Col.replace(Pos,1,"[blank]");
    RowFile[m_OutputColumns.m_ColSep] = Col;

    RowFile[m_OutputColumns.m_DtFormat] = OutputFileDesc.getDateFormat();
    RowFile[m_OutputColumns.m_CommentChar] = OutputFileDesc.getCommentChar();

    RowFile[m_OutputColumns.m_ComboColSep] = m_refColSepModel;
    RowFile[m_OutputColumns.m_ComboDTFormat] = m_refDTFormatModel;
    RowFile[m_OutputColumns.m_ComboCommentChar] = m_refCommentCharModel;


    int SetSize = m_OutputDesc.getFileSets()[i].getSets().size();

    for(int j=0 ; j<SetSize ; j++)
    {
      Gtk::TreeRow RowSet = *(MainTreeModel->append(RowFile->children()));

      openfluid::base::OutputSetDescriptor OutputSetDesc = OutputFileDesc.getSets()[j];

      RowSet[m_OutputColumns.m_SetDesc] = &(m_OutputDesc.getFileSets()[i].getSets()[j]);

      RowSet[m_OutputColumns.m_IsAFile] = false;
      RowSet[m_OutputColumns.m_IsASet] = true;

      RowSet[m_OutputColumns.m_SetName] = OutputSetDesc.getName();
      RowSet[m_OutputColumns.m_UnitClass] = OutputSetDesc.getUnitsClass();

      Glib::RefPtr<Gtk::TreeModelFilter> refUnitsClassFilter = Gtk::TreeModelFilter::create(m_refUnitsModel);
      refUnitsClassFilter->set_visible_func(sigc::mem_fun(*this,&SimulationOutput::isUnitClassVisible));
      RowSet[m_OutputColumns.m_ComboUnitClass] = refUnitsClassFilter;

      if(OutputSetDesc.isAllUnits())
        RowSet[m_OutputColumns.m_UnitIDsList] = "*";
      else
        RowSet[m_OutputColumns.m_UnitIDsList] = vectorToString(OutputSetDesc.getUnitsIDs());

      if(OutputSetDesc.isAllScalars() && OutputSetDesc.isAllVectors())
        RowSet[m_OutputColumns.m_VarsList] = "*";
      else
      {
        Glib::ustring StrScalar = "";
        Glib::ustring StrVector = "";

        StrScalar = vectorToString(OutputSetDesc.getScalars());
        StrVector = vectorToString(OutputSetDesc.getVectors(), true);

        Glib::ustring Sep = (StrScalar != "" && StrVector != "") ?  ";" : "";

        RowSet[m_OutputColumns.m_VarsList] = Glib::ustring::compose("%1%2%3",StrScalar,Sep,StrVector);
      }

      RowSet[m_OutputColumns.m_Precision] = OutputSetDesc.getPrecision();
    }
  }

  return MainTreeModel;
}


// =====================================================================
// =====================================================================


void SimulationOutput::createComboFixedTreeModels()
{
  std::vector<Glib::ustring> Chars;
  Chars.push_back(":");
  Chars.push_back("%");
  Chars.push_back(";");
  Chars.push_back(",");
  Chars.push_back("#");
  Chars.push_back("*");
  Chars.push_back("[blank]");

  std::vector<Glib::ustring> DTFormats;
  DTFormats.push_back("%Y %m %d %H %M %S");
  DTFormats.push_back("%d/%m/%Y %H:%M:%S");
  DTFormats.push_back("%Y-%m-%d %H.%M");


  m_refCommentCharModel = Gtk::ListStore::create(m_ComboColumns);
  m_refColSepModel = Gtk::ListStore::create(m_ComboColumns);
  m_refDTFormatModel = Gtk::ListStore::create(m_ComboColumns);

  Gtk::TreeRow Row;
  unsigned int i;

  for(i=0 ; i<Chars.size() ; i++)
  {
    Row = *(m_refColSepModel->append());
    Row[m_ComboColumns.m_Value] = Chars[i];

    Row = *(m_refCommentCharModel->append());
    Row[m_ComboColumns.m_Value] = Chars[i];
  }

  for(i=0 ; i<DTFormats.size() ; i++)
  {
    Row = *(m_refDTFormatModel->append());
    Row[m_ComboColumns.m_Value] = DTFormats[i];
  }

}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeStore> SimulationOutput::createComboByClassTreeModel(ItemsStrByClassMap_t ItemsByClassMap)
{
  Glib::RefPtr<Gtk::TreeStore> ModelFilter = Gtk::TreeStore::create(m_ComboColumns);

  ItemsStrByClassMap_t::iterator itClass;

  Gtk::TreeRow RowClass;
  Gtk::TreeRow RowItem;

  for(itClass=ItemsByClassMap.begin() ; itClass!=ItemsByClassMap.end() ; ++itClass)
  {
    RowClass = *(ModelFilter->append());
    RowClass[m_ComboColumns.m_Value] = itClass->first;

    std::vector<std::string> ItemsVect = itClass->second;
    int size = ItemsVect.size();

    for(int i=0; i<size ; i++)
    {
      RowItem = *(ModelFilter->append(RowClass->children()));

      RowItem[m_ComboColumns.m_Value] = ItemsVect[i];
    }
  }

  return ModelFilter;
}


// =====================================================================
// =====================================================================


void SimulationOutput::initTreeView()
{
  // OutputFilesDescriptor Format Name (not stored)
  Gtk::TreeView::Column * pColFormatName = Gtk::manage(new Gtk::TreeView::Column("") );
  Gtk::CellRendererText * pRendFormatName = Gtk::manage(new Gtk::CellRendererText);
  pColFormatName->pack_start(*pRendFormatName);
  mp_TreeViewOutput->append_column(*pColFormatName);
  pColFormatName->add_attribute(pRendFormatName->property_text(),m_OutputColumns.m_FormatName);
  pColFormatName->add_attribute(pRendFormatName->property_editable(), m_OutputColumns.m_IsAFile);
  pColFormatName->set_resizable(true);

  // OutputFilesDescriptor Column Separator
  Gtk::TreeView::Column * pColColSep = Gtk::manage(new Gtk::TreeView::Column("Column Separator") );
  Gtk::CellRendererCombo * pRendColSep = Gtk::manage(new Gtk::CellRendererCombo);
  pColColSep->pack_start(*pRendColSep);
  mp_TreeViewOutput->append_column(*pColColSep);
  pColColSep->add_attribute(pRendColSep->property_text(),m_OutputColumns.m_ColSep);
  pColColSep->add_attribute(pRendColSep->property_model(), m_OutputColumns.m_ComboColSep);
  pRendColSep->property_text_column() = 0;
  pColColSep->add_attribute(pRendColSep->property_editable(), m_OutputColumns.m_IsAFile);
  pColColSep->set_resizable(true);
  pRendColSep->signal_edited().connect(
      sigc::mem_fun(*this,&SimulationOutput::onColSepEdited));

  // OutputFilesDescriptor Date Time Format
  Gtk::TreeView::Column * pColDTFormat = Gtk::manage(new Gtk::TreeView::Column("Date Format") );
  Gtk::CellRendererCombo * pRendDTFormat = Gtk::manage(new Gtk::CellRendererCombo);
  pColDTFormat->pack_start(*pRendDTFormat);
  mp_TreeViewOutput->append_column(*pColDTFormat);
  pColDTFormat->add_attribute(pRendDTFormat->property_text(),m_OutputColumns.m_DtFormat);
  pColDTFormat->add_attribute(pRendDTFormat->property_model(), m_OutputColumns.m_ComboDTFormat);
  pRendDTFormat->property_text_column() = 0;
  pColDTFormat->add_attribute(pRendDTFormat->property_editable(), m_OutputColumns.m_IsAFile);
  pColDTFormat->set_resizable(true);
  pRendDTFormat->signal_edited().connect(
        sigc::mem_fun(*this,&SimulationOutput::onDTFormatEdited));

  // OutputFilesDescriptor Comment Char
  Gtk::TreeView::Column * pColCommentChar = Gtk::manage(new Gtk::TreeView::Column("Comment Char") );
  Gtk::CellRendererCombo * pRendCommentChar = Gtk::manage(new Gtk::CellRendererCombo);
  pColCommentChar->pack_start(*pRendCommentChar);
  mp_TreeViewOutput->append_column(*pColCommentChar);
  pColCommentChar->add_attribute(pRendCommentChar->property_text(),m_OutputColumns.m_CommentChar);
  pColCommentChar->add_attribute(pRendCommentChar->property_model(), m_OutputColumns.m_ComboCommentChar);
  pRendCommentChar->property_text_column() = 0;
  pColCommentChar->add_attribute(pRendCommentChar->property_editable(), m_OutputColumns.m_IsAFile);
  pColCommentChar->set_resizable(true);
  pRendCommentChar->signal_edited().connect(
      sigc::mem_fun(*this,&SimulationOutput::onCommentCharEdited));

  // OutputSetDescriptor Name
  Gtk::TreeView::Column * pColSetName = Gtk::manage(new Gtk::TreeView::Column("Set Name") );
  Gtk::CellRendererText * pRendSetName = Gtk::manage(new Gtk::CellRendererText);
  pColSetName->pack_start(*pRendSetName);
  mp_TreeViewOutput->append_column(*pColSetName);
  pColSetName->add_attribute(pRendSetName->property_text(),m_OutputColumns.m_SetName);
  pColSetName->add_attribute(pRendSetName->property_editable(), m_OutputColumns.m_IsASet);
  pColSetName->set_resizable(true);
  pRendSetName->signal_edited().connect(
      sigc::mem_fun(*this,&SimulationOutput::onSetNameEdited));

  // OutputFilesDescriptor Unit Class
  Gtk::TreeView::Column * pColUnitClass = Gtk::manage(new Gtk::TreeView::Column("Unit Class") );
  Gtk::CellRendererCombo * pRendUnitClass = Gtk::manage(new Gtk::CellRendererCombo);
  pColUnitClass->pack_start(*pRendUnitClass);
  mp_TreeViewOutput->append_column(*pColUnitClass);
  pColUnitClass->add_attribute(pRendUnitClass->property_text(),m_OutputColumns.m_UnitClass);
  pColUnitClass->add_attribute(pRendUnitClass->property_model(), m_OutputColumns.m_ComboUnitClass);
  pRendUnitClass->property_text_column() = 0;
  pColUnitClass->add_attribute(pRendUnitClass->property_editable(), m_OutputColumns.m_IsASet);
  pColUnitClass->set_resizable(true);
  pRendUnitClass->signal_edited().connect(
      sigc::mem_fun(*this,&SimulationOutput::onUnitClassEdited));

  // OutputFilesDescriptor Unit IDs
  Gtk::TreeView::Column * pColUnitIDs = Gtk::manage(new Gtk::TreeView::Column("Unit IDs") );
  Gtk::CellRendererText * pRendUnitIDs = Gtk::manage(new Gtk::CellRendererText);
  pColUnitIDs->pack_start(*pRendUnitIDs);
  mp_TreeViewOutput->append_column(*pColUnitIDs);
  pColUnitIDs->add_attribute(pRendUnitIDs->property_text(),m_OutputColumns.m_UnitIDsList);
  pColUnitIDs->add_attribute(pRendUnitIDs->property_editable(), m_OutputColumns.m_IsASet);
  pColUnitIDs->set_resizable(true);
//  pColUnitIDs->set_max_width(200);
  pRendUnitIDs->signal_editing_started().connect(
      sigc::mem_fun(*this,&SimulationOutput::onUnitIDsEditingStarted));

  // OutputFilesDescriptor Vars
  Gtk::TreeView::Column * pColVars = Gtk::manage(new Gtk::TreeView::Column("Variables") );
  Gtk::CellRendererText * pRendVars = Gtk::manage(new Gtk::CellRendererText);
  pColVars->pack_start(*pRendVars);
  mp_TreeViewOutput->append_column(*pColVars);
  pColVars->add_attribute(pRendVars->property_text(),m_OutputColumns.m_VarsList);
  pColVars->add_attribute(pRendVars->property_editable(), m_OutputColumns.m_IsASet);
  pColVars->set_resizable(true);
  //  pColVars->set_max_width(200);
  pRendVars->signal_editing_started().connect(
      sigc::mem_fun(*this,&SimulationOutput::onVarsEditingStarted));

  // OutputFilesDescriptor Precision
  Gtk::TreeView::Column * pColSetPrecision = Gtk::manage(new Gtk::TreeView::Column("Precision") );
  Gtk::CellRendererText * pRendSetPrecision = Gtk::manage(new Gtk::CellRendererText);
  pColSetPrecision->pack_start(*pRendSetPrecision);
  mp_TreeViewOutput->append_column(*pColSetPrecision);
  pColSetPrecision->add_attribute(pRendSetPrecision->property_text(),m_OutputColumns.m_Precision);
  pColSetPrecision->add_attribute(pRendSetPrecision->property_editable(), m_OutputColumns.m_IsASet);
  pColSetPrecision->set_resizable(true);
  pRendSetPrecision->signal_edited().connect(
      sigc::mem_fun(*this,&SimulationOutput::onSetPrecisionEdited));

  mp_TreeViewOutput->expand_all();
}


// =====================================================================
// =====================================================================


Glib::ustring SimulationOutput::vectorToString(std::vector<std::string> vect, bool VectorValues)
{
  Glib::ustring Str = "";
  Glib::ustring VectorSuffix = "";

  if(VectorValues)
    VectorSuffix = "[]";

  int Size = vect.size();

  if(Size > 0)
    Str = Glib::ustring::compose("%1%2",vect[0],VectorSuffix);

  for(int i=1 ; i<Size ; i++)
  {
    Str += Glib::ustring::compose(";%1%2",vect[i],VectorSuffix);
  }

  return Str;
}


// =====================================================================
// =====================================================================


Glib::ustring SimulationOutput::vectorToString(std::vector<unsigned int> vect)
{
  Glib::ustring Str = "";

  int Size = vect.size();

  if(Size > 0)
    Str = Glib::ustring::compose("%1",vect[0]);

  for(int i=1 ; i<Size ; i++)
  {
    Str += Glib::ustring::compose(";%1",vect[i]);
  }

  return Str;
}


// =====================================================================
// =====================================================================


bool SimulationOutput::isUnitClassVisible(const Gtk::TreeModel::const_iterator & Iter)
{
  // return true only for root nodes (unit class nodes)
  Gtk::TreeRow Row = *Iter;

  if(Row.parent())
    return false;

  return true;
}


// =====================================================================
// =====================================================================


void SimulationOutput::onColSepEdited(const Glib::ustring PathString, const Glib::ustring NewText)
{
  Gtk::TreePath Path(PathString);

  Gtk::TreeModel::iterator Iter = mp_MainTreeModel->get_iter(Path);

  if(Iter)
  {
    Gtk::TreeRow Row = *Iter;

    Row[m_OutputColumns.m_ColSep] = NewText;

    openfluid::base::OutputFilesDescriptor * OutputDesc = Row[m_OutputColumns.m_FileDesc];

    if(NewText == "[blank]")
      OutputDesc->setColSeparator(" ");
    else
      OutputDesc->setColSeparator(NewText);

  }
}


// =====================================================================
// =====================================================================


void SimulationOutput::onDTFormatEdited(const Glib::ustring PathString, const Glib::ustring NewText)
{
  Gtk::TreePath Path(PathString);

  Gtk::TreeModel::iterator Iter = mp_MainTreeModel->get_iter(Path);

  if(Iter)
  {
    Gtk::TreeRow Row = *Iter;

    Row[m_OutputColumns.m_DtFormat] = NewText;

    openfluid::base::OutputFilesDescriptor * OutputDesc = Row[m_OutputColumns.m_FileDesc];

    OutputDesc->setDateFormat(NewText);

  }
}


// =====================================================================
// =====================================================================


void SimulationOutput::onCommentCharEdited(const Glib::ustring PathString, const Glib::ustring NewText)
{
  Gtk::TreePath Path(PathString);

  Gtk::TreeModel::iterator Iter = mp_MainTreeModel->get_iter(Path);

  if(Iter)
  {
    Gtk::TreeRow Row = *Iter;

    Row[m_OutputColumns.m_CommentChar] = NewText;

    openfluid::base::OutputFilesDescriptor * OutputDesc = Row[m_OutputColumns.m_FileDesc];

    if(NewText == "[blank]")
      OutputDesc->setCommentChar(" ");
    else
      OutputDesc->setCommentChar(NewText);

  }
}


// =====================================================================
// =====================================================================


void SimulationOutput::onSetNameEdited(const Glib::ustring PathString, const Glib::ustring NewText)
{
  Gtk::TreePath Path(PathString);

  Gtk::TreeModel::iterator Iter = mp_MainTreeModel->get_iter(Path);

  if(Iter)
  {
    Gtk::TreeRow Row = *Iter;

    Row[m_OutputColumns.m_SetName] = NewText;

    openfluid::base::OutputSetDescriptor * OutputDesc = Row[m_OutputColumns.m_SetDesc];

    OutputDesc->setName(NewText);

  }
}


// =====================================================================
// =====================================================================


void SimulationOutput::onUnitClassEdited(const Glib::ustring PathString, const Glib::ustring NewText)
{
  Gtk::TreePath Path(PathString);

  Gtk::TreeModel::iterator Iter = mp_MainTreeModel->get_iter(Path);

  if(Iter)
  {
    Gtk::TreeRow Row = *Iter;

    Row[m_OutputColumns.m_UnitClass] = NewText;

    openfluid::base::OutputSetDescriptor * OutputDesc = Row[m_OutputColumns.m_SetDesc];

    OutputDesc->setUnitsClass(NewText);

  }
}


// =====================================================================
// =====================================================================


void SimulationOutput::onUnitIDsEditingStarted(Gtk::CellEditable * /*Editable*/, const Glib::ustring & PathString)
{
  bool HasChanged;

  std::pair<bool,std::vector<std::string> > DialogRes = editListInDialogBox(PathString,"IDs","ID Selection",m_refUnitsModel,m_OutputColumns.m_UnitIDsList,HasChanged);


  if(HasChanged)
  {
    Gtk::TreePath Path(PathString);

    Gtk::TreeModel::iterator Iter = mp_MainTreeModel->get_iter(Path);

    if(Iter)
    {
      Gtk::TreeRow Row = *Iter;

      openfluid::base::OutputSetDescriptor * OutputDesc = Row[m_OutputColumns.m_SetDesc];

      if(DialogRes.first)
      {
        Row[m_OutputColumns.m_UnitIDsList] = "*";

        OutputDesc->setAllUnits(true);
      }
      else
      {
        std::vector<std::string> IDs = DialogRes.second;

        Row[m_OutputColumns.m_UnitIDsList] = vectorToString(IDs);

        OutputDesc->setAllUnits(false);

        //TODO: Change this way of doing !

        OutputDesc->getUnitsIDs().clear();

        for(unsigned int i=0 ; i<IDs.size() ; i++)
        {
          std::stringstream ss(IDs[i]);
          int ValInt;
          ss >> ValInt;

          OutputDesc->getUnitsIDs().push_back(ValInt);
        }

      }

    }

  }

}


// =====================================================================
// =====================================================================


void SimulationOutput::onVarsEditingStarted(Gtk::CellEditable * /*Editable*/, const Glib::ustring & PathString)
{
  bool HasChanged;

  std::pair<bool,std::vector<std::string> > DialogRes = editListInDialogBox(PathString,"Variables","Variable Selection",m_refVarsModel,m_OutputColumns.m_VarsList,HasChanged);


  if(HasChanged)
  {
    Gtk::TreePath Path(PathString);

    Gtk::TreeModel::iterator Iter = mp_MainTreeModel->get_iter(Path);

    if(Iter)
    {
      Gtk::TreeRow Row = *Iter;

      openfluid::base::OutputSetDescriptor * OutputDesc = Row[m_OutputColumns.m_SetDesc];

      if(DialogRes.first)
      {
        Row[m_OutputColumns.m_VarsList] = "*";

        OutputDesc->setAllScalars(true);
        OutputDesc->setAllVectors(true);
      }
      else
      {
        std::vector<std::string> Vars = DialogRes.second;

        Row[m_OutputColumns.m_VarsList] = vectorToString(Vars);

        OutputDesc->setAllScalars(false);
        OutputDesc->setAllVectors(false);

        OutputDesc->getScalars().clear();
        OutputDesc->getVectors().clear();

        for(unsigned int i=0 ; i<Vars.size() ; i++)
        {
          if(openfluid::tools::IsVectorNamedVariable(Vars[i]))
            OutputDesc->getVectors().push_back(openfluid::tools::GetVectorNamedVariableName(Vars[i]));
          else
            OutputDesc->getScalars().push_back(Vars[i]);
        }
      }

    }

  }
}


// =====================================================================
// =====================================================================


void SimulationOutput::onSetPrecisionEdited(const Glib::ustring PathString, const Glib::ustring NewText)
{
  Gtk::TreePath Path(PathString);

  Gtk::TreeModel::iterator Iter = mp_MainTreeModel->get_iter(Path);

  if(Iter)
  {
    Gtk::TreeRow Row = *Iter;

    std::stringstream ss(NewText);
    int ValInt;
    ss >> ValInt;

    Row[m_OutputColumns.m_Precision] = ValInt;

    openfluid::base::OutputSetDescriptor * OutputDesc = Row[m_OutputColumns.m_SetDesc];

    OutputDesc->setPrecision(ValInt);

  }
}


// =====================================================================
// =====================================================================


std::pair<bool,std::vector<std::string> > SimulationOutput::editListInDialogBox(const Glib::ustring & PathString, Glib::ustring ColumnHeader, Glib::ustring DialogTitle,
    Glib::RefPtr<Gtk::TreeModel> ListModel, Gtk::TreeModelColumn<std::string> MainModelListColumn, bool & HasChanged)
{
  std::vector<std::string> ItemsVectNew;
  bool AllItems = false;
  HasChanged = false;

  Gtk::TreePath Path(PathString);

  Gtk::TreeModel::iterator Iter = mp_MainTreeModel->get_iter(Path);


  if(Iter)
  {
    Gtk::TreeRow Row = *Iter;

    std::string UnitClass = Row[m_OutputColumns.m_UnitClass];

    Gtk::TreeModel::Children Classes = ListModel->children();
    Gtk::TreeModel::Children::iterator itClass;

    for(itClass=Classes.begin() ; itClass!=Classes.end() ; ++itClass)
    {
      if(itClass->get_value(m_ComboColumns.m_Value) == UnitClass)
      {
        Gtk::TreePath ClassPath(itClass);

        // filter List model to keep this UnitClass ID only
        Glib::RefPtr<Gtk::TreeModelFilter> refModelFilter = Gtk::TreeModelFilter::create(ListModel,ClassPath);

        // create the TreeView
        Gtk::TreeView * View = Gtk::manage(new Gtk::TreeView(refModelFilter));

        View->append_column(ColumnHeader,m_ComboColumns.m_Value);

        View->set_visible(true);

        Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = View->get_selection();

        refTreeSelection->set_mode(Gtk::SELECTION_MULTIPLE);

        // set selected items
        std::string ItemListStr =  Row[MainModelListColumn];

        Gtk::TreeModel::Children Items = refModelFilter->children();
        Gtk::TreeModel::Children::iterator itItems;

        if(ItemListStr == "*")
        {
          refTreeSelection->select_all();
        }
        else
        {
          std::vector<std::string> ItemsVect = openfluid::tools::SplitString(ItemListStr,";",false);

          for(itItems=Items.begin() ; itItems!=Items.end() ; ++itItems)
          {

            if(std::find(ItemsVect.begin(),ItemsVect.end(),itItems->get_value(m_ComboColumns.m_Value)) != ItemsVect.end())
              refTreeSelection->select(itItems);
          }
        }

        // display view in a Dialog Box
        Gtk::Dialog Dialog(DialogTitle, true, true);

        Dialog.get_vbox()->pack_start(*View);

        Dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
        Dialog.add_button(Gtk::Stock::APPLY,Gtk::RESPONSE_OK);

        if(Dialog.run() == Gtk::RESPONSE_OK)
        {
          HasChanged = true;

          std::list<Gtk::TreeModel::Path> SelectedPaths = refTreeSelection->get_selected_rows();
          std::list<Gtk::TreeModel::Path>::iterator itPath;

          for(itPath=SelectedPaths.begin() ; itPath!=SelectedPaths.end() ; ++itPath)
          {
            Gtk::TreeIter itSelect = refModelFilter->get_iter(*itPath);

            ItemsVectNew.push_back(itSelect->get_value(m_ComboColumns.m_Value));
          }

          if(refTreeSelection->count_selected_rows() == (int)Items.size())
            AllItems = true;

        }

        break;
      }

    }

  }

  return std::make_pair(AllItems,ItemsVectNew);
}


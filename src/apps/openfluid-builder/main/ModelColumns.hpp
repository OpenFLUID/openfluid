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
 \file ModelColumns.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */


#ifndef __MODELCOLUMNS_HPP__
#define __MODELCOLUMNS_HPP__

#include <gtkmm.h>

#include <openfluid/machine.hpp>


// =====================================================================
// =====================================================================


class ModelColumns : public Gtk::TreeModel::ColumnRecord
{
  public:

  // each RowType 1<7 has a tab in Notebook of Available functions (page num is RowType value)
  enum RowType {  SimulationFunctions = 100,
                  Generators = 200,
                  Parameters = 1,
                  InputData = 2,
                  Variables = 3,
                  Events = 4,
                  ExtraFiles = 5,
                  SpatialUnits = 6 };

  enum GeneratorType { Fixed, Random, Interp, MaxGeneratorType/*for iter only*/ };


  ModelColumns()
    { add(m_Type); add(m_IsTitle); add(m_Id); add(m_FunctionName); add(m_FunctionPath); add(m_Description);
    add(m_FunctionVersion); add(m_FunctionStatusStr);add(m_FunctionStatusIcon); add(m_FunctionDomain);
    add(m_FunctionProcess); add(m_FunctionMethod); add(m_FunctionAuthorName); add(m_FunctionAuthorEmail);
    add(m_HandleDataUnitClass); add(m_HandleDataUnit); add(m_ModelItemInstance); add(m_NotebookParamsPage); }

  Gtk::TreeModelColumn<RowType> m_Type;
  Gtk::TreeModelColumn<bool> m_IsTitle;

  Gtk::TreeModelColumn<Glib::ustring> m_Id;
  Gtk::TreeModelColumn<Glib::ustring> m_Description;

  Gtk::TreeModelColumn<Glib::ustring> m_FunctionName;
  Gtk::TreeModelColumn<Glib::ustring> m_FunctionPath;
  Gtk::TreeModelColumn<Glib::ustring> m_FunctionVersion;
  Gtk::TreeModelColumn<Glib::ustring> m_FunctionStatusStr;
  Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_FunctionStatusIcon;
  Gtk::TreeModelColumn<Glib::ustring> m_FunctionDomain;
  Gtk::TreeModelColumn<Glib::ustring> m_FunctionProcess;
  Gtk::TreeModelColumn<Glib::ustring> m_FunctionMethod;
  Gtk::TreeModelColumn<Glib::ustring> m_FunctionAuthorName;
  Gtk::TreeModelColumn<Glib::ustring> m_FunctionAuthorEmail;

  Gtk::TreeModelColumn<Glib::ustring> m_HandleDataUnitClass;
  Gtk::TreeModelColumn<Glib::ustring> m_HandleDataUnit;

  Gtk::TreeModelColumn<openfluid::machine::ModelItemInstance *> m_ModelItemInstance;

  Gtk::TreeModelColumn<Gtk::Widget *> m_NotebookParamsPage;

};


#endif /* __MODELCOLUMNS_HPP__ */

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
 \file DomainIDataView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainIDataView.hpp"


#include "DomainIDataColumns.hpp"

void DomainIDataViewImpl::onDataEdited(const Glib::ustring /*PathString*/,
    const Glib::ustring NewText, std::string DataName)
{
  m_EditedDataInfo = std::make_pair(DataName, NewText);
  m_signal_DataEdited.emit();
}

DomainIDataViewImpl::DomainIDataViewImpl() :
  m_EditedDataInfo(std::make_pair("", ""))
{
}
sigc::signal<void> DomainIDataViewImpl::signal_ClassSelectionChanged()
{
  return BuilderByClassTreeView::signal_ClassSelectionChanged();
}
sigc::signal<void> DomainIDataViewImpl::signal_DataEdited()
{
  return m_signal_DataEdited;
}
sigc::signal<void> DomainIDataViewImpl::signal_UnitSelectionChanged()
{
  return BuilderByClassTreeView::signal_UnitSelectionChanged();
}
void DomainIDataViewImpl::setClassesTreeModel(
    Glib::RefPtr<Gtk::TreeModel> ClassesModel)
{
  BuilderByClassTreeView::setClassesTreeModel(ClassesModel);
}
void DomainIDataViewImpl::setUnitsTreeColumns(DomainIDataColumns* Columns)
{
  mp_UnitsView->remove_all_columns();

  if (Columns)
  {
    mp_UnitsView->append_column("Id", *Columns->getIdColumn());
    mp_UnitsView->get_column(0)->set_sort_column(*Columns->getIdColumn());

    std::map<std::string, Gtk::TreeModelColumn<std::string>*> ColumnsByTitle =
        Columns->getByTitleColumns();

    for (std::map<std::string, Gtk::TreeModelColumn<std::string>*>::iterator
        it = ColumnsByTitle.begin(); it != ColumnsByTitle.end(); ++it)
    {
      int ColNb = mp_UnitsView->append_column_editable(it->first, *it->second);
      mp_UnitsView->get_column(ColNb - 1)->set_sort_column(*it->second);

      Gtk::CellRendererText * CellRend =
          (Gtk::CellRendererText *) mp_UnitsView->get_column_cell_renderer(
              ColNb - 1);
      CellRend->signal_edited().connect(sigc::bind<std::string>(sigc::mem_fun(
          *this, &DomainIDataViewImpl::onDataEdited), it->first), false); // after=false for the value being updated before the view sort
    }
  }
}
void DomainIDataViewImpl::setUnitsTreeModel(
    Glib::RefPtr<Gtk::TreeModel> UnitsModel)
{
  BuilderByClassTreeView::setUnitsTreeModel(UnitsModel);
}
void DomainIDataViewImpl::requestClassSelection(Gtk::TreeIter Iter)
{
  BuilderByClassTreeView::requestClassSelection(Iter);
}
void DomainIDataViewImpl::requestUnitSelection(Gtk::TreeIter Iter)
{
  BuilderByClassTreeView::requestUnitSelection(Iter);
}
Gtk::TreeIter DomainIDataViewImpl::getSelectedClassIter()
{
  return BuilderByClassTreeView::getSelectedClassIter();
}
Gtk::TreeIter DomainIDataViewImpl::getSelectedUnitIter()
{
  return BuilderByClassTreeView::getSelectedUnitIter();
}
std::pair<std::string, std::string> DomainIDataViewImpl::getEditedDataInfo()
{
  return m_EditedDataInfo;
}
Gtk::Widget* DomainIDataViewImpl::asWidget()
{
  return BuilderByClassTreeView::asWidget();
}

int DomainIDataViewSub::getClassesViewRowCount()
{
  if (mp_ClassesView->get_model())
    return mp_ClassesView->get_model()->children().size();
  return 0;
}
int DomainIDataViewSub::getUnitsViewRowCount()
{
  if (mp_UnitsView->get_model())
    return mp_UnitsView->get_model()->children().size();
  return 0;
}
std::string DomainIDataViewSub::getSelectedClassName()
{
  if (getSelectedClassIter())
    return getSelectedClassIter()->get_value(m_ClassColumns.m_Class);
  return "";
}
int DomainIDataViewSub::getSelectedUnitId()
{
  int Id = -1;
  if (getSelectedUnitIter())
    getSelectedUnitIter()->get_value(0, Id);
  return Id;
}
void DomainIDataViewSub::selectClassWithIndex(int Index)
{
  requestClassSelection(mp_ClassesView->get_model()->children()[Index]);
}
void DomainIDataViewSub::selectUnitWithIndex(int Index)
{
  requestUnitSelection(mp_UnitsView->get_model()->children()[Index]);
}
void DomainIDataViewSub::setEditedDataInfo(std::string DataName,
    std::string NewValue)
{
  m_EditedDataInfo = std::make_pair(DataName, NewValue);
}
int DomainIDataViewSub::getUnitsColumnCount()
{
  return mp_UnitsView->get_columns().size();
}

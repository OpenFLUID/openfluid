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

#ifndef SIMULOUTSETSADAPTERMODEL_HPP_
#define SIMULOUTSETSADAPTERMODEL_HPP_

#include <gtkmm.h>

#include <openfluid/base/OutputSetDescriptor.hpp>

#include "SimulOutSetsColumns.hpp"

class BuilderListStore;

class SimulOutSetsAdapterModel
{
  public:
    virtual void setSets(std::map<std::string, std::pair<std::string,
        openfluid::base::OutputSetDescriptor> > SetsByName) = 0;
    virtual Glib::RefPtr<Gtk::TreeModel> getTreeModel() = 0;
    virtual void setSelectedSet(Gtk::TreeIter SelectedIter) = 0;
    virtual std::string getSelectedSetName() = 0;
};

class SimulOutSetsAdapterModelImpl: public SimulOutSetsAdapterModel
{
  private:
    SimulOutSetsColumns m_Columns;
    Glib::RefPtr<BuilderListStore> m_refListStore;
    Gtk::TreeRowReference* m_SelectedRowRef;
      protected:
    /* leave templates in header files for boost tests to compile */
    template<class T>
    Glib::ustring getStringListFromVect(std::vector<T> Vect, bool VectorValues =
        false)
    {
      Glib::ustring Str = "";
      Glib::ustring VectorSuffix = "";

      if (VectorValues)
        VectorSuffix = "[]";

      int Size = Vect.size();

      if (Size > 0)
        Str = Glib::ustring::compose("%1%2", Vect[0], VectorSuffix);

      for (int i = 1; i < Size; i++)
        Str += Glib::ustring::compose(";%1%2", Vect[i], VectorSuffix);

      return Str;
    }
    ;
    std::string extractIDsString(
        openfluid::base::OutputSetDescriptor OutSetDesc);
    std::string extractVariablesString(
        openfluid::base::OutputSetDescriptor OutSetDesc);
  public:
    SimulOutSetsAdapterModelImpl();
    void setSets(std::map<std::string, std::pair<std::string,
        openfluid::base::OutputSetDescriptor> > SetsByName);
    Glib::RefPtr<Gtk::TreeModel> getTreeModel();
    void setSelectedSet(Gtk::TreeIter SelectedIter);
    std::string getSelectedSetName();
};

class SimulOutSetsAdapterModelSub: public SimulOutSetsAdapterModelImpl
{
  public:
    template<class T>
    Glib::ustring getStringListFromVect(std::vector<T> Vect, bool VectorValues =
        false)
    {
      return SimulOutSetsAdapterModelImpl::getStringListFromVect(Vect, VectorValues);
    };
    std::string extractIDsString(
        openfluid::base::OutputSetDescriptor OutSetDesc);
    std::string extractVariablesString(
        openfluid::base::OutputSetDescriptor OutSetDesc);
};

#endif /* SIMULOUTSETSADAPTERMODEL_HPP_ */

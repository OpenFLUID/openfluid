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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MODELINSTANCE_HPP__
#define __MODELINSTANCE_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/core.hpp>
#include <openfluid/base.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/machine/SimulationBlob.hpp>


namespace openfluid { namespace machine {


class DLLEXPORT ModelInstance
{
  private:

    std::list<ModelItemInstance*> m_ModelItems;

    openfluid::machine::MachineListener* mp_Listener;

    openfluid::machine::SimulationBlob& m_SimulationBlob;

  public:

    ModelInstance(openfluid::machine::SimulationBlob& SimulationBlob,
                  openfluid::machine::MachineListener* Listener);

    ~ModelInstance();

    void appendItem(ModelItemInstance* ItemInstance);

    /**
      Insert a ModelItemInstance before the given postion (positions starts at index 0)
      @param[in] ItemInstance the ModelItemInstance to insert
      @param[in] Position the position
    */
    void insertItem(ModelItemInstance* ItemInstance, unsigned int Position);

    /**
      Delete the ModelItemInstance located at the given postion (positions starts at index 0), and remove it from the ModelInstance
      @param[in] Position the position
    */
    void deleteItem(unsigned int Position);

    void clear();

    unsigned int getItemsCount() const { return m_ModelItems.size(); };

    const std::list<ModelItemInstance*>& getItems() const { return m_ModelItems; };

    void initLoggers() const;

    bool call_initParams() const;

    bool call_prepareData() const;

    bool call_checkConsistency() const;

    bool call_initializeRun(const openfluid::base::SimulationInfo* SimInfo) const;

    bool call_runStep(const openfluid::base::SimulationStatus* SimStatus) const;

    bool call_finalizeRun(const openfluid::base::SimulationInfo* SimInfo) const;

};


} } //namespaces


#endif /* __MODELINSTANCE_H__ */

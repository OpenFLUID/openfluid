/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file DomainFactory.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/machine/DomainFactory.hpp>
#include <openfluid/core.hpp>
#include <openfluid/base.hpp>
#include <openfluid/tools.hpp>


namespace openfluid { namespace machine {


// =====================================================================
// =====================================================================


DomainFactory::DomainFactory()
{
}


// =====================================================================
// =====================================================================


void DomainFactory::buildDomainFromDescriptor(openfluid::base::DomainDescriptor& Descriptor)
{
  openfluid::core::CoreRepository* Repository = openfluid::core::CoreRepository::getInstance();
  openfluid::base::ExecutionMessages* ExecMsgs = openfluid::base::ExecutionMessages::getInstance();

  // ============== Domain definition ==============

  std::list<openfluid::base::UnitDescriptor>::iterator itUnits;
  std::list<openfluid::core::UnitClassID_t>::iterator itLinkedUnits;

  openfluid::core::Unit *FromUnit, *ToUnit, *ParentUnit, *ChildUnit;

  // creating units
  for (itUnits = Descriptor.getUnits().begin();itUnits != Descriptor.getUnits().end();++itUnits)
  {
    Repository->addUnit(openfluid::core::Unit((*itUnits).getUnitClass(),(*itUnits).getUnitID(),(*itUnits).getProcessOrder()));
  }

  // linking to units
  for (itUnits = Descriptor.getUnits().begin();itUnits != Descriptor.getUnits().end();++itUnits)
  {

    for (itLinkedUnits = (*itUnits).getUnitsTos().begin();itLinkedUnits != (*itUnits).getUnitsTos().end();++itLinkedUnits)
    {
      FromUnit = Repository->getUnit((*itUnits).getUnitClass(),(*itUnits).getUnitID());
      ToUnit = Repository->getUnit((*itLinkedUnits).first,(*itLinkedUnits).second);

      if (ToUnit != NULL)
      {
        FromUnit->addToUnit(ToUnit);
        ToUnit->addFromUnit(FromUnit);
      }
      else
      {
        std::ostringstream UnitStr;
        UnitStr << FromUnit->getClass() << "#" << FromUnit->getID();
        throw openfluid::base::OFException("kernel","DomainFactory::buildGraphFromDescriptor","Target -to- unit referenced by " + UnitStr.str() + " does not exist" );
      }
    }
  }


  // linking child units
  for (itUnits = Descriptor.getUnits().begin();itUnits != Descriptor.getUnits().end();++itUnits)
  {

    for (itLinkedUnits = (*itUnits).getUnitsParents().begin();itLinkedUnits != (*itUnits).getUnitsParents().end();++itLinkedUnits)
    {
      ChildUnit = Repository->getUnit((*itUnits).getUnitClass(),(*itUnits).getUnitID());
      ParentUnit = Repository->getUnit((*itLinkedUnits).first,(*itLinkedUnits).second);

      if (ParentUnit != NULL)
      {
        ParentUnit->addChildUnit(ChildUnit);
        ChildUnit->addParentUnit(ParentUnit);
      }
      else
      {
        std::ostringstream UnitStr;
        UnitStr << ChildUnit->getClass() << "#" << ChildUnit->getID();
        throw openfluid::base::OFException("kernel","DomainFactory::buildGraphFromDescriptor","Target -parent- unit referenced by " + UnitStr.str() + " does not exist" );
      }
    }
  }


  Repository->sortUnitsByProcessOrder();



  // ============== Input Data ==============


  std::list<openfluid::base::InputDataDescriptor>::iterator itIData;

  for (itIData = Descriptor.getInputData().begin();itIData != Descriptor.getInputData().end();++itIData)
  {
    openfluid::tools::ColumnTextParser DataParser("%");

    if (DataParser.setFromString((*itIData).getDataBlob(),(*itIData).getColumnsOrder().size()+1))
    {
      openfluid::core::Unit* TheUnit;
      int i,j;
      bool IsOK = true;
      long ID;
      openfluid::core::InputDataValue Value;

      // parses data in file and loads it in the input data table for each unit, ordered by columns
      i = 0;
      while (i<DataParser.getLinesCount() && IsOK)
      {
        IsOK = DataParser.getLongValue(i,0,&ID);

        if (IsOK)
        {

          TheUnit = Repository->getUnit((*itIData).getUnitsClass(),(openfluid::core::UnitID_t)ID);

          if (TheUnit != NULL)
          {
            for (j=1;j<DataParser.getColsCount();j++)
            {
              if (DataParser.getStringValue(i,j,&Value))
              {
                TheUnit->getInputData()->setValue((*itIData).getColumnsOrder()[j-1],Value);
              }
            }
          }
          else
          {
            std::string TmpStr;
            openfluid::tools::ConvertValue(ID,&TmpStr);
            ExecMsgs->addWarning("DomainFactory::buildDomainFromDescriptor",(*itIData).getUnitsClass() + " " + TmpStr + " does not exist in input data");
          }
          i++;
        }
        else
          throw openfluid::base::OFException("kernel","DomainFactory::buildDomainFromDescriptor","Input data format error");
      }
    }
    else
      throw openfluid::base::OFException("kernel","DomainFactory::buildDomainFromDescriptor","Error in input data, cannot be parsed");

  }

  // ============== Events ==============


  std::list<openfluid::base::EventDescriptor>::iterator itEvent;
  openfluid::core::Unit* EventUnit;

  for (itEvent = Descriptor.getEvents().begin();itEvent != Descriptor.getEvents().end();++itEvent)
  {

    EventUnit = Repository->getUnit((*itEvent).getUnitClass(),(*itEvent).getUnitID());

    if (EventUnit != NULL)
    {
      EventUnit->getEvents()->addEvent(&((*itEvent).getEvent()));
    }

  }

}


} } //namespaces



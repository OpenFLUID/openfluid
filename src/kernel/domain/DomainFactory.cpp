/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DomainFactory.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "DomainFactory.h"

#include "openfluid-core.h"
#include "openfluid-base.h"
#include "openfluid-tools.h"


// =====================================================================
// =====================================================================


DomainFactory::DomainFactory()
{
}


// =====================================================================
// =====================================================================


void DomainFactory::buildDomainFromDescriptor(DomainDescriptor& Descriptor)
{
  openfluid::core::CoreRepository* Repository = openfluid::core::CoreRepository::getInstance();
  openfluid::base::ExecutionMessages* ExecMsgs = openfluid::base::ExecutionMessages::getInstance();

  // ============== Domain definition ==============

  std::list<UnitDescriptor>::iterator itUnits;
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


  std::list<InputDataDescriptor>::iterator itIData;

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


  std::list<EventDescriptor>::iterator itEvent;
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





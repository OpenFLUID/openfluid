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
  \file SimulationContributorWare.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/SimulationContributorWare.hpp>


namespace openfluid { namespace ware {


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t& InputName,
                                const openfluid::core::Value& Val)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_SetInputData","Inputdata can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_SetInputData","Inputdata can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->setValue(InputName,Val))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetInputData","Unable to set value for input data "+ InputName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                               const openfluid::core::InputDataName_t& InputName,
                                               const double& Val)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_SetInputData","Inputdata can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_SetInputData","Inputdata can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->setValue(InputName,openfluid::core::DoubleValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetInputData","Unable to set double value for input data "+ InputName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                               const openfluid::core::InputDataName_t& InputName,
                                               const long& Val)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_SetInputData","Inputdata can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_SetInputData","Inputdata can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->setValue(InputName,openfluid::core::IntegerValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetInputData","Unable to set long value for input data "+ InputName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                               const openfluid::core::InputDataName_t& InputName,
                                               const std::string& Val)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_SetInputData","Inputdata can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_SetInputData","Inputdata can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->setValue(InputName,Val))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetInputData","Unable to set string value for input data "+ InputName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const openfluid::core::Value& Val)
{
  OPENFLUID_InitializeVariable(*UnitPtr,VarName,Val);
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::Unit& aUnit,
                                        const openfluid::core::VariableName_t& VarName,
                                        const openfluid::core::Value& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "SimulationContributorWare::OPENFLUID_InitializeVariable","Variables can be initialized during INITIALIZERUN stage only")

  if (&aUnit != NULL)
  {
    if (!aUnit.getVariables()->appendValue(VarName,0,Val))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Error initializing value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Unit is NULL");
}

// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const double& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                              "SimulationContributorWare::OPENFLUID_InitializeVariable","Variables can be initialized during INITIALIZERUN stage only")

  /* Do not call OPENFLUID_InitializeVariable(UnitPtr,VarName,openfluid::core::DoubleValue(Value))
   * because of cast operator, this function is called (recursively)
   */
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,0,openfluid::core::DoubleValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Error appending double value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const long& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "SimulationContributorWare::OPENFLUID_InitializeVariable","Variables can be initialized during INITIALIZERUN stage only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,0,openfluid::core::IntegerValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Error appending long value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const bool& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "SimulationContributorWare::OPENFLUID_InitializeVariable","Variables can be initialized during INITIALIZERUN stage only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,0,openfluid::core::BooleanValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Error appending boolean value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const std::string& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "SimulationContributorWare::OPENFLUID_InitializeVariable","Variables can be initialized during INITIALIZERUN stage only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,0,openfluid::core::StringValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Error appending string value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_InitializeVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const openfluid::core::Value& Val)
{
  OPENFLUID_AppendVariable(*UnitPtr,VarName,Val);
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::Unit& aUnit,
                                        const openfluid::core::VariableName_t& VarName,
                                        const openfluid::core::Value& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_AppendVariable","Variables values cannot be added outside RUNSTEP stage")

  if (&aUnit != NULL)
  {
    if (!aUnit.getVariables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),Val))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Error appending value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Unit is NULL");
}

// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const double& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_AppendVariable","Variables values cannot be added outside RUNSTEP stage")

  /* Do not call OPENFLUID_AppendVariable(UnitPtr,VarName,openfluid::core::DoubleValue(Value))
   * because of cast operator, this function is called (recursively)
   */
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::DoubleValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Error appending double value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const long& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_AppendVariable","Variables values cannot be added outside RUNSTEP stage")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::IntegerValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Error appending long value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const bool& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_AppendVariable","Variables values cannot be added outside RUNSTEP stage")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::BooleanValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Error appending boolean value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const std::string& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_AppendVariable","Variables values cannot be added outside RUNSTEP stage")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::StringValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Error appending string value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const openfluid::core::Value& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_SetVariable","Variables can be modified during RUNSTEP stage only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),Val))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Error setting value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t& VarName,
                                        const double& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_SetVariable","Variables can be modified during RUNSTEP stage only")

  /* Do not call OPENFLUID_SetVariable(UnitPtr,VarName,openfluid::core::DoubleValue(Value))
   * because of cast operator, THIS function is called (recursively)
   */
  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::DoubleValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Error setting double value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                       const openfluid::core::VariableName_t& VarName,
                                       const long& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_SetVariable","Variables can be modified during RUNSTEP stage only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::IntegerValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Error setting long value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                       const openfluid::core::VariableName_t& VarName,
                                       const bool& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_SetVariable","Variables can be modified during RUNSTEP stage only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::BooleanValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Error setting boolean value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                                       const openfluid::core::VariableName_t& VarName,
                                       const std::string& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_SetVariable","Variables can be modified during RUNSTEP stage only")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::StringValue(Val)))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Error setting string value for variable "+ VarName);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_SetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendEvent(openfluid::core::Unit *UnitPtr,
                                              openfluid::core::Event& Ev)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "SimulationContributorWare::OPENFLUID_SetVariable","Variables can be modified during RUNSTEP stage only")

  if (UnitPtr != NULL)
  {
    UnitPtr->getEvents()->addEvent(Ev);
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AppendEvent","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AddUnit(openfluid::core::UnitClass_t ClassName,
                                          openfluid::core::UnitID_t ID,
                                          openfluid::core::PcsOrd_t PcsOrder)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_AddUnit","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_Addunit","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (!mp_CoreData->addUnit(openfluid::core::Unit(ClassName,ID,PcsOrder)))
   throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AddUnit","Error adding unit");

  mp_CoreData->sortUnitsByProcessOrder();
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_DeleteUnit(openfluid::core::UnitClass_t ClassName,
                                             openfluid::core::UnitID_t ID)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_DeleteUnit","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_DeleteUnit","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  openfluid::core::Unit* TheUnit = mp_CoreData->getUnit(ClassName,ID);

  if (TheUnit == NULL)
    openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_DeleteUnit","error deleting unit");

  mp_CoreData->deleteUnit(TheUnit);

  mp_CoreData->sortUnitsByProcessOrder();
}

// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_AddFromToConnection(openfluid::core::UnitClass_t ClassNameFrom,
                                                      openfluid::core::UnitID_t IDFrom,
                                                      openfluid::core::UnitClass_t ClassNameTo,
                                                      openfluid::core::UnitID_t IDTo)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_AddFromToConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_AddFromToConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  openfluid::core::Unit* FromUnit = mp_CoreData->getUnit(ClassNameFrom, IDFrom);
  openfluid::core::Unit* ToUnit = mp_CoreData->getUnit(ClassNameTo, IDTo);

  return OPENFLUID_AddFromToConnection(FromUnit, ToUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_AddFromToConnection(openfluid::core::Unit* FromUnit,
                                                      openfluid::core::Unit* ToUnit)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_AddFromToConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_AddFromToConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (FromUnit != NULL || ToUnit != NULL)
  {
    return (FromUnit->addToUnit(ToUnit) && ToUnit->addFromUnit(FromUnit));
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AddFromToConnection","Error adding from-to connection");
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_RemoveFromToConnection(openfluid::core::UnitClass_t ClassNameFrom,
                                                         openfluid::core::UnitID_t IDFrom,
                                                         openfluid::core::UnitClass_t ClassNameTo,
                                                         openfluid::core::UnitID_t IDTo)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_RemoveFromToConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_RemoveFromToConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  openfluid::core::Unit* FromUnit = mp_CoreData->getUnit(ClassNameFrom, IDFrom);
  openfluid::core::Unit* ToUnit = mp_CoreData->getUnit(ClassNameTo, IDTo);

  return OPENFLUID_RemoveFromToConnection(FromUnit, ToUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_RemoveFromToConnection(openfluid::core::Unit* FromUnit,
                                                         openfluid::core::Unit* ToUnit)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_RemoveFromToConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_RemoveFromToConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (FromUnit == NULL || ToUnit == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","CoreRepository::removeFromToConnection","Error removing from-to connection");
  else
    return mp_CoreData->removeFromToConnection(FromUnit,ToUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_AddChildParentConnection(openfluid::core::UnitClass_t ClassNameChild,
                                                           openfluid::core::UnitID_t IDChild,
                                                           openfluid::core::UnitClass_t ClassNameParent,
                                                           openfluid::core::UnitID_t IDParent)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_AddChildParentConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_AddChildParentConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  openfluid::core::Unit* ChildUnit = mp_CoreData->getUnit(ClassNameChild, IDChild);
  openfluid::core::Unit* ParentUnit = mp_CoreData->getUnit(ClassNameParent, IDParent);

  return OPENFLUID_AddChildParentConnection(ChildUnit, ParentUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_AddChildParentConnection(openfluid::core::Unit* ChildUnit,
                                                           openfluid::core::Unit* ParentUnit)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_AddChildParentConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_AddChildParentConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (ChildUnit != NULL || ParentUnit != NULL)
  {
    return (ChildUnit->addParentUnit(ParentUnit) && ParentUnit->addChildUnit(ChildUnit));
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework","SimulationContributorWare::OPENFLUID_AddChildParentConnection","Error adding child-parent connection");
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_RemoveChildParentConnection(openfluid::core::UnitClass_t ClassNameChild,
                                                           openfluid::core::UnitID_t IDChild,
                                                           openfluid::core::UnitClass_t ClassNameParent,
                                                           openfluid::core::UnitID_t IDParent)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_RemoveChildParentConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_RemoveChildParentConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  openfluid::core::Unit* ChildUnit = mp_CoreData->getUnit(ClassNameChild, IDChild);
  openfluid::core::Unit* ParentUnit = mp_CoreData->getUnit(ClassNameParent, IDParent);

  return OPENFLUID_RemoveChildParentConnection(ChildUnit, ParentUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_RemoveChildParentConnection(openfluid::core::Unit* ChildUnit,
                                                           openfluid::core::Unit* ParentUnit)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_RemoveChildParentConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_RemoveChildParentConnection","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (ChildUnit == NULL || ParentUnit == NULL)
    throw openfluid::base::OFException("OpenFLUID framework","CoreRepository::removeChildParentConnection","Error removing child-parent connection");
  else
    return mp_CoreData->removeChildParentConnection(ChildUnit,ParentUnit);
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_BuildUnitsMatrix(const openfluid::core::UnitClass_t& UnitsClass,
                                                   const unsigned int& ColsNbr,
                                                   const unsigned int& RowsNbr)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "SimulationContributorWare::OPENFLUID_BuildUnitsMatrix","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "SimulationContributorWare::OPENFLUID_BuildUnitsMatrix","Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  openfluid::core::UnitID_t CurrID = 0;
  openfluid::core::UnitID_t ToID = 0;

  for (unsigned int i=0; i<RowsNbr;i++)
  {
    for (unsigned int j=0; j<ColsNbr;j++)
    {
      CurrID = 1+((ColsNbr*i)+j);
      OPENFLUID_AddUnit(UnitsClass,CurrID,1);
    }
  }

  for (unsigned int i=0; i<RowsNbr;i++)
  {
    for (unsigned int j=0; j<ColsNbr;j++)
    {
      CurrID = 1+((ColsNbr*i)+j);

      if (i == 0)
      {
        // top

        if (j == 0)
        {
          // top-left
          ToID = CurrID+1;
          OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

          ToID = CurrID+ColsNbr;
          OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

        }
        else
        {
          if (j == ColsNbr-1)
          {
            // top-right
            ToID = CurrID-1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID+ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

          }
          else
          {
            // top-center
            ToID = CurrID-1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID+1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID+ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

          }
        }

      }
      else
      {
        if (i == RowsNbr-1)
        {
          // bottom

          if (j == 0)
          {
            // bottom-left
            ToID = CurrID+1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID-ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);
          }
          else
          {
            if (j == ColsNbr-1)
            {
              // bottom-right
              ToID = CurrID-1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID-ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            }
            else
            {
              // bottom-center
              ToID = CurrID-1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID+1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID-ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);
            }
          }

        }
        else
        {
          // middle

          if (j == 0)
          {
            // middle-left
            ToID = CurrID+1;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID-ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            ToID = CurrID+ColsNbr;
            OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

          }
          else
          {
            if (j == ColsNbr-1)
            {
              // middle-right
              ToID = CurrID-1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID-ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID+ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            }
            else
            {
              // middle-center
              ToID = CurrID-1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID+1;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID-ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

              ToID = CurrID+ColsNbr;
              OPENFLUID_AddFromToConnection(UnitsClass,CurrID,UnitsClass,ToID);

            }
          }


        }
      }
    }
  }

//  throw openfluid::base::OFException("SimulationContributorWare::OPENFLUID_BuildUnitsMatrix : under construction");

}



} } // openfluid::ware



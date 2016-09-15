/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/



/**
  @file SimulationContributorWare.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/ware/SimulationContributorWare.hpp>
#include <openfluid/tools/IDHelpers.hpp>


namespace openfluid { namespace ware {


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                                       const openfluid::core::AttributeName_t& AttrName,
                                                       const openfluid::core::Value& Val)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Attributes can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "Attributes can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->attributes()->setValue(AttrName,Val))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Unable to set value for attribute "+ AttrName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is nullptr");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                                       const openfluid::core::AttributeName_t& AttrName,
                                                       const double& Val)
{
  OPENFLUID_SetAttribute(UnitPtr,AttrName,openfluid::core::DoubleValue(Val));
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                                       const openfluid::core::AttributeName_t& AttrName,
                                                       bool Val)
{
  OPENFLUID_SetAttribute(UnitPtr,AttrName,openfluid::core::BooleanValue(Val));
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                                       const openfluid::core::AttributeName_t& AttrName,
                                                       const long& Val)
{
  OPENFLUID_SetAttribute(UnitPtr,AttrName,openfluid::core::IntegerValue(Val));
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                                       const openfluid::core::AttributeName_t& AttrName,
                                                       const std::string& Val)
{
  OPENFLUID_SetAttribute(UnitPtr,AttrName,openfluid::core::StringValue(Val));
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                             const openfluid::core::VariableName_t& VarName,
                                                             const openfluid::core::Value& Val)
{
  OPENFLUID_InitializeVariable(*UnitPtr,VarName,Val);
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit& aUnit,
                                                             const openfluid::core::VariableName_t& VarName,
                                                             const openfluid::core::Value& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "Variables can be initialized during INITIALIZERUN stage only")

  if (!aUnit.variables()->appendValue(VarName,0,Val))
  {
    openfluid::base::ExceptionContext Context =
        computeFrameworkContext(OPENFLUID_CODE_LOCATION)
        .addSpatialUnit(openfluid::tools::classIDToString(aUnit.getClass(),aUnit.getID()));

    throw openfluid::base::FrameworkException(Context,"Error initializing value for variable "+ VarName);
  }
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                             const openfluid::core::VariableName_t& VarName,
                                                             const double& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "Variables can be initialized during INITIALIZERUN stage only")

  /* Do not call OPENFLUID_InitializeVariable(UnitPtr,VarName,openfluid::core::DoubleValue(Value))
   * because of cast operator, this function is called (recursively)
   */
  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->appendValue(VarName,0,openfluid::core::DoubleValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error appending double value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                             const openfluid::core::VariableName_t& VarName,
                                                             const long& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "Variables can be initialized during INITIALIZERUN stage only")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->appendValue(VarName,0,openfluid::core::IntegerValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error appending long value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                             const openfluid::core::VariableName_t& VarName,
                                                             const bool& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "Variables can be initialized during INITIALIZERUN stage only")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->appendValue(VarName,0,openfluid::core::BooleanValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error appending boolean value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                             const openfluid::core::VariableName_t& VarName,
                                                             const std::string& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::INITIALIZERUN,
                           "Variables can be initialized during INITIALIZERUN stage only")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->appendValue(VarName,0,openfluid::core::StringValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error appending string value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                         const openfluid::core::VariableName_t& VarName,
                                                         const openfluid::core::Value& Val)
{
  OPENFLUID_AppendVariable(*UnitPtr,VarName,Val);
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::SpatialUnit& aUnit,
                                                         const openfluid::core::VariableName_t& VarName,
                                                         const openfluid::core::Value& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables values cannot be added outside RUNSTEP stage")

  if (!aUnit.variables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),Val))
  {
    openfluid::base::ExceptionContext Context =
        computeFrameworkContext(OPENFLUID_CODE_LOCATION)
        .addSpatialUnit(openfluid::tools::classIDToString(aUnit.getClass(),aUnit.getID()));

    throw openfluid::base::FrameworkException(Context,"Error appending value for variable "+ VarName);
  }
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                         const openfluid::core::VariableName_t& VarName,
                                                         const double& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables values cannot be added outside RUNSTEP stage")

  /* Do not call OPENFLUID_AppendVariable(UnitPtr,VarName,openfluid::core::DoubleValue(Value))
   * because of cast operator, this function is called (recursively)
   */
  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::DoubleValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error appending double value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                         const openfluid::core::VariableName_t& VarName,
                                                         const long& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables values cannot be added outside RUNSTEP stage")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::IntegerValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error appending long value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                         const openfluid::core::VariableName_t& VarName,
                                                         const bool& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables values cannot be added outside RUNSTEP stage")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::BooleanValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error appending boolean value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is nullptr");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                         const openfluid::core::VariableName_t& VarName,
                                                         const std::string& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables values cannot be added outside RUNSTEP stage")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->appendValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::StringValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error appending string value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                      const openfluid::core::VariableName_t& VarName,
                                                      const openfluid::core::Value& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables can be modified during RUNSTEP stage only")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),Val))    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error setting value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                      const openfluid::core::VariableName_t& VarName,
                                                      const double& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables can be modified during RUNSTEP stage only")

  /* Do not call OPENFLUID_SetVariable(UnitPtr,VarName,openfluid::core::DoubleValue(Value))
   * because of cast operator, THIS function is called (recursively)
   */
  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::DoubleValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error setting double value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                      const openfluid::core::VariableName_t& VarName,
                                                      const long& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables can be modified during RUNSTEP stage only")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::IntegerValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error setting long value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                      const openfluid::core::VariableName_t& VarName,
                                                      const bool& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables can be modified during RUNSTEP stage only")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::BooleanValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error setting boolean value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                                                      const openfluid::core::VariableName_t& VarName,
                                                      const std::string& Val)
{
  REQUIRE_SIMULATION_STAGE(openfluid::base::SimulationStatus::RUNSTEP,
                           "Variables can be modified during RUNSTEP stage only")

  if (UnitPtr != nullptr)
  {
    if (!UnitPtr->variables()->modifyValue(VarName,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::StringValue(Val)))
    {
      openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
          .addSpatialUnit(openfluid::tools::classIDToString(UnitPtr->getClass(),UnitPtr->getID()));
      throw openfluid::base::FrameworkException(Context,
                                                "Error setting string value for variable "+ VarName);
    }
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AppendEvent(openfluid::core::SpatialUnit *UnitPtr,
                                                      openfluid::core::Event& Ev)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Events can be modified during PREPAREDATA and later stages only")

  if (UnitPtr != nullptr)
  {
    UnitPtr->events()->addEvent(Ev);
  }
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),"Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_AddUnit(const openfluid::core::UnitsClass_t& ClassName,
                                                  openfluid::core::UnitID_t ID,
                                                  openfluid::core::PcsOrd_t PcsOrder)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  if (!mp_SpatialData->addUnit(openfluid::core::SpatialUnit(ClassName,ID,PcsOrder)))
  {
    openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
        .addSpatialUnit(openfluid::tools::classIDToString(ClassName,ID));
    throw openfluid::base::FrameworkException(Context,"Error adding unit");
  }

  mp_SpatialData->sortUnitsByProcessOrder();
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_DeleteUnit(const openfluid::core::UnitsClass_t& ClassName,
                                                     openfluid::core::UnitID_t ID)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

  openfluid::core::SpatialUnit* TheUnit = mp_SpatialData->spatialUnit(ClassName,ID);

  if (TheUnit == nullptr)
  {
    openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
        .addSpatialUnit(openfluid::tools::classIDToString(ClassName,ID));
    throw openfluid::base::FrameworkException(Context,
                                              "Error deleting unit");
  }

  mp_SpatialData->deleteUnit(TheUnit);

  mp_SpatialData->sortUnitsByProcessOrder();
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_AddFromToConnection(const openfluid::core::UnitsClass_t& ClassNameFrom,
                                                              openfluid::core::UnitID_t IDFrom,
                                                              const openfluid::core::UnitsClass_t& ClassNameTo,
                                                              openfluid::core::UnitID_t IDTo)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph connections can be modified during PREPAREDATA and later stages only")

  openfluid::core::SpatialUnit* FromUnit = mp_SpatialData->spatialUnit(ClassNameFrom, IDFrom);
  openfluid::core::SpatialUnit* ToUnit = mp_SpatialData->spatialUnit(ClassNameTo, IDTo);

  if (FromUnit == nullptr || ToUnit == nullptr)
  {
    openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
        .addSpatialConnection(openfluid::tools::classIDToString(ClassNameFrom,IDFrom),
                              openfluid::tools::classIDToString(ClassNameTo,IDTo));
    throw openfluid::base::FrameworkException(Context,
                                              "Error adding from-to connection");
  }

  return OPENFLUID_AddFromToConnection(FromUnit, ToUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_AddFromToConnection(openfluid::core::SpatialUnit* FromUnit,
                                                              openfluid::core::SpatialUnit* ToUnit)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph connections can be modified during PREPAREDATA and later stages only")

  if (FromUnit != nullptr || ToUnit != nullptr)
  {
    return (FromUnit->addToUnit(ToUnit) && ToUnit->addFromUnit(FromUnit));
  }
  else
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error adding from-to connection");
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_RemoveFromToConnection(const openfluid::core::UnitsClass_t& ClassNameFrom,
                                                                 openfluid::core::UnitID_t IDFrom,
                                                                 const openfluid::core::UnitsClass_t& ClassNameTo,
                                                                 openfluid::core::UnitID_t IDTo)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph connections can be modified during PREPAREDATA and later stages only")

  openfluid::core::SpatialUnit* FromUnit = mp_SpatialData->spatialUnit(ClassNameFrom, IDFrom);
  openfluid::core::SpatialUnit* ToUnit = mp_SpatialData->spatialUnit(ClassNameTo, IDTo);

  if (FromUnit == nullptr || ToUnit == nullptr)
  {
    openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
        .addSpatialConnection(openfluid::tools::classIDToString(ClassNameFrom,IDFrom),
                              openfluid::tools::classIDToString(ClassNameTo,IDTo));
    throw openfluid::base::FrameworkException(Context,
                                              "Error removing from-to connection");
  }

  return OPENFLUID_RemoveFromToConnection(FromUnit, ToUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_RemoveFromToConnection(openfluid::core::SpatialUnit* FromUnit,
                                                                 openfluid::core::SpatialUnit* ToUnit)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph connections can be modified during PREPAREDATA and later stages only")

  if (FromUnit == nullptr || ToUnit == nullptr)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error removing from-to connection");
  else
    return mp_SpatialData->removeFromToConnection(FromUnit,ToUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_AddChildParentConnection(const openfluid::core::UnitsClass_t& ClassNameChild,
                                                                   openfluid::core::UnitID_t IDChild,
                                                                   const openfluid::core::UnitsClass_t& ClassNameParent,
                                                                   openfluid::core::UnitID_t IDParent)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph connections can be modified during PREPAREDATA and later stages only")

  openfluid::core::SpatialUnit* ChildUnit = mp_SpatialData->spatialUnit(ClassNameChild, IDChild);
  openfluid::core::SpatialUnit* ParentUnit = mp_SpatialData->spatialUnit(ClassNameParent, IDParent);

  if (ChildUnit == nullptr || ParentUnit == nullptr)
  {
    openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
        .addSpatialConnection(openfluid::tools::classIDToString(ClassNameChild,IDChild),
                              openfluid::tools::classIDToString(ClassNameParent,IDParent));
    throw openfluid::base::FrameworkException(Context,
                                              "Error adding child-parent connection");
  }

  return OPENFLUID_AddChildParentConnection(ChildUnit, ParentUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_AddChildParentConnection(openfluid::core::SpatialUnit* ChildUnit,
                                                                   openfluid::core::SpatialUnit* ParentUnit)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph connections can be modified during PREPAREDATA and later stages only")

  if (ChildUnit != nullptr || ParentUnit != nullptr)
  {
    return (ChildUnit->addParentUnit(ParentUnit) && ParentUnit->addChildUnit(ChildUnit));
  }
  else
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error adding child-parent connection");
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_RemoveChildParentConnection(
                                                   const openfluid::core::UnitsClass_t& ClassNameChild,
                                                   openfluid::core::UnitID_t IDChild,
                                                   const openfluid::core::UnitsClass_t& ClassNameParent,
                                                   openfluid::core::UnitID_t IDParent)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph connections can be modified during PREPAREDATA and later stages only")

  openfluid::core::SpatialUnit* ChildUnit = mp_SpatialData->spatialUnit(ClassNameChild, IDChild);
  openfluid::core::SpatialUnit* ParentUnit = mp_SpatialData->spatialUnit(ClassNameParent, IDParent);

  if (ChildUnit == nullptr || ParentUnit == nullptr)
   {
     openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION)
         .addSpatialConnection(openfluid::tools::classIDToString(ClassNameChild,IDChild),
                               openfluid::tools::classIDToString(ClassNameParent,IDParent));
     throw openfluid::base::FrameworkException(Context,
                                               "Error removing child-parent connection");
   }

  return OPENFLUID_RemoveChildParentConnection(ChildUnit, ParentUnit);
}


// =====================================================================
// =====================================================================


bool SimulationContributorWare::OPENFLUID_RemoveChildParentConnection(openfluid::core::SpatialUnit* ChildUnit,
                                                                      openfluid::core::SpatialUnit* ParentUnit)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph connections can be modified during PREPAREDATA and later stages only")

  if (ChildUnit == nullptr || ParentUnit == nullptr)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error removing child-parent connection");
  else
    return mp_SpatialData->removeChildParentConnection(ChildUnit,ParentUnit);
}


// =====================================================================
// =====================================================================


void SimulationContributorWare::OPENFLUID_BuildUnitsMatrix(const openfluid::core::UnitsClass_t& UnitsClass,
                                                           const unsigned int& ColsNbr,
                                                           const unsigned int& RowsNbr)
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                              "Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")
  REQUIRE_SIMULATION_STAGE_LE(openfluid::base::SimulationStatus::CHECKCONSISTENCY,
                              "Spatial graph can be modified during PREPAREDATA and CHECKCONSISTENCY stages only")

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
}


} } // namespaces



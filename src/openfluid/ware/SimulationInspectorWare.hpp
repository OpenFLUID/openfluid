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
  @file SimulationInspectorWare.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_WARE_SIMULATIONINSPECTORWARE_HPP__
#define __OPENFLUID_WARE_SIMULATIONINSPECTORWARE_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulationDrivenWare.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/Datastore.hpp>
#include <openfluid/core/SpatialGraph.hpp>


namespace openfluid { namespace ware {


class OPENFLUID_API SimulationInspectorWare : public SimulationDrivenWare
{
  private:

    static bool IsUnitIDInPtrList(const openfluid::core::UnitsPtrList_t* UnitsList,
                                  const openfluid::core::UnitID_t& ID);


    openfluid::core::Datastore* mp_Datastore;


  protected:

    // TODO check if const
    /**
         Pointer to the spatial graph. It should be used with care. Prefer using the OPENFLUID_Xxxx methods.
     */
    openfluid::core::SpatialGraph* mp_SpatialData;


    virtual bool isLinked() const
    { return (SimulationDrivenWare::isLinked() && mp_SpatialData != nullptr && mp_Datastore != nullptr); };


    /**
      Returns true if a distributed attribute exists, false otherwise
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the queried attribute
    */
    bool OPENFLUID_IsAttributeExist(const openfluid::core::SpatialUnit *UnitPtr,
                                    const openfluid::core::AttributeName_t& AttrName) const;


    /**
      Gets attribute for a unit
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::SpatialUnit *UnitPtr,
                                const openfluid::core::AttributeName_t& AttrName,
                                openfluid::core::Value& Val) const;

    // TODO
#if 0
    /**
      Gets attribute for a unit, as a StringValue
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::SpatialUnit *UnitPtr,
                                const openfluid::core::AttributeName_t& AttrName,
                                openfluid::core::StringValue& Val) const;

    /**
      Gets attribute for a unit, as a DoubleValue
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::SpatialUnit *UnitPtr,
                                const openfluid::core::AttributeName_t& AttrName,
                                openfluid::core::DoubleValue& Val) const;

    /**
      Gets attribute for a unit, as a VectorValue
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::SpatialUnit *UnitPtr,
                                const openfluid::core::AttributeName_t& AttrName,
                                openfluid::core::VectorValue& Val) const;

    /**
      Gets attribute for a unit, as a MatrixValue
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::SpatialUnit *UnitPtr,
                                const openfluid::core::AttributeName_t& AttrName,
                                openfluid::core::MatrixValue& Val) const;
#endif
    /**
      Gets attribute for a unit, as a double
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::SpatialUnit *UnitPtr,
                                const openfluid::core::AttributeName_t& AttrName,
                                double& Val) const;

    /**
      Gets attribute for a unit, as a long integer
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::SpatialUnit *UnitPtr,
                                const openfluid::core::AttributeName_t& AttrName,
                                long& Val) const;

    /**
      Gets attribute for a unit, as a string
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::SpatialUnit *UnitPtr,
                                const openfluid::core::AttributeName_t& AttrName,
                                std::string& Val) const;

    /**
      Returns attribute for a unit, as a StringValue
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @return constant pointer to the value of the requested attribute
    */
    const openfluid::core::Value* OPENFLUID_GetAttribute(const openfluid::core::SpatialUnit *UnitPtr,
                                                         const openfluid::core::AttributeName_t& AttrName) const;

    /**
       Returns true if a distributed variable exists, false otherwise
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the requested variable
     */
     bool OPENFLUID_IsVariableExist(const openfluid::core::SpatialUnit *UnitPtr,
                                    const openfluid::core::VariableName_t& VarName) const;

    /**
       Returns true if a distributed variable exists and if a value has been set for the given index, false otherwise
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the requested variable
       @param[in] Index the time index for the value of the variable
     */
     bool OPENFLUID_IsVariableExist(const openfluid::core::SpatialUnit *UnitPtr,
                                    const openfluid::core::VariableName_t& VarName,
                                    const openfluid::core::TimeIndex_t Index) const;

     /**
        Returns true if a distributed variable exists and if a value has been set for the given index
        and if the value type is the given type, false otherwise
        @param[in] UnitPtr a Unit
        @param[in] VarName the name of the requested variable
        @param[in] Index the time index for the value of the variable
        @param[in] ValueType the type of the value
      */
      bool OPENFLUID_IsVariableExist(const openfluid::core::SpatialUnit *UnitPtr,
                                     const openfluid::core::VariableName_t& VarName,
                                     const openfluid::core::TimeIndex_t Index,
                                     const openfluid::core::Value::Type ValueType) const;

     /**
        Returns true if a distributed variable exists
        and if the type set for this variable is the given type, false otherwise
        @param[in] UnitPtr a Unit
        @param[in] VarName the name of the requested variable
        @param[in] VarType the type of the variable
      */
     bool OPENFLUID_IsTypedVariableExist(const openfluid::core::SpatialUnit *UnitPtr,
                                    const openfluid::core::VariableName_t& VarName,
                                    const openfluid::core::Value::Type VarType) const;

     /**
        Returns true if a distributed variable exists and if a value has been set for the given index
        and if the type set for this variable is the given type, false otherwise
        @param[in] UnitPtr a Unit
        @param[in] VarName the name of the requested variable
        @param[in] Index the time index for the value of the variable
        @param[in] VarType the type of the variable
      */
      bool OPENFLUID_IsTypedVariableExist(const openfluid::core::SpatialUnit *UnitPtr,
                                     const openfluid::core::VariableName_t& VarName,
                                     const openfluid::core::TimeIndex_t Index,
                                     const openfluid::core::Value::Type VarType) const;

      /**
        Gets the distributed variable value for a unit at a time index
        @param[in] UnitPtr a Unit
        @param[in] VarName the name of the requested variable
        @param[in] Index the time index for the value of the requested variable
        @param[out] Val the value of the requested variable
      */
      void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                                 const openfluid::core::VariableName_t& VarName,
                                 const openfluid::core::TimeIndex_t Index,
                                 openfluid::core::Value& Val) const;

      /**
        Gets the distributed variable value for a unit at a time index
        @param[in] UnitPtr a Unit
        @param[in] VarName the name of the requested variable
        @param[in] Index the time index for the value of the requested variable
        @param[out] Val the value of the requested variable
      */
      void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                                 const openfluid::core::VariableName_t& VarName,
                                 const openfluid::core::TimeIndex_t Index,
                                 double& Val) const;

      /**
        Gets the distributed variable value for a unit at a time index
        @param[in] UnitPtr a Unit
        @param[in] VarName the name of the requested variable
        @param[in] Index the time index for the value of the requested variable
        @param[out] Val the value of the requested variable
      */
      void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                                 const openfluid::core::VariableName_t& VarName,
                                 const openfluid::core::TimeIndex_t Index,
                                 long& Val) const;

      /**
        Gets the distributed variable value for a unit at a time index
        @param[in] UnitPtr a Unit
        @param[in] VarName the name of the requested variable
        @param[in] Index the time index for the value of the requested variable
        @param[out] Val the value of the requested variable
      */
      void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                                 const openfluid::core::VariableName_t& VarName,
                                 const openfluid::core::TimeIndex_t Index,
                                 bool& Val) const;

      /**
        Gets the distributed variable value for a unit at a time index
        @param[in] UnitPtr a Unit
        @param[in] VarName the name of the requested variable
        @param[in] Index the time index for the value of the requested variable
        @param[out] Val the value of the requested variable
      */
      void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                                 const openfluid::core::VariableName_t& VarName,
                                 const openfluid::core::TimeIndex_t Index,
                                 std::string& Val) const;

      /**
        Returns the distributed variable value for a unit at a time index
        @param[in] UnitPtr a Unit
        @param[in] VarName the name of the requested variable
        @param[in] Index the time index for the value of the requested variable
        @return a constant pointer the value of the requested variable
      */
      const openfluid::core::Value* OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                                                          const openfluid::core::VariableName_t& VarName,
                                                          const openfluid::core::TimeIndex_t Index) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               openfluid::core::Value& Val) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               double& Val) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               long& Val) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               bool& Val) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               std::string& Val) const;

    /**
      Returns the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @return a constant pointer the value of the requested variable
    */
    const openfluid::core::Value* OPENFLUID_GetVariable(const openfluid::core::SpatialUnit* UnitPtr,
                                                        const openfluid::core::VariableName_t& VarName) const;


    /**
      Gets the latest available variable for a unit
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] IndVal the value and timeindex of the requested variable
    */
    void OPENFLUID_GetLatestVariable(const openfluid::core::SpatialUnit* UnitPtr,
                                     const openfluid::core::VariableName_t& VarName,
                                     openfluid::core::IndexedValue& IndVal) const;

    /**
      Returns the latest available variable for a unit
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @return the value and timeindex of the requested variable
    */
    openfluid::core::IndexedValue OPENFLUID_GetLatestVariable(const openfluid::core::SpatialUnit* UnitPtr,
                                                              const openfluid::core::VariableName_t& VarName) const;


    /**
      Gets the latest available variables for a unit since the given time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] BeginIndex The beginning time index of the search period
      @param[out] IndValList the list of time-indexed values of the requested variable
    */
    void OPENFLUID_GetLatestVariables(const openfluid::core::SpatialUnit* UnitPtr,
                                     const openfluid::core::VariableName_t& VarName,
                                     const openfluid::core::TimeIndex_t BeginIndex,
                                     openfluid::core::IndexedValueList& IndValList) const;

    /**
      Returns the latest available variables for a unit since the given time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] BeginIndex The beginning time index of the search period
      @return the list of time-indexed values of the requested variable
    */
    openfluid::core::IndexedValueList OPENFLUID_GetLatestVariables(const openfluid::core::SpatialUnit* UnitPtr,
                                                                   const openfluid::core::VariableName_t& VarName,
                                                                   const openfluid::core::TimeIndex_t BeginIndex) const;

    /**
      Gets the available variables for a unit during a given period (between two time indexes)
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] BeginIndex the time index for the beginning of the period
      @param[in] EndIndex the time index for the end of the period
      @param[out] IndValList the list of time-indexed values of the requested variable
    */
    void OPENFLUID_GetVariables(const openfluid::core::SpatialUnit* UnitPtr,
                                const openfluid::core::VariableName_t& VarName,
                                const openfluid::core::TimeIndex_t BeginIndex,
                                const openfluid::core::TimeIndex_t EndIndex,
                                openfluid::core::IndexedValueList& IndValList) const;

    /**
      Returns the available variables for a unit during a given period (between two time indexes)
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] BeginIndex the time index for the beginning of the period
      @param[in] EndIndex the time index for the end of the period
      @return the list of time-indexed values of the requested variable
    */
    openfluid::core::IndexedValueList OPENFLUID_GetVariables(const openfluid::core::SpatialUnit* UnitPtr,
                                                             const openfluid::core::VariableName_t& VarName,
                                                             const openfluid::core::TimeIndex_t BeginIndex,
                                                             const openfluid::core::TimeIndex_t EndIndex) const;

    /**
      Gets discrete events happening on a unit during a time period
      @param[in] UnitPtr a Unit
      @param[in] BeginDate the beginning of the time period
      @param[in] EndDate the ending of the time period
      @param[out] Events the collection of event corresponding to the request
    */
    void OPENFLUID_GetEvents(const openfluid::core::SpatialUnit *UnitPtr,
                             const openfluid::core::DateTime BeginDate,
                             const openfluid::core::DateTime EndDate,
                             openfluid::core::EventsCollection& Events) const;

    /**
      Returns discrete events happening on a unit during a time period
      @param[in] UnitPtr a Unit
      @param[in] BeginDate the beginning of the time period
      @param[in] EndDate the ending of the time period
      @return the collection of event corresponding to the request
    */
    openfluid::core::EventsCollection OPENFLUID_GetEvents(const openfluid::core::SpatialUnit *UnitPtr,
                                                          const openfluid::core::DateTime BeginDate,
                                                          const openfluid::core::DateTime EndDate) const;

    /**
      Returns true if the queried unit class exists
      @param[in] ClassName the queried class name
    */
    bool OPENFLUID_IsUnitsClassExist(const openfluid::core::UnitsClass_t& ClassName) const;

    /**
      @deprecated Since version 2.1.0.
      Use openfluid::ware::SimulationInspectorWare::OPENFLUID_IsUnitsClassExist instead
    */
    [[deprecated]] bool OPENFLUID_IsUnitClassExist(const openfluid::core::UnitsClass_t& ClassName) const
    { return OPENFLUID_IsUnitsClassExist(ClassName); }

    /**
      Returns true if the queried unit exists
      @param[in] ClassName the class of the queried unit
      @param[in] ID the ID of the queried unit
    */
    bool OPENFLUID_IsUnitExist(const openfluid::core::UnitsClass_t& ClassName,
                               openfluid::core::UnitID_t ID) const;

    /**
      Returns the total number of units
      @param[out] UnitsCount the total units count
    */
    void OPENFLUID_GetUnitsCount(unsigned int& UnitsCount) const;

    /**
      Returns the total number of units
      @return the total units count
    */
    unsigned int OPENFLUID_GetUnitsCount() const;

    /**
      Gets the number of units of the class if the queried unit class exists
      @param[in] ClassName the queried class name
      @param[out] UnitsCount the number of units in the queried class
      @return false if the unit class does not exist
    */
    bool OPENFLUID_GetUnitsCount(const openfluid::core::UnitsClass_t& ClassName,
                                 unsigned int& UnitsCount) const;

    /**
      Returns the number of units of the class if the queried unit class exists
      @param[in] ClassName the queried class name
      @return the number of units in the queried class, or 0 if the units class does not exist
    */
    unsigned int OPENFLUID_GetUnitsCount(const openfluid::core::UnitsClass_t& ClassName) const;

    /**
      Returns a pointer to the queried unit if exists
      @param[in] ClassName the queried class name
      @param[in] ID the queried unit ID
      @param[out] aUnit a pointer to the requested Unit, nullptr if the unit does not exist
      @return false if the unit does not exist
    */
    bool OPENFLUID_GetUnit(const openfluid::core::UnitsClass_t& ClassName,
                           const openfluid::core::UnitID_t& ID,
                           openfluid::core::SpatialUnit* aUnit) const;

    openfluid::core::SpatialUnit* OPENFLUID_GetUnit(const openfluid::core::UnitsClass_t& ClassName,
                                                    const openfluid::core::UnitID_t& ID) const;

    /**
      Returns a list of units of the requested class
      Returns an empty list if the units class does not exist.
      @param[in] ClassName the requested class
    */
    openfluid::core::UnitsPtrList_t OPENFLUID_GetUnits(const openfluid::core::UnitsClass_t& ClassName);

    /**
      Returns true if a given unit is connected "to" another unit
      @param[in] aUnit the given unit
      @param[in] ClassNameTo the class name of the other unit to test
      @param[in] IDTo the ID of the other unit to test
      @return true if the given unit is connected "to" the other unit
    */
    bool OPENFLUID_IsUnitConnectedTo(openfluid::core::SpatialUnit* aUnit,
                                     const openfluid::core::UnitsClass_t& ClassNameTo,
                                     const openfluid::core::UnitID_t& IDTo) const;


    /**
      Returns true if a given unit is connected "from" another unit
      @param[in] aUnit the given unit
      @param[in] ClassNameFrom the class name of the other unit to test
      @param[in] IDFrom the ID of the other unit to test
      @return true if the given unit is connected "from" the other unit
    */
    bool OPENFLUID_IsUnitConnectedFrom(openfluid::core::SpatialUnit* aUnit,
                                       const openfluid::core::UnitsClass_t& ClassNameFrom,
                                       const openfluid::core::UnitID_t& IDFrom) const;


    /**
      Returns true if a given unit is "a child of" another unit
      @param[in] aUnit the given unit
      @param[in] ClassNameParent the class name of the other unit to test
      @param[in] IDParent the ID of the other unit to test
      @return true if the given unit is "a child of" the other unit
    */
    bool OPENFLUID_IsUnitChildOf(openfluid::core::SpatialUnit* aUnit,
                                 const openfluid::core::UnitsClass_t& ClassNameParent,
                                 const openfluid::core::UnitID_t& IDParent) const;


    /**
      Returns true if a given unit is "parent of" another unit
      @param[in] aUnit the given unit
      @param[in] ClassNameChild the class name of the other unit to test
      @param[in] IDChild the ID of the other unit to test
      @return true if the given unit is "parent of" the other unit
    */
    bool OPENFLUID_IsUnitParentOf(openfluid::core::SpatialUnit* aUnit,
                                  const openfluid::core::UnitsClass_t& ClassNameChild,
                                  const openfluid::core::UnitID_t& IDChild) const;


    SimulationInspectorWare(WareType WType) : SimulationDrivenWare(WType),
      mp_Datastore(nullptr), mp_SpatialData(nullptr)
    { }


  public:

    virtual ~SimulationInspectorWare() 
    { }

    void linkToSpatialGraph(openfluid::core::SpatialGraph* SGraph)
    {
      mp_SpatialData = SGraph;
    }

    void linkToDatastore(openfluid::core::Datastore* DStore)
    {
      mp_Datastore = DStore;
    }

};


} } // openfluid::ware


#endif /* __OPENFLUID_WARE_SIMULATIONINSPECTORWARE_HPP__ */

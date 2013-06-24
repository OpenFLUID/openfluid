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
  \file SimulationInspectorWare.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __SIMULATIONINSPECTORWARE_HPP__
#define __SIMULATIONINSPECTORWARE_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulationDrivenWare.hpp>
#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/Datastore.hpp>


namespace openfluid { namespace ware {


class DLLEXPORT SimulationInspectorWare : public SimulationDrivenWare
{
  private:

    static bool IsUnitIDInPtrList(const openfluid::core::UnitsPtrList_t* UnitsList,
                                  const openfluid::core::UnitID_t& ID);


    openfluid::core::Datastore* mp_Datastore;


  protected:

    // TODO check if const
    /**
         Pointer to the core repository (const). It should be used with care. Prefer to use the OPENFLUID_Xxxx methods.
     */
    openfluid::core::CoreRepository* mp_CoreData;


    virtual bool isLinked() const { return (SimulationDrivenWare::isLinked() && mp_CoreData != NULL && mp_Datastore != NULL); };


    /**
      Returns true if a distributed attribute exists, false otherwise
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the queried attribute
    */
    bool OPENFLUID_IsAttributeExist(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t& AttrName) const;

    /**
      Gets attribute for a unit, as a StringValue
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t& AttrName,
                                openfluid::core::StringValue& Val) const;

    /**
      Gets attribute for a unit, as a DoubleValue
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t& AttrName,
                                openfluid::core::DoubleValue& Val) const;

    /**
      Gets attribute for a unit, as a VectorValue
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t& AttrName,
                                openfluid::core::VectorValue& Val) const;

    /**
      Gets attribute for a unit, as a MatrixValue
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t& AttrName,
                                openfluid::core::MatrixValue& Val) const;

    /**
      Gets attribute for a unit, as a double
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t& AttrName,
                                double& Val) const;

    /**
      Gets attribute for a unit, as a long integer
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t& AttrName,
                                long& Val) const;

    /**
      Gets attribute for a unit, as a string
      @param[in] UnitPtr a Unit
      @param[in] AttrName the name of the requested attribute
      @param[out] Val the value of the requested attribute
    */
    void OPENFLUID_GetAttribute(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t& AttrName,
                                std::string& Val) const;

    /**
       Returns true if a distributed variable exists, false otherwise
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the requested variable
     */
     bool OPENFLUID_IsVariableExist(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::VariableName_t& VarName) const;

    /**
       Returns true if a distributed variable exists and if a value has been set for the given index, false otherwise
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the requested variable
       @param[in] Index the time index for the value of the variable
     */
     bool OPENFLUID_IsVariableExist(const openfluid::core::Unit *UnitPtr,
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
      bool OPENFLUID_IsVariableExist(const openfluid::core::Unit *UnitPtr,
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
     bool OPENFLUID_IsTypedVariableExist(const openfluid::core::Unit *UnitPtr,
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
      bool OPENFLUID_IsTypedVariableExist(const openfluid::core::Unit *UnitPtr,
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
      void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
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
      void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
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
      void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
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
      void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
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
      void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                                 const openfluid::core::VariableName_t& VarName,
                                 const openfluid::core::TimeIndex_t Index,
                                 std::string& Val) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               openfluid::core::Value& Val) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               double& Val) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               long& Val) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Index the time index for the value of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               bool& Val) const;

    /**
      Gets the distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] Val the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               std::string& Val) const;


    /**
      Gets the latest available variable for a unit
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] IndVal the value and timeindex of the requested variable
    */
    void OPENFLUID_GetLatestVariable(const openfluid::core::Unit* UnitPtr,
                                     const openfluid::core::VariableName_t& VarName,
                                     openfluid::core::IndexedValue& IndVal);

    /**
      Gets the latest available variables for a unit since the given time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[out] IndVal the value and timeindex of the requested variable
    */
    void OPENFLUID_GetLatestVariables(const openfluid::core::Unit* UnitPtr,
                                     const openfluid::core::VariableName_t& VarName,
                                     const openfluid::core::TimeIndex_t BeginIndex,
                                     openfluid::core::IndexedValueList& IndValList);


    /**
      Gets the available variables for a unit during a given period (between two time indexes)
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] BeginIndex the time index for the beginning of the period
      @param[in] EndIndex the time index for the end of the period
      @param[out] IndVal the value and timeindex of the requested variable
    */
    void OPENFLUID_GetVariables(const openfluid::core::Unit* UnitPtr,
                                const openfluid::core::VariableName_t& VarName,
                                const openfluid::core::TimeIndex_t BeginIndex,
                                const openfluid::core::TimeIndex_t EndIndex,
                                openfluid::core::IndexedValueList& IndValList);

      /**
        Gets discrete events happening on a unit during a time period
        @param[in] UnitPtr a Unit
        @param[in] BeginDate the beginning of the time period
        @param[in] EndDate the ending of the time period
        @param[out] Events the collection of event corresponding to the request
      */
      void OPENFLUID_GetEvents(const openfluid::core::Unit *UnitPtr,
                               const openfluid::core::DateTime BeginDate,
                               const openfluid::core::DateTime EndDate,
                               openfluid::core::EventsCollection& Events) const;

      /**
        Returns true if the queried unit class exists
        @param[in] ClassName the queried class name
      */
      bool OPENFLUID_IsUnitClassExist(openfluid::core::UnitClass_t ClassName) const;

      /**
        Returns true if the queried unit exists
        @param[in] ClassName the class of the queried unit
        @param[in] ID the ID of the queried unit
      */
      bool OPENFLUID_IsUnitExist(openfluid::core::UnitClass_t ClassName,
                                 openfluid::core::UnitID_t ID) const;

      /**
        Returns the total number of units
        @param[out] UnitsCount the queried class name
      */
      void OPENFLUID_GetUnitsCount(unsigned int& UnitsCount) const;

      /**
        Returns the number of units of the class if the queried unit class exists
        @param[in] ClassName the queried class name
        @param[out] UnitsCount the queried class name
        @return false if the unit class does not exist
      */
      bool OPENFLUID_GetUnitsCount(const openfluid::core::UnitClass_t ClassName,
                                   unsigned int& UnitsCount) const;

      /**
        Returns a pointer to the queried unit if exists
        @param[in] ClassName the queried class name
        @param[in] ID the queried unit ID
        @param[out] aUnit a pointer to the requested Unit, NULL if the unit does not exist
        @return false if the unit does not exist
      */
      bool OPENFLUID_GetUnit(const openfluid::core::UnitClass_t& ClassName,
                             const openfluid::core::UnitID_t& ID,
                             openfluid::core::Unit* aUnit) const;

      openfluid::core::Unit* OPENFLUID_GetUnit(const openfluid::core::UnitClass_t& ClassName,
                                               const openfluid::core::UnitID_t& ID) const;

      /**
        Returns true if a given unit is connected "to" another unit
        @param[in] aUnit the given unit
        @param[in] ClassNameTo the class name of the other unit to test
        @param[in] IDTo the ID of the other unit to test
        @return true if the given unit is connected "to" the other unit
      */
      bool OPENFLUID_IsUnitConnectedTo(openfluid::core::Unit* aUnit,
                                       const openfluid::core::UnitClass_t& ClassNameTo,
                                       const openfluid::core::UnitID_t& IDTo) const;


      /**
        Returns true if a given unit is connected "from" another unit
        @param[in] aUnit the given unit
        @param[in] ClassNameFrom the class name of the other unit to test
        @param[in] IDFrom the ID of the other unit to test
        @return true if the given unit is connected "from" the other unit
      */
      bool OPENFLUID_IsUnitConnectedFrom(openfluid::core::Unit* aUnit,
                                         const openfluid::core::UnitClass_t& ClassNameFrom,
                                         const openfluid::core::UnitID_t& IDFrom) const;


      /**
        Returns true if a given unit is "a child of" another unit
        @param[in] aUnit the given unit
        @param[in] ClassNameParent the class name of the other unit to test
        @param[in] IDParent the ID of the other unit to test
        @return true if the given unit is "a child of" the other unit
      */
      bool OPENFLUID_IsUnitChildOf(openfluid::core::Unit* aUnit,
                                   const openfluid::core::UnitClass_t& ClassNameParent,
                                   const openfluid::core::UnitID_t& IDParent) const;


      /**
        Returns true if a given unit is "parent of" another unit
        @param[in] aUnit the given unit
        @param[in] ClassNameChild the class name of the other unit to test
        @param[in] IDChild the ID of the other unit to test
        @return true if the given unit is "parent of" the other unit
      */
      bool OPENFLUID_IsUnitParentOf(openfluid::core::Unit* aUnit,
                                    const openfluid::core::UnitClass_t& ClassNameChild,
                                    const openfluid::core::UnitID_t& IDChild) const;


      SimulationInspectorWare(WareType WType) : SimulationDrivenWare(WType), mp_Datastore(NULL), mp_CoreData(NULL)
      {};


  public:

    virtual ~SimulationInspectorWare() {};

    void linkToCoreRepository(openfluid::core::CoreRepository* CoreRepos)
    {
      mp_CoreData = CoreRepos;
    };

    void linkToDatastore(openfluid::core::Datastore* DStore)
    {
      mp_Datastore = DStore;
    };

};



} } // openfluid::ware


#endif /* __SIMULATIONINSPECTORWARE_HPP__ */

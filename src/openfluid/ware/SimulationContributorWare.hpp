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
  \file SimulationContributorWare.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __SIMULATIONCONTRIBUTORWARE_HPP__
#define __SIMULATIONCONTRIBUTORWARE_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulationInspectorWare.hpp>


namespace openfluid { namespace ware {


class DLLEXPORT SimulationContributorWare : public SimulationInspectorWare
{
  private:

  protected:

    /**
       Sets input data for a unit
       @param[in] UnitPtr a Unit
       @param[in] InputName the name of the set property
       @param[in] Val the value of the set property
     */
     void OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                 const openfluid::core::InputDataName_t& InputName,
                                 const openfluid::core::Value& Val);

     /**
       Sets input data for a unit, as a double
       @param[in] UnitPtr a Unit
       @param[in] InputName the name of the set property
       @param[in] Val the value of the set property
     */
     void OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                 const openfluid::core::InputDataName_t& InputName,
                                 const double& Val);

     /**
       Sets input data for a unit, as a long integer
       @param[in] UnitPtr a Unit
       @param[in] InputName the name of the set property
       @param[in] Val the value of the set property
     */
     void OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                 const openfluid::core::InputDataName_t& InputName,
                                 const long& Val);

     /**
       Sets input data for a unit, as a string
       @param[in] UnitPtr a Unit
       @param[in] InputName the name of the set property
       @param[out] Val the value of the set property
     */
     void OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                 const openfluid::core::InputDataName_t& InputName,
                                 const std::string& Val);


     /**
       Initializes a distributed variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable
       @deprecated
     */
     void OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                   const openfluid::core::VariableName_t& VarName,
                                   const openfluid::core::Value& Val);

     /**
       Initializes a distributed variable value for a unit
       @param[in] aUnit a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable
     */
     void OPENFLUID_InitializeVariable(openfluid::core::Unit& aUnit,
                                       const openfluid::core::VariableName_t& VarName,
                                       const openfluid::core::Value& Val);

     /**
       Initializes a distributed double variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable (double)
     */
     void OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                   const openfluid::core::VariableName_t& VarName,
                                   const double& Val);

     /**
       Initializes a distributed long variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable (long)
     */
     void OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                   const openfluid::core::VariableName_t& VarName,
                                   const long& Val);

     /**
       Initializes a distributed boolean variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable (bool)
     */
     void OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                   const openfluid::core::VariableName_t& VarName,
                                   const bool& Val);

     /**
       Initializes a distributed string variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable (string)
     */
     void OPENFLUID_InitializeVariable(openfluid::core::Unit *UnitPtr,
                                   const openfluid::core::VariableName_t& VarName,
                                   const std::string& Val);

    /**
      Appends a distributed variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable
      @deprecated
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const openfluid::core::Value& Val);

    /**
      Appends a distributed variable value for a unit at the end of the previously added values for this variable
      @param[in] aUnit a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit& aUnit,
                                  const openfluid::core::VariableName_t& VarName,
                                  const openfluid::core::Value& Val);

    /**
      Appends a distributed double variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (double)
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const double& Val);

    /**
      Appends a distributed long variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (long)
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const long& Val);

    /**
      Appends a distributed boolean variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (bool)
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const bool& Val);

    /**
      Appends a distributed string variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (string)
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const std::string& Val);


    /**
      Sets a distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const openfluid::core::Value& Val);

    /**
      Sets a distributed double variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (double)
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const double& Val);

    /**
      Sets a distributed double variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (long)
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const long& Val);

    /**
      Sets a distributed double variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (bool)
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const bool& Val);

    /**
      Sets a distributed double variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (string)
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const std::string& Val);

    /**
      Appends an event on a unit
      @param[in] UnitPtr a Unit
      @param[in] Ev the event to append
    */
    void OPENFLUID_AppendEvent(openfluid::core::Unit *UnitPtr,
                               openfluid::core::Event& Ev);


    /**
      Adds a unit to the set of units if not already exists
      @param[in] ClassName class name of the added unit
      @param[in] ID ID of the added unit
      @param[in] PcsOrder Process order of the added unit
    */
    void OPENFLUID_AddUnit(openfluid::core::UnitClass_t ClassName,
                           openfluid::core::UnitID_t ID,
                           openfluid::core::PcsOrd_t PcsOrder);

    /**
      Deletes a unit from the set of units if exists
      @param[in] ClassName class name of the removed unit
      @param[in] ID ID of the added unit
      @return true if the unit has been correctly deleted
    */
    void OPENFLUID_DeleteUnit(openfluid::core::UnitClass_t ClassName,
                              openfluid::core::UnitID_t ID);


    /**
      Adds a from-to connection between two units
      @param[in] ClassNameFrom class name of the "from" unit
      @param[in] IDFrom ID of the "from" unit
      @param[in] ClassNameTo class name of the "to" unit
      @param[in] IDTo ID of the "to" unit
      @return false if the unit connection already exists
    */
    bool OPENFLUID_AddFromToConnection(openfluid::core::UnitClass_t ClassNameFrom,
                                       openfluid::core::UnitID_t IDFrom,
                                       openfluid::core::UnitClass_t ClassNameTo,
                                       openfluid::core::UnitID_t IDTo);

    /**
      Adds a from-to connection between two units
      @param[in] FromUnit pointer to the "from" unit
      @param[in] ToUnit pointer to the "to" unit
      @return false if the connection already exists
    */
    bool OPENFLUID_AddFromToConnection(openfluid::core::Unit* FromUnit,
                                       openfluid::core::Unit* ToUnit);

    /**
      Removes a from-to connection between two units
      @param[in] ClassNameFrom class name of the "from" unit
      @param[in] IDFrom ID of the "from" unit
      @param[in] ClassNameTo class name of the "to" unit
      @param[in] IDTo ID of the "to" unit
      @return false if the unit connection does not exist
    */
    bool OPENFLUID_RemoveFromToConnection(openfluid::core::UnitClass_t ClassNameFrom,
                                          openfluid::core::UnitID_t IDFrom,
                                          openfluid::core::UnitClass_t ClassNameTo,
                                          openfluid::core::UnitID_t IDTo);

    /**
      Removes a from-to connection between two units
      @param[in] FromUnit pointer to the "from" unit
      @param[in] ToUnit pointer to the "to" unit
      @return false if the connection does not exist
    */
    bool OPENFLUID_RemoveFromToConnection(openfluid::core::Unit* FromUnit,
                                          openfluid::core::Unit* ToUnit);


    /**
      Adds a child-parent connection between two units
      @param[in] ClassNameChild class name of the "child" unit
      @param[in] IDChild ID of the "child" unit
      @param[in] ClassNameParent class name of the "parent" unit
      @param[in] IDParent ID of the "parent" unit
      @return false if the connection already exists
    */
    bool OPENFLUID_AddChildParentConnection(openfluid::core::UnitClass_t ClassNameChild,
                                            openfluid::core::UnitID_t IDChild,
                                            openfluid::core::UnitClass_t ClassNameParent,
                                            openfluid::core::UnitID_t IDParent);

    /**
      Adds a child-parent connection between two units
      @param[in] ChildUnit pointer to the "child" unit
      @param[in] ParentUnit pointer to the "parent" unit
      @return false if the connection already exists
    */
    bool OPENFLUID_AddChildParentConnection(openfluid::core::Unit* ChildUnit,
                                            openfluid::core::Unit* ParentUnit);


    /**
      Removes a child-parent connection between two units
      @param[in] ClassNameChild class name of the "child" unit
      @param[in] IDChild ID of the "child" unit
      @param[in] ClassNameParent class name of the "parent" unit
      @param[in] IDParent ID of the "parent" unit
      @return false if the connection does not exist
    */
    bool OPENFLUID_RemoveChildParentConnection(openfluid::core::UnitClass_t ClassNameChild,
                                                openfluid::core::UnitID_t IDChild,
                                                openfluid::core::UnitClass_t ClassNameParent,
                                                openfluid::core::UnitID_t IDParent);

    /**
      Removes a child-parent connection between two units
      @param[in] ChildUnit pointer to the "child" unit
      @param[in] ParentUnit pointer to the "parent" unit
      @return false if the connection does not exist
    */
    bool OPENFLUID_RemoveChildParentConnection(openfluid::core::Unit* ChildUnit,
                                                openfluid::core::Unit* ParentUnit);

    /**
      Builds a ColsNbr x RowsNbr unix matrix with bi-directionnal connections
      @param[in] UnitsClass the name of units class
      @param[in] ColsNbr the number of units on the X axis
      @param[in] RowsNbr the number of units on the Y axis
    */
    void OPENFLUID_BuildUnitsMatrix(const openfluid::core::UnitClass_t& UnitsClass,
                                    const unsigned int& ColsNbr,
                                    const unsigned int& RowsNbr);

    SimulationContributorWare(WareType WType) : SimulationInspectorWare(WType)
    {};


  public:


    virtual ~SimulationContributorWare() {};


};



} } // openfluid::ware



#endif /* __SIMULATIONCONTRIBUTORWARE_HPP__ */

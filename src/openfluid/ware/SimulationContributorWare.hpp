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
  @file SimulationContributorWare.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_WARE_SIMULATIONCONTRIBUTORWARE_HPP__
#define __OPENFLUID_WARE_SIMULATIONCONTRIBUTORWARE_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulationInspectorWare.hpp>


namespace openfluid { namespace ware {


class OPENFLUID_API SimulationContributorWare : public SimulationInspectorWare
{
  private:

  protected:

    /**
       Sets attribute for a unit
       @param[in] UnitPtr a Unit
       @param[in] AttrName the name of the set attribute
       @param[in] Val the value of the set attribute

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Attributes"],
        "title" : "Set attribute value",
        "text" : "OPENFLUID_SetAttribute(%%SEL_START%%UnitPtr%%SEL_END%%,\"attrname\",Val)"
      }
      @endcond
     */
     void OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                 const openfluid::core::AttributeName_t& AttrName,
                                 const openfluid::core::Value& Val);

     /**
       Sets attribute for a unit, as a double
       @param[in] UnitPtr a Unit
       @param[in] AttrName the name of the set attribute
       @param[in] Val the value of the set attribute
     */
     void OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                 const openfluid::core::AttributeName_t& AttrName,
                                 const double& Val);

     /**
       Sets attribute for a unit, as a boolean
       @param[in] UnitPtr a Unit
       @param[in] AttrName the name of the set attribute
       @param[in] Val the value of the set attribute
     */
     void OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                 const openfluid::core::AttributeName_t& AttrName,
                                 bool Val);

     /**
       Sets attribute for a unit, as a long integer
       @param[in] UnitPtr a Unit
       @param[in] AttrName the name of the set attribute
       @param[in] Val the value of the set attribute
     */
     void OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                 const openfluid::core::AttributeName_t& AttrName,
                                 const long& Val);

     /**
       Sets attribute for a unit, as a string
       @param[in] UnitPtr a Unit
       @param[in] AttrName the name of the set attribute
       @param[out] Val the value of the set attribute
     */
     void OPENFLUID_SetAttribute(openfluid::core::SpatialUnit *UnitPtr,
                                 const openfluid::core::AttributeName_t& AttrName,
                                 const std::string& Val);


     /**
       Initializes a distributed variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable
     */
     void OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                   const openfluid::core::VariableName_t& VarName,
                                   const openfluid::core::Value& Val);

     /**
       Initializes a distributed variable value for a unit
       @param[in] aUnit a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Variables"],
        "title" : "Initialize variable with value",
        "text" : "OPENFLUID_InitializeVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",Val)"
      }
      @endcond
     */
     void OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit& aUnit,
                                       const openfluid::core::VariableName_t& VarName,
                                       const openfluid::core::Value& Val);

     /**
       Initializes a distributed double variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable (double)
     */
     void OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                       const openfluid::core::VariableName_t& VarName,
                                       const double& Val);

     /**
       Initializes a distributed long variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable (long)
     */
     void OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                       const openfluid::core::VariableName_t& VarName,
                                       const long& Val);

     /**
       Initializes a distributed boolean variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable (bool)
     */
     void OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                       const openfluid::core::VariableName_t& VarName,
                                       const bool& Val);

     /**
       Initializes a distributed string variable value for a unit
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the variable
       @param[in] Val the added value of the variable (string)
       @warning Use std::string("text") instead of "text" as a value for the Val parameter
                to avoid the automatic overloading between char* and bool types
     */
     void OPENFLUID_InitializeVariable(openfluid::core::SpatialUnit *UnitPtr,
                                       const openfluid::core::VariableName_t& VarName,
                                       const std::string& Val);

    /**
      Appends a distributed variable value for a unit at the end
      of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Variables"],
        "title" : "Append value to a variable",
        "text" : "OPENFLUID_AppendVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",Val)"
      }
      @endcond
    */
    void OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const openfluid::core::Value& Val);

    /**
      Appends a distributed variable value for a unit at the end
      of the previously added values for this variable
      @param[in] aUnit a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable
    */
    void OPENFLUID_AppendVariable(openfluid::core::SpatialUnit& aUnit,
                                  const openfluid::core::VariableName_t& VarName,
                                  const openfluid::core::Value& Val);

    /**
      Appends a distributed double variable value for a unit at the end
      of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (double)
    */
    void OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const double& Val);

    /**
      Appends a distributed long variable value for a unit at the end
      of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (long)
    */
    void OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const long& Val);

    /**
      Appends a distributed boolean variable value for a unit at the end
      of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (bool)
    */
    void OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const bool& Val);

    /**
      Appends a distributed string variable value for a unit at the end
      of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (string)
      @warning Use std::string("text") instead of "text" as a value for the Val parameter
               to avoid the automatic overloading between char* and bool types
    */
    void OPENFLUID_AppendVariable(openfluid::core::SpatialUnit *UnitPtr,
                                  const openfluid::core::VariableName_t& VarName,
                                  const std::string& Val);


    /**
      Sets a distributed variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Variables"],
        "title" : "Update current value of a variable",
        "text" : "OPENFLUID_SetVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",Val)"
      }
      @endcond
    */
    void OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const openfluid::core::Value& Val);

    /**
      Sets a distributed double variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (double)
    */
    void OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const double& Val);

    /**
      Sets a distributed double variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (long)
    */
    void OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const long& Val);

    /**
      Sets a distributed double variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (bool)
    */
    void OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const bool& Val);

    /**
      Sets a distributed double variable value for a unit at the current time index
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Val the added value of the variable (string)
    */
    void OPENFLUID_SetVariable(openfluid::core::SpatialUnit *UnitPtr,
                               const openfluid::core::VariableName_t& VarName,
                               const std::string& Val);

    /**
      Appends an event on a unit
      @param[in] UnitPtr a Unit
      @param[in] Ev the event to append

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Events"],
        "title" : "Append an event",
        "text" : "OPENFLUID_AppendEvent(%%SEL_START%%UnitPtr%%SEL_END%%,Event)"
      }
      @endcond
    */
    void OPENFLUID_AppendEvent(openfluid::core::SpatialUnit *UnitPtr,
                               openfluid::core::Event& Ev);


    /**
      Adds a unit to the set of units if not already exists
      @param[in] ClassName class name of the added unit
      @param[in] ID ID of the added unit
      @param[in] PcsOrder Process order of the added unit

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Add a spatial unit to the spatial graph",
        "text" : "OPENFLUID_AddUnit(%%SEL_START%%ClassName%%SEL_END%%,UnitID,PcsOrder)"
      }
      @endcond
    */
    void OPENFLUID_AddUnit(const openfluid::core::UnitsClass_t& ClassName,
                           openfluid::core::UnitID_t ID,
                           openfluid::core::PcsOrd_t PcsOrder);

    /**
      Deletes a unit from the set of units if exists
      @param[in] ClassName class name of the removed unit
      @param[in] ID ID of the added unit

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Delete a spatial unit of the spatial graph",
        "text" : "OPENFLUID_DeleteUnit(%%SEL_START%%ClassName%%SEL_END%%,ID)"
      }
      @endcond
    */
    void OPENFLUID_DeleteUnit(const openfluid::core::UnitsClass_t& ClassName,
                              openfluid::core::UnitID_t ID);


    /**
      Adds a from-to connection between two units
      @param[in] ClassNameFrom class name of the "from" unit
      @param[in] IDFrom ID of the "from" unit
      @param[in] ClassNameTo class name of the "to" unit
      @param[in] IDTo ID of the "to" unit
      @return false if the unit connection already exists

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Add a From-To connection using classes names and IDs",
        "text" : "OPENFLUID_AddFromToConnection(%%SEL_START%%ClassNameFrom%%SEL_END%%,UnitIDFrom,ClassNameTo,UnitIDTo)"
      }
      @endcond
    */
    bool OPENFLUID_AddFromToConnection(const openfluid::core::UnitsClass_t& ClassNameFrom,
                                       openfluid::core::UnitID_t IDFrom,
                                       const openfluid::core::UnitsClass_t& ClassNameTo,
                                       openfluid::core::UnitID_t IDTo);

    /**
      Adds a from-to connection between two units
      @param[in] FromUnit pointer to the "from" unit
      @param[in] ToUnit pointer to the "to" unit
      @return false if the connection already exists

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Add a From-To connection using pointers to spatial units",
        "text" : "OPENFLUID_AddFromToConnection(%%SEL_START%%FromUnitPtr%%SEL_END%%,ToUnitPtr)"
      }
      @endcond
    */
    bool OPENFLUID_AddFromToConnection(openfluid::core::SpatialUnit* FromUnit,
                                       openfluid::core::SpatialUnit* ToUnit);

    /**
      Removes a from-to connection between two units
      @param[in] ClassNameFrom class name of the "from" unit
      @param[in] IDFrom ID of the "from" unit
      @param[in] ClassNameTo class name of the "to" unit
      @param[in] IDTo ID of the "to" unit
      @return false if the unit connection does not exist

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Remove a From-To connection using classes names and IDs",
        "text" : ["OPENFLUID_RemoveFromToConnection(%%SEL_START%%ClassNameFrom%%SEL_END%%,",
                  "UnitIDFrom,ClassNameTo,UnitIDTo)"]
      }
      @endcond
    */
    bool OPENFLUID_RemoveFromToConnection(const openfluid::core::UnitsClass_t& ClassNameFrom,
                                          openfluid::core::UnitID_t IDFrom,
                                          const openfluid::core::UnitsClass_t& ClassNameTo,
                                          openfluid::core::UnitID_t IDTo);

    /**
      Removes a from-to connection between two units
      @param[in] FromUnit pointer to the "from" unit
      @param[in] ToUnit pointer to the "to" unit
      @return false if the connection does not exist

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Remove a From-To connection using spatial units pointers",
        "text" : "OPENFLUID_RemoveFromToConnection(%%SEL_START%%FromUnitPtr%%SEL_END%%,ToUnitPtr)"
      }
      @endcond
    */
    bool OPENFLUID_RemoveFromToConnection(openfluid::core::SpatialUnit* FromUnit,
                                          openfluid::core::SpatialUnit* ToUnit);


    /**
      Adds a child-parent connection between two units
      @param[in] ClassNameChild class name of the "child" unit
      @param[in] IDChild ID of the "child" unit
      @param[in] ClassNameParent class name of the "parent" unit
      @param[in] IDParent ID of the "parent" unit
      @return false if the connection already exists

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Add a Child-parent connection using classes names and IDs",
        "text" : ["OPENFLUID_AddChildParentConnection(%%SEL_START%%ClassNameChild%%SEL_END%%,",
                  "UnitIDChild,ClassNameParent,UnitIDParent)"]
      }
      @endcond
    */
    bool OPENFLUID_AddChildParentConnection(const openfluid::core::UnitsClass_t& ClassNameChild,
                                            openfluid::core::UnitID_t IDChild,
                                            const openfluid::core::UnitsClass_t& ClassNameParent,
                                            openfluid::core::UnitID_t IDParent);

    /**
      Adds a child-parent connection between two units
      @param[in] ChildUnit pointer to the "child" unit
      @param[in] ParentUnit pointer to the "parent" unit
      @return false if the connection already exists

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Add a Child-parent connection using pointers to spatial units",
        "text" : "OPENFLUID_AddChildParentConnection(%%SEL_START%%ChildUnitPtr%%SEL_END%%,ParentUnitPtr)"
      }
      @endcond
    */
    bool OPENFLUID_AddChildParentConnection(openfluid::core::SpatialUnit* ChildUnit,
                                            openfluid::core::SpatialUnit* ParentUnit);


    /**
      Removes a child-parent connection between two units
      @param[in] ClassNameChild class name of the "child" unit
      @param[in] IDChild ID of the "child" unit
      @param[in] ClassNameParent class name of the "parent" unit
      @param[in] IDParent ID of the "parent" unit
      @return false if the connection does not exist

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Remove a Child-parent connection using classes names and IDs",
        "text" : ["OPENFLUID_RemoveChildParentConnection(%%SEL_START%%ClassNameChild%%SEL_END%%,",
                  "UnitIDChild,ClassNameParent,UnitIDParent)"]
      }
      @endcond
    */
    bool OPENFLUID_RemoveChildParentConnection(const openfluid::core::UnitsClass_t& ClassNameChild,
                                               openfluid::core::UnitID_t IDChild,
                                               const openfluid::core::UnitsClass_t& ClassNameParent,
                                               openfluid::core::UnitID_t IDParent);

    /**
      Removes a child-parent connection between two units
      @param[in] ChildUnit pointer to the "child" unit
      @param[in] ParentUnit pointer to the "parent" unit
      @return false if the connection does not exist

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Remove a Child-parent connection using pointers to spatial units",
        "text" : "OPENFLUID_RemoveChildParentConnection(%%SEL_START%%ChildUnitPtr%%SEL_END%%,ParentUnitPtr)"
      }
      @endcond
    */
    bool OPENFLUID_RemoveChildParentConnection(openfluid::core::SpatialUnit* ChildUnit,
                                               openfluid::core::SpatialUnit* ParentUnit);

    /**
      Builds a ColsNbr x RowsNbr unix matrix with bi-directionnal connections
      @param[in] UnitsClass the name of units class
      @param[in] ColsNbr the number of units on the X axis
      @param[in] RowsNbr the number of units on the Y axis

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Spatial structure"],
        "title" : "Build a matrix of spatial units",
        "text" : "OPENFLUID_BuildUnitsMatrix(%%SEL_START%%ClassName%%SEL_END%%,ColsNbr,RowsNbr)"
      }
      @endcond
    */
    void OPENFLUID_BuildUnitsMatrix(const openfluid::core::UnitsClass_t& UnitsClass,
                                    const unsigned int& ColsNbr,
                                    const unsigned int& RowsNbr);

    SimulationContributorWare(WareType WType) : SimulationInspectorWare(WType)
    { }


  public:

    virtual ~SimulationContributorWare() 
    { }

};


} } // openfluid::ware


#endif /* __OPENFLUID_WARE_SIMULATIONCONTRIBUTORWARE_HPP__ */

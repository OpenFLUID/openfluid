/**
  @file
  @brief Implements LandPrimitivesFunction

  @author  <>
 */


#include <openfluid/ware/PluggableFunction.hpp>


// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN


BEGIN_FUNCTION_SIGNATURE("tests.landprimitives")

  DECLARE_SIGNATURE_NAME("");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("10.07");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

  DECLARE_UPDATED_UNITSGRAPH("Created units graph from scratch");
  DECLARE_UPDATED_UNITSCLASS("VU","Units class for virtual units, parents of OU and TU units");
  DECLARE_UPDATED_UNITSCLASS("TU","Test Units class");
  DECLARE_UPDATED_UNITSCLASS("OU","Other Units class");
  DECLARE_UPDATED_UNITSCLASS("MU","Matrix Units class");
END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


class LandPrimitivesFunction : public openfluid::ware::PluggableFunction
{
  private:

  public:


    LandPrimitivesFunction() : PluggableFunction()
    {


    }


    // =====================================================================
    // =====================================================================


    ~LandPrimitivesFunction()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {  }

    // =====================================================================
    // =====================================================================


    void prepareData()
    {

 /*
      TU.1         TU.2
        |            |
        -->  TU.22 <--
               |
               --> TU.18
                     |
          TU.52 --> OU.5 <-- OU.13
                     |
                     --> OU.25

       VU1 <-> VU2

   with:
   TU1, TU2, TU22, TU18 are children of VU1
   TU52, OU5, OU13, OU25 are children of VU2
*/
      OPENFLUID_AddUnit("VU",1,1);
      OPENFLUID_AddUnit("VU",2,2);
      OPENFLUID_AddUnit("TU",1,1);
      OPENFLUID_AddUnit("TU",2,1);
      OPENFLUID_AddUnit("TU",22,2);
      OPENFLUID_AddUnit("TU",18,3);
      OPENFLUID_AddUnit("TU",52,1);
      OPENFLUID_AddUnit("OU",5,4);
      OPENFLUID_AddUnit("OU",13,1);
      OPENFLUID_AddUnit("OU",25,5);

      OPENFLUID_AddFromToConnection("VU",1,"VU",2);
      OPENFLUID_AddFromToConnection("VU",2,"VU",1);
      OPENFLUID_AddFromToConnection("TU",1,"TU",22);
      OPENFLUID_AddFromToConnection("TU",2,"TU",22);
      OPENFLUID_AddFromToConnection("TU",22,"TU",18);
      OPENFLUID_AddFromToConnection("TU",18,"OU",5);
      OPENFLUID_AddFromToConnection("TU",52,"OU",5);
      OPENFLUID_AddFromToConnection("OU",13,"OU",5);
      OPENFLUID_AddFromToConnection("OU",5,"OU",25);

      OPENFLUID_AddChildParentConnection("TU",1,"VU",1);
      OPENFLUID_AddChildParentConnection("TU",2,"VU",1);
      OPENFLUID_AddChildParentConnection("TU",22,"VU",1);
      OPENFLUID_AddChildParentConnection("TU",18,"VU",1);
      OPENFLUID_AddChildParentConnection("TU",52,"VU",2);
      OPENFLUID_AddChildParentConnection("OU",5,"VU",2);
      OPENFLUID_AddChildParentConnection("OU",13,"VU",2);
      OPENFLUID_AddChildParentConnection("OU",25,"VU",2);


      unsigned int Cols = 5;
      unsigned int Rows = 7;
      OPENFLUID_BuildUnitsMatrix("MU",Cols,Rows);


      unsigned int UnitsCount;

      OPENFLUID_GetUnitsCount("VU",UnitsCount);
      if (UnitsCount != 2)
        OPENFLUID_RaiseError("tests.landprimitives","incorrect number of VU units");

      OPENFLUID_GetUnitsCount("TU",UnitsCount);
      if (UnitsCount != 5)
        OPENFLUID_RaiseError("tests.landprimitives","incorrect number of TU units");

      OPENFLUID_GetUnitsCount("OU",UnitsCount);
      if (UnitsCount != 3)
        OPENFLUID_RaiseError("tests.landprimitives","incorrect number of OU units");

      OPENFLUID_GetUnitsCount("MU",UnitsCount);
      if (UnitsCount != (Cols*Rows))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect number of MU units");


      OPENFLUID_GetUnitsCount(UnitsCount);
      if (UnitsCount != (10+Cols*Rows))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect total number of units");


      openfluid::core::Unit* CurrentUnit = NULL;

      CurrentUnit = OPENFLUID_GetUnit("TU",1);
      if (!OPENFLUID_IsUnitConnectedTo(CurrentUnit,"TU",22))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect from-to connection for unit TU#1");

      CurrentUnit = OPENFLUID_GetUnit("TU",2);
      if (!OPENFLUID_IsUnitConnectedTo(CurrentUnit,"TU",22))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect from-to connection for unit TU#2");

      CurrentUnit = OPENFLUID_GetUnit("TU",2);
      if (OPENFLUID_IsUnitConnectedTo(CurrentUnit,"TU",522))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect from-to connection for unit TU#2 (522)");


      CurrentUnit = OPENFLUID_GetUnit("TU",22);
      if (!OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",1) || !OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect from-to connection for unit TU#22");

      CurrentUnit = OPENFLUID_GetUnit("TU",18);
      if (!OPENFLUID_IsUnitConnectedTo(CurrentUnit,"OU",5))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect from-to connection for unit TU#18");

      CurrentUnit = OPENFLUID_GetUnit("OU",5);
      if (!OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",18))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect from-to connection for unit OU#5");


      CurrentUnit = OPENFLUID_GetUnit("TU",1);
      if (!OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",1))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect parent-child connection for unit TU#1");

      CurrentUnit = OPENFLUID_GetUnit("TU",22);
      if (!OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",1))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect parent-child connection for unit TU#22");

      CurrentUnit = OPENFLUID_GetUnit("TU",18);
      if (OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",2))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect parent-child connection for unit TU#18");

      CurrentUnit = OPENFLUID_GetUnit("OU",5);
      if (!OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",2))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect parent-child connection for unit OU#5");


      openfluid::core::Unit* TU;


      OPENFLUID_UNITS_ORDERED_LOOP("TU", TU)
      {
        OPENFLUID_SetInputData(TU,"indatadbl",double(TU->getID())*0.1);
        OPENFLUID_SetInputData(TU,"indatastr","C0DE1");
      }





      CurrentUnit = OPENFLUID_GetUnit("TU",2);

      if (CurrentUnit == NULL)
        OPENFLUID_RaiseError("tests.landprimitives","incorrect unit TU#2 before deletion");

      CurrentUnit = OPENFLUID_GetUnit("TU",22);
      if (!OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect from-to connection before deletion of unit TU#2");


      CurrentUnit = OPENFLUID_GetUnit("VU",1);
      if (!OPENFLUID_IsUnitParentOf(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect parent-child connection before deletion of unit TU#2");


      OPENFLUID_DeleteUnit("TU",2);

      CurrentUnit = OPENFLUID_GetUnit("TU",2);
      if (CurrentUnit != NULL)
        OPENFLUID_RaiseError("tests.landprimitives","incorrect deletion for unit TU#2");


      CurrentUnit = OPENFLUID_GetUnit("TU",22);
      if (OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect from-to connection after deletion of unit TU#2");

      CurrentUnit = OPENFLUID_GetUnit("VU",1);
      if (OPENFLUID_IsUnitParentOf(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect parent-child connection after deletion of unit TU#2");



      CurrentUnit = OPENFLUID_GetUnit("OU",13);

      if (!OPENFLUID_IsUnitConnectedTo(CurrentUnit,"OU",5))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect from-to connection between units OU#13 and OU#5 before removing");

      OPENFLUID_RemoveFromToConnection("OU",13,"OU",5);

      if (OPENFLUID_IsUnitConnectedTo(CurrentUnit,"OU",5))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect removing of from-to connection between units OU#13 and OU#5");


      CurrentUnit = OPENFLUID_GetUnit("TU",18);

      if (!OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",1))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect child-parent connection between units TU#18 and VU#1 before removing");

      OPENFLUID_RemoveChildParentConnection("TU",18,"VU",1);

      if (OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",1))
        OPENFLUID_RaiseError("tests.landprimitives","incorrect removing of child-parent connection between units TU#18 and VU#1");


    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    { }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      openfluid::core::Unit* TU;
      double DblValue;
      std::string StrValue;

      OPENFLUID_UNITS_ORDERED_LOOP("TU", TU)
      {
        OPENFLUID_GetInputData(TU,"indatadbl",&DblValue);
        if (!openfluid::tools::IsVeryClose(double(TU->getID())*0.1,DblValue))
          OPENFLUID_RaiseError("tests.landprimitives","inputdata error for indatadbl");

        OPENFLUID_GetInputData(TU,"indatastr",&StrValue);
        if (StrValue != "C0DE1")
          OPENFLUID_RaiseError("tests.landprimitives","inputdata error for indatastr");

      }

      return DefaultDeltaT();
    }

    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      return Never();
    }

    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_CLASS(LandPrimitivesFunction)


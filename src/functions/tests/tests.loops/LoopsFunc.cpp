/**
  \file LoopsFunc.cpp
  \brief Implements ...
*/


#include "LoopsFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(LoopsFunction);


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.loops"));
  DECLARE_SIGNATURE_NAME(("test function for units loops"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));
END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


LoopsFunction::LoopsFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


LoopsFunction::~LoopsFunction()
{


}


// =====================================================================
// =====================================================================


bool LoopsFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LoopsFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LoopsFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LoopsFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LoopsFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

/*
  Tests are based on the following spatial domain,
  with classes TestUnits (TU) and OtherUnits (OU),
  represented here as class.ID


TU.1         TU.2
  |            |
  -->  TU.22 <--
         |
         --> TU.18
               |
    TU.52 --> OU.5 <-- OU.13
               |
               --> OU.25



<?xml version="1.0" standalone="yes"?>
<openfluid>
  <domain>
    <definition>

      <unit class="TestUnits" ID="1" pcsorder="1">
        <to class="TestUnits" ID="22" />
      </unit>
      <unit class="TestUnits" ID="2" pcsorder="1">
        <to class="TestUnits" ID="22" />
      </unit>
      <unit class="TestUnits" ID="52" pcsorder="1">
        <to class="OtherUnits" ID="5" />
      </unit>
      <unit class="OtherUnits" ID="13" pcsorder="1">
        <to class="OtherUnits" ID="5" />
      </unit>


      <unit class="TestUnits" ID="22" pcsorder="2">
        <to class="TestUnits" ID="18" />
      </unit>


      <unit class="TestUnits" ID="18" pcsorder="3">
        <to class="OtherUnits" ID="5" />
      </unit>


      <unit class="OtherUnits" ID="5" pcsorder="4">
        <to class="OtherUnits" ID="25" />
      </unit>
      <unit class="OtherUnits" ID="25" pcsorder="5">
      </unit>


    </definition>
  </domain>
</openfluid>


*/


  openfluid::core::Unit *TU;
  openfluid::core::Unit *OU;
  openfluid::core::Unit *ToUnit;
  openfluid::core::Unit *FromUnit;
  openfluid::core::UnitsPtrList_t *ToList;
  openfluid::core::UnitsPtrList_t *FromList;
  unsigned int CountInside;

  DECLARE_UNITS_ORDERED_LOOP(11)
  DECLARE_UNITS_ORDERED_LOOP(1)
  DECLARE_UNITS_ORDERED_LOOP(2)
  DECLARE_UNITS_LIST_LOOP(1)



  // ===== loop inside loop =====

  CountInside = 0;
  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    BEGIN_UNITS_ORDERED_LOOP(2,"TestUnits",TU)
      CountInside++;
    END_LOOP

  END_LOOP

  if (CountInside != 25)
    OPENFLUID_RaiseError("tests.loops","runStep()","wrong units for while loop inside loop");


  // ===== unit class =====

  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)
    if (TU->getClass() != "TestUnits")
      OPENFLUID_RaiseError("tests.loops","runStep()","wrong units class");
  END_LOOP



  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    if (TU->getID() == 22)
    {
      if (TU->getFromUnits("TestUnits") == NULL || TU->getFromUnits("TestUnits")->size() != 2)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(TestUnits) count for TestUnit 22");

      if (TU->getToUnits("TestUnits") == NULL || TU->getToUnits("TestUnits")->size() != 1)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong to-units(TestUnits) count for TestUnit 22");

    }

  END_LOOP


  BEGIN_UNITS_ORDERED_LOOP(11,"OtherUnits",OU)

    if (OU->getID() == 5)
    {
      if (OU->getFromUnits("TestUnits") == NULL || OU->getFromUnits("TestUnits")->size() != 2)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(TestUnits) count for OtherUnit 5");

      if (OU->getFromUnits("OtherUnits") == NULL || OU->getFromUnits("OtherUnits")->size() != 1)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(OtherUnits) count for OtherUnit 5");

      if (OU->getToUnits("OtherUnits") == NULL || OU->getToUnits("OtherUnits")->size() != 1)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong To-units(OtherUnits) count for OtherUnit 5");


      FromList = OU->getFromUnits("TestUnits");
      BEGIN_UNITS_LIST_LOOP(1,FromList,FromUnit)

        if (FromUnit->getID() != 18 && FromUnit->getID() != 52)
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(TestUnits) content for OtherUnit 5");

      END_LOOP

      FromList = OU->getFromUnits("OtherUnits");
      BEGIN_UNITS_LIST_LOOP(1,FromList,FromUnit)

        if (FromUnit->getID() != 13)
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(OtherUnits) content for OtherUnit 5");

      END_LOOP


      ToList = OU->getToUnits("OtherUnits");
      BEGIN_UNITS_LIST_LOOP(1,ToList,ToUnit)

        if (ToUnit->getID() != 25)
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong to-units(OtherUnits) content for OtherUnit 5");

      END_LOOP



    }

  END_LOOP


  // ===== process order =====

  unsigned int LastPcsOrd;
  std::string LastStr, CurrentStr, IDStr;

  LastPcsOrd = 0;
  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)
    if (TU->getProcessOrder() < LastPcsOrd)
    {

      openfluid::tools::ConvertValue(LastPcsOrd,&LastStr);
      openfluid::tools::ConvertValue(TU->getProcessOrder(),&CurrentStr);
      openfluid::tools::ConvertValue(TU->getID(),&IDStr);
      OPENFLUID_RaiseError("tests.loops","runStep()","wrong process order at unit TestUnits#"+ IDStr + " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
    }

    LastPcsOrd = TU->getProcessOrder();
  END_LOOP

  LastPcsOrd = 0;
  BEGIN_UNITS_ORDERED_LOOP(1,"OtherUnits",OU)
    if (OU->getProcessOrder() < LastPcsOrd)
    {
      openfluid::tools::ConvertValue(LastPcsOrd,&LastStr);
      openfluid::tools::ConvertValue(OU->getProcessOrder(),&CurrentStr);
      openfluid::tools::ConvertValue(OU->getID(),&IDStr);
      OPENFLUID_RaiseError("tests.loops","runStep()","wrong process order at unit OtherUnits#"+ IDStr + " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
    }

    LastPcsOrd = TU->getProcessOrder();
  END_LOOP



  return true;
}

// =====================================================================
// =====================================================================


bool LoopsFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}


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
  \file LoopsFunc.cpp
  \brief Implements ...
*/

#include <openfluid/ware/PluggableFunction.hpp>



// =====================================================================
// =====================================================================

DECLARE_FUNCTION_PLUGIN

// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE
  DECLARE_SIGNATURE_ID(("tests.loops"));
  DECLARE_SIGNATURE_NAME(("test function for units loops"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));
END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


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


DECLARE_FUNCTION_PLUGIN;


// =====================================================================
// =====================================================================


/**

*/
class LoopsFunction : public openfluid::ware::PluggableFunction
{
  private:

  public:

    LoopsFunction()
    {

    }


    // =====================================================================
    // =====================================================================


    ~LoopsFunction()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    { }


    // =====================================================================
    // =====================================================================


    void prepareData()
    { }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    { }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      openfluid::core::Unit* CurrentUnit;
      const openfluid::core::UnitsListByClassMap_t* UnitsByClass;
      const openfluid::core::UnitsList_t* UnitsList;
      const openfluid::core::UnitsPtrList_t* ToUnitsPtrList;
      const openfluid::core::UnitsPtrList_t* FromUnitsPtrList;
      std::vector<openfluid::core::UnitClass_t> ClassVector;

      openfluid::core::UnitsListByClassMap_t::const_iterator itUnitsClass;
      openfluid::core::UnitsList_t::const_iterator itUnitsList;
      openfluid::core::UnitsPtrList_t::const_iterator itToUnitsPtrList;
      openfluid::core::UnitsPtrList_t::const_iterator itFromUnitsPtrList;



      UnitsByClass = const_cast<openfluid::core::UnitsListByClassMap_t*>(mp_CoreData->getUnitsByClass());

      std::cout << std::endl;
      std::cout.flush();

      for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
      {
        ClassVector.push_back((*itUnitsClass).first);
      }

      // To List
      std::cout << " -------  To  ------- " << std::endl;
      std::cout.flush();

      for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
      {

        UnitsList = const_cast<openfluid::core::UnitsList_t*>(((*itUnitsClass).second).getList());

        for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
        {
          CurrentUnit = const_cast<openfluid::core::Unit*>(&(*itUnitsList));

          std::string SrcClassStr = CurrentUnit->getClass();
          std::string SrcIDStr = "";
          openfluid::tools::ConvertValue(CurrentUnit->getID(),&SrcIDStr);

          for (unsigned int i=0;i<ClassVector.size();i++)
          {
            ToUnitsPtrList = const_cast<openfluid::core::UnitsPtrList_t*>(CurrentUnit->getToUnits(ClassVector[i]));

            if (ToUnitsPtrList != NULL)
            {
              std::string DestClassStr = ClassVector[i];

              for (itToUnitsPtrList=ToUnitsPtrList->begin();itToUnitsPtrList!=ToUnitsPtrList->end();++itToUnitsPtrList)
              {
                std::string DestIDStr = "";
                openfluid::tools::ConvertValue((*itToUnitsPtrList)->getID(),&DestIDStr);
                std::cout << SrcClassStr << "#" << SrcIDStr << " --> "<< DestClassStr << "#" << DestIDStr << std::endl;
                std::cout.flush();
              }
            }
          }
        }
      }

      // From List
      std::cout << " -------  From  ------- " << std::endl;
      std::cout.flush();

      for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
      {

        UnitsList = const_cast<openfluid::core::UnitsList_t*>(((*itUnitsClass).second).getList());

        for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
        {
          CurrentUnit = const_cast<openfluid::core::Unit*>(&(*itUnitsList));

          std::string SrcClassStr = CurrentUnit->getClass();
          std::string SrcIDStr = "";
          openfluid::tools::ConvertValue(CurrentUnit->getID(),&SrcIDStr);

          for (unsigned int i=0;i<ClassVector.size();i++)
          {
            FromUnitsPtrList = const_cast<openfluid::core::UnitsPtrList_t*>(CurrentUnit->getFromUnits(ClassVector[i]));

            if (FromUnitsPtrList != NULL)
            {
              std::string DestClassStr = ClassVector[i];

              for (itFromUnitsPtrList=FromUnitsPtrList->begin();itFromUnitsPtrList!=FromUnitsPtrList->end();++itFromUnitsPtrList)
              {
                std::string DestIDStr = "";
                openfluid::tools::ConvertValue((*itFromUnitsPtrList)->getID(),&DestIDStr);
                std::cout << SrcClassStr << "#" << SrcIDStr << " <-- "<< DestClassStr << "#" << DestIDStr << std::endl;
                std::cout.flush();
              }
            }
          }
        }


      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::Unit *TU = NULL;
      openfluid::core::Unit *OU = NULL;
      openfluid::core::Unit *ZU = NULL;
      openfluid::core::Unit *FU = NULL;
      openfluid::core::Unit *ToUnit = NULL;
      openfluid::core::Unit *FromUnit = NULL;
      openfluid::core::UnitsPtrList_t *ToList = NULL;
      openfluid::core::UnitsPtrList_t *FromList = NULL;
      openfluid::core::UnitsPtrList_t *FakeList = NULL;
      unsigned int CountInside;
      bool LoopIsOK;


      // ===== loop inside loop =====

      CountInside = 0;
      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          CountInside++;
        }
      }

      if (CountInside != 25)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong units for while loop inside loop");


      // ===== unit class =====

      LoopIsOK = false;
      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {
        LoopIsOK = true;
        if (TU->getClass() != "TestUnits")
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong units class");
      }
      if (!LoopIsOK)
        OPENFLUID_RaiseError("tests.loops","runStep()","Loop is not OK #1");


      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {

        if (TU->getID() == 22)
        {
          if (TU->getFromUnits("TestUnits") == NULL || TU->getFromUnits("TestUnits")->size() != 2)
            OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(TestUnits) count for TestUnit 22");

          if (TU->getToUnits("TestUnits") == NULL || TU->getToUnits("TestUnits")->size() != 1)
            OPENFLUID_RaiseError("tests.loops","runStep()","wrong to-units(TestUnits) count for TestUnit 22");

        }

      }


      OPENFLUID_UNITS_ORDERED_LOOP("OtherUnits",OU)
      {

        if (OU->getID() == 5)
        {

          if (OU->getFromUnits("TestUnits") == NULL || OU->getFromUnits("TestUnits")->size() != 2)
            OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(TestUnits) count for OtherUnit 5");

          if (OU->getFromUnits("OtherUnits") == NULL || OU->getFromUnits("OtherUnits")->size() != 1)
            OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(OtherUnits) count for OtherUnit 5");

          if (OU->getToUnits("OtherUnits") == NULL || OU->getToUnits("OtherUnits")->size() != 1)
            OPENFLUID_RaiseError("tests.loops","runStep()","wrong To-units(OtherUnits) count for OtherUnit 5");

          FromList = OU->getFromUnits("TestUnits");
          OPENFLUID_UNITSLIST_LOOP(FromList,FromUnit)
          {

            if (FromUnit->getID() != 18 && FromUnit->getID() != 52)
              OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(TestUnits) content for OtherUnit 5");

          }

          FromList = OU->getFromUnits("OtherUnits");
          OPENFLUID_UNITSLIST_LOOP(FromList,FromUnit)
          {

            if (FromUnit->getID() != 13)
              OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(OtherUnits) content for OtherUnit 5");

          }


          ToList = OU->getToUnits("OtherUnits");
          OPENFLUID_UNITSLIST_LOOP(ToList,ToUnit)
          {

            if (ToUnit->getID() != 25)
              OPENFLUID_RaiseError("tests.loops","runStep()","wrong to-units(OtherUnits) content for OtherUnit 5");

          }

        }

      }


      // ===== process order =====

      unsigned int LastPcsOrd;
      std::string LastStr, CurrentStr, IDStr, ClassStr;

      LastPcsOrd = 0;
      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {
        if (TU->getProcessOrder() < LastPcsOrd)
        {

          openfluid::tools::ConvertValue(LastPcsOrd,&LastStr);
          openfluid::tools::ConvertValue(TU->getProcessOrder(),&CurrentStr);
          openfluid::tools::ConvertValue(TU->getID(),&IDStr);
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong process order at unit TestUnits#"+ IDStr + " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
        }

        LastPcsOrd = TU->getProcessOrder();
      }

      LastPcsOrd = 0;
      OPENFLUID_UNITS_ORDERED_LOOP("OtherUnits",OU)
      {
        if (OU->getProcessOrder() < LastPcsOrd)
        {
          openfluid::tools::ConvertValue(LastPcsOrd,&LastStr);
          openfluid::tools::ConvertValue(OU->getProcessOrder(),&CurrentStr);
          openfluid::tools::ConvertValue(OU->getID(),&IDStr);
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong process order at unit OtherUnits#"+ IDStr + " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
        }

        LastPcsOrd = OU->getProcessOrder();
      }


      LastPcsOrd = 0;
      OPENFLUID_ALLUNITS_ORDERED_LOOP(ZU)
      {

        if (ZU->getProcessOrder() < LastPcsOrd)
        {
          openfluid::tools::ConvertValue(LastPcsOrd,&LastStr);
          openfluid::tools::ConvertValue(ZU->getProcessOrder(),&CurrentStr);
          openfluid::tools::ConvertValue(ZU->getID(),&IDStr);
          ClassStr = ZU->getClass();
        }
        LastPcsOrd = ZU->getProcessOrder();
      }


      OPENFLUID_UNITS_ORDERED_LOOP("FakeUnits",FU)
      {
        OPENFLUID_RaiseError("tests.loops","runStep()","error in ordered loop on fake units");
      }

      OPENFLUID_UNITSLIST_LOOP(FakeList,FU)
        OPENFLUID_RaiseError("tests.loops","runStep()","error in list loop on fake units");



      // ===== performance =====

      LastPcsOrd = 0;
      OPENFLUID_UNITS_ORDERED_LOOP("PerfUnits",ZU)
      {
        if (ZU->getProcessOrder() < LastPcsOrd)
        {
          openfluid::tools::ConvertValue(LastPcsOrd,&LastStr);
          openfluid::tools::ConvertValue(ZU->getProcessOrder(),&CurrentStr);
          openfluid::tools::ConvertValue(ZU->getID(),&IDStr);
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong process order at unit PerfUnits#"+ IDStr + " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
        }

        LastPcsOrd = ZU->getProcessOrder();
      }


      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    { }

};


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_CLASS(LoopsFunction);



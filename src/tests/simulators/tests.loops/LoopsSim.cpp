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
  @file LoopsSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/DataHelpers.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.loops")

  DECLARE_NAME("test simulator for units loops");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


END_SIMULATOR_SIGNATURE


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


// =====================================================================
// =====================================================================


/**

*/
class LoopsSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:

    LoopsSimulator()
    {

    }


    // =====================================================================
    // =====================================================================


    ~LoopsSimulator()
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

      openfluid::core::SpatialUnit* CurrentUnit;
      const openfluid::core::UnitsListByClassMap_t* UnitsByClass;
      const openfluid::core::UnitsList_t* UnitsList;
      const openfluid::core::UnitsPtrList_t* ToUnitsPtrList;
      const openfluid::core::UnitsPtrList_t* FromUnitsPtrList;
      std::vector<openfluid::core::UnitsClass_t> ClassVector;

      openfluid::core::UnitsListByClassMap_t::const_iterator itUnitsClass;
      openfluid::core::UnitsList_t::const_iterator itUnitsList;
      openfluid::core::UnitsPtrList_t::const_iterator itToUnitsPtrList;
      openfluid::core::UnitsPtrList_t::const_iterator itFromUnitsPtrList;



      UnitsByClass = const_cast<openfluid::core::UnitsListByClassMap_t*>(mp_SpatialData->allSpatialUnitsByClass());

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

        UnitsList = const_cast<openfluid::core::UnitsList_t*>(((*itUnitsClass).second).list());

        for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
        {
          CurrentUnit = const_cast<openfluid::core::SpatialUnit*>(&(*itUnitsList));

          std::string SrcClassStr = CurrentUnit->getClass();
          std::string SrcIDStr = "";
          openfluid::tools::convertValue(CurrentUnit->getID(),&SrcIDStr);

          for (unsigned int i=0;i<ClassVector.size();i++)
          {
            ToUnitsPtrList = const_cast<openfluid::core::UnitsPtrList_t*>(CurrentUnit->toSpatialUnits(ClassVector[i]));

            if (ToUnitsPtrList != nullptr)
            {
              std::string DestClassStr = ClassVector[i];

              for (itToUnitsPtrList=ToUnitsPtrList->begin();itToUnitsPtrList!=ToUnitsPtrList->end();++itToUnitsPtrList)
              {
                std::string DestIDStr = "";
                openfluid::tools::convertValue((*itToUnitsPtrList)->getID(),&DestIDStr);
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

        UnitsList = const_cast<openfluid::core::UnitsList_t*>(((*itUnitsClass).second).list());

        for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
        {
          CurrentUnit = const_cast<openfluid::core::SpatialUnit*>(&(*itUnitsList));

          std::string SrcClassStr = CurrentUnit->getClass();
          std::string SrcIDStr = "";
          openfluid::tools::convertValue(CurrentUnit->getID(),&SrcIDStr);

          for (unsigned int i=0;i<ClassVector.size();i++)
          {
            FromUnitsPtrList =
                const_cast<openfluid::core::UnitsPtrList_t*>(CurrentUnit->fromSpatialUnits(ClassVector[i]));

            if (FromUnitsPtrList != nullptr)
            {
              std::string DestClassStr = ClassVector[i];

              for (itFromUnitsPtrList=FromUnitsPtrList->begin();
                   itFromUnitsPtrList!=FromUnitsPtrList->end();
                   ++itFromUnitsPtrList)
              {
                std::string DestIDStr = "";
                openfluid::tools::convertValue((*itFromUnitsPtrList)->getID(),&DestIDStr);
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
      openfluid::core::SpatialUnit *TU = nullptr;
      openfluid::core::SpatialUnit *OU = nullptr;
      openfluid::core::SpatialUnit *ZU = nullptr;
      openfluid::core::SpatialUnit *FU = nullptr;
      openfluid::core::SpatialUnit *ToUnit = nullptr;
      openfluid::core::SpatialUnit *FromUnit = nullptr;
      openfluid::core::UnitsPtrList_t *ToList = nullptr;
      openfluid::core::UnitsPtrList_t *FromList = nullptr;
      openfluid::core::UnitsPtrList_t *FakeList = nullptr;
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
        OPENFLUID_RaiseError("wrong units for while loop inside loop");


      // ===== unit class =====

      LoopIsOK = false;
      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {
        LoopIsOK = true;
        if (TU->getClass() != "TestUnits")
          OPENFLUID_RaiseError("wrong units class");
      }
      if (!LoopIsOK)
        OPENFLUID_RaiseError("Loop is not OK #1");


      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {

        if (TU->getID() == 22)
        {
          if (TU->fromSpatialUnits("TestUnits") == nullptr || TU->fromSpatialUnits("TestUnits")->size() != 2)
            OPENFLUID_RaiseError("wrong from-units(TestUnits) count for TestUnit 22");

          if (TU->toSpatialUnits("TestUnits") == nullptr || TU->toSpatialUnits("TestUnits")->size() != 1)
            OPENFLUID_RaiseError("wrong to-units(TestUnits) count for TestUnit 22");

        }

      }


      OPENFLUID_UNITS_ORDERED_LOOP("OtherUnits",OU)
      {

        if (OU->getID() == 5)
        {

          if (OU->fromSpatialUnits("TestUnits") == nullptr || OU->fromSpatialUnits("TestUnits")->size() != 2)
            OPENFLUID_RaiseError("wrong from-units(TestUnits) count for OtherUnit 5");

          if (OU->fromSpatialUnits("OtherUnits") == nullptr || OU->fromSpatialUnits("OtherUnits")->size() != 1)
            OPENFLUID_RaiseError("wrong from-units(OtherUnits) count for OtherUnit 5");

          if (OU->toSpatialUnits("OtherUnits") == nullptr || OU->toSpatialUnits("OtherUnits")->size() != 1)
            OPENFLUID_RaiseError("wrong To-units(OtherUnits) count for OtherUnit 5");

          FromList = OU->fromSpatialUnits("TestUnits");
          OPENFLUID_UNITSLIST_LOOP(FromList,FromUnit)
          {

            if (FromUnit->getID() != 18 && FromUnit->getID() != 52)
              OPENFLUID_RaiseError("wrong from-units(TestUnits) content for OtherUnit 5");

          }

          FromList = OU->fromSpatialUnits("OtherUnits");
          OPENFLUID_UNITSLIST_LOOP(FromList,FromUnit)
          {

            if (FromUnit->getID() != 13)
              OPENFLUID_RaiseError("wrong from-units(OtherUnits) content for OtherUnit 5");

          }


          ToList = OU->toSpatialUnits("OtherUnits");
          OPENFLUID_UNITSLIST_LOOP(ToList,ToUnit)
          {

            if (ToUnit->getID() != 25)
              OPENFLUID_RaiseError("wrong to-units(OtherUnits) content for OtherUnit 5");

          }

        }

      }


      // ===== process order =====

      openfluid::core::PcsOrd_t LastPcsOrd;
      std::string LastStr, CurrentStr, IDStr;

      LastPcsOrd = 0;
      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {
        if (TU->getProcessOrder() < LastPcsOrd)
        {

          openfluid::tools::convertValue(LastPcsOrd,&LastStr);
          openfluid::tools::convertValue(TU->getProcessOrder(),&CurrentStr);
          openfluid::tools::convertValue(TU->getID(),&IDStr);
          OPENFLUID_RaiseError("wrong process order at unit TestUnits#"+ IDStr +
                               " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
        }

        LastPcsOrd = TU->getProcessOrder();
      }

      LastPcsOrd = 0;
      OPENFLUID_UNITS_ORDERED_LOOP("OtherUnits",OU)
      {
        if (OU->getProcessOrder() < LastPcsOrd)
        {
          openfluid::tools::convertValue(LastPcsOrd,&LastStr);
          openfluid::tools::convertValue(OU->getProcessOrder(),&CurrentStr);
          openfluid::tools::convertValue(OU->getID(),&IDStr);
          OPENFLUID_RaiseError("wrong process order at unit OtherUnits#"+ IDStr +
                               " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
        }

        LastPcsOrd = OU->getProcessOrder();
      }


      LastPcsOrd = 0;
      OPENFLUID_ALLUNITS_ORDERED_LOOP(ZU)
      {

        if (ZU->getProcessOrder() < LastPcsOrd)
        {
          openfluid::tools::convertValue(LastPcsOrd,&LastStr);
          openfluid::tools::convertValue(ZU->getProcessOrder(),&CurrentStr);
          openfluid::tools::convertValue(ZU->getID(),&IDStr);
        }
        LastPcsOrd = ZU->getProcessOrder();
      }


      OPENFLUID_UNITS_ORDERED_LOOP("FakeUnits",FU)
      {
        OPENFLUID_RaiseError("error in ordered loop on fake units");
        FU->getID();
      }

      OPENFLUID_UNITSLIST_LOOP(FakeList,FU)
        OPENFLUID_RaiseError("error in list loop on fake units");



      // ===== performance =====

      LastPcsOrd = 0;
      OPENFLUID_UNITS_ORDERED_LOOP("PerfUnits",ZU)
      {
        if (ZU->getProcessOrder() < LastPcsOrd)
        {
          openfluid::tools::convertValue(LastPcsOrd,&LastStr);
          openfluid::tools::convertValue(ZU->getProcessOrder(),&CurrentStr);
          openfluid::tools::convertValue(ZU->getID(),&IDStr);
          OPENFLUID_RaiseError("wrong process order at unit PerfUnits#"+ IDStr +
                               " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
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


DEFINE_SIMULATOR_CLASS(LoopsSimulator);



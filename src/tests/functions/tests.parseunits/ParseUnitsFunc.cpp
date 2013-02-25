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
  \file GlobalParams.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableFunction.hpp>


// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN

// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE("tests.parseunits")

  DECLARE_SIGNATURE_NAME("test function for parsing units");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

END_FUNCTION_SIGNATURE

/**

*/
class ParseUnitsFunction : public openfluid::ware::PluggableFunction
{
  private:

  public:


  ParseUnitsFunction() : PluggableFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  ~ParseUnitsFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  void initParams(const openfluid::ware::WareParams_t& /*Params*/)
  {  }

  // =====================================================================
  // =====================================================================


  void prepareData()
  {  }


  // =====================================================================
  // =====================================================================


  void checkConsistency()
  {  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest initializeRun()
  {
    return DefaultDeltaT();
  }

  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
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
/*
    std::cout << std::endl;
    std::cout.flush();
*/
    for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
    {
      ClassVector.push_back((*itUnitsClass).first);
    }

    // To List
/*    std::cout << " -------  To  ------- " << std::endl;
    std::cout.flush();*/

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
/*              std::cout << SrcClassStr << "#" << SrcIDStr << " --> "<< DestClassStr << "#" << DestIDStr << std::endl;
              std::cout.flush();*/
            }
          }
        }
      }
    }

    // From List
/*    std::cout << " -------  From  ------- " << std::endl;
    std::cout.flush();*/

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
/*              std::cout << SrcClassStr << "#" << SrcIDStr << " <-- "<< DestClassStr << "#" << DestIDStr << std::endl;
              std::cout.flush();*/
            }
          }
        }
      }


    }


    return DefaultDeltaT();
  }

  // =====================================================================
  // =====================================================================


  void finalizeRun()
  {  }

};

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_CLASS(ParseUnitsFunction)


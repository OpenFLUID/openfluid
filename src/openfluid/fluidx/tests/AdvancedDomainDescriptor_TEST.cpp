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
 \file AdvancedDomainDescriptor_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_AdvancedDomainDescriptor
#include <boost/test/unit_test.hpp>

#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>
#include "tests-config.hpp"
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <algorithm>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  const openfluid::fluidx::AdvancedDomainDescriptor::UnitsByIdByClass_t* Units =
      &(Domain.spatialUnitsByIdByClass());

  BOOST_CHECK_EQUAL(Units->size(), 3);

  // unitsA
  openfluid::fluidx::AdvancedDomainDescriptor::UnitsByIdByClass_t::const_iterator it =
      Units->begin();
  BOOST_CHECK_EQUAL(it->first, "unitsA");
  BOOST_CHECK_EQUAL(it->second.size(), 8);
  for (std::map<int, openfluid::fluidx::AdvancedUnitDescriptor>::const_iterator it2 =
      it->second.begin(); it2 != it->second.end(); ++it2)
  {
    BOOST_CHECK_EQUAL(it2->first, it2->second.UnitDescriptor->getID());
    BOOST_CHECK_EQUAL(it2->second.UnitDescriptor->getUnitsClass(), "unitsA");
    BOOST_CHECK_EQUAL(*(it2->second.Attributes.at("indataA")), "1.1");
  }
  BOOST_CHECK_EQUAL(it->second.at(1).EventsDescriptors.size(), 2);
  BOOST_CHECK(Domain.isClassNameExists("unitsA"));
  BOOST_CHECK_EQUAL(Domain.getIDsOfClass("unitsA").size(), 8);
  BOOST_CHECK_EQUAL(Domain.getAttributesNames("unitsA").size(), 1);
  BOOST_CHECK_EQUAL(*Domain.getAttributesNames("unitsA").begin(), "indataA");

  // unitsB
  it++;
  BOOST_CHECK_EQUAL(it->first, "unitsB");
  BOOST_CHECK_EQUAL(it->second.size(), 5);
  for (std::map<int, openfluid::fluidx::AdvancedUnitDescriptor>::const_iterator it2 =
      it->second.begin(); it2 != it->second.end(); ++it2)
  {
    BOOST_CHECK_EQUAL(it2->first, it2->second.UnitDescriptor->getID());
    BOOST_CHECK_EQUAL(it2->second.UnitDescriptor->getUnitsClass(), "unitsB");
  }
  BOOST_CHECK(Domain.isClassNameExists("unitsB"));
  BOOST_CHECK_EQUAL(Domain.getIDsOfClass("unitsB").size(), 5);
  BOOST_CHECK_EQUAL(Domain.getAttributesNames("unitsB").size(), 3);

  // unitsP
  it++;
  BOOST_CHECK_EQUAL(it->first, "unitsP");
  BOOST_CHECK_EQUAL(it->second.size(), 1);
  for (std::map<int, openfluid::fluidx::AdvancedUnitDescriptor>::const_iterator it2 =
      it->second.begin(); it2 != it->second.end(); ++it2)
  {
    BOOST_CHECK_EQUAL(it2->first, it2->second.UnitDescriptor->getID());
    BOOST_CHECK_EQUAL(it2->second.UnitDescriptor->getUnitsClass(), "unitsP");
  }
  BOOST_CHECK(Domain.isClassNameExists("unitsP"));
  BOOST_CHECK_EQUAL(Domain.getIDsOfClass("unitsP").size(), 1);
  BOOST_CHECK_EQUAL(Domain.getAttributesNames("unitsP").size(), 0);

  BOOST_CHECK_EQUAL(Domain.spatialUnit("unitsP",1).UnitDescriptor,
                    &*(FXDesc.spatialDomainDescriptor().spatialUnits().begin()));
  BOOST_CHECK_EQUAL(&Domain.spatialUnitDescriptor("unitsP",1),
                    &*(FXDesc.spatialDomainDescriptor().spatialUnits().begin()));

  // relations
  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> A3_Tos = Domain.toSpatialUnits(A3);
  std::list<openfluid::core::UnitClassID_t> A3_Froms = Domain.getFromSpatialUnits(
      A3);
  std::list<openfluid::core::UnitClassID_t> A3_Parents =
      Domain.parentSpatialUnits(A3);
  std::list<openfluid::core::UnitClassID_t> A3_Children =
      Domain.getChildSpatialUnits(A3);

  BOOST_CHECK_EQUAL(A3_Tos.size(), 1);
  BOOST_CHECK_EQUAL(std::count(A3_Tos.begin(), A3_Tos.end(),B11), 1);
  BOOST_CHECK(A3_Froms.empty());
  BOOST_CHECK_EQUAL(A3_Parents.size(), 1);
  BOOST_CHECK_EQUAL(std::count(A3_Parents.begin(), A3_Parents.end(),P1), 1);
  BOOST_CHECK(A3_Children.empty());

  std::list<openfluid::core::UnitClassID_t> B11_Tos = Domain.toSpatialUnits(B11);
  std::list<openfluid::core::UnitClassID_t> B11_Froms = Domain.getFromSpatialUnits(
      B11);
  std::list<openfluid::core::UnitClassID_t> B11_Parents =
      Domain.parentSpatialUnits(B11);
  std::list<openfluid::core::UnitClassID_t> B11_Children =
      Domain.getChildSpatialUnits(B11);

  BOOST_CHECK_EQUAL(B11_Tos.size(), 1);
  BOOST_CHECK_EQUAL(std::count(B11_Tos.begin(), B11_Tos.end(),B3), 1);
  BOOST_CHECK_EQUAL(B11_Froms.size(), 1);
  BOOST_CHECK_EQUAL(std::count(B11_Froms.begin(), B11_Froms.end(),A3), 1);
  BOOST_CHECK_EQUAL(B11_Parents.size(), 1);
  BOOST_CHECK_EQUAL(std::count(B11_Parents.begin(), B11_Parents.end(),P1), 1);
  BOOST_CHECK(B11_Children.empty());

  std::list<openfluid::core::UnitClassID_t> P1_Children =
      Domain.getChildSpatialUnits(P1);

  BOOST_CHECK_EQUAL(P1_Children.size(), 2);
  BOOST_CHECK_EQUAL(std::count(P1_Children.begin(), P1_Children.end(),A3), 1);
  BOOST_CHECK_EQUAL(std::count(P1_Children.begin(), P1_Children.end(),B11), 1);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_wrong_construction)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);

  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/wrongattrs");

  // "Attribute indataB1 doesn't exist for Unit 3 of class unitsB"
  BOOST_CHECK_THROW(
      openfluid::fluidx::AdvancedDomainDescriptor Domain(FXDesc.spatialDomainDescriptor()),
      openfluid::base::FrameworkException);

  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/wrongrelation");

  // "Unit 99 of class unitsB in "To" relation of unit 1 of class unitsA doesn't exist"
  BOOST_CHECK_THROW(
      openfluid::fluidx::AdvancedDomainDescriptor Domain(FXDesc.spatialDomainDescriptor()),
      openfluid::base::FrameworkException);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_addUnit)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  openfluid::fluidx::SpatialUnitDescriptor U;
  U.setUnitsClass("unitsA");
  U.setID(1);

  BOOST_CHECK_THROW(Domain.addUnit(&U), openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.spatialUnit("unitsZ",1), openfluid::base::FrameworkException);
  BOOST_CHECK(!Domain.isClassNameExists("unitsZ"));
  BOOST_CHECK_EQUAL(Domain.getIDsOfClass("unitsZ").size(), 0);
  BOOST_CHECK(Domain.getClassNames().count("unitsA"));
  BOOST_CHECK(!Domain.getClassNames().count("unitsZ"));

  U.setUnitsClass("unitsZ");
  Domain.addUnit(&U);

  BOOST_CHECK_EQUAL(Domain.spatialUnitsByIdByClass().size(), 4);
  BOOST_CHECK(Domain.isClassNameExists("unitsZ"));
  BOOST_CHECK_EQUAL(Domain.getIDsOfClass("unitsZ").size(), 1);
  BOOST_CHECK(Domain.getClassNames().count("unitsA"));
  BOOST_CHECK(Domain.getClassNames().count("unitsZ"));

  openfluid::fluidx::SpatialUnitDescriptor U2;
  U2.setUnitsClass("unitsB");
  U2.setID(99);
  Domain.addUnit(&U2);

  BOOST_CHECK_EQUAL(Domain.spatialUnit("unitsB",99).Attributes.size(), 3);
  BOOST_CHECK_EQUAL(Domain.attribute("unitsB",99,"indataB1"), "-");
  BOOST_CHECK_EQUAL(Domain.attribute("unitsB",99,"indataB1"), "-");
  BOOST_CHECK_EQUAL(Domain.attribute("unitsB",99,"indataB1"), "-");

  openfluid::fluidx::SpatialUnitDescriptor U3;
  U3.setUnitsClass("unitsB");
  U3.setID(999);
  U3.parentSpatialUnits().push_back(std::make_pair("unitsB", 555));
  BOOST_CHECK_THROW(Domain.addUnit(&U3), openfluid::base::FrameworkException);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_deleteUnit)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  Domain.deleteUnit("unitsZ", 1);

  Domain.deleteUnit("unitsA", 7);
  Domain.deleteUnit("unitsP", 1);

  const openfluid::fluidx::AdvancedDomainDescriptor::UnitsByIdByClass_t* Units =
      &(Domain.spatialUnitsByIdByClass());

  BOOST_CHECK_EQUAL(Units->size(), 2);
  BOOST_CHECK_EQUAL(Units->begin()->second.size(), 7);
  BOOST_CHECK_THROW(Domain.spatialUnit("unitsA",7), openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.spatialUnit("unitsP",1), openfluid::base::FrameworkException);
  BOOST_CHECK_EQUAL(Domain.getIDsOfClass("unitsZ").size(), 0);
  BOOST_CHECK_EQUAL(Domain.getIDsOfClass("unitsA").size(), 7);
  BOOST_CHECK(Domain.isClassNameExists("unitsA"));
  BOOST_CHECK(!Domain.isClassNameExists("unitsZ"));

  BOOST_CHECK_EQUAL(FXDesc.spatialDomainDescriptor().spatialUnits().size(), 12);

  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(FXDesc.spatialDomainDescriptor().attributes());
  for (std::list<openfluid::fluidx::AttributesDescriptor>::iterator it =
      Attrs->begin(); it != Attrs->end(); ++it)
  {
    BOOST_CHECK(!(it->getUnitsClass() == "unitsA" && it->attributes().count(7)));
    BOOST_CHECK(!(it->getUnitsClass() == "unitsP"));
  }

  std::list<openfluid::fluidx::EventDescriptor>* Events =
      &(FXDesc.spatialDomainDescriptor().events());
  for (std::list<openfluid::fluidx::EventDescriptor>::iterator it =
      Events->begin(); it != Events->end(); ++it)
  {
    BOOST_CHECK(!(it->getUnitClass() == "unitsA" && it->getUnitID() == 7));
    BOOST_CHECK(!(it->getUnitClass() == "unitsP"));
  }

  // relations
  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t A7 = std::make_pair("unitsA", 7);
  openfluid::core::UnitClassID_t B1 = std::make_pair("unitsB", 1);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);

  std::list<openfluid::core::UnitClassID_t> B3_Froms = Domain.getFromSpatialUnits(
      B3);
  BOOST_CHECK_EQUAL(B3_Froms.size(), 2);
  BOOST_CHECK(std::count(B3_Froms.begin(), B3_Froms.end(),B1));
  BOOST_CHECK(std::count(B3_Froms.begin(), B3_Froms.end(),B11));
  BOOST_CHECK(!std::count(B3_Froms.begin(), B3_Froms.end(),A7));

  BOOST_CHECK_EQUAL(Domain.parentSpatialUnits(A3).size(), 0);
  BOOST_CHECK_EQUAL(Domain.parentSpatialUnits(B11).size(), 0);

  // delete all class unitsB
  BOOST_CHECK_EQUAL(Domain.getAttributesNames("unitsB").size(), 3);
  std::set<int> IDs = Domain.getIDsOfClass("unitsB");
  for (std::set<int>::iterator it = IDs.begin(); it != IDs.end(); ++it)
    Domain.deleteUnit("unitsB", *it);
  for (std::set<int>::iterator it = IDs.begin(); it != IDs.end(); ++it)
    BOOST_CHECK_THROW(Domain.spatialUnit("unitsB",*it),
                      openfluid::base::FrameworkException);
  BOOST_CHECK_EQUAL(Domain.getIDsOfClass("unitsB").size(), 0);
  BOOST_CHECK(!Domain.isClassNameExists("unitsB"));
  BOOST_CHECK_EQUAL(Domain.getAttributesNames("unitsB").size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_deleteUnit_manyAttrsDesc)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/manyattrdescs");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  std::set<std::string> AttrsNames = Domain.getAttributesNames("unitsA");
  BOOST_CHECK_EQUAL(AttrsNames.size(), 3);
  BOOST_CHECK(AttrsNames.count("indataA"));
  BOOST_CHECK(AttrsNames.count("indataB"));
  BOOST_CHECK(AttrsNames.count("indataC"));

  Domain.deleteUnit("unitsA", 2);
  AttrsNames = Domain.getAttributesNames("unitsA");
  BOOST_CHECK_EQUAL(AttrsNames.size(), 3);
  BOOST_CHECK(AttrsNames.count("indataA"));
  BOOST_CHECK(AttrsNames.count("indataB"));
  BOOST_CHECK(AttrsNames.count("indataC"));

  Domain.deleteUnit("unitsA", 3);
  AttrsNames = Domain.getAttributesNames("unitsA");
  BOOST_CHECK_EQUAL(AttrsNames.size(), 3);
  BOOST_CHECK(AttrsNames.count("indataA"));
  BOOST_CHECK(AttrsNames.count("indataB"));
  BOOST_CHECK(AttrsNames.count("indataC"));

  Domain.deleteUnit("unitsA", 1);
  AttrsNames = Domain.getAttributesNames("unitsA");
  BOOST_CHECK_EQUAL(AttrsNames.size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_deleteAttrs_manyAttrsDesc)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/manyattrdescs");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  std::set<std::string> AttrsNames = Domain.getAttributesNames("unitsA");
  BOOST_CHECK_EQUAL(AttrsNames.size(), 3);
  BOOST_CHECK(AttrsNames.count("indataA"));
  BOOST_CHECK(AttrsNames.count("indataB"));
  BOOST_CHECK(AttrsNames.count("indataC"));

  Domain.deleteAttribute("unitsA", "indataB");
  AttrsNames = Domain.getAttributesNames("unitsA");
  BOOST_CHECK_EQUAL(AttrsNames.size(), 2);
  BOOST_CHECK(AttrsNames.count("indataA"));
  BOOST_CHECK(AttrsNames.count("indataC"));

  Domain.deleteAttribute("unitsA", "indataA");
  AttrsNames = Domain.getAttributesNames("unitsA");
  BOOST_CHECK_EQUAL(AttrsNames.size(), 1);
  BOOST_CHECK(AttrsNames.count("indataC"));

  Domain.deleteAttribute("unitsA", "indataC");
  AttrsNames = Domain.getAttributesNames("unitsA");
  BOOST_CHECK_EQUAL(AttrsNames.size(), 0);

  std::string OutputDirMany = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXDescriptorMany";
  FXDesc.writeToManyFiles(OutputDirMany);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_add_replace_getAttr)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  BOOST_CHECK_THROW(Domain.addAttribute("unitsZ", "IData", "123"),
                    openfluid::base::FrameworkException);

  BOOST_CHECK_EQUAL(Domain.getAttributesNames("unitsA").size(), 1);
  BOOST_CHECK(!Domain.getAttributesNames("unitsA").count("NewData"));

  Domain.addAttribute("unitsA", "NewData", "123");

  BOOST_CHECK_EQUAL(Domain.getAttributesNames("unitsA").size(), 2);
  BOOST_CHECK(Domain.getAttributesNames("unitsA").count("NewData"));

  std::string* FXValue = 0;

  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(FXDesc.spatialDomainDescriptor().attributes());

  for (std::list<openfluid::fluidx::AttributesDescriptor>::iterator it =
      Attrs->begin(); it != Attrs->end(); ++it)
  {
    if (it->getUnitsClass() == "unitsA")
    {
      std::map<openfluid::core::UnitID_t,
          openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>* Data =
          &(it->attributes());

      for (std::map<openfluid::core::UnitID_t,
          openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>::iterator it2 =
          Data->begin(); it2 != Data->end(); ++it2)
      {
        if (it2->first == 7 && it2->second.count("NewData"))
        {
          FXValue = &it2->second.at("NewData");
          break;
        }
      }
    }
  }

  BOOST_CHECK_EQUAL(*FXValue, "123");

  BOOST_CHECK_EQUAL(*Domain.spatialUnit("unitsA",7).Attributes.at("NewData"), "123");

  *FXValue = "456";

  BOOST_CHECK_EQUAL(*Domain.spatialUnit("unitsA",7).Attributes.at("NewData"), "456");

  BOOST_CHECK_THROW(Domain.attribute("unitsZ",1, "NewData"),
                    openfluid::base::FrameworkException);

  Domain.attribute("unitsA", 7, "NewData") = "789";

  BOOST_CHECK_EQUAL(*Domain.spatialUnit("unitsA",7).Attributes.at("NewData"), "789");
  BOOST_CHECK_EQUAL(*FXValue, "789");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_deleteAttrs)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  BOOST_CHECK_EQUAL(Domain.spatialUnit("unitsA",7).Attributes.size(), 1);
  BOOST_CHECK_EQUAL(Domain.spatialUnit("unitsB",7).Attributes.size(), 3);
  BOOST_CHECK(Domain.getAttributesNames("unitsA").count("indataA"));
  BOOST_CHECK(Domain.getAttributesNames("unitsB").count("indataB3"));

  Domain.deleteAttribute("unitsA", "indataA");
  Domain.deleteAttribute("unitsB", "indataB3");

  BOOST_CHECK_EQUAL(Domain.spatialUnit("unitsA",7).Attributes.size(), 0);
  BOOST_CHECK_EQUAL(Domain.spatialUnit("unitsB",7).Attributes.size(), 2);
  BOOST_CHECK(!Domain.getAttributesNames("unitsA").count("indataA"));
  BOOST_CHECK(!Domain.getAttributesNames("unitsB").count("indataB3"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_renameAttrs)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  BOOST_CHECK_EQUAL(Domain.attribute("unitsA",7, "indataA"), "1.1");
  BOOST_CHECK_THROW(Domain.attribute("unitsA",7, "NewData"),
                    openfluid::base::FrameworkException);
  BOOST_CHECK(Domain.getAttributesNames("unitsA").count("indataA"));
  BOOST_CHECK(!Domain.getAttributesNames("unitsA").count("NewData"));

  Domain.renameAttribute("unitsA", "indataA", "NewData");

  BOOST_CHECK_EQUAL(Domain.attribute("unitsA",7, "NewData"), "1.1");
  BOOST_CHECK_THROW(Domain.attribute("unitsA",7, "indataA"),
                    openfluid::base::FrameworkException);
  BOOST_CHECK(!Domain.getAttributesNames("unitsA").count("indataA"));
  BOOST_CHECK(Domain.getAttributesNames("unitsA").count("NewData"));

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations_on_events)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  openfluid::fluidx::SpatialUnitDescriptor UnitDesc;
  UnitDesc.setUnitsClass("TU");
  UnitDesc.setID(1);

  Domain.addUnit(&UnitDesc);

  for (int i=0; i< 10; i++)
  {
    openfluid::core::Event Ev(openfluid::core::DateTime(2000,1,i,12,13,14));
    Domain.addEvent("TU",1,Ev);
  }

  Domain.deleteEvent("TU",1,1);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations_on_relations)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t A5 = std::make_pair("unitsA", 5);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);
  openfluid::core::UnitClassID_t Z2 = std::make_pair("wrongUnit", 2);

  BOOST_CHECK_THROW(Domain.addFromToRelation(B11, Z2),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.addFromToRelation(Z2, B11),
                    openfluid::base::FrameworkException);

  Domain.addFromToRelation(B11, A5);

  std::list<openfluid::core::UnitClassID_t> B11_Tos = Domain.toSpatialUnits(B11);
  std::list<openfluid::core::UnitClassID_t> A5_Froms = Domain.getFromSpatialUnits(
      A5);

  BOOST_CHECK_EQUAL(B11_Tos.size(), 2);
  BOOST_CHECK_EQUAL(std::count(B11_Tos.begin(), B11_Tos.end(),B3), 1);
  BOOST_CHECK_EQUAL(std::count(B11_Tos.begin(), B11_Tos.end(),A5), 1);
  BOOST_CHECK_EQUAL(A5_Froms.size(), 1);
  BOOST_CHECK_EQUAL(std::count(A5_Froms.begin(), A5_Froms.end(),B11), 1);

  BOOST_CHECK_THROW(Domain.addParentChildRelation(B11, Z2),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.addParentChildRelation(Z2, B11),
                    openfluid::base::FrameworkException);

  Domain.addParentChildRelation(A3, B11);

  std::list<openfluid::core::UnitClassID_t> B11_Parents =
      Domain.parentSpatialUnits(B11);
  std::list<openfluid::core::UnitClassID_t> A3_Children =
      Domain.getChildSpatialUnits(A3);

  BOOST_CHECK_EQUAL(B11_Parents.size(), 2);
  BOOST_CHECK_EQUAL(std::count(B11_Parents.begin(), B11_Parents.end(),P1), 1);
  BOOST_CHECK_EQUAL(std::count(B11_Parents.begin(), B11_Parents.end(),A3), 1);
  BOOST_CHECK_EQUAL(A3_Children.size(), 1);
  BOOST_CHECK_EQUAL(std::count(A3_Children.begin(), A3_Children.end(),B11), 1);

  BOOST_CHECK_THROW(Domain.removeFromToRelation(Z2, B11),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.removeFromToRelation(B11, Z2),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.removeFromToRelation(B11, A3),
                    openfluid::base::FrameworkException);

  Domain.removeFromToRelation(A3, B11);

  std::list<openfluid::core::UnitClassID_t> B11_Froms = Domain.getFromSpatialUnits(
      B11);
  std::list<openfluid::core::UnitClassID_t> A3_Tos = Domain.toSpatialUnits(A3);

  BOOST_CHECK(B11_Froms.empty());
  BOOST_CHECK(A3_Tos.empty());

  BOOST_CHECK_THROW(Domain.removeParentChildRelation(Z2, B11),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.removeParentChildRelation(B11, Z2),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.removeParentChildRelation(A3, P1),
                    openfluid::base::FrameworkException);

  Domain.removeParentChildRelation(P1, A3);

  std::list<openfluid::core::UnitClassID_t> A3_Parents =
      Domain.parentSpatialUnits(A3);

  BOOST_CHECK(A3_Parents.empty());

  std::list<openfluid::core::UnitClassID_t> P1_Children =
      Domain.getChildSpatialUnits(P1);

  BOOST_CHECK_EQUAL(P1_Children.size(), 1);
  BOOST_CHECK_EQUAL(std::count(P1_Children.begin(), P1_Children.end(),B11), 1);

  Domain.addParentChildRelation(P1, A3);
  Domain.addFromToRelation(A3, B11);
  Domain.addFromToRelation(P1, A3);

  BOOST_CHECK(!Domain.getChildSpatialUnits(A3).empty());
  BOOST_CHECK(!Domain.parentSpatialUnits(A3).empty());
  BOOST_CHECK(!Domain.getFromSpatialUnits(A3).empty());
  BOOST_CHECK(!Domain.toSpatialUnits(A3).empty());

  Domain.clearRelations(A3);

  BOOST_CHECK(Domain.getChildSpatialUnits(A3).empty());
  BOOST_CHECK(Domain.parentSpatialUnits(A3).empty());
  BOOST_CHECK(Domain.getFromSpatialUnits(A3).empty());
  BOOST_CHECK(Domain.toSpatialUnits(A3).empty());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations_on_relations_afterAdd)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  openfluid::core::UnitClassID_t A1 = std::make_pair("unitsA", 1);
  openfluid::core::UnitClassID_t A2 = std::make_pair("unitsA", 2);

  openfluid::fluidx::SpatialUnitDescriptor U1;
  U1.setUnitsClass("unitsA");
  U1.setID(1);

  openfluid::fluidx::SpatialUnitDescriptor U2;
  U2.setUnitsClass("unitsA");
  U2.setID(2);

  Domain.addUnit(&U1);
  Domain.addUnit(&U2);

  Domain.addParentChildRelation(A1, A2);

  std::list<openfluid::core::UnitClassID_t> A1Children =
      Domain.getChildSpatialUnits(A1);
  BOOST_CHECK_EQUAL(A1Children.size(), 1);
  BOOST_CHECK(std::count(A1Children.begin(),A1Children.end(), A2));

  std::list<openfluid::core::UnitClassID_t> A2Parents =
      Domain.parentSpatialUnits(A2);
  BOOST_CHECK_EQUAL(A2Parents.size(), 1);
  BOOST_CHECK(std::count(A2Parents.begin(),A2Parents.end(), A1));

  Domain.addFromToRelation(A1, A2);

  std::list<openfluid::core::UnitClassID_t> A1Tos = Domain.toSpatialUnits(A1);
  BOOST_CHECK_EQUAL(A1Tos.size(), 1);
  BOOST_CHECK(std::count(A1Tos.begin(),A1Tos.end(), A2));

  std::list<openfluid::core::UnitClassID_t> A2Froms = Domain.getFromSpatialUnits(A2);
  BOOST_CHECK_EQUAL(A2Froms.size(), 1);
  BOOST_CHECK(std::count(A2Froms.begin(),A2Froms.end(), A1));
}
// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_clearDomain)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedDomainDescriptor Domain(
      FXDesc.spatialDomainDescriptor());

  Domain.clearDomain();

  BOOST_CHECK(Domain.spatialUnitsByIdByClass().empty());
  BOOST_CHECK(Domain.getClassNames().empty());
  BOOST_CHECK(Domain.getIDsOfClass("unitsA").empty());
  BOOST_CHECK(Domain.getAttributesNames("unitsA").empty());
  BOOST_CHECK_THROW(Domain.spatialUnit("unitsA",1), openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.spatialUnitDescriptor("unitsA",1),
                    openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(Domain.attribute("unitsA",1,"indataA"),
                    openfluid::base::FrameworkException);
}

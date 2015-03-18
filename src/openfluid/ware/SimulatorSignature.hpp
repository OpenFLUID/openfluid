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
  @file SimulatorSignature.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#ifndef __OPENFLUID_WARE_SIMULATORSIGNATURE_HPP__
#define __OPENFLUID_WARE_SIMULATORSIGNATURE_HPP__


#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/ware/WareSignature.hpp>



namespace openfluid { namespace ware {


typedef std::string SimMethod_t;

typedef std::string SimProcess_t;

typedef std::string SimDomain_t;


// =====================================================================
// =====================================================================


/**
Class for storage of the definition of data handled by the simulator.
*/
class OPENFLUID_API SignatureDataItem
{
  public:

    std::string DataName;
    std::string Description;
    std::string DataUnit;

    SignatureDataItem() :
      DataName(""),Description(""),DataUnit("")
    {  }

    SignatureDataItem(std::string DName, std::string DDescription, std::string DUnit) :
          DataName(DName),Description(DDescription),DataUnit(DUnit)
    {  }
};



/**
Class for storage of the definition of data handled by the simulator.
*/
class OPENFLUID_API SignatureSpatialDataItem : public SignatureDataItem
{
  public:

    openfluid::core::UnitClass_t UnitClass;

    SignatureSpatialDataItem() :
      SignatureDataItem(), UnitClass("")
    {  }

    SignatureSpatialDataItem(std::string DName, openfluid::core::UnitClass_t UClass,
        std::string DDescription, std::string DUnit) :
          SignatureDataItem(DName,DDescription,DUnit),UnitClass(UClass)
    {  }
};


/**
Class for storage of the definition of typed data handled by the simulator.
*/
class OPENFLUID_API SignatureTypedSpatialDataItem : public SignatureSpatialDataItem
{

  public:

    openfluid::core::Value::Type DataType;

    SignatureTypedSpatialDataItem() :
      SignatureSpatialDataItem(), DataType(openfluid::core::Value::NONE) {}

    SignatureTypedSpatialDataItem(std::string DName, openfluid::core::UnitClass_t UClass,
        std::string DDescription, std::string DUnit);
};


/**
  Class for storage of the definition of the data handled by the simulator. This is part of the signature.
*/
class OPENFLUID_API SignatureHandledData
{
  public:

    std::vector<SignatureDataItem> UsedParams;

    std::vector<SignatureDataItem> RequiredParams;

    std::vector<SignatureTypedSpatialDataItem> ProducedVars;

    std::vector<SignatureTypedSpatialDataItem> UpdatedVars;

    std::vector<SignatureTypedSpatialDataItem> RequiredVars;

    std::vector<SignatureTypedSpatialDataItem> UsedVars;

    std::vector<SignatureSpatialDataItem> ProducedAttribute;

    std::vector<SignatureSpatialDataItem> RequiredAttribute;

    std::vector<SignatureSpatialDataItem> UsedAttribute;

    std::vector<std::string> RequiredExtraFiles;

    std::vector<std::string> UsedExtraFiles;

    std::vector<openfluid::core::UnitClass_t> UsedEventsOnUnits;


    SignatureHandledData()
    {

    }

};


/**
  Class for storage of the definition of spatial units handled by the simulator.
*/
class OPENFLUID_API SignatureUnitsClassItem
{
  public:

    openfluid::core::UnitClass_t UnitsClass;
    std::string Description;

    SignatureUnitsClassItem() :
      UnitsClass(""),Description("") {}

    SignatureUnitsClassItem(openfluid::core::UnitClass_t UClass,
        std::string DDescription) :
          UnitsClass(UClass),Description(DDescription) {}
};


class OPENFLUID_API SignatureUnitsGraph
{
  public:

    std::string UpdatedUnitsGraph;

    std::vector<SignatureUnitsClassItem> UpdatedUnitsClass;

    SignatureUnitsGraph()
    {
      UpdatedUnitsGraph.clear();
    }
};


class OPENFLUID_API SignatureTimeScheduling
{
  public:

    enum SchedulingType { UNDEFINED, DEFAULT, FIXED, RANGE };

    SchedulingType Type;

    openfluid::core::Duration_t Min;

    openfluid::core::Duration_t Max;

    SignatureTimeScheduling():
      Type(UNDEFINED), Min(0), Max(0)
    { }

    void setAsUndefined()
    {
      Type = UNDEFINED;
      Min = 0;
      Max = 0;
    }

    void setAsDefaultDeltaT()
    {
      Type = DEFAULT;
      Min = 0;
      Max = 0;
    }

    void setAsFixed(openfluid::core::Duration_t Val)
    {
      Type = FIXED;
      Min = Val;
      Max = Val;
    }

    void setAsRange(openfluid::core::Duration_t MinVal, openfluid::core::Duration_t MaxVal)
    {
      Type = RANGE;
      Min = MinVal;
      Max = MaxVal;
    }

};


/**
  Class encapsulating the plugin signature,
  returned from the plugin to the host app for registering
*/
class OPENFLUID_API SimulatorSignature : public WareSignature
{

  public:


    /**
    Plugin domain (i.e. hydrology, pop, erosion, ...)
    */
    SimDomain_t Domain;

    /**
    Plugin simulated process (i.e. surface rainfall-runoff production, ditch infiltration, ...)
    */
    SimProcess_t Process;

    /**
    Plugin involved method (i.e. morel-seytoux, hayami, ...)
    */
    SimMethod_t Method;

    /**
    Handled data
    */
    SignatureHandledData HandledData;

    /**
    Handled units graph
    */
    SignatureUnitsGraph HandledUnitsGraph;

    /**
      Time scheduling
    */
    SignatureTimeScheduling TimeScheduling;

    SimulatorSignature() : WareSignature(),
      Domain(""),Process(""),Method("")
      {}

};



} } // namespaces



#endif

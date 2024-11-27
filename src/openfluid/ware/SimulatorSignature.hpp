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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/

#ifndef __OPENFLUID_WARE_SIMULATORSIGNATURE_HPP__
#define __OPENFLUID_WARE_SIMULATORSIGNATURE_HPP__


#include <string>

#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/ware/WareSignature.hpp>


namespace openfluid { namespace ware {


/**
  Class for storage of the definition of the data handled by the simulator. This is part of the signature.
*/
class OPENFLUID_API SimulatorSignatureHandledData
{
  public:

    // TODO use the same naming style for all members


    std::vector<SignatureSpatialDataItem> ProducedVars;

    std::vector<SignatureSpatialDataItem> UpdatedVars;

    std::vector<SignatureSpatialDataItem> ProducedAttribute; // TOIMPL add plural

    std::vector<openfluid::core::UnitsClass_t> UsedEventsOnUnits; // TODO add description to units class events?


    SimulatorSignatureHandledData()
    {
      clear();
    }


    void clear()
    {
      ProducedVars.clear();
      UpdatedVars.clear();
      ProducedAttribute.clear();
      UsedEventsOnUnits.clear();
    }

};


// =====================================================================
// =====================================================================


/**
  Class for storage of the definition of spatial units handled by the simulator.
*/
class OPENFLUID_API SignatureUnitsClassItem
{
  public:

    openfluid::core::UnitsClass_t UnitsClass;

    std::string Description;

    SignatureUnitsClassItem() :
      UnitsClass(""),Description("") 
    { }

    SignatureUnitsClassItem(const openfluid::core::UnitsClass_t& UClass,const std::string& DDescription) :
      UnitsClass(UClass),Description(DDescription) 
    { }
};


// =====================================================================
// =====================================================================


class OPENFLUID_API SignatureUnitsGraph
{
  public:

    std::string UpdatedUnitsGraph;

    std::vector<SignatureUnitsClassItem> UpdatedUnitsClass; // TOIMPL add plural


    SignatureUnitsGraph()
    {
      clear();
    }


    void clear()
    {
      UpdatedUnitsGraph.clear();
      UpdatedUnitsClass.clear();
    }
};


// =====================================================================
// =====================================================================


class OPENFLUID_API SignatureTimeScheduling
{
  public:

    enum class SchedulingType { UNDEFINED, DEFAULT, FIXED, RANGE };

    SchedulingType Type;

    openfluid::core::Duration_t Min;

    openfluid::core::Duration_t Max;


    SignatureTimeScheduling()
    {
      setAsUndefined();
    }

    void setAsUndefined()
    {
      Type = SchedulingType::UNDEFINED;
      Min = 0;
      Max = 0;
    }

    void setAsDefaultDeltaT()
    {
      Type = SchedulingType::DEFAULT;
      Min = 0;
      Max = 0;
    }

    void setAsFixed(openfluid::core::Duration_t Val)
    {
      Type = SchedulingType::FIXED;
      Min = Val;
      Max = Val;
    }

    void setAsRange(openfluid::core::Duration_t MinVal, openfluid::core::Duration_t MaxVal)
    {
      Type = SchedulingType::RANGE;
      Min = MinVal;
      Max = MaxVal;
    }

    std::string getTypeAsString() const
    {
      if (Type == SchedulingType::DEFAULT)
      {
        return "default";
      }
      else if (Type == SchedulingType::FIXED)
      {
        return "fixed";
      }
      else if (Type == SchedulingType::RANGE)
      {
        return "range";
      }
      return "undefined";
    }

    void setTypeFromString(const std::string& ST)
    {
      Type = SchedulingType::UNDEFINED;

      if (ST == "default")
      {
        Type = SchedulingType::DEFAULT;
      }
      else if (ST == "fixed")
      {
        Type = SchedulingType::FIXED;
      }
      else if (ST == "range")
      {
        Type = SchedulingType::RANGE;
      }
    }
};


// =====================================================================
// =====================================================================


/**
  Class encapsulating the plugin signature,
  returned from the plugin to the host app for registering
*/
class OPENFLUID_API SimulatorSignature : public DataWareSignature
{

  public:

    SimulatorSignatureHandledData SimulatorHandledData;

    virtual WareType getType() const
    {
      return WareType::SIMULATOR;
    }

    /**
      Handled units graph
    */
    SignatureUnitsGraph HandledUnitsGraph;

    /**
      Time scheduling
    */
    SignatureTimeScheduling TimeScheduling;


    SimulatorSignature() : DataWareSignature()
    {
      clear();
    }

    virtual ~SimulatorSignature() = default;

    void clear()
    {
      DataWareSignature::clear();
      HandledUnitsGraph.clear();
      TimeScheduling.setAsUndefined();
    }
};


} } // namespaces


#endif /* __OPENFLUID_WARE_SIMULATORSIGNATURE_HPP__ */

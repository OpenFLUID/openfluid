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
  @file InjectGenerator.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_MACHINE_INJECTGENERATOR_HPP__
#define __OPENFLUID_MACHINE_INJECTGENERATOR_HPP__


#include <queue>
#include <map>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/machine/Generator.hpp>
#include <openfluid/tools/DistributionBindings.hpp>


namespace openfluid { namespace machine {


template<class T=double, class TV=openfluid::core::DoubleValue>
class OPENFLUID_API GenericInjectGenerator : public MonoGenerator
{
  protected:

    TV m_Default;
    
    std::string m_SourcesFile;
    std::string m_DistriFile;

    openfluid::tools::SimpleDistributionBindings<T, TV>* m_DistriBindings;


  public:

    GenericInjectGenerator() : MonoGenerator(),
      m_SourcesFile(""),m_DistriFile(""), m_DistriBindings(nullptr)
    {

    }


    ~GenericInjectGenerator()
    {
      if (m_DistriBindings != nullptr)
      {
        delete m_DistriBindings;
      }

    }


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      if (!OPENFLUID_GetWareParameter(Params,"sources",m_SourcesFile))
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "missing sources value for generator");
      }

      if (!OPENFLUID_GetWareParameter(Params,"distribution",m_DistriFile))
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "missing distribution value for generator");
      }
    }

    void prepareData()
    {
      openfluid::tools::DistributionTables DistriTables;
      std::string InputDir;

      OPENFLUID_GetRunEnvironment("dir.input",InputDir);

      DistriTables.build(InputDir,m_SourcesFile,m_DistriFile);
      m_DistriBindings = new openfluid::tools::SimpleDistributionBindings<T, TV>(DistriTables);
    }

    void checkConsistency() {}

    openfluid::base::SchedulingRequest initializeRun()
    {
      m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());
      TV Value;
      openfluid::core::SpatialUnit* LU;
      openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

      OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
      {
        if (m_DistriBindings->getValue(LU->getID(),CurrentDT,Value))
        {

        }
        else
        {
          Value = m_Default;
        }

        OPENFLUID_InitializeVariable(LU,m_VarName,Value);
      }

      openfluid::core::DateTime NextDT;

      if (m_DistriBindings->advanceToNextTimeAfter(CurrentDT,NextDT))
      {
        return Duration(NextDT.diffInSeconds(CurrentDT));
      }
      else
      {
        return Never();
      }
    }


    openfluid::base::SchedulingRequest runStep()
    {
      m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());

      TV Value;
      openfluid::core::SpatialUnit* LU;
      openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

      OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
      {
        if (m_DistriBindings->getValue(LU->getID(),CurrentDT,Value))
        {

          if (m_VarDimensions.isVector())
          {
            // TOIMPL ERROR SINCE NOT IMPLEMENTED
          }
          else
          {
            OPENFLUID_AppendVariable(LU,m_VarName,Value);
          }
        }
      }

      openfluid::core::DateTime NextDT;

      if (m_DistriBindings->advanceToNextTimeAfter(CurrentDT,NextDT))
      {
        return Duration(NextDT.diffInSeconds(CurrentDT));
      }
      else
      {
        return Never();
      }
    }

    void finalizeRun()
    { }

};


template<class T=double, class TV=openfluid::core::DoubleValue>
class NumericalInjectGenerator : public GenericInjectGenerator<T,TV>
{
  protected:

    bool m_IsMin;
    bool m_IsMax;

    T m_Min;
    T m_Max;
  
  public:

    NumericalInjectGenerator() : GenericInjectGenerator<T,TV>(), m_IsMin(false), m_IsMax(false), m_Min(0), m_Max(0)
    {
      this->m_Default = 0;
    }

    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      GenericInjectGenerator<T,TV>::initParams(Params);

      if (this->OPENFLUID_GetWareParameter(Params,"thresholdmin",m_Min))
      {
        m_IsMin = true;
      }

      if (this->OPENFLUID_GetWareParameter(Params,"thresholdmax",m_Max))
      {
        m_IsMax = true;
      }
    }

    void checkConsistency() override
    {
      if (m_IsMin && m_IsMax && m_Min > m_Max)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "threshold max value must be greater or equal "
                                                  "to threshold min value for generator");
      }
    }

    openfluid::base::SchedulingRequest runStep()
    {
      this->m_DistriBindings->advanceToTime(this->OPENFLUID_GetCurrentDate());

      openfluid::core::SpatialUnit* LU;
      openfluid::core::DateTime CurrentDT(this->OPENFLUID_GetCurrentDate());

      OPENFLUID_UNITS_ORDERED_LOOP(this->m_UnitsClass,LU)
      {
        TV Value;
        if (this->m_DistriBindings->getValue(LU->getID(),CurrentDT,Value))
        {
          if (m_IsMax && Value > m_Max)
          {
            Value = m_Max;
          }
          if (m_IsMin && Value < m_Min)
          {
            Value = m_Min;
          }

          if (this->m_VarDimensions.isVector())
          {
            openfluid::core::VectorValue VV(this->m_VarDimensions.Rows,Value);
            this->OPENFLUID_AppendVariable(LU,this->m_VarName,VV);
          }
          else
          {
            this->OPENFLUID_AppendVariable(LU,this->m_VarName,Value);
          }
        }
      }

      
      openfluid::core::DateTime NextDT;

      if (this->m_DistriBindings->advanceToNextTimeAfter(CurrentDT,NextDT))
      {
        return this->Duration(NextDT.diffInSeconds(CurrentDT));
      }
      else
      {
        return this->Never();
      }
    }
};


class InjectGenerator : public NumericalInjectGenerator<double, openfluid::core::DoubleValue>
{
  public: 
    
    openfluid::base::SchedulingRequest initializeRun();

    openfluid::base::SchedulingRequest runStep();

};

} } //namespaces


#endif /* __OPENFLUID_MACHINE_INJECTGENERATOR_HPP__ */

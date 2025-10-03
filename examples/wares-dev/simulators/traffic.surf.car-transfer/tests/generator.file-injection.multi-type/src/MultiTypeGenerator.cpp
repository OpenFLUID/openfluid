/**
  @file MultiTypeGenerator.cpp
*/


/*
<sim2doc>

</sim2doc>
*/

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/DistributionTables.hpp>
#include <openfluid/core/${type}.hpp>
#include "DistributionBindings.hpp"


/**

*/
class MultiTypeGenerator : public openfluid::ware::PluggableSimulator
{
  private:
    
    std::string m_SourcesFile;
    std::string m_DistriFile;
    
    DistributionBindings<openfluid::core::${type}>* m_DistriBindings;
  
  public:
  
    MultiTypeGenerator(): PluggableSimulator(),
    m_SourcesFile(""), m_DistriFile(""), m_DistriBindings(nullptr)
    { }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~MultiTypeGenerator()
    { }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void initParams(const openfluid::ware::WareParams_t& Params)
    {  
      if (!OPENFLUID_GetSimulatorParameter(Params,"sources",m_SourcesFile))
      {
       throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "missing sources value for multi-type generator");
      }

      if (!OPENFLUID_GetSimulatorParameter(Params,"distribution",m_DistriFile))
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "missing distribution value for multi-type generator");
      }
    }


    // =====================================================================
    // =====================================================================
  
  
    void prepareData()
    {
      openfluid::tools::DistributionTables DistriTables;
      std::string InputDir;

      OPENFLUID_GetRunEnvironment("dir.input",InputDir);

      DistriTables.build(InputDir,m_SourcesFile,m_DistriFile);
      m_DistriBindings = new DistributionBindings<openfluid::core::${type}>(DistriTables, "${missing_value}");
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void checkConsistency()
    { }
  
  
// =====================================================================
// =====================================================================

  
    openfluid::base::SchedulingRequest initializeRun()
    {  
      m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());
      openfluid::core::${type} Value;
      openfluid::core::SpatialUnit* SU;
      openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

      OPENFLUID_UNITS_ORDERED_LOOP("${unitsclass}",SU)
      {
        if(m_DistriBindings->getValue(SU->getID(),CurrentDT,Value) == 1)
        {
          OPENFLUID_InitializeVariable(SU, "${varname}", Value);
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                "Value not injected since issue occured during injection: Units class: ${unitsclass}; "
                "variable: ${varname}; unit ID: " + std::to_string(SU->getID()) + " at time " + CurrentDT.getAsISOString());
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


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {
      m_DistriBindings->advanceToTime(OPENFLUID_GetCurrentDate());

      openfluid::core::${type} Value;
      openfluid::core::SpatialUnit* SU;
      openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

      OPENFLUID_UNITS_ORDERED_LOOP("${unitsclass}",SU)
      {
        
        int Status = m_DistriBindings->getValue(SU->getID(),CurrentDT,Value);
        if(Status == 1)
        {
          OPENFLUID_AppendVariable(SU, "${varname}", Value);
        }
        else if(Status == -1)
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                "Value not injected since issue occured during injection: Units class: ${unitsclass}; "
                "variable: ${varname}; unit ID: " + std::to_string(SU->getID()) + " at time " + CurrentDT.getAsISOString());          
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


    // =====================================================================
    // =====================================================================
  
  
    void finalizeRun()
    {
  
  
    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(MultiTypeGenerator);


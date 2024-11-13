/**
  @file WareMain.cpp
*/


#include <openfluid/ware/PluggableSimulator.hpp>


/**

*/
class %%CLASSNAME%% : public openfluid::ware::PluggableSimulator
{
  private:

  
  public:

  
    %%CLASSNAME%%(): PluggableSimulator()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~%%CLASSNAME%%()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {


    }


    // =====================================================================
    // =====================================================================
  
  
    void prepareData()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void checkConsistency()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest initializeRun()
    {  
%%SIMINITCODE%%      
      return %%SIMSCHEDRETURN%%;
    }


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {
%%SIMRUNCODE%%
      return %%SIMSCHEDRETURN%%;
    }


    // =====================================================================
    // =====================================================================
  
  
    void finalizeRun()
    {
  
  
    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(%%CLASSNAME%%);


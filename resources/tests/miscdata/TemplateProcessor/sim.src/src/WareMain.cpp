// OpenFLUID:stylecheck:!lich
// OpenFLUID:stylecheck:!auth
// OpenFLUID:stylecheck:!fnam


/**
  @file %%ROOTCPPFILENAME%%
*/


/*
<sim2doc>

</sim2doc>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("%%WAREID%%")

  DECLARE_DESCRIPTION("%%WAREDESCRIPTION%%")

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


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

      return %%SIMSCHEDRETURN%%;
    }


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {

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

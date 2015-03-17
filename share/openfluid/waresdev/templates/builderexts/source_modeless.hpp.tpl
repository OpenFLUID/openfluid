/**
  @file %%ROOTHPPFILENAME%%
*/


#ifndef %%HPPHEADERGUARD%%
#define %%HPPHEADERGUARD%%


#include <openfluid/builderext/PluggableModelessExtension.hpp>


DECLARE_BUILDEREXT_PLUGIN


// =====================================================================
// =====================================================================



class %%CLASSNAME%% : public openfluid::builderext::PluggableModelessExtension
{
  Q_OBJECT;


  public:

    %%CLASSNAME%%();

    virtual ~%%CLASSNAME%%();

    bool initialize();

    void update(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags);

    void manageSimulationStart();
 
    void manageSimulationFinish();

};


#endif /* %%HPPHEADERGUARD%% */



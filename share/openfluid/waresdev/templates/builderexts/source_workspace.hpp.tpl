/**
  @file %%ROOTHPPFILENAME%%
*/


#ifndef %%HPPHEADERGUARD%%
#define %%HPPHEADERGUARD%%


#include <openfluid/builderext/PluggableWorkspaceExtension.hpp>


// =====================================================================
// =====================================================================


class %%CLASSNAME%% : public openfluid::builderext::PluggableWorkspaceExtension
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



/**
  @file WareMain.hpp
*/


#ifndef __WAREMAIN_HPP__
#define __WAREMAIN_HPP__


#include <openfluid/builderext/PluggableModelessExtension.hpp>


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


#endif /* __WAREMAIN_HPP__ */

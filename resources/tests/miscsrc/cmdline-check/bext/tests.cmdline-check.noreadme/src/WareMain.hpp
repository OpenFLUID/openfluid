/**
  @file WareMain.hpp
*/


#ifndef __WAREMAIN_HPP__
#define __WAREMAIN_HPP__


#include <openfluid/ui/builderext/PluggableModalExtension.hpp>


// =====================================================================
// =====================================================================


class WareMain : public openfluid::ui::builderext::PluggableModalExtension
{
  Q_OBJECT;


  public:

    WareMain();

    virtual ~WareMain();

    bool initialize();

    void update(openfluid::ui::builderext::FluidXUpdateFlags::Flags UpdateFlags);

    void manageSimulationStart();
 
    void manageSimulationFinish();

};


#endif /* __WAREMAIN_HPP__ */

/**
  @file WareMain.cpp

*/


#include "WareMain.hpp"


%%CLASSNAME%%::%%CLASSNAME%%() :
  openfluid::ui::builderext::PluggableModelessExtension()
{

}


// =====================================================================
// =====================================================================


%%CLASSNAME%%::~%%CLASSNAME%%()
{

}


// =====================================================================
// =====================================================================


bool %%CLASSNAME%%::initialize()
{
  return true;
}


// =====================================================================
// =====================================================================


void %%CLASSNAME%%::update(openfluid::ui::builderext::FluidXUpdateFlags::Flags /*UpdateFlags*/)
{

}


// =====================================================================
// =====================================================================


void %%CLASSNAME%%::manageSimulationStart()
{

}


// =====================================================================
// =====================================================================


void %%CLASSNAME%%::manageSimulationFinish()
{

}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(%%CLASSNAME%%)

/**
  \file PluginManager.h
  \brief header of the plugins management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "openfluid-base.h"
#include "openfluid-core.h"
#include "RuntimeEnv.h"


struct PluginContainer
{
  
  wxString Filename;
  openfluid::base::Signature* Signature;
  openfluid::base::PluggableFunction* Function;
  
  PluginContainer()
  {
    Filename = wxT("");
    Signature = NULL;
    Function = NULL;
  }
  
};


WX_DEFINE_ARRAY(openfluid::base::Signature*, ArrayOfPluginsSignatures);
WX_DEFINE_ARRAY(PluginContainer*, ArrayOfPluginsContainers);

/**
  Management class for plugins
*/
class PluginManager
{
  private:

    RuntimeEnvironment* mp_RunEnv;
    openfluid::base::ExecutionMessages* mp_ExecMsgs;

/*    openfluid::base::PluggableFunction *getPluggableFunction(wxString PluginFilename);*/
    PluginContainer *buildPluginContainer(wxString PluginFilename);
    


  public:
    /**
      Constructor
    */
    PluginManager(openfluid::base::ExecutionMessages* ExecMsgs, RuntimeEnvironment* RunEnv);

    /**
      Destructor
    */
    ~PluginManager();

    /**
      Lists available simulation functions
    */
//    ArrayOfPluginsSignatures getAvailableFunctionsList();
    
    ArrayOfPluginsContainers getAvailableFunctions();


    /**
      Returns function and function types
    */
    PluginContainer *getPlugin(wxString PluginName,
                               openfluid::base::FunctionTypeList ReqFuncType,
                               openfluid::core::CoreRepository* CoreData);

/*
    openfluid::base::Function *getFunctionFromPlugin(wxString PluginName,
                                          openfluid::base::ModuleTypeList ReqModType,
                                          openfluid::base::FunctionTypeList ReqFuncType);
*/

};





#endif




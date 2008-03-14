/**
  \file PluginManager.h
  \brief header of the plugins management class

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "mhydasdk-base.h"
#include "mhydasdk-core.h"
#include "RuntimeEnv.h"


struct PluginContainer
{
  
  wxString Filename;
  mhydasdk::base::Signature* Signature;
  mhydasdk::base::PluggableFunction* Function;
  
  PluginContainer()
  {
    Filename = wxT("");
    Signature = NULL;
    Function = NULL;
  }
  
};


WX_DEFINE_ARRAY(mhydasdk::base::Signature*, ArrayOfPluginsSignatures);
WX_DEFINE_ARRAY(PluginContainer*, ArrayOfPluginsContainers);

/**
  Management class for plugins
*/
class PluginManager
{
  private:

    RuntimeEnvironment* mp_RunEnv;
    mhydasdk::base::ExecutionMessages* mp_ExecMsgs;

/*    mhydasdk::base::PluggableFunction *getPluggableFunction(wxString PluginFilename);*/
    PluginContainer *buildPluginContainer(wxString PluginFilename);
    


  public:
    /**
      Constructor
    */
    PluginManager(mhydasdk::base::ExecutionMessages* ExecMsgs, RuntimeEnvironment* RunEnv);

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
                               mhydasdk::base::FunctionTypeList ReqFuncType,
                               mhydasdk::core::CoreRepository* CoreData);

/*
    mhydasdk::base::Function *getFunctionFromPlugin(wxString PluginName,
                                          mhydasdk::base::ModuleTypeList ReqModType,
                                          mhydasdk::base::FunctionTypeList ReqFuncType);
*/

};





#endif




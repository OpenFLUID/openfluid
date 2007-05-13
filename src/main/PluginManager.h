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



WX_DEFINE_ARRAY(mhydasdk::base::Signature*, ArrayOfPluginsSignatures);

/**
  Management class for plugins
*/
class PluginManager
{
  private:

    RuntimeEnvironment* mp_RunEnv;

    mhydasdk::base::PluggableFunction *getPluggableFunction(wxString PluginFilename);


  public:
    /**
      Constructor
    */
    PluginManager(RuntimeEnvironment* RunEnv);

    /**
      Destructor
    */
    ~PluginManager();

    /**
      Lists available functions, buil-in or from plugins
    */
    ArrayOfPluginsSignatures getAvailableFunctionsList();


    /**
      Returns function and function types
    */
    mhydasdk::base::PluggableFunction *getFunctionFromPlugin(wxString PluginName,
                                                             mhydasdk::base::FunctionTypeList ReqFuncType,
                                                             mhydasdk::core::CoreRepository* CoreData);

/*
    mhydasdk::base::Function *getFunctionFromPlugin(wxString PluginName,
                                          mhydasdk::base::ModuleTypeList ReqModType,
                                          mhydasdk::base::FunctionTypeList ReqFuncType);
*/

};





#endif




/**
  \file PluginManager.h
  \brief header of the plugins management class

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "mhydasdk-base.h"
#include "mhydasdk-core.h"


WX_DEFINE_ARRAY(mhydasdk::base::Signature*, ArrayOfPluginsSignatures);

/**
  Management class for plugins
*/
class PluginManager
{
  private:

    mhydasdk::base::RuntimeEnvironment* mp_RunEnv;

    mhydasdk::base::Plugin *getPlugin(wxString PluginFilename);


  public:
    /**
      Constructor
    */
    PluginManager(mhydasdk::base::RuntimeEnvironment* RunEnv);

    /**
      Destructor
    */
    ~PluginManager();

    /**
      Lists available functions, buil-in or from plugins
    */
    ArrayOfPluginsSignatures getAvailableFunctionsList();


    /**
      Returns function, matching module and function types
    */
    mhydasdk::base::Function *getFunctionFromPlugin(wxString PluginName,
                                                    mhydasdk::base::FunctionTypeList ReqFuncType,
                                                    mhydasdk::core::CoreRepository* CoreData);

/*
    mhydasdk::base::Function *getFunctionFromPlugin(wxString PluginName,
                                          mhydasdk::base::ModuleTypeList ReqModType,
                                          mhydasdk::base::FunctionTypeList ReqFuncType);
*/

};





#endif




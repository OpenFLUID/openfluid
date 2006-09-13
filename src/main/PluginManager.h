/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "sdk-base.h"
#include "sdk-core.h"


WX_DEFINE_ARRAY(mhydasdk::base::Signature*, ArrayOfPluginsSignatures);

/**

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
                                                    mhydasdk::base::ModuleTypeList ReqModType,
                                                    mhydasdk::core::CoreRepository* CoreData);

/*
    mhydasdk::base::Function *getFunctionFromPlugin(wxString PluginName,
                                          mhydasdk::base::ModuleTypeList ReqModType,
                                          mhydasdk::base::FunctionTypeList ReqFuncType);
*/

};





#endif




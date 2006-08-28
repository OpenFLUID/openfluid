/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <wx/dynlib.h>

#include "setup.h"

#include "PluginManager.h"


// =====================================================================
// =====================================================================


PluginManager::PluginManager(mhydasdk::base::RuntimeEnvironment* RunEnv)
{
  mp_RunEnv = RunEnv;
}

// =====================================================================
// =====================================================================

PluginManager::~PluginManager()
{

}


// =====================================================================
// =====================================================================

mhydasdk::base::Plugin *PluginManager::getPlugin(wxString PluginFilename)
{
  wxDynamicLibrary *PlugLib = new wxDynamicLibrary();
  wxString PluginFile =  mp_RunEnv->getAppDir() + wxFILE_SEP_PATH + MHYDAS_PLUGINS_SUBDIR + wxFILE_SEP_PATH + PluginFilename;

  mhydasdk::base::Plugin* Plug = NULL;

  // library loading
  if (PlugLib->Load(PluginFile))
  {

    // checks if the handle proc exists
    if (PlugLib->HasSymbol(wxT("GetMHYDASPlugin")))
    {

    	// hooks the handle proc
    	mhydasdk::base::GetPluginProc PlugProc = (mhydasdk::base::GetPluginProc)PlugLib->GetSymbol(wxT("GetMHYDASPlugin"));

      if (PlugProc != NULL)
      {
        Plug = PlugProc();
      }

      // unloads the library
      PlugLib->Unload();


      /*if (PlugProc == NULL)
      {
        PlugLib->Unload();
      }
      else
      {
        Plugin* Plug = PlugProc();
      }
      */
    }
  }

  return Plug;
}


// =====================================================================
// =====================================================================

ArrayOfPluginsSignatures PluginManager::getAvailableFunctionsList()
{
  ArrayOfPluginsSignatures Signatures;

  return Signatures;
}



// =====================================================================
// =====================================================================

mhydasdk::base::Function *PluginManager::getFunctionFromPlugin(wxString PluginName,
                                                     mhydasdk::base::ModuleTypeList ReqModType,
                                                     mhydasdk::base::FunctionTypeList ReqFuncType)
{
  mhydasdk::base::Function *Func = NULL;

  mhydasdk::base::Plugin *Plug = getPlugin(PluginName+wxT(".")+MHYDAS_PLUGINS_EXT);


  if (Plug != NULL)
  {
    if ((Plug->getSignature().ModuleType = ReqModType) &&
        (Plug->getSignature().FunctionType = ReqFuncType))
    {
      Func = Plug->getFunction();
    }
  }


  return Func;
}



/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <wx/dynlib.h>

#include "setup.h"

#include "PluginManager.h"
#include "AppTools.h"


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
    //std::cerr << "plug" << std::endl;

    // checks if the handle proc exists
    if (PlugLib->HasSymbol(wxT("GetMHYDASPlugin")))
    {
      //std::cerr << "symbol" << std::endl;
    	// hooks the handle proc
    	mhydasdk::base::GetPluginProc PlugProc = (mhydasdk::base::GetPluginProc)PlugLib->GetSymbol(wxT("GetMHYDASPlugin"));

      //std::cerr << "symbol 2" << std::endl;

      if (PlugProc != NULL)
      {
        Plug = PlugProc();
        // std::cerr << "proc" << std::endl;
      }

      // unloads the library
      //PlugLib->Unload();

    }
  }

  return Plug;
}


// =====================================================================
// =====================================================================

ArrayOfPluginsSignatures PluginManager::getAvailableFunctionsList()
{
  ArrayOfPluginsSignatures Signatures;

  wxArrayString PluginFiles = GetFilesByExt(mp_RunEnv->getAppDir() + wxFILE_SEP_PATH + MHYDAS_PLUGINS_SUBDIR,MHYDAS_PLUGINS_EXT);

  mhydasdk::base::Plugin* CurrentPlug;

  for (int i=0;i<PluginFiles.GetCount();i++)
  {
    //std::cerr << "ici " << i << std::endl;
    CurrentPlug = getPlugin(PluginFiles[i]);
    //std::cerr << "la " << i << std::endl;
    if (CurrentPlug != NULL) Signatures.Add(CurrentPlug->getSignature());
    //std::cerr << "fini " << i << std::endl;
  }

  return Signatures;
}



// =====================================================================
// =====================================================================

mhydasdk::base::Function *PluginManager::getFunctionFromPlugin(wxString PluginName,
                                                               mhydasdk::base::ModuleTypeList ReqModType,
                                                               mhydasdk::core::CoreRepository* CoreData)
{
  mhydasdk::base::Function *Func = NULL;


  mhydasdk::base::Plugin *Plug = getPlugin(PluginName+wxT(".")+MHYDAS_PLUGINS_EXT);


  if (Plug != NULL)
  {
    if (Plug->getSignature()->ModuleType == ReqModType)
    {
      Func = Plug->getFunction(CoreData);
    }
  }


  return Func;
}


/*
mhydasdk::base::Function *PluginManager::getFunctionFromPlugin(wxString PluginName,
                                                     mhydasdk::base::ModuleTypeList ReqModType,
                                                     mhydasdk::base::FunctionTypeList ReqFuncType)
{
  mhydasdk::base::Function *Func = NULL;

  mhydasdk::base::Plugin *Plug = getPlugin(PluginName+wxT(".")+MHYDAS_PLUGINS_EXT);


  if (Plug != NULL)
  {
    if ((Plug->getSignature()->ModuleType == ReqModType) &&
        (Plug->getSignature()->FunctionType == ReqFuncType))
    {
      Func = Plug->getFunction();
    }
  }


  return Func;
}

*/

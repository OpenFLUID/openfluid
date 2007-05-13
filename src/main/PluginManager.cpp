/**
  \file PluginManager.cpp
  \brief implements the plugins management class

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <wx/dynlib.h>

#include "setup.h"

#include "PluginManager.h"
#include "AppTools.h"

#include <iostream>

// =====================================================================
// =====================================================================


PluginManager::PluginManager(RuntimeEnvironment* RunEnv)
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

mhydasdk::base::PluggableFunction *PluginManager::getPluggableFunction(wxString PluginFilename)
{
  wxDynamicLibrary *PlugLib = new wxDynamicLibrary();
  wxString PluginFile =  mp_RunEnv->getAppDir() + wxFILE_SEP_PATH + MHYDAS_PLUGINS_SUBDIR + wxFILE_SEP_PATH + PluginFilename;

  mhydasdk::base::PluggableFunction* Plug = NULL;

  // library loading
  if (PlugLib->Load(PluginFile))
  {
    // checks if the handle proc exists
    if (PlugLib->HasSymbol(wxT("GetMHYDASPluggableFunction")))
    {
    	// hooks the handle proc
    	mhydasdk::base::GetPluggableFunctionProc PlugProc = (mhydasdk::base::GetPluggableFunctionProc)PlugLib->GetSymbol(wxT("GetMHYDASPluggableFunction"));

      if (PlugProc != NULL)
      {
        Plug = PlugProc();            
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

  mhydasdk::base::PluggableFunction* CurrentPlug;

  for (int i=0;i<PluginFiles.GetCount();i++)
  {
    CurrentPlug = getPluggableFunction(PluginFiles[i]);
    if (CurrentPlug != NULL) Signatures.Add(CurrentPlug->getSignature());
  }

  return Signatures;
}



// =====================================================================
// =====================================================================

mhydasdk::base::PluggableFunction *PluginManager::getFunctionFromPlugin(wxString PluginName,
                                                               mhydasdk::base::FunctionTypeList ReqFuncType,
                                                               mhydasdk::core::CoreRepository* CoreData)
{

  mhydasdk::base::PluggableFunction *Plug = getPluggableFunction(PluginName+wxT(".")+MHYDAS_PLUGINS_EXT);

  if (Plug != NULL)
  {
    if (Plug->getSignature()->FunctionType == ReqFuncType)
    {
      Plug->setDataRepository(CoreData);
      return Plug;      
    }
  }


  return Plug;
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

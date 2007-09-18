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


PluginManager::PluginManager(mhydasdk::base::ExecutionMessages* ExecMsgs, RuntimeEnvironment* RunEnv)
{
  mp_RunEnv = RunEnv;
  mp_ExecMsgs = ExecMsgs;
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
  wxString PluginFile =  mp_RunEnv->getPluginFullPath(PluginFilename);
  mhydasdk::base::PluggableFunction* Plug = NULL;  
  
  
  // library loading
  if (PluginFile.Length()>0 && PlugLib->Load(PluginFile))
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
  wxArrayString PluginsPaths = mp_RunEnv->getPluginsPaths();
  wxArrayString PluginFiles;
  wxArrayString TmpFiles;
  int i,j;
  
  for (i=0;i<PluginsPaths.GetCount();i++)
  {
    TmpFiles = GetFilesByExt(PluginsPaths[i],MHYDAS_PLUGINS_EXT);    
    for (j=0;j<TmpFiles.GetCount();j++) PluginFiles.Add(TmpFiles[j]);
  }
  

  mhydasdk::base::PluggableFunction* CurrentPlug;

  for (i=0;i<PluginFiles.GetCount();i++)
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
      Plug->setExecutionMessages(mp_ExecMsgs);
      return Plug;      
    }
  }


  return Plug;
}



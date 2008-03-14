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
/*
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
*/

PluginContainer *PluginManager::buildPluginContainer(wxString PluginFilename)
{
  wxDynamicLibrary *PlugLib = new wxDynamicLibrary();
  wxString PluginFile =  mp_RunEnv->getPluginFullPath(PluginFilename);
  PluginContainer* Plug = NULL;  
  
 
  // library loading
  if (PluginFile.Length()>0 && PlugLib->Load(PluginFile))
  {
    
    // checks if the handle proc exists
    if (PlugLib->HasSymbol(wxT(PLUGFUNCTION_PROC_NAME)) && PlugLib->HasSymbol(wxT(PLUGSIGNATURE_PROC_NAME)))
    {
      // hooks the handle proc
      mhydasdk::base::GetSignatureProc SignProc = (mhydasdk::base::GetSignatureProc)PlugLib->GetSymbol(wxT(PLUGSIGNATURE_PROC_NAME));      
      
      if (SignProc != NULL)
      {
        Plug = new PluginContainer();
        
        Plug->Signature = SignProc();

        mhydasdk::base::GetPluggableFunctionProc PlugProc = (mhydasdk::base::GetPluggableFunctionProc)PlugLib->GetSymbol(wxT(PLUGFUNCTION_PROC_NAME));
        if (PlugProc != NULL)
        {
          Plug->Function = PlugProc();
          Plug->Filename = PluginFile; 
        }
        else 
        {
          Plug->Function = NULL;
          Plug->Signature = NULL;
          Plug = NULL;
        }
      }

      // unloads the library
      //PlugLib->Unload();

    }
  }

  return Plug;
}



// =====================================================================
// =====================================================================
/*
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
*/

ArrayOfPluginsContainers PluginManager::getAvailableFunctions()
{
  ArrayOfPluginsContainers PluginsContainers;
  wxArrayString PluginsPaths = mp_RunEnv->getPluginsPaths();
  wxArrayString PluginFiles;
  wxArrayString TmpFiles;
  int i,j;
  
  
  for (i=0;i<PluginsPaths.GetCount();i++)
  {
    TmpFiles = GetFilesByExt(PluginsPaths[i],MHYDAS_PLUGINS_EXT);    
    for (j=0;j<TmpFiles.GetCount();j++) PluginFiles.Add(TmpFiles[j]);
  }
  

  PluginContainer* CurrentPlug;

  for (i=0;i<PluginFiles.GetCount();i++)
  {
    CurrentPlug = buildPluginContainer(PluginFiles[i]);
    
    if (CurrentPlug != NULL) PluginsContainers.Add(CurrentPlug);
  }
  
  return PluginsContainers;
}




// =====================================================================
// =====================================================================

PluginContainer *PluginManager::getPlugin(wxString PluginName,
                                          mhydasdk::base::FunctionTypeList ReqFuncType,
                                          mhydasdk::core::CoreRepository* CoreData)
{

  PluginContainer *Plug = buildPluginContainer(PluginName+wxT(".")+MHYDAS_PLUGINS_EXT);
  

  
  if (Plug != NULL)
  {
    if (Plug->Signature->FunctionType == ReqFuncType)
    {
      Plug->Function->setDataRepository(CoreData);
      Plug->Function->setExecutionMessages(mp_ExecMsgs);
      Plug->Function->setFunctionEnvironment(mp_RunEnv->createFunctionEnvironment());
      return Plug;      
    }
  }


  return Plug;
}



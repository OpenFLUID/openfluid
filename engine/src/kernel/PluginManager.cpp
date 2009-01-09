/**
  \file PluginManager.cpp
  \brief implements the plugins management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <wx/dynlib.h>

#include "config.h"

#include "PluginManager.h"
#include "AppTools.h"
#include "base/OFException.h"

#include <iostream>

// =====================================================================
// =====================================================================


PluginManager::PluginManager(openfluid::base::ExecutionMessages* ExecMsgs, RuntimeEnvironment* RunEnv)
{
  mp_RunEnv = RunEnv;
  mp_ExecMsgs = ExecMsgs;
}

// =====================================================================
// =====================================================================

PluginManager::~PluginManager()
{

}



PluginContainer *PluginManager::buildPluginContainer(wxString PluginFilename)
{
  wxDynamicLibrary *PlugLib = new wxDynamicLibrary();
  wxString PluginFile =  mp_RunEnv->getPluginFullPath(PluginFilename);
  PluginContainer* Plug = NULL;

  //if (!wxFileExists(PluginFile)) throw openfluid::base::OFException("Function file "+ std::string(PluginFilename.mb_str(wxConvUTF8)) +"not found");

  // library loading
  if (PluginFile.Length()>0 && PlugLib->Load(PluginFile))
  {

    // checks if the handle proc exists
    if (PlugLib->HasSymbol(wxT(PLUGFUNCTION_PROC_NAME)) && PlugLib->HasSymbol(wxT(PLUGSIGNATURE_PROC_NAME)))
    {
      // hooks the handle proc
      openfluid::base::GetSignatureProc SignProc = (openfluid::base::GetSignatureProc)PlugLib->GetSymbol(wxT(PLUGSIGNATURE_PROC_NAME));

      if (SignProc != NULL)
      {
        Plug = new PluginContainer();

        Plug->Signature = SignProc();

        openfluid::base::GetPluggableFunctionProc PlugProc = (openfluid::base::GetPluggableFunctionProc)PlugLib->GetSymbol(wxT(PLUGFUNCTION_PROC_NAME));
        if (PlugProc != NULL)
        {
          Plug->Function = PlugProc();
          Plug->Filename = PluginFile;
        }
        else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find function in plugin file " + std::string(PluginFilename.mb_str(wxConvUTF8)));
      }
      else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find signature in plugin file " + std::string(PluginFilename.mb_str(wxConvUTF8)));

      // unloads the library
      //PlugLib->Unload();

    }
  }
  else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find plugin file " + std::string(PluginFilename.mb_str(wxConvUTF8)));

  return Plug;
}



// =============================================================signature========
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
    TmpFiles = GetFilesByExt(PluginsPaths[i],OPENFLUID_PLUGINS_EXT);
    for (j=0;j<TmpFiles.GetCount();j++) PluginFiles.Add(TmpFiles[j]);
  }


  openfluid::base::PluggableFunction* CurrentPlug;

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
    TmpFiles = GetFilesByExt(PluginsPaths[i],OPENFLUID_PLUGINS_EXT,false,true);
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
                                          openfluid::base::FunctionTypeList ReqFuncType,
                                          openfluid::core::CoreRepository* CoreData)
{

  PluginContainer *Plug = buildPluginContainer(PluginName+OPENFLUID_PLUGINS_EXT);



  if (Plug != NULL)
  {
    if (Plug->Signature->FunctionType == ReqFuncType)
    {
      Plug->Function->setDataRepository(CoreData);
      Plug->Function->setExecutionMessages(mp_ExecMsgs);
      Plug->Function->setFunctionEnvironment(mp_RunEnv->getFunctionEnvironment());
      return Plug;
    }
  }


  return Plug;
}



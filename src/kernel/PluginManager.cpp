/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file PluginManager.cpp
  \brief implements the plugins management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "config.h"

#include "DynamicLib.h"
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


// =====================================================================
// =====================================================================


PluginContainer *PluginManager::buildPluginContainer(std::string PluginFilename)
{
  std::string PluginFile =  mp_RunEnv->getPluginFullPath(PluginFilename);
  PluginContainer* Plug = NULL;

  DynamicLib *PlugLib = new DynamicLib(PluginFile);

  // library loading
  if (PluginFile.length()>0 && PlugLib->load())
  {
    // TODO to develop

    Plug = new PluginContainer();
    Plug->Filename = PluginFile;

    if (PlugLib->hasSymbol(PLUGSDKVERSION_PROC_NAME))
    {
      openfluid::base::GetSDKVersionProc SDKProc;
      if ((SDKProc = (openfluid::base::GetSDKVersionProc)PlugLib->getSymbol(PLUGSDKVERSION_PROC_NAME)));
        Plug->SDKCompatible = (FULL_VERSION == SDKProc());
    }

    if (Plug->SDKCompatible)
    {
      // checks if the handle proc exists
      if (PlugLib->hasSymbol((PLUGFUNCTION_PROC_NAME)) && PlugLib->hasSymbol(PLUGSIGNATURE_PROC_NAME))
      {

        // hooks the handle proc
        openfluid::base::GetSignatureProc SignProc = (openfluid::base::GetSignatureProc)PlugLib->getSymbol(PLUGSIGNATURE_PROC_NAME);


        if (SignProc != NULL)
        {
          Plug->Signature = SignProc();

          openfluid::base::GetPluggableFunctionProc PlugProc = (openfluid::base::GetPluggableFunctionProc)PlugLib->getSymbol(PLUGFUNCTION_PROC_NAME);
          if (PlugProc != NULL)
          {
            Plug->Function = PlugProc();
          }
          else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find function in plugin file " + PluginFilename);
        }
        else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find signature in plugin file " + PluginFilename);

        // unloads the library
        //PlugLib->unload();
        //delete PlugLib;

      }
    }
  }
  else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find plugin file " + PluginFilename);

  return Plug;
}


// =====================================================================
// =====================================================================


ArrayOfPluginsContainers PluginManager::getAvailableFunctions(const std::string Pattern)
{
  ArrayOfPluginsContainers PluginsContainers;
  std::vector<std::string> PluginsPaths = mp_RunEnv->getPluginsPaths();
  std::vector<std::string> PluginFiles;
  std::vector<std::string> TmpFiles;
  unsigned int i,j;


  for (i=0;i<PluginsPaths.size();i++)
  {
    TmpFiles = GetFilesByExt(PluginsPaths[i],OPENFLUID_PLUGINS_EXT,false,true);
    for (j=0;j<TmpFiles.size();j++) PluginFiles.push_back(TmpFiles[j]);
  }


  PluginContainer* CurrentPlug;

  for (i=0;i<PluginFiles.size();i++)
  {
    CurrentPlug = buildPluginContainer(PluginFiles[i]);

    if (CurrentPlug != NULL && CurrentPlug->SDKCompatible)
    {
      if (Pattern != "")
      {
        if (openfluid::tools::WildcardMatching(Pattern,CurrentPlug->Signature->ID))
          PluginsContainers.push_back(CurrentPlug);
      }
      else PluginsContainers.push_back(CurrentPlug);
    }

  }

  return PluginsContainers;
}


// =====================================================================
// =====================================================================


PluginContainer *PluginManager::getPlugin(std::string PluginName,
                                          openfluid::core::CoreRepository* CoreData)
{

  PluginContainer *Plug = buildPluginContainer(PluginName+OPENFLUID_PLUGINS_EXT);



  if (Plug != NULL)
  {
    Plug->Function->setDataRepository(CoreData);
    Plug->Function->setExecutionMessages(mp_ExecMsgs);
    Plug->Function->setFunctionEnvironment(mp_RunEnv->getFunctionEnvironment());
    return Plug;
  }


  return Plug;
}



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



PluginManager* PluginManager::mp_Singleton = NULL;


// =====================================================================
// =====================================================================


PluginManager::PluginManager()
{

}

// =====================================================================
// =====================================================================

PluginManager::~PluginManager()
{

}


// =====================================================================
// =====================================================================


PluginManager* PluginManager::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new PluginManager();
  return mp_Singleton;
}


// =====================================================================
// =====================================================================


ModelItemInstance* PluginManager::buildPluginContainer(std::string PluginFilename)
{
  std::string PluginFile =  RuntimeEnvironment::getInstance()->getPluginFullPath(PluginFilename);
  ModelItemInstance* Plug = NULL;

  DynamicLib *PlugLib = new DynamicLib(PluginFile);

  // library loading
  if (PluginFile.length()>0 && PlugLib->load())
  {
    Plug = new ModelItemInstance();
    Plug->Filename = PluginFile;

    if (PlugLib->hasSymbol(PLUGSDKVERSION_PROC_NAME))
    {
      openfluid::base::GetSDKVersionProc SDKProc;

      SDKProc = (openfluid::base::GetSDKVersionProc)PlugLib->getSymbol(PLUGSDKVERSION_PROC_NAME);
      Plug->SDKCompatible = (CONFIG_FULL_VERSION == SDKProc());
    }
    else Plug->SDKCompatible = false;


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

          if (Plug->Signature == NULL)
            throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Signature from plugin file " + PluginFilename + " cannot be instanciated");

          openfluid::base::GetPluggableFunctionProc PlugProc = (openfluid::base::GetPluggableFunctionProc)PlugLib->getSymbol(PLUGFUNCTION_PROC_NAME);
          if (PlugProc != NULL)
          {
            Plug->Function = PlugProc();

            if (Plug->Function == NULL)
              throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Function from plugin file " + PluginFilename + " cannot be instanciated");

          }
          else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find function in plugin file " + PluginFilename);
        }
        else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find signature in plugin file " + PluginFilename);

        // unloads the library
        //PlugLib->unload();
        //delete PlugLib;

      }
      else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Format error in plugin file " + PluginFilename);
    }
  }
  else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find plugin file " + PluginFilename);

  return Plug;
}


// =====================================================================
// =====================================================================


ArrayOfModelItemInstance PluginManager::getAvailableFunctions(const std::string Pattern)
{
  ArrayOfModelItemInstance PluginsContainers;
  std::vector<std::string> PluginsPaths = RuntimeEnvironment::getInstance()->getPluginsPaths();
  std::vector<std::string> PluginFiles;
  std::vector<std::string> TmpFiles;
  unsigned int i,j;


  for (i=0;i<PluginsPaths.size();i++)
  {
    TmpFiles = GetFilesByExt(PluginsPaths[i],CONFIG_PLUGINS_EXT,false,true);
    for (j=0;j<TmpFiles.size();j++) PluginFiles.push_back(TmpFiles[j]);
  }


  ModelItemInstance* CurrentPlug;

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


ModelItemInstance* PluginManager::getPlugin(std::string PluginName,
                                          openfluid::base::ExecutionMessages* ExecMsgs,
                                          openfluid::core::CoreRepository* CoreData)
{

  ModelItemInstance* Plug = buildPluginContainer(PluginName+CONFIG_PLUGINS_EXT);



  if (Plug != NULL && Plug->SDKCompatible)
  {
    Plug->Function->setDataRepository(CoreData);
    Plug->Function->setExecutionMessages(ExecMsgs);
    Plug->Function->setFunctionEnvironment(RuntimeEnvironment::getInstance()->getFunctionEnvironment());
    return Plug;
  }


  return NULL;
}



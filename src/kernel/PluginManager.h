/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file PluginManager.h
  \brief header of the plugins management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "openfluid-base.h"
#include "openfluid-core.h"
#include "RuntimeEnv.h"


struct PluginContainer
{

  std::string Filename;
  bool SDKCompatible;
  openfluid::base::FunctionSignature* Signature;
  openfluid::base::PluggableFunction* Function;

  PluginContainer()
  {
    Filename = "";
    Signature = NULL;
    Function = NULL;
    SDKCompatible = false;
  }

};


typedef std::vector<PluginContainer*> ArrayOfPluginsContainers;

/**
  Management class for plugins
*/
class PluginManager
{
  private:

    RuntimeEnvironment* mp_RunEnv;

    PluginContainer *buildPluginContainer(std::string PluginFilename);


  public:
    /**
      Constructor
    */
    PluginManager(RuntimeEnvironment* RunEnv);

    /**
      Destructor
    */
    ~PluginManager();

    /**
      Lists available simulation functions
    */
    ArrayOfPluginsContainers getAvailableFunctions(const std::string Pattern = "");


    /**
      Returns function and function types
    */
    PluginContainer *getPlugin(std::string PluginName,
                               openfluid::base::ExecutionMessages* ExecMsgs,
                               openfluid::core::CoreRepository* CoreData);


};





#endif




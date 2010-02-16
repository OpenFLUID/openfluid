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
#include "model/ModelItemInstance.h"

#include "RuntimeEnv.h"


typedef std::vector<ModelItemInstance*> ArrayOfModelItemInstance;

/**
  Management class for plugins
*/
class PluginManager
{
  private:

    RuntimeEnvironment* mp_RunEnv;

    ModelItemInstance* buildPluginContainer(std::string PluginFilename);


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
    ArrayOfModelItemInstance getAvailableFunctions(const std::string Pattern = "");


    /**
      Returns function and function types
    */
    ModelItemInstance* getPlugin(std::string PluginName,
                               openfluid::base::ExecutionMessages* ExecMsgs,
                               openfluid::core::CoreRepository* CoreData);


};





#endif




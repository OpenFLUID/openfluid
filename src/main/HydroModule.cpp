/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "HydroModule.h"


HydroModule::HydroModule(mhydasdk::core::CoreRepository* CoreData,
                         mhydasdk::base::Function* SUProdFunc,
                         mhydasdk::base::Function* SUTransFunc,
                         mhydasdk::base::Function* RSTransFunc,
                         mhydasdk::base::Function* RSGUExchFunc,
                         mhydasdk::base::Function* SUGUExchFunc)
      : Module(CoreData)
{

}

// =====================================================================
// =====================================================================

HydroModule::~HydroModule()
{

}


// =====================================================================
// =====================================================================


bool HydroModule::loadData()
{

}

// =====================================================================
// =====================================================================

bool HydroModule::saveResults()
{

}


// =====================================================================
// =====================================================================


bool HydroModule::checkConsistency()
{


}


// =====================================================================
// =====================================================================


bool HydroModule::initialize()
{

}


// =====================================================================
// =====================================================================


bool HydroModule::run(mhydasdk::base::SimulationStatus SimStatus)
{

}


// =====================================================================
// =====================================================================


bool HydroModule::finalize()
{

}




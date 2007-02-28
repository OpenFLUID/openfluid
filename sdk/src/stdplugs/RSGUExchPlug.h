/**
  \file RSGUExchPlug.h
  \brief header of RSGUExchangePlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __RSGUEXCHPLUG_H__
#define __RSGUEXCHPLUG_H__


#include "mhydasdk-core.h"
#include "mhydasdk-base.h"

// =====================================================================
// =====================================================================

extern "C"
{
  DLLIMPORT mhydasdk::base::Plugin* GetMHYDASPlugin();
};

// =====================================================================
// =====================================================================

/**

*/
class RSGUExchangePlug : public mhydasdk::base::Plugin
{
  private:

  public:
    RSGUExchangePlug();

    ~RSGUExchangePlug();

    mhydasdk::base::Function *getFunction(mhydasdk::core::CoreRepository* CoreData);

};


#endif  // __RSGUEXCHPLUG_H__



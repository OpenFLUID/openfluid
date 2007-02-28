/**
  \file GUGUExchPlug.h
  \brief header of GUGUExchangePlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __GUGUEXCHPLUG_H__
#define __GUGUEXCHPLUG_H__


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
class GUGUExchangePlug : public mhydasdk::base::Plugin
{
  private:

  public:
    GUGUExchangePlug();

    ~GUGUExchangePlug();

    mhydasdk::base::Function *getFunction(mhydasdk::core::CoreRepository* CoreData);

};


#endif  // __GUGUEXCHPLUG_H__



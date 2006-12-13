/**
  \file SUGUExchPlug.h
  \brief header of SUGUExchangePlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __SUGUEXCHPLUG_H__
#define __SUGUEXCHPLUG_H__


#include "sdk-core.h"
#include "sdk-base.h"

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
class SUGUExchangePlug : public mhydasdk::base::Plugin
{
  private:

  public:
    SUGUExchangePlug();

    ~SUGUExchangePlug();

    mhydasdk::base::Function *getFunction(mhydasdk::core::CoreRepository* CoreData);

};


#endif  // __SUGUEXCHPLUG_H__



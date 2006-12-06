/**
  \file HayamiRSPlug.h
  \brief header of HayamiRSPlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HAYAMIRSPLUG_H__
#define __HAYAMIRSPLUG_H__


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
class HayamiRSPlug : public mhydasdk::base::Plugin
{
  private:

  public:
    HayamiRSPlug();

    ~HayamiRSPlug();

    mhydasdk::base::Function *getFunction(mhydasdk::core::CoreRepository* CoreData);

};


#endif  // __HAYAMIRSPLUG_H__



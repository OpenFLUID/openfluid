/**
  \file HayamiSUPlug.h
  \brief header of HayamiSUPlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HAYAMISUPLUG_H__
#define __HAYAMISUPLUG_H__


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
class HayamiSUPlug : public mhydasdk::base::Plugin
{
  private:

  public:
    HayamiSUPlug();

    ~HayamiSUPlug();

    mhydasdk::base::Function *getFunction(mhydasdk::core::CoreRepository* CoreData);

};


#endif  // __HAYAMISUPLUG_H__



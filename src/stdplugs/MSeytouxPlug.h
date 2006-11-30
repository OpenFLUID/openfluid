/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __MSEYTOUXPLUG_H__
#define __MSEYTOUXPLUG_H__



#include "sdk-core.h"
#include "sdk-base.h"

// =====================================================================
// =====================================================================

extern "C"
{
  DLLIMPORT mhydasdk::base::Plugin* GetMHYDASPlugin();
};


/**

*/

class MorelSeytouxPlug : public mhydasdk::base::Plugin
{
  private:


  public:
    MorelSeytouxPlug();

    ~MorelSeytouxPlug();

    mhydasdk::base::Function *getFunction(mhydasdk::core::CoreRepository* CoreData);


};




#endif // __MSEYTOUXPLUG_H__

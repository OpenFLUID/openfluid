/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __MSEYTOUXPLUG_H__
#define __MSEYTOUXPLUG_H__


#include "sdk-core.h"
#include "sdk-base.h"


/**

*/
class MorelSeytouxPlug : public mhydasdk::base::Plugin
{
  private:

  protected:

  public:
    /**
      Constructor
    */
    MorelSeytouxPlug();

    /**
      Destructor
    */
    ~MorelSeytouxPlug();

    /**
      Returns the plugin signature
      \return \link Signature Signature \endlink
    */
    mhydasdk::base::Signature getSignature();

    /**
      Returns the plugin function
      \return Function
    */
    mhydasdk::base::Function *getFunction();


};


// =====================================================================
// =====================================================================

extern "C"
{
  DLLIMPORT mhydasdk::base::Plugin* GetMHYDASPlugin();
};





#endif // __MSEYTOUXPLUG_H__

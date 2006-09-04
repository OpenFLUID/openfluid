/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "MSeytouxFunc.h"
#include "MSeytouxPlug.h"

MorelSeytouxPlug::MorelSeytouxPlug()
                : mhydasdk::base::Plugin()
{

}

// =====================================================================
// =====================================================================

MorelSeytouxPlug::~MorelSeytouxPlug()
{

}


// =====================================================================
// =====================================================================


mhydasdk::base::Signature MorelSeytouxPlug::getSignature()
{

  mhydasdk::base::Signature PlugSign;



  return PlugSign;


}



// =====================================================================
// =====================================================================



mhydasdk::base::Function* MorelSeytouxPlug::getFunction()
{
  return NULL;
}


// =====================================================================
// =====================================================================

mhydasdk::base::Plugin* GetMHYDASPlugin()
{
  return new MorelSeytouxPlug();
}


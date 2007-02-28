/**
  \file GUGUExchPlug.h
  \brief implementation of GUGUExchangePlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "GUGUExchFunc.h"
#include "GUGUExchPlug.h"


mhydasdk::base::Plugin* GetMHYDASPlugin()
{
  return new GUGUExchangePlug();
}


// =====================================================================
// =====================================================================


GUGUExchangePlug::GUGUExchangePlug()
          : mhydasdk::base::Plugin()
{
  mp_Signature = new mhydasdk::base::Signature();

  mp_Signature->Author = wxT("Jean-Christophe FABRE");
  mp_Signature->AuthorEmail = wxT("fabrejc@ensam.inra.fr");
  mp_Signature->ID = wxT("guguexchange");
  mp_Signature->FunctionType = mhydasdk::base::SIMULATION;
  mp_Signature->Name = wxT("");
  mp_Signature->Description = wxT("");

}


// =====================================================================
// =====================================================================


GUGUExchangePlug::~GUGUExchangePlug()
{

}


// =====================================================================
// =====================================================================


mhydasdk::base::Function* GUGUExchangePlug::getFunction(mhydasdk::core::CoreRepository* CoreData)
{
  return new GUGUExchangeFunction(CoreData);
}



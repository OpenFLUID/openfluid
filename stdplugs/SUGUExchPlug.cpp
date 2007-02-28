/**
  \file SUGUExchPlug.h
  \brief implementation of SUGUExchangePlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "SUGUExchFunc.h"
#include "SUGUExchPlug.h"


mhydasdk::base::Plugin* GetMHYDASPlugin()
{
  return new SUGUExchangePlug();
}


// =====================================================================
// =====================================================================


SUGUExchangePlug::SUGUExchangePlug()
          : mhydasdk::base::Plugin()
{
  mp_Signature = new mhydasdk::base::Signature();

  mp_Signature->Author = wxT("Jean-Christophe FABRE");
  mp_Signature->AuthorEmail = wxT("fabrejc@ensam.inra.fr");
  mp_Signature->ID = wxT("suguexchange");
  mp_Signature->FunctionType = mhydasdk::base::SIMULATION;
  mp_Signature->Name = wxT("");
  mp_Signature->Description = wxT("");

}


// =====================================================================
// =====================================================================


SUGUExchangePlug::~SUGUExchangePlug()
{

}


// =====================================================================
// =====================================================================


mhydasdk::base::Function* SUGUExchangePlug::getFunction(mhydasdk::core::CoreRepository* CoreData)
{
  return new SUGUExchangeFunction(CoreData);
}



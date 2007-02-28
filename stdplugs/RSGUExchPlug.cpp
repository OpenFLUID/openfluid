/**
  \file RSGUExchPlug.h
  \brief implementation of RSGUExchangePlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "RSGUExchFunc.h"
#include "RSGUExchPlug.h"


mhydasdk::base::Plugin* GetMHYDASPlugin()
{
  return new RSGUExchangePlug();
}


// =====================================================================
// =====================================================================


RSGUExchangePlug::RSGUExchangePlug()
          : mhydasdk::base::Plugin()
{
  mp_Signature = new mhydasdk::base::Signature();

  mp_Signature->Author = wxT("Jean-Christophe FABRE");
  mp_Signature->AuthorEmail = wxT("fabrejc@ensam.inra.fr");
  mp_Signature->ID = wxT("rsguexchange");
  mp_Signature->FunctionType = mhydasdk::base::SIMULATION;
  mp_Signature->Name = wxT("");
  mp_Signature->Description = wxT("");

}


// =====================================================================
// =====================================================================


RSGUExchangePlug::~RSGUExchangePlug()
{

}


// =====================================================================
// =====================================================================


mhydasdk::base::Function* RSGUExchangePlug::getFunction(mhydasdk::core::CoreRepository* CoreData)
{
  return new RSGUExchangeFunction(CoreData);
}



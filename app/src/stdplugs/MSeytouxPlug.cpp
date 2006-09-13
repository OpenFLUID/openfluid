/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "MSeytouxFunc.h"
#include "MSeytouxPlug.h"



mhydasdk::base::Plugin* GetMHYDASPlugin()
{
  return new MorelSeytouxPlug();
}


// =====================================================================
// =====================================================================


MorelSeytouxPlug::MorelSeytouxPlug()
              : mhydasdk::base::Plugin()
{
  mp_Signature = new mhydasdk::base::Signature();

  mp_Signature->Author = wxT("Jean-Christophe Fabre");
  mp_Signature->AuthorEmail = wxT("");
  mp_Signature->ID = wxT("mseytoux");
  mp_Signature->ModuleType = mhydasdk::base::MOD_HYDROLOGY;
  // mp_Signature->FunctionType = mhydasdk::base::FUNC_SU_PRODUCTION;
  mp_Signature->Name = wxT("Morel-Seytoux production");


}

// =====================================================================
// =====================================================================

MorelSeytouxPlug::~MorelSeytouxPlug()
{

}


// =====================================================================
// =====================================================================



mhydasdk::base::Function* MorelSeytouxPlug::getFunction(mhydasdk::core::CoreRepository* CoreData)
{
  return new MorelSeytouxFunc(CoreData);
}


// =====================================================================
// =====================================================================




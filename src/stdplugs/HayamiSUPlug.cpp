/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "HayamiSUFunc.h"
#include "HayamiSUPlug.h"



mhydasdk::base::Plugin* GetMHYDASPlugin()
{
  return new HayamiSUPlug();
}


// =====================================================================
// =====================================================================


HayamiSUPlug::HayamiSUPlug()
              : mhydasdk::base::Plugin()
{
  mp_Signature = new mhydasdk::base::Signature();

  mp_Signature->Author = wxT("Jean-Christophe Fabre");
  mp_Signature->AuthorEmail = wxT("");
  mp_Signature->ID = wxT("hayamiSU");
  mp_Signature->ModuleType = mhydasdk::base::MOD_HYDROLOGY;
  mp_Signature->FunctionType = mhydasdk::base::FUNC_SU_TRANSFER;
  mp_Signature->Name = wxT("Diffusive wave transfert on SU (Hayami method)");


}

// =====================================================================
// =====================================================================

HayamiSUPlug::~HayamiSUPlug()
{

}


// =====================================================================
// =====================================================================



mhydasdk::base::Function* HayamiSUPlug::getFunction()
{
  return NULL;
}


// =====================================================================
// =====================================================================




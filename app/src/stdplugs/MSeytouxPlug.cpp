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
  mp_Signature->AuthorEmail = wxT("fabrejc@ensam.inra.fr");
  mp_Signature->ID = wxT("mseytoux");
  mp_Signature->FunctionType = mhydasdk::base::SIMULATION;
  // mp_Signature->FunctionType = mhydasdk::base::FUNC_SU_PRODUCTION;
  mp_Signature->Name = wxT("Morel-Seytoux production on surface units");
  mp_Signature->Description = wxT("Production function computing infiltration and runoff at the surface of a unit using the Morel-Seytoux method, based on the Green and Ampt method;");

  mp_Signature->HandledVarsPropsParams.Add(wxT("pvar;SU;runoff;Runoff on the surface of the unit;m/s"));
  mp_Signature->HandledVarsPropsParams.Add(wxT("pvar;SU;infiltration;Infiltration through the surface of the unit;m/s"));

  mp_Signature->HandledVarsPropsParams.Add(wxT("prop;SU;ks;Hydraulic conductivity when saturated;m/s"));

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




/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "Module.h"

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(FunctionsList);


// =====================================================================
// =====================================================================

#define PARSE_FUNCTION_LIST(calledmethod,statevar) \
    FunctionsList::Node *Node = m_Functions.GetFirst(); \
    while (Node && statevar) \
    { \
      mhydasdk::base::Function* CurrentFunction = (mhydasdk::base::Function*)Node->GetData(); \
      if (CurrentFunction != NULL) statevar = statevar && CurrentFunction->calledmethod; \
      Node = Node->GetNext(); \
    }

// =====================================================================
// =====================================================================



Module::Module(mhydasdk::core::CoreRepository* CoreData, FunctionsList Functions)
      : mhydasdk::base::ComputationBlock(CoreData)
{
  m_Functions = Functions;
}

// =====================================================================
// =====================================================================



Module::~Module()
{

}



// =====================================================================
// =====================================================================


bool Module::checkConsistency()
{
  bool IsOK = true;

  if (m_Functions.GetCount() == 0) IsOK = false;
  else
  {
    PARSE_FUNCTION_LIST(checkConsistency(),IsOK);
  }

  return IsOK;
}


// =====================================================================
// =====================================================================


bool Module::initialize()
{
  bool IsOK = true;

  PARSE_FUNCTION_LIST(initialize(),IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool Module::run(mhydasdk::base::SimulationStatus* SimStatus)
{
  bool IsOK = true;

  PARSE_FUNCTION_LIST(run(SimStatus),IsOK);

  return IsOK;

}


// =====================================================================
// =====================================================================


bool Module::finalize()
{
  bool IsOK = true;

  PARSE_FUNCTION_LIST(finalize(),IsOK);

  return IsOK;

}








/**
  \file FortranFunc.cpp
  \brief Implements ...
*/


#include "FortranFunc.h"
#include "openfluid-tools.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(FortranFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.fortran"));
  DECLARE_SIGNATURE_NAME(("test function for fortran code wrapping"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));
END_SIGNATURE_HOOK

// =====================================================================
// =====================================================================


BEGIN_EXTERN_FORTRAN
  EXTERN_FSUBROUTINE(multrealvalue)(FREAL8*,FREAL8*,FREAL8*);
  EXTERN_FSUBROUTINE(multintvalue)(FINT*,FINT*,FINT*);
  EXTERN_FSUBROUTINE(catstrings)(FCHARACTER*,FCHARACTER*,FCHARACTER*);
  EXTERN_FSUBROUTINE(multrealmatrix)(FREAL8*,FINT*,FINT*,FINT*,FREAL8*);
END_EXTERN_FORTRAN


// =====================================================================
// =====================================================================


FortranFunction::FortranFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


FortranFunction::~FortranFunction()
{


}


// =====================================================================
// =====================================================================


bool FortranFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool FortranFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool FortranFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool FortranFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  m_Precision = 0.000001;

  return true;
}

// =====================================================================
// =====================================================================


bool FortranFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  int i;


  // ====== double ======

  double DValue, DMult, DResult;
  DValue = 1.5436;
  DMult = 2.5;
  DResult = 0.0;

  CALL_FSUBROUTINE(multrealvalue)(&DValue,&DMult,&DResult);

  if (abs(DResult - (DValue*DMult)) > m_Precision)
    OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multrealvalue)");


  // ====== int ======

  int IValue, IMult, IResult;
  IValue = 45;
  IMult = 18;
  IResult = 0;

  CALL_FSUBROUTINE(multintvalue)(&IValue,&IMult,&IResult);

  if (IResult != (IValue*IMult))
    OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multintvalue)");


  // ====== string ======

  // TODO finish this
/*  std::string SStr1, SStr2, SResult;
  char* CResult;
  SStr1 = "Hello";
  SStr2 = "from fortran";

  CALL_FSUBROUTINE(catstrings)(STD2FSTRING(SStr1),STD2FSTRING(SStr2),CResult);

  SResult = std::string(CResult);

  if (SResult != (SStr1 + " " + SStr2))
    OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (catstrings)");
*/


  // ====== matrix ======

  int MMult, MDim1,MDim2;
  double *MValue;
  double *MResult;
  MMult = 3;
  MDim1 = 2;
  MDim2 = 3;


  MValue = new double[MDim1*MDim2];
  MResult = new double[MDim1*MDim2];

  for (i=0; i < MDim1*MDim2;i++) MValue[i] = 1.5;
  for (i=0; i < MDim1*MDim2;i++) MResult[i] = 0.0;

  CALL_FSUBROUTINE(multrealmatrix)(MValue,&MDim1,&MDim2,&MMult,MResult);

  for (i=0; i < MDim1*MDim2;i++)
  {
    if (abs(MResult[i] - (MValue[i] * MMult)) > m_Precision)
      OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multrealmatrix)");
  }

  return true;
}

// =====================================================================
// =====================================================================


bool FortranFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}


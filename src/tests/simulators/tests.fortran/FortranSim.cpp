/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file FortranSim.cpp
  \brief Implements ...
*/


#include "FortranSim.h"
#include <openfluid/tools/FortranCPP.hpp>
#include <openfluid/core/Value.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <cmath>


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(FortranSimulator)


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.fortran")

  DECLARE_SIGNATURE_NAME("test simulator for fortran code wrapping");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

  DECLARE_SCHEDULING_DEFAULT();
END_SIMULATOR_SIGNATURE

// =====================================================================
// =====================================================================


BEGIN_EXTERN_FORTRAN
  EXTERN_FSUBROUTINE(multrealvalue)(FREAL8*,FREAL8*,FREAL8*);
  EXTERN_FSUBROUTINE(multintvalue)(FINT*,FINT*,FINT*);
  EXTERN_FSUBROUTINE(catstrings)(FCHARACTER*,FCHARACTER*,FCHARACTER*);
  EXTERN_FSUBROUTINE(multrealmatrix)(FREAL8*,FINT*,FINT*,FREAL8*,FREAL8*);
END_EXTERN_FORTRAN


// =====================================================================
// =====================================================================


FortranSimulator::FortranSimulator()
                : PluggableSimulator()
{


}


// =====================================================================
// =====================================================================


FortranSimulator::~FortranSimulator()
{


}


// =====================================================================
// =====================================================================


void FortranSimulator::initParams(const openfluid::ware::WareParams_t& /*Params*/)
{  }

// =====================================================================
// =====================================================================


void FortranSimulator::prepareData()
{  }


// =====================================================================
// =====================================================================


void FortranSimulator::checkConsistency()
{  }


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest FortranSimulator::initializeRun()
{

  m_Precision = 0.000001;

  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest FortranSimulator::runStep()
{
  int i,j;

  // ====== double ======

  double DValue, DMult, DResult;
  DValue = 1.5436;
  DMult = 2.5;
  DResult = 0.0;

  CALL_FSUBROUTINE(multrealvalue)(&DValue,&DMult,&DResult);

  if (std::abs(DResult - (DValue*DMult)) > m_Precision)
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

  int MDim1,MDim2;
  double MMult;
  openfluid::core::MatrixValue MValue;
  openfluid::core::MatrixValue MResult;
  double* MTmpResult;

  MDim1 = 3;
  MDim2 = 5;

  MMult = 2.18;

  MValue = openfluid::core::MatrixValue(MDim1,MDim2);

  MTmpResult = new double[MDim1*MDim2];

  for (j=0; j < MDim2;j++)
    for (i=0; i < MDim1;i++)
      MValue.setElement(i,j,(j*MDim1)+i+1);

  std::cout << std::endl;



  std::cout << "MValue:"<< std::endl;
  for (j=0; j < MDim2;j++)
  {
    for (i=0; i < MDim1;i++)
    {
        std::cout << MValue.getElement(i,j) << " ";
    }
    std::cout << std::endl;
  }

  MResult.fill(0.0);

  CALL_FSUBROUTINE(multrealmatrix)(MValue.getData(),&MDim1,&MDim2,&MMult,MTmpResult);

  MResult = openfluid::core::MatrixValue(MDim1,MDim2);
  MResult.setData(MTmpResult);

  std::cout << "MTmpResult:"<< std::endl;
  for (j=0; j < MDim2;j++)
  {
    for (i=0; i < MDim1;i++)
    {
        std::cout << MTmpResult[i+(j*MDim1)] << " ";
    }
    std::cout << std::endl;
  }


  std::cout << "MResult:"<< std::endl;
  for (j=0; j < MDim2;j++)
  {
    for (i=0; i < MDim1;i++)
    {
        std::cout << MResult.getElement(i,j) << " ";
    }
    std::cout << std::endl;
  }


  std::cout << std::endl;


  for (j=0; j < MDim2;j++)
  {
    for (i=0; i < MDim1;i++)
    {
//      std::cout << MResult.get(i,j) << "  " << (MValue.get(i,j) * MMult) << std::endl;
      if (std::abs(MResult.get(i,j) - (MValue.get(i,j) * MMult)) > m_Precision)
        OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multrealmatrix)");
    }
  }

  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


void FortranSimulator::finalizeRun()
{  }


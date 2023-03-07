/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file FortranMoreSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


// OpenFLUID:stylecheck:!incs


#include <openfluid/ware/PluggableSimulator.hpp>


//! [fortran_include]
#include <openfluid/tools/FortranCPP.hpp>
//! [fortran_include]


//! [fortran_decl]
BEGIN_EXTERN_FORTRAN
  EXTERN_FSUBROUTINE(multrealvalue)(FREAL8*,FREAL8*,FREAL8*);
  EXTERN_FSUBROUTINE(multintvalue)(FINT*,FINT*,FINT*);
  EXTERN_FSUBROUTINE(multrealmatrix)(FREAL8*,FINT*,FINT*,FINT*,FREAL8*);
END_EXTERN_FORTRAN
//! [fortran_decl]


// =====================================================================
// =====================================================================


class SnippetsSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    SnippetsSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~SnippetsSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {

    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


//! [fortran_use]
openfluid::base::SchedulingRequest runStep()
{
  int i;


  // ====== double ======

  double DValue, DMult, DResult;
  DValue = 1.5436;
  DMult = 2.5;
  DResult = 0.0;

  CALL_FSUBROUTINE(multrealvalue)(&DValue,&DMult,&DResult);

  if (abs(DResult - (DValue*DMult)) > 0.0001)
  {
    OPENFLUID_RaiseError("Wrong fortran call (multrealvalue)");
  }


  // ====== int ======

  int IValue, IMult, IResult;
  IValue = 45;
  IMult = 18;
  IResult = 0;

  CALL_FSUBROUTINE(multintvalue)(&IValue,&IMult,&IResult);

  if (IResult != (IValue*IMult))
  {
    OPENFLUID_RaiseError("Wrong fortran call (multintvalue)");
  }


  // ====== matrix of double ======

  int MMult, MDim1,MDim2;
  double *MValue;
  double *MResult;
  MMult = 3;
  MDim1 = 2;
  MDim2 = 3;

  MValue = new double[MDim1*MDim2];
  MResult = new double[MDim1*MDim2];

  for (i=0; i < MDim1*MDim2;i++)
  {
     MValue[i] = 1.5;
     MResult[i] = 0.0;
  }

  CALL_FSUBROUTINE(multrealmatrix)(MValue,&MDim1,&MDim2,&MMult,MResult);

  for (i=0; i < MDim1*MDim2;i++)
  {
    if (abs(MResult[i] - (MValue[i] * MMult)) > 0.0001)
    {
      OPENFLUID_RaiseError("Wrong fortran call (multrealmatrix)");
    }
  }

  return DefaultDeltaT();
}
//! [fortran_use]


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};


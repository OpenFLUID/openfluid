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
  @file Fortran90Sim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <cmath>

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/FortranCPP.hpp>
#include <openfluid/core/Value.hpp>
#include <openfluid/core/MatrixValue.hpp>


BEGIN_EXTERN_FORTRAN
  EXTERN_FMODSUBROUTINE(testmodule,multrealvalue)(FREAL8*,FREAL8*,FREAL8*);
  EXTERN_FMODSUBROUTINE(testmodule,multintvalue)(FINT*,FINT*,FINT*);
  EXTERN_FMODSUBROUTINE(testmodule,catstrings)(FCHARACTER*,FCHARACTER*,FCHARACTER*);
  EXTERN_FMODSUBROUTINE(testmodule,multrealmatrix)(FREAL8*,FINT*,FINT*,FREAL8*,FREAL8*);
END_EXTERN_FORTRAN


// =====================================================================
// =====================================================================


class Fortran90Simulator : public openfluid::ware::PluggableSimulator
{
  private:

    double m_Precision;

  public:

    Fortran90Simulator() : PluggableSimulator(),
      m_Precision(0.000001)
    {

    }


    // =====================================================================
    // =====================================================================


    ~Fortran90Simulator()
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


    openfluid::base::SchedulingRequest runStep()
    {
      int i,j;

      // ====== double ======

      double DValue, DMult, DResult;
      DValue = 1.5436;
      DMult = 2.5;
      DResult = 0.0;

      CALL_FMODSUBROUTINE(testmodule,multrealvalue)(&DValue,&DMult,&DResult);

      if (std::abs(DResult - (DValue*DMult)) > m_Precision)
      {
        OPENFLUID_RaiseError("incorrect fortran call (multrealvalue)");
      }


      // ====== int ======

      int IValue, IMult, IResult;
      IValue = 45;
      IMult = 18;
      IResult = 0;

      CALL_FMODSUBROUTINE(testmodule,multintvalue)(&IValue,&IMult,&IResult);

      if (IResult != (IValue*IMult))
      {
        OPENFLUID_RaiseError("incorrect fortran call (multintvalue)");
      }


      // ====== string ======

      // TODO finish this
      /*  std::string SStr1, SStr2, SResult;
  char* CResult;
  SStr1 = "Hello";
  SStr2 = "from fortran";

  CALL_FSUBROUTINE(catstrings)(STD2FSTRING(SStr1),STD2FSTRING(SStr2),CResult);

  SResult = std::string(CResult);

  if (SResult != (SStr1 + " " + SStr2))
  {
    OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (catstrings)");
  }
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
      {
        for (i=0; i < MDim1;i++)
        {
          MValue.setElement(i,j,(j*MDim1)+i+1);
        }
      }
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

      CALL_FMODSUBROUTINE(testmodule,multrealmatrix)(MValue.data(),&MDim1,&MDim2,&MMult,MTmpResult);

      MResult = openfluid::core::MatrixValue(MDim1,MDim2);
      MResult.setData(MTmpResult,MDim1,MDim2);

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
          {
            OPENFLUID_RaiseError("incorrect fortran call (multrealmatrix)");
          }
        }
      }

      delete[] MTmpResult;

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(Fortran90Simulator)

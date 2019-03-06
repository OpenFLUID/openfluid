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
  @file HayamiTools.hpp
  @brief header of tools for Hayami propagation method

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __HAYAMITOOLS_H__
#define __HAYAMITOOLS_H__


#include <vector>
#include <map>
#include <cmath>

#include <openfluid/core/TypeDefs.hpp>


typedef std::vector<double> t_HayamiKernel;
typedef std::map<int, t_HayamiKernel> IDKernelMap;


// =====================================================================
// =====================================================================


/**
  Computes the Hayami Kernel
  \param[in] Celerity Wave propagation (m/s)
  \param[in] Sigma Wave diffusivity (m2/s)
  \param[in] Length Transfer Length (m)
  \param[in] MaxSteps Maximum steps for the kernel length
  \param[in] TimeStep Time step duration (s)
  \param[out] Kernel Vector of double containing the kernel
*/
inline void ComputeHayamiKernel(double Celerity, double Sigma, double Length,
                         unsigned int MaxSteps, int TimeStep,
                         t_HayamiKernel& Kernel)
{
  float Theta, Zed;
  float Value1, Value2, Value3;
  float T;
  float Volume;

  unsigned int i;


  Kernel.clear();
  Kernel.resize(MaxSteps,0);

  Theta = Length / Celerity;
  Zed = (Celerity * Length) / ( 4 * Sigma);

  if (Zed < 0.5)
  {
    Zed = 0.5;
  }
  if (Zed > 50)
  {
    Zed = 50;
  }


  Value1 = pow((Theta * Zed / 3.1411592654),0.5);
  if (Theta > (0.1*TimeStep))
  {
    for (i=0;i<MaxSteps;i++)
    {
      T = ((i+1)-0.5) * TimeStep;
      Value2 = exp(Zed * (2 - (T/Theta) - (Theta/T)));
      Value3 = pow(T,1.5);
      Kernel[i] = Value1 * Value2 / Value3;
    }
  }
  else
  {
    Kernel[0] = 0.5 / TimeStep;
    Kernel[1] = 0.5 / TimeStep;
  }


  Volume = 0;


  for (i=0;i<Kernel.size();i++)
  {
    Volume = Volume + (Kernel[i] * TimeStep);
  }

  for (i=0;i<Kernel.size();i++)
  {
    Kernel[i] = Kernel[i] * (1/Volume);
  }
}


// =====================================================================
// =====================================================================


/**
  Propagates the wave using the Hayami kernel
*/
inline float DoHayamiPropagation(const t_HayamiKernel& Kernel,
                          int CurrentStep, const openfluid::core::SerieOfDoubleValue* QInput,
                          int MaxSteps, int TimeStep)
{
  float QOutput;
  int ZeEnd;


  ZeEnd = MaxSteps;
  if (CurrentStep < ZeEnd)
  {
    ZeEnd = CurrentStep;
  }

  QOutput = 0;

  for (int i=0;i<ZeEnd;i++)
  {
    QOutput = QOutput + (Kernel[i] * QInput->at(CurrentStep - i) * TimeStep);
  }


  return QOutput;
}

#endif // __HAYAMITOOLS_H__

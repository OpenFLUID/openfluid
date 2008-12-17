/**
  \file HayamiTools.cpp
  \brief implements tools for Hayami propagation method

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HayamiTools.h"
#include <math.h>

#include <iostream>

// =====================================================================
// =====================================================================

/*
t_HayamiKernel ComputeHayamiKernel(double Celerity, double Sigma, double Length, int MaxSteps, int TimeStep)
{

  float Theta, Zed;
  float Value1, Value2, Value3;
  float T;
  float Volume;

  int i;

  t_HayamiKernel ZeKernel;

  ZeKernel.resize(MaxSteps,0);


  Theta = Length / Celerity;
  Zed = (Celerity * Length) / ( 4 * Sigma);

  if (Zed < 0.5) Zed = 0.5;
  if (Zed > 50) Zed = 50;

  Value1 = pow((Theta * Zed / 3.1411592654),0.5);
  if (Theta > (0.1*TimeStep))
  {
    for (i=0;i<MaxSteps;i++)
    {
      T = ((i+1)-0.5) * TimeStep;
      Value2 = exp(Zed * (2 - (T/Theta) - (Theta/T)));
      Value3 = pow(T,1.5);
      ZeKernel[i] = Value1 * Value2 / Value3;
    }
  }
  else
  {
    ZeKernel[0] = 0.5 / TimeStep;
    ZeKernel[1] = 0.5 / TimeStep;
  }


  Volume = 0;


  for (i=0;i<ZeKernel.size();i++)
  {
    Volume = Volume + (ZeKernel[i] * TimeStep);
  }


  for (i=0;i<ZeKernel.size();i++) ZeKernel[i] = ZeKernel[i] * (1/Volume);


  return ZeKernel;
}
*/

// =====================================================================
// =====================================================================

void ComputeHayamiKernel(double Celerity, double Sigma, double Length, int MaxSteps, int TimeStep, t_HayamiKernel *HKernel)
{
  float Theta, Zed;
  float Value1, Value2, Value3;
  float T;
  float Volume;

  int i;


  (*HKernel).clear();

  (*HKernel).resize(MaxSteps,0);


  Theta = Length / Celerity;
  Zed = (Celerity * Length) / ( 4 * Sigma);

  if (Zed < 0.5) Zed = 0.5;
  if (Zed > 50) Zed = 50;

  Value1 = pow((Theta * Zed / 3.1411592654),0.5);
  if (Theta > (0.1*TimeStep))
  {
    for (i=0;i<MaxSteps;i++)
    {
      T = ((i+1)-0.5) * TimeStep;
      Value2 = exp(Zed * (2 - (T/Theta) - (Theta/T)));
      Value3 = pow(T,1.5);
      (*HKernel)[i] = Value1 * Value2 / Value3;
    }
  }
  else
  {
    (*HKernel)[0] = 0.5 / TimeStep;
    (*HKernel)[1] = 0.5 / TimeStep;
  }


  Volume = 0;


  for (i=0;i<(*HKernel).size();i++)
  {
    Volume = Volume + ((*HKernel)[i] * TimeStep);
  }

  for (i=0;i<(*HKernel).size();i++) (*HKernel)[i] = (*HKernel)[i] * (1/Volume);

}

// =====================================================================
// =====================================================================



float DoHayamiPropagation(t_HayamiKernel Kernel, int CurrentStep, openfluid::core::SerieOfScalarValue* QInput, int MaxSteps, int TimeStep)
{
  float QOutput;

  int ZeEnd;


  ZeEnd = MaxSteps;

  if (CurrentStep < ZeEnd) ZeEnd = CurrentStep;

  QOutput = 0;

  for (int i=0;i<ZeEnd;i++)
  {
    QOutput = QOutput + (Kernel[i] * QInput->at(CurrentStep - i) * TimeStep);
  }

  return QOutput;
}


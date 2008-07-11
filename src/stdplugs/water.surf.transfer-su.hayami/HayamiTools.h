/**
  \file HayamiTools.cpp
  \brief header of tools for Hayami propagation method

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HAYAMITOOLS_H__
#define __HAYAMITOOLS_H__

#include <vector>
#include "openfluid-core.h"

typedef std::vector<double> t_HayamiKernel;

WX_DECLARE_HASH_MAP(int, t_HayamiKernel, wxIntegerHash, wxIntegerEqual, IDKernelMap);

/**
  Computes the Hayami Kernel
  \param[in] Celerity Wave propagation (m/s)
  \param[in] Sigma Wave diffusivity (m2/s)
  \param[in] Length Transfer Length (m)
  \param[in] MaxSteps Maximum steps for the kernel length
  \param[in] TimeStep Time step duration (s)
  \param[out] HKernel Vector of double containing the kernel      
*/
void ComputeHayamiKernel(double Celerity, double Sigma, double Length, int MaxSteps, int TimeStep, t_HayamiKernel *HKernel);

//t_HayamiKernel ComputeHayamiKernel(double Celerity, double Sigma, double Length, int MaxSteps, int TimeStep);
 



/**
  Propagates the wave using the Hayami kernel
*/
float DoHayamiPropagation(t_HayamiKernel Kernel, int CurrentStep, openfluid::core::SerieOfMHYDASScalarValue* QInput, int MaxSteps, int TimeStep);


#endif // __HAYAMITOOLS_H__

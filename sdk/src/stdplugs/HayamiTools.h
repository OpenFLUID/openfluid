/**
  \file HayamiTools.cpp
  \brief header of tools for Hayami propagation method

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HAYAMITOOLS_H__
#define __HAYAMITOOLS_H__

#include <vector>
#include "mhydasdk-core.h"

typedef std::vector<float> t_HayamiKernel;

WX_DECLARE_HASH_MAP(int, t_HayamiKernel, wxIntegerHash, wxIntegerEqual, IDKernelMap);

/**
  Computes the Hayami Kernel
  \param[in] Celerity Wave propagation (m/s)
  \param[in] Sigma Wave diffusivity (m2/s)
  \param[in] Length Transfer Length (m)
  \param[in] MaxSteps Maximum steps for the kernel length
  \param[in] TimeStep Time step duration (s)
  \return a vector of double containing the kernel      
*/

 t_HayamiKernel ComputeHayamiKernel(float Celerity, float Sigma, float Length, int MaxSteps, int TimeStep);


/**
  Propagates the wave using the Hayami kernel
*/
float DoHayamiPropagation(t_HayamiKernel Kernel, int CurrentStep, mhydasdk::core::VectorOfDouble* QInput, int MaxSteps, int TimeStep);


#endif // __HAYAMITOOLS_H__

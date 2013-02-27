/**
  \file RasterMan.cpp
  \brief Implements ...
*/


#include "RasterMan.hpp"
#include <iostream>
#include <cmath>


// =====================================================================
// =====================================================================


SimpleRasterManager::SimpleRasterManager()
: m_Data(NULL)
{
  GDALAllRegister();
}


// =====================================================================
// =====================================================================


SimpleRasterManager::~SimpleRasterManager()
{
  close();
}


// =====================================================================
// =====================================================================


bool SimpleRasterManager::open(const std::string& Filepath)
{
  m_Data = (GDALDataset*)GDALOpen(Filepath.c_str(),GA_ReadOnly);

  if (m_Data != NULL)
  {
    double GeoTransform[6];

    m_Band = m_Data->GetRasterBand(1);
    m_Data->GetGeoTransform(GeoTransform);

    m_TLX = GeoTransform[0];
    m_TLY = GeoTransform[3];
    m_XPixelSize = GeoTransform[1];
    m_YPixelSize = GeoTransform[5];
    m_XSize = m_Data->GetRasterXSize();
    m_YSize = m_Data->GetRasterYSize();

    int GotMin, GotMax;
    double MinMax[2];
    MinMax[0] = m_Band->GetMinimum(&GotMin);
    MinMax[1] = m_Band->GetMaximum(&GotMax);
    if (!(GotMin && GotMax)) GDALComputeRasterMinMax((GDALRasterBandH)m_Band,true,MinMax);
    else std::cout << "not got " << GotMin << " " << GotMax << std::endl;

    m_MinValue = MinMax[0];
    m_MaxValue = MinMax[1];


    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


void SimpleRasterManager::close()
{
  if (m_Data != NULL)
  {
    GDALClose((GDALDataset*)m_Data);
    m_Band = NULL;
    m_Data = NULL;
  }
}

// =====================================================================
// =====================================================================


bool SimpleRasterManager::getRasterValue(const double& XCoord, const double& YCoord, double& Value)
{
  Value = 0.0;

  int XRaster = (int)std::floor((XCoord - m_TLX) / m_XPixelSize);
  int YRaster = (int)std::floor((YCoord - m_TLY) / m_YPixelSize);

  if (XRaster<0 || XRaster>=m_XSize || YRaster<0 || YRaster>=m_YSize)
  {
    return false;
  }


  float _Value = 0.0;

  if (m_Band->RasterIO(GF_Read,(int)XRaster,(int)YRaster, 1, 1,
                            &_Value, 1, 1, GDT_Float32,
                            0, 0) == CE_None)
  {
    Value = double(_Value);
  }
  else
  {
    return false;
  }

  if (Value > m_MaxValue) Value = m_MaxValue;
  if (Value < m_MinValue) Value = m_MinValue;

  return true;
}




/**
  \file RasterManager.hpp
  \brief Implements ...
*/


#ifndef __RASTERMAN_HPP__
#define __RASTERMAN_HPP__



#include <gdal.h>
#include <gdal_priv.h>

class SimpleRasterManager
{
  private:
    GDALDataset *m_Data;
    GDALRasterBand *m_Band;

    double m_TLX;
    double m_TLY;
    int m_XSize;
    int m_YSize;
    double m_XPixelSize;
    double m_YPixelSize;
    double m_MaxValue;
    double m_MinValue;

  public:

    SimpleRasterManager();

    ~SimpleRasterManager();

    bool open(const std::string& Filepath);

    void close();

    bool isOpened() const { return (m_Data != NULL); };

    bool getRasterValue(const double& XCoord, const double& YCoord, double& Value);

};



#endif /* __RASTERMAN_HPP__ */

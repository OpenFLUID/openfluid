/**
  \file DistribInterp.h
  \brief Header of class for time interpolation and spatial distribution of data

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DISTRIBINTERP_H__
#define __DISTRIBINTERP_H__


#include "openfluid-core.h"
#include "DTSerie.h"
#include "DataSrcFile.h"



namespace openfluid { namespace tools {

typedef std::map<int,DateTimeSerie*> DTSeriesMap;
typedef std::map<int,DateTimeSerie*> UnitsDTSeriesMap;
typedef std::map<int,IndexedSerie*> IndexedSeriesMap;
typedef std::map<int,IndexedSerie*> UnitsIndexedSeriesMap;



enum SeriePreprocess
{
  SERIEPREPCS_NONE,
  SERIEPREPCS_CUMULATE
};

/**
  Class for time interpolation and spatial distribution of data
*/
class DistributeInterpolate
{

  private:

    DTSeriesMap m_InterpData;
    IndexedSeriesMap m_InterpIndexedData;
    UnitsDTSeriesMap m_UnitsData;
    UnitsIndexedSeriesMap m_UnitsIndexedData;


    std::string m_DataSourcesFilename;
    std::string m_DataDir;
    std::string m_DistributionFilename;
    SeriePreprocess m_SPpcs;
    openfluid::core::DateTime m_Begin;
    openfluid::core::DateTime m_End;
    int m_TimeStep;

    bool m_Configured;

    bool loadDataAsSerie(std::string FilePath, SeriePreprocess SPpcs, DateTimeSerie *Serie);

    bool loadDistributionAndDistribute(std::string FilePath);

  public:
    DistributeInterpolate();

    void setConfig(std::string DataDir, std::string DataSourcesFilename, std::string DistributionFilename, SeriePreprocess SPpcs,
                   openfluid::core::DateTime Begin,openfluid::core::DateTime End, int TimeStep);

    bool loadAndPrepareData();

    bool getValue(int ID, openfluid::core::DateTime DT, openfluid::core::ScalarValue *Value);

    bool getValue(int ID, int Index, openfluid::core::ScalarValue *Value);

    virtual ~DistributeInterpolate();
};

} }


#endif /*__DISTRIBINTERP_H__*/


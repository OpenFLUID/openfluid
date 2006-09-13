/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __IOMAN_H__
#define __IOMAN_H__

#include "sdk-base.h"
#include "sdk-core.h"

#include <wx/list.h>

struct FunctionConfig
{
  wxString Name;
  wxString File;
  mhydasdk::core::ParamsMap Params;

};


WX_DECLARE_LIST(FunctionConfig, FunctionConfigsList);


struct EngineConfig
{
  int DeltaT;

  FunctionConfigsList HydroModuleConfig;

};


WX_DECLARE_HASH_MAP(int, wxString,wxIntegerHash, wxIntegerEqual, RainEventFilesMap);

/**
  File inputs/outputs management class
*/
class IOManager
{
  private:
    mhydasdk::base::RuntimeEnvironment* mp_RunEnv;

    mhydasdk::core::SUFlowCode getSUFlowCode(wxString Code);

    RainEventFilesMap buildRainEventFileMap();

    bool loadRainFile(mhydasdk::core::RainEvent *RainData, mhydasdk::core::cdsid_t ID, wxString Filename);

  public:
    /**
      Constructor
    */
    IOManager(mhydasdk::base::RuntimeEnvironment* RunEnv);

    /**
      Destructor
    */
    ~IOManager();

    /**
      Loads model engine configuration
      \param[out] Configuration definition to populate
    */
    bool loadModelConfig(EngineConfig* Config);

    /**
      Loads spatial hydro objects definition and topology
      \param[out] Spatial data structure to populate
    */
    bool loadHydroObjects(mhydasdk::core::SpatialRepository *SpatialData);

    /**
      Loads rain sources and builds rain event
      \param[out] Rain event data structure to populate
    */
    bool loadRainEvent(mhydasdk::core::RainEvent *RainData);


    /**
      Loads rain distribution and distributes rain
      \param[out] Rain event data structure to populate
    */
    bool loadRainDistribution(mhydasdk::core::CoreRepository *Data);



};





#endif // __IOMAN_H__




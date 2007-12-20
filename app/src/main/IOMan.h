/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __IOMAN_H__
#define __IOMAN_H__

#include "mhydasdk-base.h"
#include "mhydasdk-core.h"
#include "RuntimeEnv.h"


#include <wx/list.h>
//#include <wx/dynarray.h>
#include <vector>

// =====================================================================
// =====================================================================


/**
  Structure storing the config of a function, read from the xml model file
*/
struct FunctionConfig
{
  wxString FileID;  // Plug-in fileID (filename without ext)
  mhydasdk::core::ParamsMap Params;  // Function parameters set
};


WX_DECLARE_LIST(FunctionConfig, FunctionConfigsList);

/**
  Structure storing the full engine configuration, including global parameters
*/
struct EngineConfig
{
  int DeltaT;
  
  wxString SimulationID;

  FunctionConfigsList FuncConfigs;

};


// =====================================================================
// =====================================================================


/**
  Hash map type indexing rain files of the rain event
*/
WX_DECLARE_HASH_MAP(int, wxString,wxIntegerHash, wxIntegerEqual, RainEventFilesMap);


// =====================================================================
// =====================================================================


/**
  Structure storing definition for automatic results saving files
*/
struct AutoOutfileDef
{
  wxString ObjectsKind;
  vector<mhydasdk::core::HOID> SelectedObjectIDs;
  wxString FileSuffix;
  bool SaveAllVars;
  wxArrayString Scalars;
  wxArrayString Vectors;
};


struct AutoOutfiles
{
  wxString ColSeparator; // = wxT("\t");
  wxString DTFormat; // = wxT("%Y%m%dT%H%M%S");
  wxString CommentChar; // = wxT("%");

  vector<AutoOutfileDef*> Defs;
};

// =====================================================================
// =====================================================================



/**
  File inputs/outputs management class
*/
class IOManager
{
  private:
    mhydasdk::base::ExecutionMessages* mp_ExecMsgs;
    
    RuntimeEnvironment* mp_RunEnv;

    mhydasdk::core::SUDownstreamCode getSUDownstreamCode(wxString Code);

    AutoOutfiles m_AutoOutFiles;

    bool m_ClearedOuputDir;

    RainEventFilesMap buildRainEventFileMap();

    bool loadRainFile(mhydasdk::core::RainEvent *RainData, mhydasdk::core::cdsid_t ID, wxString Filename);

    bool prepareOutputDir();
    

    bool extractColumnOrderAndDataFromFile(wxString Filename, wxString SpecTag,
                                           wxArrayString* ColOrder, wxString* Data);


    bool saveResultsFromDef(mhydasdk::core::SpatialRepository *SpatialData,
                            wxString ColSeparator, wxString CommentChar,
                            AutoOutfileDef* Def, wxArrayString DTStrings,
                            ExtraSimInfos ExSI);




  public:
    /**
      Constructor
    */
    IOManager(mhydasdk::base::ExecutionMessages* ExecMsgs,RuntimeEnvironment* RunEnv);

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
      Loads output files configuration
    */
    bool loadOutputConfig();


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


    bool saveResults(mhydasdk::core::CoreRepository *Data, ExtraSimInfos ExSI);
    

    bool prepareTraceDir(mhydasdk::core::CoreRepository *Data);
    
    bool saveTrace(mhydasdk::core::CoreRepository *Data, int Step, mhydasdk::core::DateTime DT);

    bool saveSimulationInfos(mhydasdk::core::CoreRepository *CoreData, ExtraSimInfos ExSI, mhydasdk::base::SimulationInfo *SimInfo);    

    bool loadHydroObjectsProperties(mhydasdk::core::SpatialRepository *SpatialData);

    bool loadHydroObjectsInitialConditions(mhydasdk::core::SpatialRepository *SpatialData);

};





#endif // __IOMAN_H__




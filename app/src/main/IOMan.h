/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __IOMAN_H__
#define __IOMAN_H__

#include "openfluid-base.h"
#include "openfluid-core.h"
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
  openfluid::core::ParamsMap Params;  // Function parameters set
};


WX_DECLARE_LIST(FunctionConfig, FunctionConfigsList);

/**
  Structure storing the full engine configuration, including global parameters
*/
struct ModelConfig
{
  int DeltaT;

  wxString SimulationID;

  FunctionConfigsList FuncConfigs;

};


struct RunConfig
{
  int DeltaT;

  wxString SimulationID;

  wxDateTime BeginDate;

  wxDateTime EndDate;

};




// =====================================================================
// =====================================================================


/**
  Hash map type indexing rain files of the rain event
*/
WX_DECLARE_HASH_MAP(int, wxString,wxIntegerHash, wxIntegerEqual, RainSourcesFilesMap);


// =====================================================================
// =====================================================================


/**
  Structure storing definition for automatic results saving files
*/
struct AutoOutfileDef
{
  wxString ObjectsKind;
  std::vector<openfluid::core::UnitID> SelectedObjectIDs;
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

  std::vector<AutoOutfileDef*> Defs;
};

// =====================================================================
// =====================================================================



/**
  File inputs/outputs management class
*/
class IOManager
{
  private:
    openfluid::base::ExecutionMessages* mp_ExecMsgs;

    RuntimeEnvironment* mp_RunEnv;

    openfluid::core::SUDownstreamCode getSUDownstreamCode(wxString Code);

    AutoOutfiles m_AutoOutFiles;

    bool m_ClearedOuputDir;




    bool saveResultsFromDef(openfluid::core::SpatialRepository *SpatialData,
                            wxString ColSeparator, wxString CommentChar,
                            AutoOutfileDef* Def, wxArrayString DTStrings,
                            ExtraSimInfos ExSI);


    bool loadDistributedDataFile(wxString Filename, openfluid::core::SpatialRepository *SpatialData);

    bool loadDistributedEventsFile(wxString Filename, openfluid::core::SpatialRepository *SpatialData);



  public:
    /**
      Constructor
    */
    IOManager(openfluid::base::ExecutionMessages* ExecMsgs,RuntimeEnvironment* RunEnv);

    /**
      Destructor
    */
    ~IOManager();


    bool loadRunConfig(RunConfig* Config);

    /**
      Loads model engine configuration
      \param[out] Configuration definition to populate
    */
    bool loadModelConfig(ModelConfig* Config);


    /**
      Loads output files configuration
    */
    bool loadOutputConfig();


    /**
      Loads spatial hydro objects definition and topology
      \param[out] Spatial data structure to populate
    */
    bool loadHydroObjects(openfluid::core::SpatialRepository *SpatialData);


    /**
      Loads rain distribution and distributes rain
      \param[out] Rain event data structure to populate
    */
    bool loadRainDistribution(openfluid::core::CoreRepository *Data);

    bool loadDistributedData(openfluid::core::SpatialRepository *SpatialData);

    bool loadDistributedEvents(openfluid::core::SpatialRepository *SpatialData);

    bool prepareOutputDir();
    //bool saveResults(openfluid::core::CoreRepository *Data, ExtraSimInfos ExSI);
    bool saveResults(openfluid::core::CoreRepository *Data, RunConfig Config, int TimeStepsCount, ExtraSimInfos ExSI);


    bool prepareTraceDir(openfluid::core::CoreRepository *Data);

    bool saveTrace(openfluid::core::CoreRepository *Data, int Step, wxDateTime DT);

    bool saveSimulationInfos(openfluid::core::CoreRepository *CoreData, ExtraSimInfos ExSI, openfluid::base::SimulationInfo *SimInfo);

};





#endif // __IOMAN_H__




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
//#include <wx/dynarray.h>
#include <vector>

// =====================================================================
// =====================================================================


/**
  Structure storing the config of a function, read from the xml model file
*/
struct FunctionConfig
{
  wxString Name;  // Name of the function
  wxString File;  // Plug-in file
  mhydasdk::core::ParamsMap Params;  // Function parameters set

};


WX_DECLARE_LIST(FunctionConfig, FunctionConfigsList);

/**
  Structure storing the full engine configuration, including global parameters
*/
struct EngineConfig
{
  int DeltaT;

  FunctionConfigsList HydroModuleConfig;

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
  vector<mhydasdk::core::hoid_t> SelectedObjectIDs;
  wxString FileSuffix;
  wxArrayString Columns;

};

//WX_DEFINE_ARRAY(AutoOutfileDef*, ArrayOfAutoOutfileDefs);

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
    mhydasdk::base::RuntimeEnvironment* mp_RunEnv;

    mhydasdk::core::SUFlowCode getSUFlowCode(wxString Code);

    AutoOutfiles m_AutoOutFiles;


    RainEventFilesMap buildRainEventFileMap();

    bool loadRainFile(mhydasdk::core::RainEvent *RainData, mhydasdk::core::cdsid_t ID, wxString Filename);

    bool prepareOutputDir();

    bool extractColumnOrderAndDataFromFile(wxString Filename, wxString SpecTag,
                                           wxArrayString* ColOrder, wxString* Data);


    bool saveResultsFromDef(mhydasdk::core::SpatialRepository *SpatialData,
                            wxString ColSeparator, wxString CommentChar,
                            AutoOutfileDef* Def, wxArrayString DTStrings);


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


    bool saveResults(mhydasdk::core::CoreRepository *Data);


    bool loadHydroObjectsProperties(mhydasdk::core::SpatialRepository *SpatialData);

    bool loadHydroObjectsInitialConditions(mhydasdk::core::SpatialRepository *SpatialData);

};





#endif // __IOMAN_H__




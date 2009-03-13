/**
  \file IOMan.h
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __IOMAN_H__
#define __IOMAN_H__

#include "openfluid-base.h"
#include "openfluid-core.h"
#include "openfluid-tools.h"
#include "RuntimeEnv.h"
#include <wx/arrstr.h>


// =====================================================================
// =====================================================================

/**
  Structure storing the config of a function, read from the xml model file
*/
struct FunctionConfig
{
  std::string FileID;  // Plug-in fileID (filename without ext)
  openfluid::core::FuncParamsMap_t Params;  // Function parameters set
};


struct ModelConfig
{
  int DeltaT;

  std::string SimulationID;

  std::list<FunctionConfig> FuncConfigs;

};


struct RunConfig
{
  int DeltaT;

  std::string SimulationID;

  openfluid::core::DateTime BeginDate;

  openfluid::core::DateTime EndDate;

};

// =====================================================================
// =====================================================================


class IOManager
{

  private:

    struct  OutputSet_t
    {
      std::string Name;
      std::string UnitsClass;
      bool AllUnits;
      std::vector<openfluid::core::Unit*>  Units;
      bool AllScalars;
      std::vector<std::string>  ScalarVariables;
      bool AllVectors;
      std::vector<std::string>  VectorVariables;

      OutputSet_t()
      {
        Name = "";
        UnitsClass = "";
        AllUnits = false;
        Units.clear();
        AllScalars = false;
        ScalarVariables.clear();
        AllVectors = false;
        VectorVariables.clear();
      }

    };


    struct FileOutput_t
    {
      std::vector<OutputSet_t> Sets;
      std::string ColSeparator;
      std::string DateFormat;
      std::string CommentChar;

      FileOutput_t()
      {
        Sets.clear();
        ColSeparator = "\t";
        DateFormat = "%Y%m%dT%H%M%S";
        CommentChar = "%";
      }

    };


    struct OutputConfig_t
    {
      std::vector<FileOutput_t> FileSets;

      OutputConfig_t()
      {
        FileSets.clear();
      }
    };



    static IOManager* mp_Singleton;

    openfluid::core::MemoryMonitor* mp_MemMonitor;
    openfluid::core::CoreRepository* mp_Repository;
    openfluid::base::ExecutionMessages* mp_ExecMsgs;
    RuntimeEnvironment* mp_RunEnv;

    OutputConfig_t m_OutputConfig;

    bool m_ClearedOuputDir;


    IOManager() { };

    bool prepareUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex,
                               std::string OutputDir);

    bool saveUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex,
                            openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                            std::string OutputDir);

    bool loadDomainFile(std::string FullFilename,
                        std::list<openfluid::core::UnitsLink_t>* ToUnitsList);

    bool loadInputDataFile(std::string FullFilename);


    bool loadEventsFile(std::string FullFilename);


    std::string generateOuputFilename(const std::string UnitClass, const openfluid::core::UnitID_t UnitID,
                                   const std::string Suffix, const std::string VectorName = "");

    std::string generateOutputScalarsFileHeader(const std::string SimulationID, const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
                                             const std::string Filename, const std::vector<std::string> ScalarsNames,
                                             const std::string CommentChar);

    std::string generateOutputVectorFileHeader(const std::string SimulationID, const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
                                            const std::string Filename, const std::string VectorName,
                                            const std::string CommentChar);


    std::string generateOutputScalarsFileContent(const openfluid::core::Unit* aUnit,
                                              const std::vector<std::string> ScalarsNames,
                                              openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                              std::string ColSeparator);

    std::string generateOutputVectorFileContent(const openfluid::core::Unit* aUnit, const std::string VectorName,
                                              openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                              std::string ColSeparator);


  public:

    static IOManager* getInstance();

    void setMemoryMonitor(openfluid::core::MemoryMonitor* MemMonitor) { mp_MemMonitor = MemMonitor; };

    void setCoreRepository(openfluid::core::CoreRepository* Repository) { mp_Repository = Repository; };

    void setExecutionMessages(openfluid::base::ExecutionMessages* ExecMsgs) { mp_ExecMsgs = ExecMsgs; };

    void setRunEnvironment(RuntimeEnvironment* RunEnv) { mp_RunEnv = RunEnv; };

    bool loadRunConfig(RunConfig* Config);

    /**
      Loads model engine configuration
      \param[out] Configuration definition to populate
    */
    bool loadModelConfig(ModelConfig* Config);

    bool loadDomainFromFiles();

    bool loadInputDataFromFiles();

    bool loadEventsFromFiles();


    bool loadOutputConfig();

    bool prepareOutputs();

    bool saveOutputs(openfluid::core::TimeStep_t CurrentStep, bool WithoutKeep);

    bool saveSimulationInfos(ExtraSimInfos ExSI, openfluid::base::SimulationInfo *SimInfo, std::string ErrorMsg);

    bool prepareOutputDir();



};


#endif /* __IOMAN_H__ */

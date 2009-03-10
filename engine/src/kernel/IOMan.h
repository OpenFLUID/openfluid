
#ifndef __IOMAN_H__
#define __IOMAN_H__


#include "CoreRepository.h"
#include "MemMonitor.h"
#include <wx/arrstr.h>

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

    OutputConfig_t m_OutputConfig;

    IOManager() { };

    bool prepareUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex,
                               wxString OutputDir);

    bool saveUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex,
                            openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                            wxString OutputDir);

    bool loadDomainFile(wxString FullFilename,
                        std::list<openfluid::core::UnitsLink_t>* ToUnitsList);

    bool loadInputDataFile(wxString FullFilename);

    wxString generateOuputFilename(const std::string UnitClass, const openfluid::core::UnitID_t UnitID,
                                   const std::string Suffix, const std::string VectorName = "");

    wxString generateOutputScalarsFileHeader(const std::string SimulationID, const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
                                             const wxString Filename, const std::vector<std::string> ScalarsNames,
                                             const std::string CommentChar);

    wxString generateOutputVectorFileHeader(const std::string SimulationID, const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
                                            const wxString Filename, const std::string VectorName,
                                            const std::string CommentChar);


    wxString generateOutputScalarsFileContent(const openfluid::core::Unit* aUnit,
                                              const std::vector<std::string> ScalarsNames,
                                              openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                              std::string ColSeparator);

    wxString generateOutputVectorFileContent(const openfluid::core::Unit* aUnit, const std::string VectorName,
                                              openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                              std::string ColSeparator);

  public:

    static IOManager* getInstance();

    void setMemoryMonitor(openfluid::core::MemoryMonitor* MemMonitor) { mp_MemMonitor = MemMonitor; };

    void setCoreRepository(openfluid::core::CoreRepository* Repository) { mp_Repository = Repository; };

    bool loadDomainFromFiles();

    bool loadInputDataFromFiles();

    bool loadOutputConfig();

    bool prepareOutputs();

    bool saveOutputs(openfluid::core::TimeStep_t CurrentStep, bool WithoutKeep);





};


#endif /* __IOMAN_H__ */

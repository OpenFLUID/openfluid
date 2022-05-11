/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/

/**
  @file Binding.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UTILS_BINDING_HPP__
#define __OPENFLUID_UTILS_BINDING_HPP__


#include <clocale>

#include <openfluid/config.hpp>
#include <openfluid/base/Init.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/utils/BindingVerboseMachineListener.hpp>
#include <openfluid/utils/BindingAbstractOutErr.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/ObserverRegistry.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/fluidx/SpatialDomainDescriptor.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/fluidx/MonitoringDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/ware/PluggableWare.hpp>


// =====================================================================
// =====================================================================


#define STRING_TO_ALLOCATED_CARRAY(str,carray) \
    char* carray = (char*)malloc((str.length()+1)*sizeof(char)); \
    strcpy(carray,str.c_str());


/**
  Macro to use for definition and initialization of the static part of the openfluid::utils::Binding class.
  It must be called in the C or C++ code of the binding wrapper.
  @param[in] erroutclass The class definition for the standard output and errors,
             derived from the openfluid::utils::BindingAbstractOutErr class.
*/
#define OPENFLUID_BINDING_DEFINE(erroutclass)\
  bool openfluid::utils::Binding::m_Initialized = false; \
  std::string openfluid::utils::Binding::m_LastErrorMsg = ""; \
  const openfluid::utils::BindingAbstractOutErr* openfluid::utils::Binding::mp_OutErr = new erroutclass();


// =====================================================================
// =====================================================================


// OpenFLUID:stylecheck:!incs


namespace openfluid { namespace utils {

/**
  @brief Class for easier binding with other programming languages.

  Class for easier binding of OpenFLUID with other programming languages, such as Python, R, JavaScript, ...
  This class provides common methods to open datasets, add wares paths, modify parameters and attributes,
  run simulations and many more. All of these methods have parameters and return values in plain old types
  for better interoperability with other languages, excepted for internal data usually handled as "external pointers"
  in bindings.

  To use this header-only class
  @li the header <openfluid/utils/Binding.hpp> must be included in the C or C++  wrapper code
      specifically developped for the binded language,
  @li a class derived from the openfluid::utils::BindingAbstractOutErr class must be defined in order to process
      the standard and error console streams,
  @li the #OPENFLUID_BINDING_DEFINE macro must be called in the C or C++  wrapper code,
      with the previous derived class as a parameter.

  <i>Example</i>
  @snippet misc/bindings.cpp bindings
*/
class Binding
{
  private:

    static bool m_Initialized;

    static std::string m_LastErrorMsg;

    static const BindingAbstractOutErr* mp_OutErr;

    openfluid::fluidx::FluidXDescriptor m_FluidXDesc;

    bool m_IsProject = false;
    bool m_IsDataset = false;

    std::string m_SourcePath = "";
    std::string m_OutputDir = "";

    bool m_IsSimulationRun = false;


    // =====================================================================
    // =====================================================================


    Binding() 
    { }


    // =====================================================================
    // =====================================================================


    ~Binding()
    { }


    // =====================================================================
    // =====================================================================


    static void unloadAllWares()
    {
      openfluid::machine::SimulatorRegistry::instance()->clear();
      openfluid::machine::ObserverRegistry::instance()->clear();
      openfluid::machine::SimulatorPluginsManager::instance()->unloadAll();
      openfluid::machine::ObserverPluginsManager::instance()->unloadAll();
    }


    // =====================================================================
    // =====================================================================


    std::map<openfluid::core::UnitsClass_t,unsigned int> getUnitsCountByClasses()
    {
      std::map<openfluid::core::UnitsClass_t,unsigned int> RetMap;

      for (const auto& UnitsByClass : m_FluidXDesc.spatialDomain().spatialUnits())
      {
        RetMap[UnitsByClass.first] = UnitsByClass.second.size();
      }

      return RetMap;
    }


    // =====================================================================
    // =====================================================================


  public:

    /**
      Static initialization of the binding. Muste be called at least once.
    */
    static void init()
    {
      if (m_Initialized)
      {
        return;
      }

      INIT_OPENFLUID_APPLICATION();

      // reset locale for "LC_NUMERIC" To "C"
      // to prevent changing locale on init
      std::setlocale(LC_NUMERIC,"C");

      m_Initialized = true;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a new instance
      @return The newly created instance
    */
    static Binding* make()
    {
      return (new Binding());
    }


    // =====================================================================
    // =====================================================================


    /**
      Deletes a given instance
      @param[in] B The instance to delete
    */
    static void destroy(Binding* B)
    {
      delete B;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the OpenFLUID version
    */
    static const char* getVersion()
    {
      STRING_TO_ALLOCATED_CARRAY(openfluid::config::VERSION_FULL,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the last error message
    */
    static const char* getLastError()
    {
      return m_LastErrorMsg.c_str();
    }


    // =====================================================================
    // =====================================================================


    /**
      Add supplementary paths to search for simulators when running simulations
      @param[in] Paths The paths to add using the path separator used on the current system
      @see openfluid::base::Environment::addExtraSimulatorsDirs
    */
    static void addExtraSimulatorsPaths(const char* Paths)
    {
      openfluid::base::Environment::addExtraSimulatorsDirs(std::string(Paths));
    }


    // =====================================================================
    // =====================================================================


    /**
      Resets the list of supplementary paths to search for simulators
    */
    static void resetExtraSimulatorsPaths()
    {
      openfluid::base::Environment::resetExtraSimulatorsDirs();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the number of default paths to search for simulators
      @return the number of paths
    */
    static unsigned int getSimulatorsPathsCount()
    {
      return openfluid::base::Environment::getSimulatorsDirs().size();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the default paths to search for simulators
      @return the paths list
    */
    static char** getSimulatorsPaths()
    {
      std::vector<std::string> SimsPaths = openfluid::base::Environment::getSimulatorsDirs();

      const unsigned int Count = SimsPaths.size();

      char** Paths = (char**)malloc(Count*sizeof(char*));

      for (unsigned int i=0;i<Count;i++)
      {
        Paths[i] = (char*)malloc(SimsPaths[i].size()+1);
        std::copy(SimsPaths[i].begin(), SimsPaths[i].end(), Paths[i]);
        Paths[i][SimsPaths[i].size()] = '\0';
      }

      return Paths;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the number of supplementary paths to search for simulators
      @return the number of paths
    */
    static unsigned int getExtraSimulatorsPathsCount()
    {
      return openfluid::base::Environment::getExtraSimulatorsDirs().size();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the supplementary paths to search for simulators
      @return the paths list
    */
    static char** getExtraSimulatorsPaths()
    {
      std::vector<std::string> ExtraSimsPaths = openfluid::base::Environment::getExtraSimulatorsDirs();

      const unsigned int Count = ExtraSimsPaths.size();

      char** Paths = (char**)malloc(Count*sizeof(char*));

      for (unsigned int i=0;i<Count;i++)
      {
        Paths[i] = (char*)malloc(ExtraSimsPaths[i].size()+1);
        std::copy(ExtraSimsPaths[i].begin(), ExtraSimsPaths[i].end(), Paths[i]);
        Paths[i][ExtraSimsPaths[i].size()] = '\0';
      }

      return Paths;
    }


    // =====================================================================
    // =====================================================================


    /**
      Add supplementary paths to search for observers when running simulations
      @param[in] Paths the paths to add using the path separator used on the current system
      @see openfluid::base::Environment::addExtraObserversDirs
    */
    static void addExtraObserversPaths(const char* Paths)
    {
      openfluid::base::Environment::addExtraObserversDirs(std::string(Paths));
    }


    // =====================================================================
    // =====================================================================


    /**
      Resets the list of supplementary paths to search for observers
    */
    static void resetExtraObserversPaths()
    {
      openfluid::base::Environment::resetExtraObserversDirs();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the number of default paths to search for observers
      @return The number of paths
    */
    static unsigned int getObserversPathsCount()
    {
      return openfluid::base::Environment::getObserversDirs().size();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the default paths to search for observers
      @return The paths list
    */
    static char** getObserversPaths()
    {
      std::vector<std::string> ObsPaths = openfluid::base::Environment::getObserversDirs();

      const unsigned int Count = ObsPaths.size();

      char** Paths = (char**)malloc(Count*sizeof(char*));

      for (unsigned int i=0;i<Count;i++)
      {
        Paths[i] = (char*)malloc(ObsPaths[i].size()+1);
        std::copy(ObsPaths[i].begin(), ObsPaths[i].end(), Paths[i]);
        Paths[i][ObsPaths[i].size()] = '\0';
      }

      return Paths;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the number of supplementary paths to search for observers
      @return The number of paths
    */
    static unsigned int getExtraObserversPathsCount()
    {
      return openfluid::base::Environment::getExtraObserversDirs().size();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the supplementary paths to search for observers
      @return The paths list
    */
    static char** getExtraObserversPaths()
    {
      std::vector<std::string> ExtraObsPaths = openfluid::base::Environment::getExtraObserversDirs();

      const unsigned int Count = ExtraObsPaths.size();

      char** Paths = (char**)malloc(Count*sizeof(char*));

      for (unsigned int i=0;i<Count;i++)
      {
        Paths[i] = (char*)malloc(ExtraObsPaths[i].size()+1);
        std::copy(ExtraObsPaths[i].begin(), ExtraObsPaths[i].end(), Paths[i]);
        Paths[i][ExtraObsPaths[i].size()] = '\0';
      }

      return Paths;
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the current output path used by default
      @param[in] Path The output path to use
    */
    static void setCurrentOutputDir(const char* Path)
    {
      openfluid::base::RunContextManager::instance()->setOutputDir(std::string(Path));
    }


    // =====================================================================
    // =====================================================================


    /**
      Opens an OpenFLUID dataset given by its path
      @param[in] Path The path to the dataset
      @returns an instance of the Binding for the opened dataset, NULL if an error occured
    */
    static Binding* openDataset(const char* Path)
    {
      Binding* Data = new Binding();

      try
      {
        init();

        openfluid::base::IOListener FluidXListener;

        openfluid::base::RunContextManager::instance()->setInputDir(std::string(Path));
        Data->m_FluidXDesc = openfluid::fluidx::FluidXIO(&FluidXListener)
                               .loadFromDirectory(openfluid::base::RunContextManager::instance()->getInputDir());

        Data->m_IsSimulationRun = false;
        Data->m_IsDataset = true;
        Data->m_SourcePath = openfluid::base::RunContextManager::instance()->getInputDir();


        return Data;
      }
      catch (openfluid::base::Exception& E)
      {
        m_LastErrorMsg = "OpenFLUID ERROR: " + std::string(E.what()) +"\n";
      }
      catch (std::bad_alloc& E)
      {
        m_LastErrorMsg = "MEMORY ALLOCATION ERROR: " + std::string(E.what()) +
                         ". Possibly not enough memory available\n";
      }
      catch (std::exception& E)
      {
        m_LastErrorMsg = "SYSTEM ERROR: " + std::string(E.what()) +"\n";
      }
      catch (...)
      {
        m_LastErrorMsg = "UNKNOWN ERROR\n";
      }

      Data->m_IsProject = false;
      Data->m_IsDataset = false;
      Data->m_SourcePath = "";

      delete Data;

      return nullptr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Opens an OpenFLUID project given by its path
      @param[in] Path The path to the project
      @returns an instance of the Binding for the opened project, NULL if an error occured
    */
    static Binding* openProject(const char* Path)
    {
      Binding* Data = new Binding();

      try
      {
        init();

        openfluid::base::IOListener FluidXListener;
        
        if (!openfluid::base::RunContextManager::instance()->openProject(std::string(Path)))
        {
          throw openfluid::base::ApplicationException(
              openfluid::base::ApplicationException::computeContext("ROpenFLUID"),
              std::string(Path) + " is not a correct project path");
        }

        Data->m_FluidXDesc = openfluid::fluidx::FluidXIO(&FluidXListener)
                               .loadFromDirectory(openfluid::base::RunContextManager::instance()->getInputDir());

        Data->m_IsSimulationRun = false;
        Data->m_IsProject = true;
        Data->m_SourcePath = openfluid::base::RunContextManager::instance()->getProjectPath();

        return Data;
        
      }
      catch (openfluid::base::Exception& E)
      {
        m_LastErrorMsg = "OpenFLUID ERROR: " + std::string(E.what()) +"\n";
      }
      catch (std::bad_alloc& E)
      {
        m_LastErrorMsg = "MEMORY ALLOCATION ERROR: " + std::string(E.what()) +
                         ". Possibly not enough memory available\n";
      }
      catch (std::exception& E)
      {
        m_LastErrorMsg = "SYSTEM ERROR: " + std::string(E.what()) +"\n";
      }
      catch (...)
      {
        m_LastErrorMsg = "UNKNOWN ERROR\n";
      }

      delete Data;

      return nullptr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Writes an OpenFLUID dataset to disk, optionaly with copy of external data from the original dataset
      @param[in] Path The path to the dataset to write
      @param[in] WithOriginData optional parameter to enable the copy of external data from the original dataset. 
      Default is false 
      @returns 1 if succeded, 0 if failed
    */
    int writeDataset(const char* Path, bool WithOriginData = false)
    {
      try
      {
        if (WithOriginData && !m_SourcePath.empty())
        {
          openfluid::tools::Filesystem::copyDirectory(m_SourcePath, std::string(Path),false,true);

          auto FluidXToRemove = openfluid::tools::Filesystem::findFilesByExtension(std::string(Path),"fluidx",true);

          for (const auto& F : FluidXToRemove)
          {
            openfluid::tools::FilesystemPath(F).removeFile();
          }
        }

        openfluid::base::IOListener FluidXListener;

        openfluid::fluidx::FluidXIO(&FluidXListener).writeToManyFiles(m_FluidXDesc,std::string(Path));

        return 1;
      }
      catch (openfluid::base::Exception& E)
      {
        m_LastErrorMsg = "OpenFLUID ERROR: " + std::string(E.what()) +"\n";
      }
      catch (std::bad_alloc& E)
      {
        m_LastErrorMsg = "MEMORY ALLOCATION ERROR: " + std::string(E.what()) +
                         ". Possibly not enough memory available\n";
      }
      catch (std::exception& E)
      {
        m_LastErrorMsg = "SYSTEM ERROR: " + std::string(E.what()) +"\n";
      }
      catch (...)
      {
        m_LastErrorMsg = "UNKNOWN ERROR\n";
      }

      return 0;
    }


    // =====================================================================
    // =====================================================================


    /**
      Runs a simulation based on the current configuration for simulation.
      @param[in] IsVerbose Set to true to enable verbosity (false by default).
    */
    unsigned short int runSimulation(int IsVerbose = false)
    {
      unsigned short int RetValue = 0;

      try
      {
        init();
        unloadAllWares();


        openfluid::machine::Engine* Engine;
        openfluid::machine::SimulationBlob SimBlob;
        std::unique_ptr<openfluid::machine::MachineListener> Listener;


        if (IsVerbose)
        {
          Listener = std::make_unique<BindingVerboseMachineListener>(mp_OutErr);
        }
        else
        {
          Listener = std::make_unique<openfluid::machine::MachineListener>();
        }


        // ===== spatial domain and run config

        if (IsVerbose)
        {
          mp_OutErr->printfOut("%s","Building spatial domain...");
        }

        openfluid::machine::Factory::buildSimulationBlobFromDescriptors(m_FluidXDesc,SimBlob);

        if (IsVerbose)
        {
          static_cast<BindingVerboseMachineListener*>(Listener.get())->
              displayStatus(openfluid::base::Listener::Status::OK_STATUS);
        }


        // ===== model instance

        if (IsVerbose)
        {
          mp_OutErr->printfOut("%s","Building model instance...");
        }

        openfluid::machine::ModelInstance Model(SimBlob,Listener.get());

        openfluid::machine::Factory::buildModelInstanceFromDescriptor(m_FluidXDesc.model(),Model);

        if (IsVerbose)
        {
          static_cast<BindingVerboseMachineListener*>(Listener.get())->
              displayStatus(openfluid::base::Listener::Status::OK_STATUS);
        }


        // ===== monitoring instance

        if (IsVerbose)
        {
          mp_OutErr->printfOut("%s","Building monitoring instance...");
        }

        openfluid::machine::MonitoringInstance Monitoring(SimBlob);

        openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(m_FluidXDesc.monitoring(),
                                                                           Monitoring);

        if (IsVerbose)
        {
          static_cast<BindingVerboseMachineListener*>(Listener.get())->
                        displayStatus(openfluid::base::Listener::Status::OK_STATUS);
        }


        // ===== simulation

        m_OutputDir = openfluid::base::RunContextManager::instance()->getOutputDir();

        Engine = new openfluid::machine::Engine(SimBlob, Model, Monitoring, Listener.get());

        Engine->initialize();

        Engine->initParams();
        Engine->prepareData();
        Engine->checkConsistency();
        Engine->run();

        Engine->finalize();

        delete Engine;

        m_IsSimulationRun = true;

        RetValue = 1;
      }
      catch (openfluid::base::Exception& E)
      {
        m_LastErrorMsg = "OpenFLUID ERROR: " + std::string(E.what()) +"\n";
      }
      catch (std::bad_alloc& E)
      {
        m_LastErrorMsg = "MEMORY ALLOCATION ERROR: " + std::string(E.what()) +
                         ". Possibly not enough memory available\n";
      }
      catch (std::exception& E)
      {
        m_LastErrorMsg = "SYSTEM ERROR: " + std::string(E.what()) +"\n";
      }
      catch (...)
      {
        m_LastErrorMsg = "UNKNOWN ERROR\n";
      }

      unloadAllWares();

      return RetValue;
    }


    // =====================================================================
    // =====================================================================


    /**
      Prints informations about simulation configuration (spatial domain, processes model, ...)
    */
    void printSimulationInfo()
    {
      // Spatial domain

      mp_OutErr->printfOut("Spatial domain is made of %i spatial units\n",
                          m_FluidXDesc.spatialDomain().getUnitsCount());

      for (const auto& UnitsClass : m_FluidXDesc.spatialDomain().spatialUnits())
      {
        mp_OutErr->printfOut(" - %i units of class %s\n",UnitsClass.second.size(),UnitsClass.first.c_str());
      }


      // Model

      mp_OutErr->printfOut("Model is made of %i simulation items\n",m_FluidXDesc.model().items().size());

      for (const auto& ModelInfos : m_FluidXDesc.model().items())
      {
        mp_OutErr->printfOut(" - ");

        if (ModelInfos->isType(openfluid::ware::WareType::SIMULATOR))
        {
          mp_OutErr->printfOut("%s simulator\n",
                               ((openfluid::fluidx::SimulatorDescriptor*)(ModelInfos))->getID().c_str());
        }

        if (ModelInfos->isType(openfluid::ware::WareType::GENERATOR))
        {
          openfluid::fluidx::GeneratorDescriptor* pGenDesc = ((openfluid::fluidx::GeneratorDescriptor*)ModelInfos);

          if (pGenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED)
          {
            mp_OutErr->printfOut("fixed");
          }

          if (pGenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM)
          {
            mp_OutErr->printfOut("random");
          }

          if (pGenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP)
          {
            mp_OutErr->printfOut("interp");
          }

          if (pGenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT)
          {
            mp_OutErr->printfOut("inject");
          }

          mp_OutErr->printfOut(" generator for variable %s on units %s\n",
                              pGenDesc->getVariableName().c_str(),pGenDesc->getUnitsClass().c_str());
        }
      }

      // Time period

      mp_OutErr->printfOut("Simulation period from %s to %s\n",
                          m_FluidXDesc.runConfiguration().getBeginDate().getAsISOString().c_str(),
                          m_FluidXDesc.runConfiguration().getEndDate().getAsISOString().c_str());

      // Time step

      mp_OutErr->printfOut("Simulation time step : %i\n",m_FluidXDesc.runConfiguration().getDeltaT());

    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the simulation output directory
      @return The path to the output directory
    */
    const char* getSimulationOutputDir()
    {
      STRING_TO_ALLOCATED_CARRAY(m_OutputDir,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the default delta T for the current simulation
      @return The delta T
    */
    int getDefaultDeltaT()
    {
      return m_FluidXDesc.runConfiguration().getDeltaT();
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the default delta T for the current simulation
      @param[in] DeltaT The delta T to set
    */
    void setDefaultDeltaT(int DeltaT)
    {
      m_FluidXDesc.runConfiguration().setDeltaT(DeltaT);
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the begin date for the simulation period,
      as an ISO date-time string (with a space instead of a T letter, e.g. : 2008-01-02 11:13:00)
      @return The begin date as a string
    */
    const char* getPeriodBeginDate()
    {
      std::string DateStr = m_FluidXDesc.runConfiguration().getBeginDate().getAsString("%Y-%m-%d %H:%M:%S");
      STRING_TO_ALLOCATED_CARRAY(DateStr,CStr);

      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the end date for the simulation period,
      as an ISO date-time string (with a space instead of a T letter, e.g. : 2011-08-06 15:00:00)
      @return The end date as a string
    */
    const char* getPeriodEndDate()
    {
      std::string DateStr = m_FluidXDesc.runConfiguration().getEndDate().getAsString("%Y-%m-%d %H:%M:%S");
      STRING_TO_ALLOCATED_CARRAY(DateStr,CStr);

      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the simulation period with begin and end dates. The dates are given as ISO date-time strings
      (with a space instead of a T letter, e.g. : 2008-01-02 11:13:00, 2011-08-06 15:00:00)
      @param[in] BeginDate The begin date as a string
      @param[in] EndDate The end date as a string
    */
    void setPeriod(const char* BeginDate, const char* EndDate)
    {
      std::string StrBeginDate(BeginDate);
      std::string StrEndDate(EndDate);
      openfluid::core::DateTime DateToSet;

      if (!StrBeginDate.empty() && DateToSet.setFromISOString(StrBeginDate))
      {
        m_FluidXDesc.runConfiguration().setBeginDate(DateToSet);
      }

      if (!StrEndDate.empty() && DateToSet.setFromISOString(StrEndDate))
      {
        m_FluidXDesc.runConfiguration().setEndDate(DateToSet);
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the IDs list of the simulators used in the current model, as a semicolon separated list.
      @return The list of IDs
    */
    const char* getSimulatorsIDs()
    {
      std::string SimIDsStr("");
      std::ostringstream ssSimIDs;
      std::string sep = "";
      openfluid::fluidx::CoupledModelDescriptor ModDesc(m_FluidXDesc.model());

      for (const auto& Item : ModDesc.items())
      {
        if (Item->isType(openfluid::ware::WareType::SIMULATOR))
        {
          ssSimIDs << sep << ModDesc.getID(Item);
          sep = ";";
        }
      }

      SimIDsStr = ssSimIDs.str();
      STRING_TO_ALLOCATED_CARRAY(SimIDsStr,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the list of the parameters names for a given simulator, as a semicolon separated list.
      @param[in] SimID The ID of the simulator
      @return The list of parameters names
    */
    const char* getSimulatorParamNames(const char* SimID)
    {
      std::string SimIDStr(SimID);
      std::string ParamNamesStr("");
      std::ostringstream ssParamNames;
      std::string sep = "";
      openfluid::fluidx::CoupledModelDescriptor ModDesc(m_FluidXDesc.model());
      int ItemIndex = ModDesc.findFirstItem(SimIDStr);

      if (ItemIndex < 0)
      {
        return "";
      }

      openfluid::fluidx::ModelItemDescriptor Item = ModDesc.itemAt(ItemIndex);
      if (!Item.isType(openfluid::ware::WareType::SIMULATOR))
      {
        return "";
      }

      for (const auto& Param : Item.parameters())
      {
        ssParamNames << sep << Param.first;
        sep = ";";
      }

      ParamNamesStr = ssParamNames.str();
      STRING_TO_ALLOCATED_CARRAY(ParamNamesStr,CStr);
      return CStr;

    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the value of a given simulator parameter, an empty string if not found.
      @param[in] SimID The ID of the simulator
      @param[in] ParamName The name of the parameter
      @return The value of the parameter, as a string
    */
    const char* getSimulatorParam(const char* SimID, const char* ParamName)
    {
      std::string ParamValStr("");
      std::string SimIDStr(SimID);
      std::string ParamNameStr(ParamName);

      for (const auto& ModelInfos : m_FluidXDesc.model().items())
      {
        if (ModelInfos->isType(openfluid::ware::WareType::SIMULATOR) &&
            ((openfluid::fluidx::SimulatorDescriptor*)ModelInfos)->getID() == SimIDStr)
        {
          openfluid::ware::WareParams_t Params = ModelInfos->getParameters();
          openfluid::ware::WareParams_t::iterator ItParam = Params.find(ParamNameStr);

          if (ItParam != Params.end())
          {
            ParamValStr = (*ItParam).second;
            STRING_TO_ALLOCATED_CARRAY(ParamValStr,CStr);
            return CStr;
          }
        }
      }

      STRING_TO_ALLOCATED_CARRAY(ParamValStr,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the value of a given simulator parameter.
      @param[in] SimID The ID of the simulator
      @param[in] ParamName The name of the parameter
      @param[in] ParamVal The new value for the parameter
    */
    void setSimulatorParam(const char* SimID, const char* ParamName, const char* ParamVal)
    {
      std::string SimIDStr(SimID);
      std::string ParamNameStr(ParamName);
      std::string ParamValStr(ParamVal);

      for (auto& ModelInfos : m_FluidXDesc.model().items())
      {
        if (ModelInfos->isType(openfluid::ware::WareType::SIMULATOR) &&
            ((openfluid::fluidx::SimulatorDescriptor*)ModelInfos)->getID() == SimIDStr)
        {
          ModelInfos->setParameter(ParamNameStr,ParamValStr);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Removes a given simulator parameter.
      @param[in] SimID The ID of the simulator
      @param[in] ParamName The name of the parameter
    */
    void removeSimulatorParam(const char* SimID, const char* ParamName)
    {
      std::string SimIDStr(SimID);
      std::string ParamNameStr(ParamName);

      for (auto& ModelInfos : m_FluidXDesc.model().items())
      {
        if (ModelInfos->isType(openfluid::ware::WareType::SIMULATOR) &&
            ((openfluid::fluidx::SimulatorDescriptor*)ModelInfos)->getID() == SimIDStr)
        {
          ModelInfos->eraseParameter(ParamNameStr);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the list of the parameters names for a given generator, as a semicolon separated list.
      @param[in] UnitsClass The name of the spatial units class
      @param[in] VarName The name of the generated variable
      @return The list of parameters names
    */
    const char* getGeneratorParamNames(const char* UnitsClass, const char* VarName)
    {
      std::string VarNameStr(VarName);
      std::string UnitsClassStr(UnitsClass);
      std::string ParamNamesStr("");
      std::ostringstream ssParamNames;
      std::string sep = "";

      for (const auto& Item : m_FluidXDesc.model().items())
      {
        if ( (Item->isType(openfluid::ware::WareType::GENERATOR))
          && (((openfluid::fluidx::GeneratorDescriptor*)Item)->getVariableName() == VarNameStr)
          && (((openfluid::fluidx::GeneratorDescriptor*)Item)->getUnitsClass() == UnitsClassStr) )
        {
          for (const auto& Param : Item->parameters())
          {
            ssParamNames << sep << Param.first;
            sep = ";";
          }
          break;
        }
      }

      ParamNamesStr = ssParamNames.str();
      STRING_TO_ALLOCATED_CARRAY(ParamNamesStr,CStr);
      return CStr;

    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the value of a given generator parameter, an empty string if not found.
      @param[in] UnitsClass The name of the spatial units class
      @param[in] VarName The name of the generated variable
      @param[in] ParamName The name of the parameter
      @return The value of the parameter, as a string
    */
    const char* getGeneratorParam(const char* UnitsClass, const char* VarName, const char* ParamName)
    {
      std::string UnitsClassStr(UnitsClass);
      std::string VarNameStr(VarName);
      std::string ParamNameStr(ParamName);
      std::string ParamValStr("");

      for (const auto& ModelInfos : m_FluidXDesc.model().items())
      {
        if (ModelInfos->isType(openfluid::ware::WareType::GENERATOR) &&
            ((openfluid::fluidx::GeneratorDescriptor*)ModelInfos)->getUnitsClass() == UnitsClassStr &&
            ((openfluid::fluidx::GeneratorDescriptor*)ModelInfos)->getVariableName() == VarNameStr)
        {
          const openfluid::ware::WareParams_t& Params = ModelInfos->getParameters();
          openfluid::ware::WareParams_t::const_iterator ItParam = Params.find(ParamNameStr);

          if (ItParam != Params.end())
          {
            ParamValStr = (*ItParam).second;
            STRING_TO_ALLOCATED_CARRAY(ParamValStr,CStr);
            return CStr;
          }

        }
      }

      STRING_TO_ALLOCATED_CARRAY(ParamValStr,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the value for a given generator parameter, an empty string if not found.
      @param[in] UnitsClass The name of the spatial units class
      @param[in] VarName The name of the generated variable
      @param[in] ParamName The name of the parameter
      @param[in] ParamVal The new value for the parameter
    */
    void setGeneratorParam(const char* UnitsClass, const char* VarName, const char* ParamName, const char* ParamVal)
    {
      std::string UnitsClassStr(UnitsClass);
      std::string VarNameStr(VarName);
      std::string ParamNameStr(ParamName);
      std::string ParamValStr(ParamVal);

      for (auto& ItModelInfos : m_FluidXDesc.model().items())
      {
        if (ItModelInfos->isType(openfluid::ware::WareType::GENERATOR) &&
            ((openfluid::fluidx::GeneratorDescriptor*)ItModelInfos)->getUnitsClass() == UnitsClassStr &&
            ((openfluid::fluidx::GeneratorDescriptor*)ItModelInfos)->getVariableName() == VarNameStr)
        {
          ItModelInfos->setParameter(ParamNameStr,ParamValStr);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the list of the variables produced by generators on a given spatial units class,
      as a semicolon separated list.
      @param[in] UnitsClass The spatial units class
      @return The list of produced variables
    */
    const char* getGeneratorsVarNames(const char* UnitsClass)
    {
      std::string VarNamesStr("");
      std::string UnitsClassStr(UnitsClass);
      std::ostringstream ssVarNames;
      std::string sep = "";

      for (const auto& Item : m_FluidXDesc.model().items())
      {
        if ( (Item->isType(openfluid::ware::WareType::GENERATOR))
          && (((openfluid::fluidx::GeneratorDescriptor*)Item)->getUnitsClass() == UnitsClassStr) )
        {
          ssVarNames << sep << ((openfluid::fluidx::GeneratorDescriptor*)Item)->getVariableName();
          sep = ";";
        }
      }

      VarNamesStr = ssVarNames.str();
      STRING_TO_ALLOCATED_CARRAY(VarNamesStr,CStr);
      return CStr;

    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the list of the global parameters for the current model, as a semicolon separated list.
      @return The list of parameters names
    */
    const char* getModelGlobalParamNames()
    {
      std::string ParamNamesStr("");
      std::ostringstream ssParamNames;
      std::string sep = "";

      for (const auto& Param : m_FluidXDesc.model().getGlobalParameters())
      {
        ssParamNames << sep << Param.first;
        sep = ";";
      }

      ParamNamesStr = ssParamNames.str();
      STRING_TO_ALLOCATED_CARRAY(ParamNamesStr,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the value of a given global parameter for the current model.
      @param[in] ParamName The name of the parameter
      @return The value of the parameter, as a string
    */
    const char* getModelGlobalParam(const char* ParamName)
    {
      std::string ParamNameStr(ParamName);
      std::string ParamValStr("");

      openfluid::ware::WareParams_t Params = m_FluidXDesc.model().getGlobalParameters();
      openfluid::ware::WareParams_t::const_iterator ItParam = Params.find(ParamNameStr);

      if (ItParam != Params.end())
      {
        ParamValStr = (*ItParam).second;
        STRING_TO_ALLOCATED_CARRAY(ParamValStr,CStr);
        return CStr;
      }

      STRING_TO_ALLOCATED_CARRAY(ParamValStr,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the value of a given global parameter for the current model.
      @param[in] ParamName The name of the parameter
      @param[in] ParamVal The new value for the parameter
    */
    void setModelGlobalParam(const char* ParamName, const char* ParamVal)
    {
      std::string ParamNameStr(ParamName);
      std::string ParamValStr(ParamVal);

      m_FluidXDesc.model().setGlobalParameter(ParamNameStr,ParamValStr);
    }


    // =====================================================================
    // =====================================================================


    /**
      Removes a given global parameter for the current model.
      @param[in] ParamName The name of the parameter
    */
    void removeModelGlobalParam(const char* ParamName)
    {
      std::string ParamNameStr(ParamName);

      m_FluidXDesc.model().eraseGlobalParameter(ParamNameStr);
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the IDs list of the observers used in the current model, as a semicolon separated list.
      @return The list of IDs
    */
    const char* getObserversIDs()
    {
      std::string ObsIDsStr("");
      std::ostringstream ssObsIDs;
      std::string sep = "";
      const openfluid::fluidx::MonitoringDescriptor& MonDesc = m_FluidXDesc.monitoring();

      for (auto& ItItem : MonDesc.items())
      {
        ssObsIDs << sep << MonDesc.getID(ItItem);
        sep = ";";
      }

      ObsIDsStr = ssObsIDs.str();
      STRING_TO_ALLOCATED_CARRAY(ObsIDsStr,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the list of the parameters names for a given observer, as a semicolon separated list.
      @param[in] ObsID The ID of the observer
      @return The list of parameters names
    */
    const char* getObserverParamNames(const char* ObsID)
    {
      std::string ObsIDStr(ObsID);
      std::string ParamNamesStr("");
      std::ostringstream ssParamNames;
      std::string sep = "";
      const openfluid::fluidx::MonitoringDescriptor& MonDesc = m_FluidXDesc.monitoring();

      int ItemIndex = MonDesc.findFirstItem(ObsIDStr);
      if (ItemIndex < 0)
      {
        return "";
      }

      openfluid::fluidx::ObserverDescriptor Item = MonDesc.itemAt(ItemIndex);
      if (!Item.isType(openfluid::ware::WareType::OBSERVER))
      {
        return "";
      }

      for (const auto& Param : Item.parameters())
      {
        ssParamNames << sep << Param.first;
        sep = ";";
      }

      ParamNamesStr = ssParamNames.str();
      STRING_TO_ALLOCATED_CARRAY(ParamNamesStr,CStr);
      return CStr;

    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the value of a given observer parameter, an empty string if not found.
      @param[in] ObsID The ID of the observer
      @param[in] ParamName The name of the parameter
      @return The value of the parameter, as a string
    */
    const char* getObserverParam(const char* ObsID, const char* ParamName)
    {
      std::string ParamValStr("");
      std::string ObsIDStr(ObsID);
      std::string ParamNameStr(ParamName);

      for (const auto& ObsInfos : m_FluidXDesc.monitoring().items())
      {
        if (ObsInfos->isType(openfluid::ware::WareType::OBSERVER) &&
            ((openfluid::fluidx::ObserverDescriptor*)ObsInfos)->getID() == ObsIDStr)
        {
          openfluid::ware::WareParams_t Params = ObsInfos->getParameters();
          openfluid::ware::WareParams_t::const_iterator ItParam = Params.find(ParamNameStr);

          if (ItParam != Params.end())
          {
            ParamValStr = (*ItParam).second;
            STRING_TO_ALLOCATED_CARRAY(ParamValStr,CStr);
            return CStr;
          }
        }
      }

      STRING_TO_ALLOCATED_CARRAY(ParamValStr,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the value of a given observer parameter.
      @param[in] ObsID The ID of the observer
      @param[in] ParamName The name of the parameter
      @param[in] ParamVal The new value for the parameter
    */
    void setObserverParam(const char* ObsID, const char* ParamName, const char* ParamVal)
    {
      std::string ObsIDStr(ObsID);
      std::string ParamNameStr(ParamName);
      std::string ParamValStr(ParamVal);

      for (auto& ObsInfos : m_FluidXDesc.monitoring().items())
      {
        if (ObsInfos->isType(openfluid::ware::WareType::OBSERVER) &&
            ((openfluid::fluidx::ObserverDescriptor*)ObsInfos)->getID() == ObsIDStr)
        {
          ObsInfos->setParameter(ParamNameStr,ParamValStr);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Removes a given observer parameter.
      @param[in] ObsID The ID of the observer
      @param[in] ParamName The name of the parameter
    */
    void removeObserverParam(const char* ObsID, const char* ParamName)
    {
      std::string ObsIDStr(ObsID);
      std::string ParamNameStr(ParamName);

      for (auto& ObsInfos : m_FluidXDesc.monitoring().items())
      {
        if (ObsInfos->isType(openfluid::ware::WareType::OBSERVER) &&
            ((openfluid::fluidx::ObserverDescriptor*)ObsInfos)->getID() == ObsIDStr)
        {
          ObsInfos->eraseParameter(ParamNameStr);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the spatial units classes names as a list of strings.
      @return The list of spatial units classes
    */
    char** getUnitsClasses()
    {
      std::map<openfluid::core::UnitsClass_t,unsigned int> UnitsCountByClasses;
      UnitsCountByClasses = getUnitsCountByClasses();

      const unsigned int Count = UnitsCountByClasses.size();

      char** Classes = (char**)malloc(Count*sizeof(char*));

      unsigned int i=0;
      for (auto& ItUCC : UnitsCountByClasses)
      {
        Classes[i] = (char*)malloc(ItUCC.first.size()+1);
        std::copy(ItUCC.first.begin(), ItUCC.first.end(), Classes[i]);
        Classes[i][ItUCC.first.size()] = '\0';
        i++;
      }

      return Classes;

    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the number of spatial units classes names.
      @return The number of spatial units classes
    */
    unsigned int getUnitsClassesCount()
    {
      return getUnitsCountByClasses().size();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the spatial units IDs for a given spatial units class, as a list of integers.
      @return The list of spatial units IDs
    */
    int* getUnitsIDs(const char* UnitsClass)
    {
      int* IDs = NULL;

      if (m_FluidXDesc.spatialDomain().isClassNameExists(UnitsClass))
      {
        const auto IDsOfClass = m_FluidXDesc.spatialDomain().getIDsOfClass(UnitsClass);

        IDs = (int*)malloc(IDsOfClass.size()*sizeof(int));
        unsigned int i=0;
        for (const auto& ID : IDsOfClass)
        {
          IDs[i] = ID;
          i++;
        }

      }

      return IDs;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the number of spatial units IDs for a given spatial units class.
      @return The number of spatial units IDs
    */
    unsigned int getUnitsIDsCount(const char* UnitsClass)
    {
      if (m_FluidXDesc.spatialDomain().isClassNameExists(UnitsClass))
      {
        return m_FluidXDesc.spatialDomain().getUnitsCount(UnitsClass);
      }

      return 0;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the list of the attributes names for a given spatial units class, as a semicolon separated list.
      @param[in] UnitsClass The units class
      @return The list of attributes names
    */
    const char* getAttributesNames(const char* UnitsClass)
    {
      std::string UnitClassStr(UnitsClass);
      std::string AttrNamesStr("");
      std::ostringstream ssAttrNames;
      std::string sep = "";

      for (const auto& Name : m_FluidXDesc.spatialDomain().getAttributesNames(UnitsClass))
      {
        ssAttrNames << sep << Name;
        sep = ";";
      }

      AttrNamesStr = ssAttrNames.str();
      STRING_TO_ALLOCATED_CARRAY(AttrNamesStr,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Creates an attribute for all spatial units of a given class, with a default value for this attribute.
      @param[in] UnitsClass The spatial units class
      @param[in] AttrName The name of the attribute
      @param[in] AttrVal The default value for this attribute
    */
    void createAttribute(const char* UnitsClass, const char* AttrName, const char* AttrVal)
    {
      std::string UnitsClassStr(UnitsClass);
      std::string AttrNameStr(AttrName);
      std::string AttrValStr(AttrVal);

      if (m_FluidXDesc.spatialDomain().isClassNameExists(UnitsClassStr))
      {
        m_FluidXDesc.spatialDomain().addAttribute(UnitsClass,AttrNameStr,AttrValStr);
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the value of an attribute of a spatial unit.
      @param[in] UnitsClass The spatial units class
      @param[in] UnitID The spatial unit ID
      @param[in] AttrName The name of the attribute
      @return The value of the attribute, as a string
    */
    const char* getAttribute(const char* UnitsClass, int UnitID, const char* AttrName)
    {
      std::string UnitClassStr(UnitsClass);
      std::string AttrNameStr(AttrName);
      std::string AttrValStr("");

      try
      {
        AttrValStr = m_FluidXDesc.spatialDomain().getAttribute(UnitClassStr,UnitID,AttrNameStr);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        // do nothing
      }

      STRING_TO_ALLOCATED_CARRAY(AttrValStr,CStr);
      return CStr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets an attribute value for a given spatial unit.
      @param[in] UnitsClass The spatial units class
      @param[in] UnitID The spatial unit ID
      @param[in] AttrName The name of the attribute
      @param[in] AttrVal The new value for this attribute
    */
    void setAttribute(const char* UnitsClass, int UnitID, const char* AttrName, const char* AttrVal)
    {
      std::string UnitClassStr(UnitsClass);
      std::string AttrNameStr(AttrName);
      std::string AttrValStr(AttrVal);

      try
      {
        m_FluidXDesc.spatialDomain().setAttribute(UnitClassStr,UnitID,AttrNameStr,AttrValStr);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        // do nothing
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Removes an attribute for all spatial units of a given class.
      @param[in] UnitsClass The spatial units class
      @param[in] AttrName The name of the attribute
    */
    void removeAttribute(const char* UnitsClass, const char* AttrName)
    {
      std::string UnitClassStr(UnitsClass);
      std::string AttrNameStr(AttrName);

      try
      {
        m_FluidXDesc.spatialDomain().deleteAttribute(UnitClassStr,AttrNameStr);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        // do nothing
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Adds simulation variable to be automatically exported using CSV format.
      @param[in] BindingName The name used as an identifier in output files names
      @param[in] UnitsClass The name of the spatial units class
      @param[in] UnitsIDs The semicolon separated list of spatial units IDs, can be '*' for all spatial units
      @param[in] VarName The name of the variable
      @param[in] Precision The floating precision to use for floating point representation,
                 default precision is used if this parameter is less or equal to 0
    */
    void addVariablesExportAsCSV(const char* BindingName,
                                 const char* UnitsClass, const char* UnitsIDs,
                                 const char* VarName, int Precision)
    {
      std::string BindingNameStr(BindingName);
      std::string UnitsClassStr(UnitsClass);
      std::string UnitsIDsStr(UnitsIDs);
      std::string VarNameStr(VarName);
      openfluid::fluidx::MonitoringDescriptor& MonDesc = m_FluidXDesc.monitoring();

      // 1. add CSV observer if not present

      if (MonDesc.findFirstItem("export.vars.files.csv") < 0)
      {
        MonDesc.appendItem(new openfluid::fluidx::ObserverDescriptor("export.vars.files.csv"));
      }

      openfluid::fluidx::ObserverDescriptor& ObsDesc = MonDesc.itemAt(MonDesc.findFirstItem("export.vars.files.csv"));

      // 2. add format for binding
      ObsDesc.setParameter("format."+BindingNameStr+".header",openfluid::core::StringValue("colnames-as-data"));
      ObsDesc.setParameter("format."+BindingNameStr+".date",openfluid::core::StringValue("%Y%m%d-%H%M%S"));
      ObsDesc.setParameter("format."+BindingNameStr+".colsep",openfluid::core::StringValue(" "));
      if (Precision > 0)
      {
        std::ostringstream ssPrec;
        ssPrec << Precision;
        std::string PrecStr(ssPrec.str());
        ObsDesc.setParameter("format."+BindingNameStr+".precision",openfluid::core::StringValue(PrecStr));
      }

      // 3. add binding output set
      ObsDesc.setParameter("set."+BindingNameStr+UnitsClassStr+".unitsclass",
                           openfluid::core::StringValue(UnitsClassStr));
      ObsDesc.setParameter("set."+BindingNameStr+UnitsClassStr+".unitsIDs",
                           openfluid::core::StringValue(UnitsIDsStr));
      ObsDesc.setParameter("set."+BindingNameStr+UnitsClassStr+".vars",
                           openfluid::core::StringValue(VarNameStr));
      ObsDesc.setParameter("set."+BindingNameStr+UnitsClassStr+".format",
                           openfluid::core::StringValue(BindingNameStr));
    }

};


} } // namespaces


#endif /* __OPENFLUID_UTILS_BINDING_HPP__ */

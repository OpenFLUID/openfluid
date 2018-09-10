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


#include <iostream>

#include <QCoreApplication>

#include <openfluid/config.hpp>
#include <openfluid/base/Init.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/utils/BindingVerboseMachineListener.hpp>
#include <openfluid/utils/BindingAbstractOutErr.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>
#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>
#include <openfluid/fluidx/AdvancedMonitoringDescriptor.hpp>
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
  int openfluid::utils::Binding::m_qapp_argc = 1; \
  char openfluid::utils::Binding::m_qapp_arg0[] = "ROpenFLUID"; \
  char* openfluid::utils::Binding::m_qapp_argv[] = { openfluid::utils::Binding::m_qapp_arg0  , NULL }; \
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
  @code
  #include <openfluid/utils/Binding.hpp>

  class MyErrOut : public openfluid::utils::BindingAbstractOutErr
  {
    public:

      void printfOut(const char* fmt, ... ) const
      {
        va_list args;
        va_start(args,fmt);
        vprintf(fmt,args);
        va_end(args);
      }

      void printfErr(const char* fmt, ... ) const
      {
        va_list args;
        va_start(args,fmt);
        vfprintf(stderr,fmt,args);
        va_end(args);
      }
  };

  OPENFLUID_BINDING_DEFINE(MyErrOut);

  // proceed here with the use of the binding class features
  @endcode
*/
class Binding
{
  private:

    static bool m_Initialized;

    static int m_qapp_argc;
    static char m_qapp_arg0[];
    static char* m_qapp_argv[];

    static std::string m_LastErrorMsg;

    static const BindingAbstractOutErr* mp_OutErr;

    openfluid::base::IOListener m_FluidXListener;

    openfluid::fluidx::FluidXDescriptor m_FluidXDesc;

    bool m_IsProject = false;
    bool m_IsDataset = false;

    std::string m_SourcePath = "";
    std::string m_OutputDir = "";

    bool m_IsSimulationRun = false;


    // =====================================================================
    // =====================================================================


    Binding() :
      m_FluidXDesc(&m_FluidXListener)
    {

    }


    // =====================================================================
    // =====================================================================


    ~Binding()
    {

    }


    // =====================================================================
    // =====================================================================


    std::map<openfluid::core::UnitsClass_t,unsigned int> getUnitsCountByClasses()
    {
      std::map<openfluid::core::UnitsClass_t,unsigned int> RetMap;

      for (auto ItUnits : m_FluidXDesc.spatialDomainDescriptor().spatialUnits())
      {
        if (RetMap.find(ItUnits.getUnitsClass()) == RetMap.end())
          RetMap[ItUnits.getUnitsClass()] = 0;
        RetMap[ItUnits.getUnitsClass()]++;
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

      INIT_OPENFLUID_APPLICATION(m_qapp_argc,m_qapp_argv);

      // reset locale for "LC_NUMERIC" To "C"
      // to prevent from Qt changing locale on init
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

        openfluid::base::IOListener IOListen;

        openfluid::base::RunContextManager::instance()->setInputDir(std::string(Path));
        Data->m_FluidXDesc.loadFromDirectory(openfluid::base::RunContextManager::instance()->getInputDir());

        Data->m_IsSimulationRun = false;

        if (!Data->m_IsProject)
        {
          Data->m_IsDataset = true;
          Data->m_SourcePath = openfluid::base::RunContextManager::instance()->getInputDir();
        }


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
      try
      {
        init();

        Binding* Data = new Binding();

        if (!openfluid::base::RunContextManager::instance()->openProject(std::string(Path)))
        {
          throw openfluid::base::ApplicationException(
              openfluid::base::ApplicationException::computeContext("ROpenFLUID"),
              std::string(Path) + " is not a correct project path");
        }

        Data->m_IsProject = true;
        Data->m_SourcePath = openfluid::base::RunContextManager::instance()->getProjectPath();

        return Binding::openDataset(openfluid::base::RunContextManager::instance()->getInputDir().c_str());
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

      return nullptr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Runs a simulation based on the current configuration for simulation.
      @param[in] IsVerbose Set to true to enable verbosity (false by default).
    */
    unsigned short int runSimulation(int IsVerbose = false)
    {
      try
      {
        init();

        openfluid::machine::Engine* Engine;


        openfluid::base::IOListener IOListen;
        openfluid::machine::SimulationBlob SimBlob;

        openfluid::machine::SimulatorPluginsManager::instance()->unloadAllWares();


        std::unique_ptr<openfluid::machine::MachineListener> Listener;

        if (IsVerbose)
          Listener.reset(new BindingVerboseMachineListener(mp_OutErr));
        else
          Listener.reset(new openfluid::machine::MachineListener());


        // ===== spatial domain and run config

        if (IsVerbose)
        {
          mp_OutErr->printfOut("%s","Building spatial domain...");
        }

        openfluid::machine::Factory::buildSimulationBlobFromDescriptors(m_FluidXDesc,SimBlob);

        if (IsVerbose)
        {
          static_cast<BindingVerboseMachineListener*>(Listener.get())->
              displayStatus(openfluid::base::Listener::LISTEN_OK);
        }


        // ===== model instance

        if (IsVerbose)
        {
          mp_OutErr->printfOut("%s","Building model instance...");
        }


        openfluid::machine::ModelInstance Model(SimBlob,Listener.get());

        openfluid::machine::Factory::buildModelInstanceFromDescriptor(m_FluidXDesc.modelDescriptor(),Model);

        if (IsVerbose)
        {
          static_cast<BindingVerboseMachineListener*>(Listener.get())->
              displayStatus(openfluid::base::Listener::LISTEN_OK);
        }


        // ===== monitoring instance

        if (IsVerbose)
        {
          mp_OutErr->printfOut("%s","Building monitoring instance...");
        }

        openfluid::machine::MonitoringInstance Monitoring(SimBlob);

        openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(m_FluidXDesc.monitoringDescriptor(),
                                                                           Monitoring);

        if (IsVerbose)
        {
          static_cast<BindingVerboseMachineListener*>(Listener.get())->
                        displayStatus(openfluid::base::Listener::LISTEN_OK);
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
      Prints informations about simulation configuration (spatial domain, processes model, ...)
    */
    void printSimulationInfo()
    {
      // Spatial domain

      std::map<openfluid::core::UnitsClass_t,unsigned int> UnitsInfos;

      for (auto& ItUnits : m_FluidXDesc.spatialDomainDescriptor().spatialUnits())
      {
        openfluid::core::UnitsClass_t ClassName = ItUnits.getUnitsClass();

        if (UnitsInfos.find(ClassName) == UnitsInfos.end())
          UnitsInfos[ClassName] = 0;

        UnitsInfos[ClassName]++;
      }

      mp_OutErr->printfOut("Spatial domain is made of %i spatial units\n",
                          m_FluidXDesc.spatialDomainDescriptor().spatialUnits().size());

      for (auto& ItUnitsInfos : UnitsInfos)
      {
        mp_OutErr->printfOut(" - %i units of class %s\n",ItUnitsInfos.second,ItUnitsInfos.first.c_str());
      }


      // Model

      mp_OutErr->printfOut("Model is made of %i simulation items\n",m_FluidXDesc.modelDescriptor().items().size());

      for (auto& ItModelInfos : m_FluidXDesc.modelDescriptor().items())
      {
        mp_OutErr->printfOut(" - ");

        if (ItModelInfos->isType(openfluid::ware::WareType::SIMULATOR))
        {
          mp_OutErr->printfOut("%s simulator\n",
                               ((openfluid::fluidx::SimulatorDescriptor*)(ItModelInfos))->getID().c_str());
        }

        if (ItModelInfos->isType(openfluid::ware::WareType::GENERATOR))
        {
          openfluid::fluidx::GeneratorDescriptor* pGenDesc = ((openfluid::fluidx::GeneratorDescriptor*)ItModelInfos);

          if (pGenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Fixed)
          {
            mp_OutErr->printfOut("fixed");
          }

          if (pGenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Random)
          {
            mp_OutErr->printfOut("random");
          }

          if (pGenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Interp)
          {
            mp_OutErr->printfOut("interp");
          }

          if (pGenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Inject)
          {
            mp_OutErr->printfOut("inject");
          }

          mp_OutErr->printfOut(" generator for variable %s on units %s\n",
                              pGenDesc->getVariableName().c_str(),pGenDesc->getUnitsClass().c_str());
        }
      }

      // Time period

      mp_OutErr->printfOut("Simulation period from %s to %s\n",
                          m_FluidXDesc.runDescriptor().getBeginDate().getAsISOString().c_str(),
                          m_FluidXDesc.runDescriptor().getEndDate().getAsISOString().c_str());

      // Time step

      mp_OutErr->printfOut("Simulation time step : %i\n",m_FluidXDesc.runDescriptor().getDeltaT());

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
      return m_FluidXDesc.runDescriptor().getDeltaT();
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the default delta T for the current simulation
      @param[in] DeltaT The delta T to set
    */
    void setDefaultDeltaT(int DeltaT)
    {
      m_FluidXDesc.runDescriptor().setDeltaT(DeltaT);
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
      std::string DateStr = m_FluidXDesc.runDescriptor().getBeginDate().getAsString("%Y-%m-%d %H:%M:%S");
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
      std::string DateStr = m_FluidXDesc.runDescriptor().getEndDate().getAsString("%Y-%m-%d %H:%M:%S");
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
        m_FluidXDesc.runDescriptor().setBeginDate(DateToSet);
      }

      if (!StrEndDate.empty() && DateToSet.setFromISOString(StrEndDate))
      {
        m_FluidXDesc.runDescriptor().setEndDate(DateToSet);
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
      openfluid::fluidx::AdvancedModelDescriptor AdvModDesc(m_FluidXDesc.modelDescriptor());

      for (auto& ItItem : AdvModDesc.items())
      {
        if (ItItem->isType(openfluid::ware::WareType::SIMULATOR))
        {
          ssSimIDs << sep << AdvModDesc.getID(ItItem);
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
      openfluid::fluidx::AdvancedModelDescriptor AdvModDesc(m_FluidXDesc.modelDescriptor());
      int ItemIndex = AdvModDesc.findFirstItem(SimIDStr);

      if (ItemIndex < 0)
      {
        return "";
      }

      openfluid::fluidx::ModelItemDescriptor Item = AdvModDesc.itemAt(ItemIndex);
      if (!Item.isType(openfluid::ware::WareType::SIMULATOR))
      {
        return "";
      }

      for (auto& ItParam : Item.parameters())
      {
        ssParamNames << sep << ItParam.first;
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

      for (auto& ItModelInfos : m_FluidXDesc.modelDescriptor().items())
      {
        if (ItModelInfos->isType(openfluid::ware::WareType::SIMULATOR) &&
            ((openfluid::fluidx::SimulatorDescriptor*)ItModelInfos)->getID() == SimIDStr)
        {
          openfluid::ware::WareParams_t Params = ItModelInfos->getParameters();
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

      for (auto& ItModelInfos : m_FluidXDesc.modelDescriptor().items())
      {
        if (ItModelInfos->isType(openfluid::ware::WareType::SIMULATOR) &&
            ((openfluid::fluidx::SimulatorDescriptor*)ItModelInfos)->getID() == SimIDStr)
          ItModelInfos->setParameter(ParamNameStr,ParamValStr);
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

      for (auto& ItModelInfos : m_FluidXDesc.modelDescriptor().items())
      {
        if (ItModelInfos->isType(openfluid::ware::WareType::SIMULATOR) &&
            ((openfluid::fluidx::SimulatorDescriptor*)ItModelInfos)->getID() == SimIDStr)
        {
          ItModelInfos->eraseParameter(ParamNameStr);
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

      for (auto& ItItem : m_FluidXDesc.modelDescriptor().items())
      {
        if ( (ItItem->isType(openfluid::ware::WareType::GENERATOR))
          && (((openfluid::fluidx::GeneratorDescriptor*)ItItem)->getVariableName() == VarNameStr)
          && (((openfluid::fluidx::GeneratorDescriptor*)ItItem)->getUnitsClass() == UnitsClassStr) )
        {
          for (auto& ItParam : ItItem->parameters())
          {
            ssParamNames << sep << ItParam.first;
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

      for (auto& ItModelInfos : m_FluidXDesc.modelDescriptor().items())
      {
        if (ItModelInfos->isType(openfluid::ware::WareType::GENERATOR) &&
            ((openfluid::fluidx::GeneratorDescriptor*)ItModelInfos)->getUnitsClass() == UnitsClassStr &&
            ((openfluid::fluidx::GeneratorDescriptor*)ItModelInfos)->getVariableName() == VarNameStr)
        {
          openfluid::ware::WareParams_t Params = ItModelInfos->getParameters();
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

      for (auto& ItModelInfos : m_FluidXDesc.modelDescriptor().items())
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

      for (auto& ItItem : m_FluidXDesc.modelDescriptor().items())
      {
        if ( (ItItem->isType(openfluid::ware::WareType::GENERATOR))
          && (((openfluid::fluidx::GeneratorDescriptor*)ItItem)->getUnitsClass() == UnitsClassStr) )
        {
          ssVarNames << sep << ((openfluid::fluidx::GeneratorDescriptor*)ItItem)->getVariableName();
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

      for (auto& ItParam : m_FluidXDesc.modelDescriptor().getGlobalParameters())
      {
        ssParamNames << sep << ItParam.first;
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

      openfluid::ware::WareParams_t Params = m_FluidXDesc.modelDescriptor().getGlobalParameters();
      openfluid::ware::WareParams_t::iterator ItParam = Params.find(ParamNameStr);

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

      m_FluidXDesc.modelDescriptor().setGlobalParameter(ParamNameStr,ParamValStr);
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

      m_FluidXDesc.modelDescriptor().eraseGlobalParameter(ParamNameStr);
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
      openfluid::fluidx::AdvancedMonitoringDescriptor AdvMonDesc(m_FluidXDesc.monitoringDescriptor());

      for (auto& ItItem : AdvMonDesc.items())
      {
        ssObsIDs << sep << AdvMonDesc.getID(ItItem);
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
      openfluid::fluidx::AdvancedMonitoringDescriptor AdvMonDesc(m_FluidXDesc.monitoringDescriptor());

      int ItemIndex = AdvMonDesc.findFirstItem(ObsIDStr);
      if (ItemIndex < 0)
      {
        return "";
      }

      openfluid::fluidx::ObserverDescriptor Item = AdvMonDesc.itemAt(ItemIndex);
      if (!Item.isType(openfluid::ware::WareType::OBSERVER))
      {
        return "";
      }

      for (auto& ItParam : Item.parameters())
      {
        ssParamNames << sep << ItParam.first;
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

      for (auto& ItObsInfos : m_FluidXDesc.monitoringDescriptor().items())
      {
        if (ItObsInfos->isType(openfluid::ware::WareType::OBSERVER) &&
            ((openfluid::fluidx::ObserverDescriptor*)ItObsInfos)->getID() == ObsIDStr)
        {
          openfluid::ware::WareParams_t Params = ItObsInfos->getParameters();
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

      for (auto& ItObsInfos : m_FluidXDesc.monitoringDescriptor().items())
      {
        if (ItObsInfos->isType(openfluid::ware::WareType::OBSERVER) &&
            ((openfluid::fluidx::ObserverDescriptor*)ItObsInfos)->getID() == ObsIDStr)
        {
          ItObsInfos->setParameter(ParamNameStr,ParamValStr);
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

      for (auto& ItObsInfos : m_FluidXDesc.monitoringDescriptor().items())
      {
        if (ItObsInfos->isType(openfluid::ware::WareType::OBSERVER) &&
            ((openfluid::fluidx::ObserverDescriptor*)ItObsInfos)->getID() == ObsIDStr)
          ItObsInfos->eraseParameter(ParamNameStr);
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

      std::map<openfluid::core::UnitsClass_t,unsigned int>::iterator ItUCC;
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
      std::map<openfluid::core::UnitsClass_t,unsigned int> UnitsCountByClasses;
      UnitsCountByClasses = getUnitsCountByClasses();

      return UnitsCountByClasses.size();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the spatial units IDs for a given spatial units class, as a list of integers.
      @return The list of spatial units IDs
    */
    int* getUnitsIDs(const char* UnitsClass)
    {
      int Count = getUnitsIDsCount(UnitsClass);

      int* IDs = NULL;

      if (Count > 0)
      {
        IDs = (int*)malloc(Count*sizeof(int));

        unsigned int i=0;
        for (auto& ItUnits : m_FluidXDesc.spatialDomainDescriptor().spatialUnits())
        {
          if (ItUnits.getUnitsClass() == std::string(UnitsClass))
          {
            IDs[i] = ItUnits.getID();
            i++;
          }
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
      std::map<openfluid::core::UnitsClass_t,unsigned int> UnitsCountByClasses;
      UnitsCountByClasses = getUnitsCountByClasses();

      unsigned int Count = 0;

      if (UnitsCountByClasses.find(std::string(UnitsClass)) != UnitsCountByClasses.end())
      {
        Count = UnitsCountByClasses[std::string(UnitsClass)];
      }

      return Count;
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
      openfluid::fluidx::AdvancedDomainDescriptor AdvDomDesc(m_FluidXDesc.spatialDomainDescriptor());

      for (std::string ItName : AdvDomDesc.getAttributesNames(UnitsClass))
      {
        ssAttrNames << sep << ItName;
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
      std::string UnitClassStr(UnitsClass);
      std::string AttrNameStr(AttrName);
      std::string AttrValStr(AttrVal);

      for (auto& ItAttr : m_FluidXDesc.spatialDomainDescriptor().attributes())
      {
        if (ItAttr.getUnitsClass() == UnitClassStr)
        {
          for (auto & ItUnitData : ItAttr.attributes())
          {
            ItUnitData.second[AttrNameStr] = AttrValStr;
          }
        }
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

      for (auto& ItAttr : m_FluidXDesc.spatialDomainDescriptor().attributes())
      {
        if (ItAttr.getUnitsClass() == UnitClassStr)
        {
          openfluid::fluidx::AttributesDescriptor::UnitIDAttribute_t::const_iterator ItUnitData =
              ItAttr.attributes().find(UnitID);
          if (ItUnitData != ItAttr.attributes().end())
          {
            if ((*ItUnitData).second.find(AttrNameStr) != (*ItUnitData).second.end())
            {
              AttrValStr = (*ItUnitData).second.at(AttrNameStr);
              STRING_TO_ALLOCATED_CARRAY(AttrValStr,CStr);
              return CStr;
            }
          }
        }
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

      for (auto& ItAttr : m_FluidXDesc.spatialDomainDescriptor().attributes())
      {
        if (ItAttr.getUnitsClass() == UnitClassStr)
        {
          openfluid::fluidx::AttributesDescriptor::UnitIDAttribute_t::iterator ItUnitData =
              ItAttr.attributes().find(UnitID);
          if (ItUnitData != ItAttr.attributes().end())
          {
            if ((*ItUnitData).second.find(AttrNameStr) != (*ItUnitData).second.end())
            {
              (*ItUnitData).second[AttrNameStr] = AttrValStr;
            }
          }
        }
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

      for (auto& ItAttr : m_FluidXDesc.spatialDomainDescriptor().attributes())
      {
        if (ItAttr.getUnitsClass() == UnitClassStr)
        {
          for (auto& ItUnitData : ItAttr.attributes())
            ItUnitData.second.erase(AttrNameStr);
        }
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
      openfluid::fluidx::AdvancedMonitoringDescriptor AdvMonDesc(m_FluidXDesc.monitoringDescriptor());

      // 1. add CSV observer if not present

      if (AdvMonDesc.findFirstItem("export.vars.files.csv") < 0)
        AdvMonDesc.appendItem(new openfluid::fluidx::ObserverDescriptor("export.vars.files.csv"));

      openfluid::fluidx::ObserverDescriptor& ObsDesc =
          AdvMonDesc.itemAt(AdvMonDesc.findFirstItem("export.vars.files.csv"));

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

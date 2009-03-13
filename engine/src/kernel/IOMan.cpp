/**
  \file IOMan.cpp
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "IOMan.h"
#include "xml/tinyxml.h"
#include "AppTools.h"
#include "config.h"
#include <sstream>
#include <wx/tokenzr.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/datetime.h>

IOManager* IOManager::mp_Singleton = NULL;



// =====================================================================
// =====================================================================

IOManager* IOManager::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new IOManager();

  return mp_Singleton;
}

// =====================================================================
// =====================================================================


bool IOManager::loadRunConfig(RunConfig* Config)
{
  TiXmlDocument LoadDoc;
  TiXmlElement* Child, *Child2;

  std::string Str;
  openfluid::core::DateTime ZeDate;


  long IntValue;

  FunctionConfig* FConf;

  if (LoadDoc.LoadFile(mp_RunEnv->getInputFullPath(OPENFLUID_DEFAULT_RUNFILE).c_str()))
  {

    TiXmlHandle DocHandle(&LoadDoc);


    // -------- DeltaT ----------------

    Child = DocHandle.FirstChild("openfluid").FirstChild("run").FirstChild("deltat").Element();

    if (Child == NULL)
    {
      throw openfluid::base::OFException("kernel","IOManager::loadRunConfig","Run config file (" + OPENFLUID_DEFAULT_RUNFILE + ") error. Missing deltat.");
      return false;
    }
    else
    {
      Str = Child->GetText();

      if (openfluid::tools::ConvertString(Str,&IntValue))
      {
        Config->DeltaT = int(IntValue);

//        std::cerr << Config->DeltaT << std::endl;
      }
      else
      {
        throw openfluid::base::OFException("kernel","IOManager::loadRunConfig","Run config file (" + OPENFLUID_DEFAULT_RUNFILE + ") error. Wrong deltat format.");
        return false;
      }
    }

    // -------- Simulation period ----------------

    Child = DocHandle.FirstChild("openfluid").FirstChild("run").FirstChild("period").Element();

    if (Child == NULL)
    {
      throw openfluid::base::OFException("kernel","IOManager::loadRunConfig","Run config file (" + OPENFLUID_DEFAULT_RUNFILE + ") error. Missing period.");
      return false;
    }
    else
    {
      if (Child->Attribute("begin") != NULL)
      {
        Str = Child->Attribute("begin");

        if (ZeDate.setFromISOString(Str))
        {
          Config->BeginDate = ZeDate;

//          std::cerr << Config->BeginDate.Format(wxT("%Y-%m-%d %H:%M:%S")).mb_str(wxConvUTF8) << std::endl;

        }
        else
        {
          throw openfluid::base::OFException("kernel","IOManager::loadRunConfig","Run config file (" + OPENFLUID_DEFAULT_RUNFILE + ") error. Wrong format of begin attribute for period.");
          return false;
        }
      }
      else
      {
        throw openfluid::base::OFException("kernel","IOManager::loadRunConfig","Run config file (" + OPENFLUID_DEFAULT_RUNFILE + ") error. Missing begin attribute for period.");
        return false;
      }


      if (Child->Attribute("end") != NULL)
      {
        Str = Child->Attribute("end");

        if (ZeDate.setFromISOString(Str))
        {
          Config->EndDate = ZeDate;

//          std::cerr << Config->EndDate.Format(wxT("%Y-%m-%d %H:%M:%S")).mb_str(wxConvUTF8) << std::endl;

        }
        else
        {
          throw openfluid::base::OFException("kernel","IOManager::loadRunConfig","Run config file (" + OPENFLUID_DEFAULT_RUNFILE + ") error. Wrong format of end attribute for period.");
          return false;
        }
      }
      else
      {
        throw openfluid::base::OFException("kernel","IOManager::loadRunConfig","Run config file (" + OPENFLUID_DEFAULT_RUNFILE + ") error. Missing end attribute for period.");
        return false;
      }

    }

    if (Config->EndDate < Config->BeginDate)
    {
      throw openfluid::base::OFException("kernel","IOManager::loadRunConfig","Run config file (" + OPENFLUID_DEFAULT_RUNFILE + ") error. Wrong simulation period definition.");
      return false;
    }


    // -------- Simulation ID ----------------

    Child = DocHandle.FirstChild("openfluid").FirstChild("run").FirstChild("simid").Element();

    if (Child != NULL)
    {
      Str = Child->GetText();
      if (Str != "" && (Str.find(" ") == -1)) Config->SimulationID = Str;
    }



  }
  else
  {
    throw openfluid::base::OFException("kernel","IOManager::loadRunConfig","in run config file (" + OPENFLUID_DEFAULT_RUNFILE + "), " + std::string(LoadDoc.ErrorDesc()));
    return false;
  }


  return true;

}


// =====================================================================
// =====================================================================


bool IOManager::loadModelConfig(ModelConfig* Config)
{

  TiXmlDocument LoadDoc;
  TiXmlElement* Child, *Child2;

  wxString Str;

  int IntValue;

  FunctionConfig* FConf;

  if (LoadDoc.LoadFile(mp_RunEnv->getInputFullPath(OPENFLUID_DEFAULT_MODELFILE).c_str()))
  {

    TiXmlHandle DocHandle(&LoadDoc);

    // ======= model structure ===========


    Child = DocHandle.FirstChild("openfluid").FirstChild("model").FirstChild("function").Element();


    for(Child;Child;Child=Child->NextSiblingElement())
    {
      if (Child->Attribute("fileID") != NULL)
      {

        // function fileID
        FConf = new FunctionConfig();
        FConf->FileID = Child->Attribute("fileID");


        if (FConf->FileID.length() > 0)
        {

          FConf->Params.clear();

          // function params parsing
          TiXmlHandle Child2Handle(Child);
          Child2 = Child2Handle.FirstChild("param").Element();

          for(Child2;Child2;Child2=Child2->NextSiblingElement())
          {
            if (Child2->Attribute("name") != NULL && Child2->Attribute("value") != NULL)
            {
              FConf->Params[Child2->Attribute("name")] = Child2->Attribute("value");

            }

          }
          // TODO check if * is correct
          Config->FuncConfigs.push_back(*FConf);

        }
        else delete FConf;

      }
      else
      {
        throw openfluid::base::OFException("kernel","IOManager::loadModelConfig","in model config file (" + OPENFLUID_DEFAULT_MODELFILE + "), " + std::string(LoadDoc.ErrorDesc()));
        return false;
      }


    }



  }
  else
  {

    throw openfluid::base::OFException("kernel","IOManager::loadModelConfig","in model config file (" + OPENFLUID_DEFAULT_MODELFILE + "), " + std::string(LoadDoc.ErrorDesc()));
    return false;
  }

  return true;

}



// =====================================================================
// =====================================================================

bool IOManager::loadDomainFile(std::string FullFilename, std::list<openfluid::core::UnitsLink_t>* ToUnitsList)
{

  TiXmlDocument LoadDoc;

//  std::cout << "Loading file " << FullFilename.mb_str(wxConvUTF8) << std::endl;

  if (LoadDoc.LoadFile(FullFilename.c_str()))
  {

    TiXmlHandle DocHandle(&LoadDoc);
    TiXmlElement* Child, *Child2, *Child3;

    Child = DocHandle.FirstChild("openfluid").FirstChild("domain").FirstChild("definition").Element();

    if (Child != NULL)
    {
      TiXmlHandle Child2Handle(Child);

      openfluid::core::UnitClass_t UnitClass, ToUnitClass;
      openfluid::core::UnitID_t UnitID, ToUnitID;
      int anInt;
      openfluid::core::PcsOrd_t UnitPcsOrder;

      //      openfluid::core::Unit* TheUnit;
      //      openfluid::core::Unit* TheToUnit;

      // read all units
      for(Child2 = Child2Handle.FirstChild("unit").Element();Child2;Child2=Child2->NextSiblingElement())
      {
        if (Child2->Attribute("class") != NULL && Child2->Attribute("ID",&anInt) != NULL)
        {

          UnitPcsOrder = -1;
          if (Child2->Attribute("pcsorder",&UnitPcsOrder) == NULL)
          {
            // TODO raise warning
            std::cerr << " no pcs order" << std::endl;
          }

          UnitClass = openfluid::core::UnitClass_t(Child2->Attribute("class"));
          UnitID = (openfluid::core::UnitID_t)anInt;
          mp_Repository->addUnit(openfluid::core::Unit(UnitClass,UnitID,UnitPcsOrder));


          TiXmlHandle Child3Handle(Child2);

          // search for to

          for(Child3 = Child3Handle.FirstChild("to").Element();Child3;Child3=Child3->NextSiblingElement())
          {

            // read "to" tags inside "units" tags
            if (Child3->Attribute("class") != NULL && Child3->Attribute("ID",&anInt) != NULL)
            {
              ToUnitClass = openfluid::core::UnitClass_t(Child3->Attribute("class"));
              ToUnitID = (openfluid::core::UnitID_t)anInt;

              // create link pair

              ToUnitsList->push_back(std::make_pair(std::make_pair(UnitClass,UnitID),
                  std::make_pair(ToUnitClass,ToUnitID)));

            }
            else
            {
              throw openfluid::base::OFException("kernel","IOManager::LoadDomainFile","Error loading file " + FullFilename + ", wrong attributes in definition of topology");
              return false;
            }
          }
        }
        else
        {
          throw openfluid::base::OFException("kernel","IOManager::LoadDomainFile","Error loading file " + FullFilename + ", wrong attributes in unit definition");
          return false;
        }
      }

      //      Repository->printSTDOUT();

    }
    else
    {
      throw openfluid::base::OFException("kernel","IOManager::LoadDomainFile","Error loading file " + FullFilename + ", wrong file format");
      return false;
    }
  }
  else
  {
    throw openfluid::base::OFException("kernel","IOManager::LoadDomainFile","Error loading file " + FullFilename);
    return false;
  }

  return true;
}

// =====================================================================
// =====================================================================


bool IOManager::loadDomainFromFiles()
{

  std::list<openfluid::core::UnitsLink_t> ToUnitsList;
  std::list<openfluid::core::UnitsLink_t>::iterator it;

  openfluid::core::Unit* FromUnit;
  openfluid::core::Unit* ToUnit;


  std::vector<std::string> FilesToLoad = GetFilesByExt(mp_RunEnv->getInputDir(),"ddef.xml",true);

  bool IsOK = true;
  unsigned int i=0;


  while (IsOK && i<FilesToLoad.size())
  {
    loadDomainFile(FilesToLoad[i], &ToUnitsList);
    i++;
  }


  // Processing topology links
  for (it=ToUnitsList.begin();it!=ToUnitsList.end();++it)
  {
    FromUnit = mp_Repository->getUnit(it->first.first,it->first.second);
    ToUnit = mp_Repository->getUnit(it->second.first,it->second.second);

    if (ToUnit != NULL)
    {
      FromUnit->addToUnit(ToUnit);
      ToUnit->addFromUnit(FromUnit);
    }
    else
    {
      std::ostringstream UnitStr;
      UnitStr << FromUnit->getClass() << "#" << FromUnit->getID();
      throw openfluid::base::OFException("kernel","IOManager::LoadDomainFromFiles","Target -to- unit referenced by " + UnitStr.str() + " does not exist" );
      IsOK = false;
    }
  }


  return IsOK;
}


// =====================================================================
// =====================================================================


bool IOManager::loadInputDataFile(std::string FullFilename)
{
//  std::cout << "Loading file " << FullFilename.mb_str(wxConvUTF8) << std::endl;

  TiXmlDocument LoadDoc;
  std::string Data;
  std::vector<std::string> ColOrder;
  std::string UnitClass;



  if (LoadDoc.LoadFile(FullFilename.c_str()))
  {

    TiXmlHandle DocHandle(&LoadDoc);
    TiXmlElement* Child;

    Child = DocHandle.FirstChild("openfluid").FirstChild("domain").FirstChild("inputdata").Element();

    if (Child != NULL)
    {
      if (Child->Attribute("unitclass") != NULL)
      {

        UnitClass = Child->Attribute("unitclass");

        if (!mp_Repository->isUnitsClassExists(UnitClass))
          throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Class "+ UnitClass +" found in " + FullFilename + " does not exist");

        Child = DocHandle.FirstChild("openfluid").FirstChild("domain").FirstChild("inputdata").FirstChild("columns").Element();

        // columns
        if (Child != NULL)
        {
          if (Child->Attribute("order") != NULL)
          {
            wxStringTokenizer Tkz(_U(Child->Attribute("order")), wxT(";"));

            while (Tkz.HasMoreTokens())
            {
              ColOrder.push_back(_S(Tkz.GetNextToken()));
            }

            // data
            if (ColOrder.size() > 0)
            {
              Child = DocHandle.FirstChild("openfluid").FirstChild("domain").FirstChild("inputdata").FirstChild("data").Element();

              if (Child != NULL)
              {
                //                Data.Clear();
                //                Data.Append(_U(Child->GetText()));
              }
              else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","No data found in file " + FullFilename);
            }
            else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Column order is empty in file " + FullFilename);
          }
          else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Column order not found in file " + FullFilename);
        }
        else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Column definition not found in file " + FullFilename);
      }
      else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Unit class not found in file " + FullFilename);
    }
    else
    {
      throw openfluid::base::OFException("kernel","IOManager::LoadInputDataFile","Error loading file " + FullFilename + ", wrong file format");
      return false;
    }
  }
  else
  {
    throw openfluid::base::OFException("kernel","IOManager::LoadInputDataFile","Error loading file " + FullFilename);
    return false;
  }


  openfluid::tools::ColumnTextParser DataParser("%");

  if (DataParser.setFromString(Data,ColOrder.size()+1))
  {
    //TODO to be continued here
  }
  else
  {
    throw openfluid::base::OFException("kernel","IOManager::LoadInputDataFile","Error loading file " + FullFilename + ",cannot parse data");
    return false;
  }



  return true;
}


// =====================================================================
// =====================================================================



bool IOManager::loadInputDataFromFiles()
{

  std::vector<std::string> FilesToLoad = GetFilesByExt(mp_RunEnv->getInputDir(),"ddata.xml",true);

  bool IsOK = true;
  unsigned int i=0;


  while (IsOK && i<FilesToLoad.size())
  {
    loadInputDataFile(FilesToLoad[i]);
    i++;
  }


  return IsOK;

}


// =====================================================================
// =====================================================================


bool IOManager::loadOutputConfig()
{
  TiXmlDocument LoadDoc;
  TiXmlElement* Child;
  TiXmlElement* Child2;

  std::string Str;
  unsigned int i;
  openfluid::core::UnitID_t UID;

//  std::cout << "Loading file ./output.xml"<< std::endl;



  if (LoadDoc.LoadFile(mp_RunEnv->getInputFullPath(OPENFLUID_DEFAULT_OUTPUTCONFFILE).c_str()))
  {

    TiXmlHandle DocHandle(&LoadDoc);

    // auto output files
    Child = DocHandle.FirstChild("openfluid").FirstChild("output").FirstChild("files").Element();

    if (Child == NULL) throw openfluid::base::OFException("kernel","IOManager::loadOutputConfig","Output config file format error (output.xml)");

    FileOutput_t FileOutput;

    for(Child;Child;Child=Child->NextSiblingElement())
    {
      FileOutput = FileOutput_t();

      if (Child->Attribute("colsep") != NULL) FileOutput.ColSeparator = std::string(Child->Attribute("colsep"));

      if (Child->Attribute("dtformat") != NULL)
      {
        std::string DTFormat = std::string(Child->Attribute("dtformat"));

        if (DTFormat == "6cols") DTFormat = "%Y\t%m\t%d\t%H\t%M\t%S";
        if (DTFormat == "iso") DTFormat = "%Y%m%dT%H%M%S";

        FileOutput.DateFormat = DTFormat;
      }

      if (Child->Attribute("commentchar") != NULL) FileOutput.CommentChar = std::string(Child->Attribute("commentchar"));


      // processing files sets

      Child2 = Child->FirstChildElement("set");

      OutputSet_t FileOutputSet;

      for(Child2;Child2;Child2=Child2->NextSiblingElement())
      {
        FileOutputSet = OutputSet_t();

        if (Child2->Attribute("name") != NULL && Child2->Attribute("unitsclass") != NULL
            && Child2->Attribute("unitsIDs") != NULL && Child2->Attribute("vars") != NULL)
        {
          std::string NameStr, ClassStr, IDStr, VarsStr;

          NameStr = std::string(Child2->Attribute("name"));
          ClassStr = std::string(Child2->Attribute("unitsclass"));
          IDStr = std::string(Child2->Attribute("unitsIDs"));
          VarsStr = std::string(Child2->Attribute("vars"));

          if (NameStr != "" && ClassStr != "" && mp_Repository->isUnitsClassExists(ClassStr) && IDStr != "" && VarsStr != "")
          {
            FileOutputSet.Name = NameStr;
            FileOutputSet.UnitsClass = ClassStr;
            openfluid::core::Unit* TmpUnit;


            // processing units of the set
            if (IDStr == "*") FileOutputSet.AllUnits = true;
            else
            {
              std::vector<std::string> StrArray = SplitString(IDStr,";");
              for (i=0;i<StrArray.size();i++)
              {
                if (openfluid::tools::ConvertString(StrArray[i],&UID))
                {
                  TmpUnit = mp_Repository->getUnit(ClassStr,UID);
                  if (TmpUnit != NULL) FileOutputSet.Units.push_back(TmpUnit);
                }
                else throw openfluid::base::OFException("kernel","IOManager::loadOutputConfig","Output config file format error: units selection ID (output.xml)");
              }
            }


            // processing vars of the set
            if (VarsStr == "*")
            {
              FileOutputSet.AllScalars = true;
              FileOutputSet.AllVectors = true;
            }
            else
            {
              std::vector<std::string> StrArray = SplitString(VarsStr,";");
              for (i=0;i<StrArray.size();i++)
              {
                if (IsVectorNamedVariable(StrArray[i])) FileOutputSet.VectorVariables.push_back(GetVectorNamedVariableName(StrArray[i]));
                else FileOutputSet.ScalarVariables.push_back(StrArray[i]);
              }
            }



            FileOutput.Sets.push_back(FileOutputSet);

          }
          // TODO else raise warning

        }
        // TODO else raise warning
      }



      m_OutputConfig.FileSets.push_back(FileOutput);


    }
  }
  else throw openfluid::base::OFException("kernel","IOManager::loadOutputConfig","Output config file not found (output.xml)");

  return true;

}

// =====================================================================
// =====================================================================


bool IOManager::prepareUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex, std::string OutputDir)
{
  /*  std::ostringstream RootFilename;
  std::ostringstream FileContent;

  RootFilename << m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].UnitsClass
           << aUnit->getID() << "_" << m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].Name;
   */

  wxFile TFile;
  std::string ScalarsFilename;
  std::string VectorFilename;
  std::string FileContent;
  std::vector<openfluid::core::VariableName_t> VarNames;
  std::string CommentChar = m_OutputConfig.FileSets[FileOutputIndex].CommentChar;

  std::string NameSuffix = m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].Name;

  // processing scalars outputs
  if (m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].AllScalars ||
      m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].ScalarVariables.size() > 0)
  {

    ScalarsFilename = generateOuputFilename(aUnit->getClass(),aUnit->getID(),NameSuffix);

    if (m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].AllScalars)
      FileContent = generateOutputScalarsFileHeader("unknown", aUnit->getClass(),aUnit->getID(),ScalarsFilename,
          aUnit->getScalarVariables()->getVariablesNames(),CommentChar);
    else
      FileContent = generateOutputScalarsFileHeader("unknown", aUnit->getClass(), aUnit->getID(),ScalarsFilename,
          m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].ScalarVariables,CommentChar);


    TFile.Open(_U(OutputDir.c_str()) + wxFILE_SEP_PATH + _U(ScalarsFilename.c_str()),wxFile::write_append);
    TFile.Write(_U(FileContent.c_str()));
    TFile.Close();
  }



  // processing vectors outputs
  if (m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].AllVectors ||
      m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].VectorVariables.size() > 0)
  {

    if (m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].AllVectors)
      VarNames = aUnit->getVectorVariables()->getVariablesNames();
    else
      VarNames = m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].VectorVariables;

    for (unsigned int j =  0; j < VarNames.size(); j++)
    {

      VectorFilename = generateOuputFilename(aUnit->getClass(),aUnit->getID(),NameSuffix,VarNames[j]);

      FileContent = generateOutputVectorFileHeader("unknown", aUnit->getClass(), aUnit->getID(),VectorFilename,
          VarNames[j],CommentChar);

      TFile.Open(_U(OutputDir.c_str()) + wxFILE_SEP_PATH + _U(VectorFilename.c_str()),wxFile::write_append);
      TFile.Write(_U(FileContent.c_str()));
      TFile.Close();

    }

  }


  return true;

}


// =====================================================================
// =====================================================================

bool IOManager::prepareOutputDir()
{
  bool IsOK = true;


  if (!wxDirExists(_U(mp_RunEnv->getOutputDir().c_str())))
  {
#ifdef __WXMSW__
    wxMkDir(mp_RunEnv->getOutputDir().c_str());
#else
    wxMkDir(mp_RunEnv->getOutputDir().c_str(),0777);
#endif
    if (!wxDirExists(_U(mp_RunEnv->getOutputDir().c_str())))
      throw openfluid::base::OFException("kernel","IOManager::prepareOutputDir","Error creating output directory");

  }
  else
  {
    if (mp_RunEnv->isClearOutputDir() && !m_ClearedOuputDir)
    {
      EmptyDirectoryRecursively(mp_RunEnv->getOutputDir().c_str());
      m_ClearedOuputDir = true;
    }
  }

  return IsOK;
}

// =====================================================================
// =====================================================================



bool IOManager::prepareOutputs()
{


  openfluid::core::UnitsList_t* Units;
  openfluid::core::UnitsList_t::iterator UnitIt;


  prepareOutputDir();


  for (unsigned int i=0; i< m_OutputConfig.FileSets.size();i++)
  {

    for (unsigned int j=0; j< m_OutputConfig.FileSets[i].Sets.size();j++)
    {

      if (m_OutputConfig.FileSets[i].Sets[j].AllUnits)
      {
        Units = mp_Repository->getUnits(m_OutputConfig.FileSets[i].Sets[j].UnitsClass)->getList();

        for (UnitIt = Units->begin();UnitIt != Units->end();++UnitIt)
        {
          prepareUnitFileOutput(&(*UnitIt),i,j,mp_RunEnv->getOutputDir());
        }

      }
      else
      {
        for (unsigned int iUnits = 0; iUnits < m_OutputConfig.FileSets[i].Sets[j].Units.size();iUnits++)
        {
          prepareUnitFileOutput(m_OutputConfig.FileSets[i].Sets[j].Units[iUnits],i,j,mp_RunEnv->getOutputDir());
        }
      }
    }


  }
  return true;
}


// =====================================================================
// =====================================================================


std::string IOManager::generateOuputFilename(const std::string UnitClass, const openfluid::core::UnitID_t UnitID,
                                             const std::string Suffix, const std::string VectorName)
{
  std::ostringstream GeneratedName;

  GeneratedName << UnitClass << UnitID << "_" << Suffix;

  if (VectorName == "")
  {
    // TODO put file extension from config file
    GeneratedName << ".scalars.out";
  }
  else
  {
    // TODO put file extension from config file
    GeneratedName << ".vector." << VectorName<< ".out";
  }

  return GeneratedName.str();
}


// =====================================================================
// =====================================================================


std::string IOManager::generateOutputScalarsFileHeader(const std::string SimulationID, const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
    const std::string Filename, const std::vector<std::string> ScalarsNames,
    const std::string CommentChar)
{
  std::ostringstream GeneratedHeader;

  GeneratedHeader << CommentChar << " simulation ID: " << SimulationID << "\n"
  << CommentChar << " file: " << Filename << "\n"
  << CommentChar << " date: " << _S(wxDateTime::Now().Format()) << "\n"
  << CommentChar << " unit: " << UnitClass << " #" << UnitID << "\n"
  << CommentChar << " scalar variables order (after date and time columns):";


  for (unsigned int i = 0 ; i < ScalarsNames.size(); i++ ) GeneratedHeader << " " << ScalarsNames[i];


  GeneratedHeader << "\n" << "\n";

  return GeneratedHeader.str();

}


// =====================================================================
// =====================================================================


std::string IOManager::generateOutputVectorFileHeader(std::string SimulationID, openfluid::core::UnitClass_t UnitClass, openfluid::core::UnitID_t UnitID,
    std::string Filename, std::string VectorName,
    std::string CommentChar)
{
  std::ostringstream GeneratedHeader;

  GeneratedHeader << CommentChar << " simulation ID: " << SimulationID << "\n"
  << CommentChar << " file: " << Filename << "\n"
  << CommentChar << " date: " << _S(wxDateTime::Now().Format()) << "\n"
  << CommentChar << " unit: " << UnitClass << " #" << UnitID << "\n"
  << CommentChar << " vector variable: " << VectorName << "\n"
  << CommentChar << " the vector values are ordered left to right, from index 0 to size-1 (after date and time columns)" << "\n" << "\n";


  return GeneratedHeader.str();
}


// =====================================================================
// =====================================================================


bool IOManager::saveOutputs(openfluid::core::TimeStep_t CurrentStep, bool WithoutKeep)
{

  std::string OutputDir = mp_RunEnv->getOutputDir();

  openfluid::core::UnitsList_t* Units;
  openfluid::core::UnitsList_t::iterator UnitIt;


  openfluid::core::TimeStep_t BeginStep, EndStep;
  mp_MemMonitor->getMemoryReleaseRange(CurrentStep,WithoutKeep,&BeginStep,&EndStep);

//  std::cout << "Saving to disk from " << BeginStep << " to " << EndStep << std::endl;

  for (unsigned int i=0; i< m_OutputConfig.FileSets.size();i++)
  {

    //    std::cout << "   sets de sortie fichier: " << m_OutputConfig.FileSets[i].Sets.size() << std::endl;

    for (unsigned int j=0; j< m_OutputConfig.FileSets[i].Sets.size();j++)
    {

      if (m_OutputConfig.FileSets[i].Sets[j].AllUnits)
      {


        Units = mp_Repository->getUnits(m_OutputConfig.FileSets[i].Sets[j].UnitsClass)->getList();

        for (UnitIt = Units->begin();UnitIt != Units->end();++UnitIt)
        {
          saveUnitFileOutput(&(*UnitIt),i,j,BeginStep,EndStep,OutputDir);
        }

      }
      else
      {
        for (unsigned int iUnits = 0; iUnits < m_OutputConfig.FileSets[i].Sets[j].Units.size();iUnits++)
        {
          saveUnitFileOutput(m_OutputConfig.FileSets[i].Sets[j].Units[iUnits],i,j,BeginStep,EndStep,OutputDir);
        }
      }
    }


  }
  return true;


}


// =====================================================================
// =====================================================================


bool IOManager::saveUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex,
    openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
    std::string OutputDir)
{

  wxFile TFile;
  std::string ScalarsFilename;
  std::string VectorFilename;
  std::string FileContent;
  std::vector<openfluid::core::VariableName_t> VarNames;
  std::string ColSep = m_OutputConfig.FileSets[FileOutputIndex].ColSeparator;

  std::string NameSuffix = m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].Name;

  // processing scalars outputs
  if (m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].AllScalars ||
      m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].ScalarVariables.size() > 0)
  {

    ScalarsFilename = generateOuputFilename(aUnit->getClass(),aUnit->getID(),NameSuffix);

    if (m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].AllScalars)
      FileContent = generateOutputScalarsFileContent(aUnit,
          aUnit->getScalarVariables()->getVariablesNames(),
          BeginStep,EndStep,
          ColSep);
    else
      FileContent = generateOutputScalarsFileContent(aUnit,
          m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].ScalarVariables,
          BeginStep,EndStep,
          ColSep);


    TFile.Open(_U(OutputDir.c_str()) + wxFILE_SEP_PATH + _U(ScalarsFilename.c_str()),wxFile::write_append);
    TFile.Write(_U(FileContent.c_str()));
    TFile.Close();
  }



  // processing vectors outputs
  if (m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].AllVectors ||
      m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].VectorVariables.size() > 0)
  {

    if (m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].AllVectors)
      VarNames = aUnit->getVectorVariables()->getVariablesNames();
    else
      VarNames = m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].VectorVariables;

    for (unsigned int j =  0; j < VarNames.size(); j++)
    {

      VectorFilename = generateOuputFilename(aUnit->getClass(),aUnit->getID(),NameSuffix,VarNames[j]);

      FileContent = generateOutputVectorFileContent(aUnit,VarNames[j],
          BeginStep,EndStep,
          ColSep);

      TFile.Open(_U(OutputDir.c_str()) + wxFILE_SEP_PATH + _U(VectorFilename.c_str()),wxFile::write_append);
      TFile.Write(_U(FileContent.c_str()));
      TFile.Close();

    }

  }


  return true;
}



// =====================================================================
// =====================================================================

std::string IOManager::generateOutputScalarsFileContent(const openfluid::core::Unit* aUnit, const std::vector<std::string> ScalarsNames,
    openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
    std::string ColSeparator)
{
  std::ostringstream GeneratedContent;
  openfluid::core::ScalarValue Value;

  for (unsigned int iStep = BeginStep; iStep <= EndStep; iStep++)
  {
    // TODO replace with correct date
    GeneratedContent << "[Step " << iStep << "]";

    //    std::cout << iStep << std::endl;

    for (unsigned int iVar = 0 ; iVar < ScalarsNames.size(); iVar++ )
    {

      if (!aUnit->getScalarVariables()->getValue(ScalarsNames[iVar],iStep,&Value))
        throw openfluid::base::OFException("kernel","IOManager::generateOutputScalarsFileContent",iStep,"value not found for variable " + ScalarsNames[iVar]);
      GeneratedContent << ColSeparator << Value;
    }

    GeneratedContent << "\n";

  }

  return GeneratedContent.str();
}

// =====================================================================
// =====================================================================

std::string IOManager::generateOutputVectorFileContent(const openfluid::core::Unit* aUnit, const std::string VectorName,
    openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
    std::string ColSeparator)
{
  std::ostringstream GeneratedContent;
  openfluid::core::VectorValue Value;

  for (unsigned int iStep = BeginStep; iStep <= EndStep; iStep++)
  {
    // TODO replace with correct date
    GeneratedContent << "[Step " << iStep << "]";

    //    std::cout << "saving Vector-a " << iStep << std::endl; std::cout.flush();

    if (!aUnit->getVectorVariables()->getValue(VectorName,iStep,&Value))
      throw openfluid::base::OFException("kernel","IOManager::generateOutputVectorFileContent",iStep,"value not found for vector variable " + VectorName);


    for (unsigned int iVal = 0 ; iVal < Value.getSize(); iVal++ )
    {
      //      std::cout << "saving Vector-aa " << iVal << " " << Value.getSize() <<  std::endl; std::cout.flush();
      GeneratedContent << ColSeparator << Value.getElement(iVal);
      //      std::cout << "saving Vector-ab " << iVal << std::endl; std::cout.flush();
    }

    //    std::cout << "saving Vector-b " << iStep << std::endl; std::cout.flush();

    GeneratedContent << "\n";

  }

  return GeneratedContent.str();
}

// =====================================================================
// =====================================================================

bool IOManager::loadEventsFromFiles()
{

 std::vector<std::string> FilesToLoad = GetFilesByExt(mp_RunEnv->getInputDir(),"events.xml",true);

  bool IsOK = true;
  int i=0;


  while (IsOK && i<FilesToLoad.size())
  {
    IsOK =  loadEventsFile(FilesToLoad[i]);
    i++;
  }


  return IsOK;
}


// =====================================================================
// =====================================================================

bool IOManager::loadEventsFile(std::string FullFilename)
{

  bool IsOK = true;


  openfluid::core::DateTime ZeDate;
  std::string ZeDateStr;
  openfluid::core::UnitClass_t UnitClass;
  std::string UnitIDStr;
  std::string InfoKey, InfoValue;

  if (wxFileExists(_U(FullFilename.c_str())))
  {
    TiXmlDocument Doc;
    TiXmlElement* Child, *Child2, *Child3;

    if (Doc.LoadFile(FullFilename.c_str()))
    {

      TiXmlHandle DocHandle(&Doc);

      Child = DocHandle.FirstChild("openfluid").FirstChild("calendar").Element();


      if (Child != NULL)
      {
        TiXmlHandle Child2Handle(Child);
        Child2 = Child2Handle.FirstChild("event").Element();

        // loop on all events in the file

        for(Child2;Child2;Child2=Child2->NextSiblingElement())
        {
          ZeDateStr = "";
          UnitClass = "";
          UnitIDStr = "";

          if (Child2->Attribute("date") != NULL) ZeDateStr = Child2->Attribute("date");
          if (Child2->Attribute("unitclass") != NULL) UnitClass = Child2->Attribute("unitclass");
          if (Child2->Attribute("unitID") != NULL) UnitIDStr = Child2->Attribute("unitID");

          if ((ZeDateStr != "") && (UnitClass != "") && (UnitIDStr != ""))
          {
            openfluid::core::DistributedEvent *DEvent;
            openfluid::core::Unit* TheUnit;
            openfluid::core::UnitID_t ID;


            if (openfluid::tools::ConvertString(UnitIDStr,&ID))
            {
              TheUnit = NULL;

              // get the righ unit

              TheUnit = mp_Repository->getUnit(UnitClass,ID);

              if (TheUnit != NULL)
              {
                ZeDate.setFromISOString(ZeDateStr);

                DEvent = new openfluid::core::DistributedEvent(ZeDate);

                // read infos by event

                TiXmlHandle Child3Handle(Child2);
                Child3 = Child3Handle.FirstChild("info").Element();

                for(Child3;Child3;Child3=Child3->NextSiblingElement())
                {
                  InfoKey = "";
                  InfoValue = "";

                  if (Child3->Attribute("key") != NULL) InfoKey = Child3->Attribute("key");
                  if (Child3->Attribute("value") != NULL) InfoValue = Child3->Attribute("value");

                  if (InfoKey != "") DEvent->addInfo(InfoKey,InfoValue);

                }

                TheUnit->getEvents()->addEvent(DEvent);

              }
            }
          }
        }
      }
      else
      {
        IsOK = false;
        throw openfluid::base::OFException("kernel","IOManager::loadDistributedEventsFile","Event file format error (" + FullFilename + ")");
      }
    }
    else
    {
      IsOK = false;
      throw openfluid::base::OFException("kernel","IOManager::loadDistributedEventsFile","Event file error (" + FullFilename + ")");
    }
  }
  else
  {
    IsOK = false;
    throw openfluid::base::OFException("kernel","IOManager::loadDistributedEventsFile","Event file not found (" + FullFilename + ")");
  }

  return IsOK;

}


// =====================================================================
// =====================================================================

bool IOManager::saveSimulationInfos(ExtraSimInfos ExSI, openfluid::base::SimulationInfo *SimInfo, std::string ErrorMsg)
{


  bool IsOK = true;


  if (prepareOutputDir())
  {

    int i;


    // ********** text file **********

    std::ostringstream FileContents;

    FileContents << "************************************************************" << "\n";
    FileContents << "*                                                          *\n";
    FileContents << "*                     Simulation report                    *" << "\n";
    FileContents << "*                                                          *\n";
    FileContents << "************************************************************" << "\n";
    FileContents << "\n";

    if (ErrorMsg != "")
    {
      FileContents << ErrorMsg << ("\n");
      FileContents << ("\n");
    }

    FileContents << ("Simulation ID: ") << ExSI.SimID << ("\n");
    FileContents << ("Date: ") << _S(ExSI.StartTime.Format(wxT("%Y-%m-%d %H:%M:%S"))) << ("\n");
    FileContents << ("Computer: ") << wxGetHostName() << ("\n");
    FileContents << ("User: ") << wxGetUserId() << (" (") << wxGetUserName() << (")") << ("\n");
    FileContents << ("\n");
    FileContents << ("Input data set: ") << mp_RunEnv->getInputDir() << ("\n");
    FileContents << ("Output data set: ") << mp_RunEnv->getOutputDir()  << ("\n");
    FileContents << ("\n");
    // TODO to complete
    /*    FileContents << ("Surface units (SU): ") << CoreData->getSpatialData()->getSUsCollection()->size() << ("\n");
    FileContents << ("Reach segments (RS): ") << CoreData->getSpatialData()->getRSsCollection()->size() << ("\n");
    FileContents << ("Groundwater units (GU): ") << CoreData->getSpatialData()->getGUsCollection()->size() << ("\n");
*/
    if (SimInfo != NULL)
    {
      FileContents << ("Simulation period: ") << (SimInfo->getStartTime().getAsString(("%Y-%m-%d %H:%M:%S"))) << (" to ") << (SimInfo->getEndTime().getAsString(("%Y-%m-%d %H:%M:%S"))) << ("\n");
      FileContents << ("Time steps: ") << SimInfo->getStepsCount() << (" of ") << SimInfo->getTimeStep() << (" seconds") << ("\n");
    }

    FileContents << ("\n");
    FileContents << ("------------------------------------------------------------") << ("\n");
    FileContents << ("\n");



    std::vector<std::string> WMessages = mp_ExecMsgs->getWarningMsgs();
    int WarningCount = WMessages.size();

    // warnings
    if ( WarningCount > 0)
    {

      for (i=0; i<WarningCount;i++)
      {
        FileContents << ("WARNING: ") << FormatExecutionMessage(WMessages.at(i)) << ("\n");
      }
    }
    else FileContents << ("NO WARNING");
    FileContents << ("\n");
    FileContents << ("------------------------------------------------------------") << ("\n");
    FileContents << ("\n");


    // write file to disk
    wxFile SimInfoFile(_U(mp_RunEnv->getOutputFullPath(OPENFLUID_DEFAULT_SIMINFOFILE).c_str()),wxFile::write);
    SimInfoFile.Write(_U(FileContents.str().c_str()));
    SimInfoFile.Close();


    // ********** xml file **********
    std::ostringstream XMLFileContents;

    XMLFileContents << "<?xml version=\"1.0\" standalone=\"yes\"?>" << "\n";
    XMLFileContents << "<openfluid>" << "\n";
    XMLFileContents << "  <simreport>" << "\n";

    if (ErrorMsg != ("")) XMLFileContents << ("    <error message=\"") << ErrorMsg << ("\" />") << ("\n");

    if (WarningCount > 0)
    {
      XMLFileContents << ("    <warnings count=\"") << WarningCount << ("\">") << ("\n");
      for (i=0; i<WarningCount;i++)
      {
        XMLFileContents << ("      <message text=\"") << FormatExecutionMessage(WMessages.at(i)) << ("\" />") << ("\n");
      }
      XMLFileContents << ("    </warnings>") << ("\n");
    }
    else XMLFileContents << ("    <warnings count=\"0\" />") << ("\n");

    XMLFileContents << ("  </simreport>") << ("\n");
    XMLFileContents << ("</openfluid>") << ("\n");

    wxFile XMLSimInfoFile(_U(mp_RunEnv->getOutputFullPath(OPENFLUID_DEFAULT_SIMINFOFILE).c_str()) + wxT(".xml"),wxFile::write);
    XMLSimInfoFile.Write(_U(XMLFileContents.str().c_str()));
    XMLSimInfoFile.Close();
  }
  else IsOK = false;


  return IsOK;
}




#include "IOMan.h"
#include "tinyxml.h"
#include "AppTools.h"
#include "TypeDefs.h"
#include "ColTextParser.h"
#include "OFException.h"
#include <sstream>
#include <wx/tokenzr.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/datetime.h>

IOManager* IOManager::mp_Singleton = NULL;

#ifdef wxUSE_UNICODE
    #define _U(x) wxString(x,wxConvUTF8) // char* to wxString
    #define _UU(x,y) wxString(x,y)       // char* to wxString
    #define _C(x) (x).mb_str(wxConvUTF8) // wxString to char*
    #define _CC(x,y) (x).mb_str(y)       // wxString to char*
    #define _S(x) (std::string((x).mb_str(wxConvUTF8))) //wxString to std::string
#else
    #define _U(x) (x)          // char* to wxString
    #define _C(x) (x).c_str()  // wxString to char*
    #define _S(x) (std::string((x).c_str()))//wxString to std::string
#endif


// TODO use wxString in methods arguments


// =====================================================================
// =====================================================================

IOManager* IOManager::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new IOManager();

  return mp_Singleton;
}

// =====================================================================
// =====================================================================


bool IOManager::loadDomainFile(wxString FullFilename, std::list<openfluid::core::UnitsLink_t>* ToUnitsList)
{

  TiXmlDocument LoadDoc;

  std::cout << "Loading file " << FullFilename.mb_str(wxConvUTF8) << std::endl;

  if (LoadDoc.LoadFile(FullFilename.mb_str(wxConvUTF8)))
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
              throw openfluid::base::OFException("kernel","IOManager::LoadDomainFile","Error loading file " + std::string(FullFilename.mb_str(wxConvUTF8)) + ", wrong attributes in definition of topology");
              return false;
            }
          }
        }
        else
        {
          throw openfluid::base::OFException("kernel","IOManager::LoadDomainFile","Error loading file " + std::string(FullFilename.mb_str(wxConvUTF8)) + ", wrong attributes in unit definition");
          return false;
        }
      }

//      Repository->printSTDOUT();

    }
    else
    {
      throw openfluid::base::OFException("kernel","IOManager::LoadDomainFile","Error loading file " + std::string(FullFilename.mb_str(wxConvUTF8)) + ", wrong file format");
      return false;
    }
  }
  else
  {
    throw openfluid::base::OFException("kernel","IOManager::LoadDomainFile","Error loading file " + std::string(FullFilename.mb_str(wxConvUTF8)));
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

  wxArrayString FilesToLoad = GetFilesByExt(wxT("."),wxT("ddef.xml"),true);

  bool IsOK = true;
  unsigned int i=0;


  while (IsOK && i<FilesToLoad.GetCount())
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


bool IOManager::loadInputDataFile(wxString FullFilename)
{
  std::cout << "Loading file " << FullFilename.mb_str(wxConvUTF8) << std::endl;

  TiXmlDocument LoadDoc;
  wxString Data;
  wxArrayString ColOrder;
  wxString UnitClass;



  if (LoadDoc.LoadFile(FullFilename.mb_str(wxConvUTF8)))
  {

    TiXmlHandle DocHandle(&LoadDoc);
    TiXmlElement* Child;

    Child = DocHandle.FirstChild("openfluid").FirstChild("domain").FirstChild("inputdata").Element();

    if (Child != NULL)
    {
      if (Child->Attribute("unitclass") != NULL)
      {

        UnitClass = _U(Child->Attribute("unitclass"));

        if (!mp_Repository->isUnitsClassExists(_S(UnitClass)))
          throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Class "+ _S(UnitClass) +" found in " + _S(FullFilename) + " does not exist");

        Child = DocHandle.FirstChild("openfluid").FirstChild("domain").FirstChild("inputdata").FirstChild("columns").Element();

        // columns
        if (Child != NULL)
        {
          if (Child->Attribute("order") != NULL)
          {
            wxStringTokenizer Tkz(_U(Child->Attribute("order")), wxT(";"));

            while (Tkz.HasMoreTokens())
            {
              ColOrder.Add(Tkz.GetNextToken());
            }

            // data
            if (ColOrder.GetCount() > 0)
            {
              Child = DocHandle.FirstChild("openfluid").FirstChild("domain").FirstChild("inputdata").FirstChild("data").Element();

              if (Child != NULL)
              {
//                Data.Clear();
//                Data.Append(_U(Child->GetText()));
              }
              else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","No data found in file " + _S(FullFilename));
            }
            else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Column order is empty in file " + _S(FullFilename));
          }
          else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Column order not found in file " + _S(FullFilename));
        }
        else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Column definition not found in file " + _S(FullFilename));
      }
      else throw openfluid::base::OFException("kernel","IOManager::loadDistributedDataFile","Unit class not found in file " + _S(FullFilename));
    }
    else
    {
      throw openfluid::base::OFException("kernel","IOManager::LoadInputDataFile","Error loading file " + std::string(FullFilename.mb_str(wxConvUTF8)) + ", wrong file format");
      return false;
    }
  }
  else
  {
    throw openfluid::base::OFException("kernel","IOManager::LoadInputDataFile","Error loading file " + std::string(FullFilename.mb_str(wxConvUTF8)));
    return false;
  }


  openfluid::tools::ColumnTextParser DataParser(wxT("%"));

  if (DataParser.setFromString(Data,ColOrder.Count()+1))
  {
    //TODO to be continued here
  }
  else
  {
    throw openfluid::base::OFException("kernel","IOManager::LoadInputDataFile","Error loading file " + std::string(FullFilename.mb_str(wxConvUTF8)) + ",cannot parse data");
    return false;
  }



  return true;
}


// =====================================================================
// =====================================================================



bool IOManager::loadInputDataFromFiles()
{
  wxArrayString FilesToLoad = GetFilesByExt(wxT("."),wxT("ddata.xml"),true);

  bool IsOK = true;
  unsigned int i=0;


  while (IsOK && i<FilesToLoad.GetCount())
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

  wxString Str;
  unsigned int i;
  long LongValue;

  std::cout << "Loading file ./output.xml"<< std::endl;



  if (LoadDoc.LoadFile("./output.xml"))
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
              wxArrayString StrArray = SplitString(_U(IDStr.c_str()),wxT(";"));
              for (i=0;i<StrArray.Count();i++)
              {
                if (StrArray[i].ToLong(&LongValue))
                {
                  TmpUnit = mp_Repository->getUnit(ClassStr,(openfluid::core::UnitID_t)LongValue);
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
              wxArrayString StrArray = SplitString(_U(VarsStr.c_str()),wxT(";"));
              for (i=0;i<StrArray.Count();i++)
              {
                if (IsVectorNamedVariable(StrArray[i])) FileOutputSet.VectorVariables.push_back(_S(GetVectorNamedVariableName(StrArray[i])));
                FileOutputSet.ScalarVariables.push_back(_S(StrArray[i]));
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


bool IOManager::prepareUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex, wxString OutputDir)
{
/*  std::ostringstream RootFilename;
  std::ostringstream FileContent;

  RootFilename << m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].UnitsClass
           << aUnit->getID() << "_" << m_OutputConfig.FileSets[FileOutputIndex].Sets[OutputSetIndex].Name;
*/

  wxFile TFile;
  wxString ScalarsFilename;
  wxString VectorFilename;
  wxString FileContent;
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


    TFile.Open(OutputDir + wxT("/") + ScalarsFilename,wxFile::write_append);
    TFile.Write(FileContent);
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

      TFile.Open(OutputDir + wxT("/") + VectorFilename,wxFile::write_append);
      TFile.Write(FileContent);
      TFile.Close();

    }

  }


  return true;

}


// =====================================================================
// =====================================================================


bool IOManager::prepareOutputs()
{
  wxString OutputDir = wxT("./OPENFLUID.OUT");

  openfluid::core::UnitsList_t* Units;
  openfluid::core::UnitsList_t::iterator UnitIt;


  if (!wxDirExists(OutputDir))
  {
#ifdef __WXMSW__
    wxMkDir(OutputDir.mb_str(wxConvUTF8));
#else
    wxMkDir(OutputDir.mb_str(wxConvUTF8),0777);
#endif
    if (!wxDirExists(OutputDir))
     throw openfluid::base::OFException("kernel","IOManager::prepareOutputs","Error creating output directory");

  }
  else
  {
  // TODO to enable
    /*    if (mp_RunEnv->isClearOutputDir() && !m_ClearedOuputDir)
    {
      EmptyDirectoryRecursively(mp_RunEnv->getOutputDir().mb_str(wxConvUTF8));
      m_ClearedOuputDir = true;
    }*/
  }

  // TODO to remove
  EmptyDirectoryRecursively(OutputDir.mb_str(wxConvUTF8));

//  std::cout << "configs de sortie fichier: " << m_OutputConfig.FileSets.size() << std::endl;

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
          prepareUnitFileOutput(&(*UnitIt),i,j,OutputDir);
        }

      }
      else
      {
        for (unsigned int iUnits = 0; iUnits < m_OutputConfig.FileSets[i].Sets[j].Units.size();iUnits++)
        {
          prepareUnitFileOutput(m_OutputConfig.FileSets[i].Sets[j].Units[iUnits],i,j,OutputDir);
        }
      }
    }


  }
  return true;
}


// =====================================================================
// =====================================================================


wxString IOManager::generateOuputFilename(const std::string UnitClass, const openfluid::core::UnitID_t UnitID,
                                          const std::string Suffix, const std::string VectorName)
{
  wxString GeneratedName = wxT("");

  GeneratedName << _U(UnitClass.c_str()) << UnitID << wxT("_") << _U(Suffix.c_str());

  if (VectorName == "")
  {
    // TODO put file extension from config file
    GeneratedName << wxT(".scalars.out");
  }
  else
  {
    // TODO put file extension from config file
    GeneratedName << wxT(".vector.") << _U(VectorName.c_str()) << wxT(".out");
  }

  return GeneratedName;
}


// =====================================================================
// =====================================================================


wxString IOManager::generateOutputScalarsFileHeader(const std::string SimulationID, const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
                                                    const wxString Filename, const std::vector<std::string> ScalarsNames,
                                                    const std::string CommentChar)
{
  wxString GeneratedHeader = wxT("");

  GeneratedHeader << _U(CommentChar.c_str()) << wxT(" simulation ID: ") << _U(SimulationID.c_str()) << wxT("\n")
                  << _U(CommentChar.c_str()) << wxT(" file: ") << Filename << wxT("\n")
                  << _U(CommentChar.c_str()) << wxT(" date: ") << wxDateTime::Now().Format() << wxT("\n")
                  << _U(CommentChar.c_str()) << wxT(" unit: ") << _U(UnitClass.c_str()) << wxT(" #") << UnitID << wxT("\n")
                  << _U(CommentChar.c_str()) << wxT(" scalar variables order (after date and time columns):");


  for (unsigned int i = 0 ; i < ScalarsNames.size(); i++ ) GeneratedHeader << wxT(" ") << _U(ScalarsNames[i].c_str());


  GeneratedHeader << wxT("\n") << wxT("\n");

  return GeneratedHeader;

}


// =====================================================================
// =====================================================================


wxString IOManager::generateOutputVectorFileHeader(std::string SimulationID, openfluid::core::UnitClass_t UnitClass, openfluid::core::UnitID_t UnitID,
                                                   wxString Filename, std::string VectorName,
                                                   std::string CommentChar)
{
  wxString GeneratedHeader = wxT("");

  GeneratedHeader << _U(CommentChar.c_str()) << wxT(" simulation ID: ") << _U(SimulationID.c_str()) << wxT("\n")
                  << _U(CommentChar.c_str()) << wxT(" file: ") << Filename << wxT("\n")
                  << _U(CommentChar.c_str()) << wxT(" date: ") << wxDateTime::Now().Format() << wxT("\n")
                  << _U(CommentChar.c_str()) << wxT(" unit: ") << _U(UnitClass.c_str()) << wxT(" #") << UnitID << wxT("\n")
                  << _U(CommentChar.c_str()) << wxT(" vector variable: ") << _U(VectorName.c_str()) << wxT("\n")
                  << _U(CommentChar.c_str()) << wxT(" the vector values are ordered left to right, from index 0 to size-1 (after date and time columns)") << wxT("\n") << wxT("\n");


  return GeneratedHeader;
}


// =====================================================================
// =====================================================================


bool IOManager::saveOutputs(openfluid::core::TimeStep_t CurrentStep, bool WithoutKeep)
{
  wxString OutputDir = wxT("./OPENFLUID.OUT");

  openfluid::core::UnitsList_t* Units;
  openfluid::core::UnitsList_t::iterator UnitIt;


  openfluid::core::TimeStep_t BeginStep, EndStep;
  mp_MemMonitor->getMemoryReleaseRange(CurrentStep,WithoutKeep,&BeginStep,&EndStep);

  std::cout << "Saving to disk from " << BeginStep << " to " << EndStep << std::endl;

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
                                   wxString OutputDir)
{

  wxFile TFile;
    wxString ScalarsFilename;
    wxString VectorFilename;
    wxString FileContent;
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


      TFile.Open(OutputDir + wxT("/") + ScalarsFilename,wxFile::write_append);
      TFile.Write(FileContent);
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

        TFile.Open(OutputDir + wxT("/") + VectorFilename,wxFile::write_append);
        TFile.Write(FileContent);
        TFile.Close();

      }

    }


  return true;
}



// =====================================================================
// =====================================================================

wxString IOManager::generateOutputScalarsFileContent(const openfluid::core::Unit* aUnit, const std::vector<std::string> ScalarsNames,
                                                     openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                                     std::string ColSeparator)
{
  wxString GeneratedContent;
  openfluid::core::ScalarValue Value;

  for (unsigned int iStep = BeginStep; iStep <= EndStep; iStep++)
  {
    // TODO replace with correct date
    GeneratedContent << wxT("[Step ") << iStep << wxT("]");

//    std::cout << iStep << std::endl;

    for (unsigned int iVar = 0 ; iVar < ScalarsNames.size(); iVar++ )
    {

      if (!aUnit->getScalarVariables()->getValue(ScalarsNames[iVar],iStep,&Value))
        throw openfluid::base::OFException("kernel","IOManager::generateOutputScalarsFileContent",iStep,"value not found for variable " + ScalarsNames[iVar]);
      GeneratedContent << _U(ColSeparator.c_str()) << Value;
    }

    GeneratedContent << wxT("\n");

  }

  return GeneratedContent;
}

// =====================================================================
// =====================================================================

wxString IOManager::generateOutputVectorFileContent(const openfluid::core::Unit* aUnit, const std::string VectorName,
                                                     openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                                     std::string ColSeparator)
{
  wxString GeneratedContent;
  openfluid::core::VectorValue Value;

  for (unsigned int iStep = BeginStep; iStep <= EndStep; iStep++)
  {
    // TODO replace with correct date
    GeneratedContent << wxT("[Step ") << iStep << wxT("]");

//    std::cout << "saving Vector-a " << iStep << std::endl; std::cout.flush();

    if (!aUnit->getVectorVariables()->getValue(VectorName,iStep,&Value))
      throw openfluid::base::OFException("kernel","IOManager::generateOutputVectorFileContent",iStep,"value not found for vector variable " + VectorName);


    for (unsigned int iVal = 0 ; iVal < Value.getSize(); iVal++ )
    {
//      std::cout << "saving Vector-aa " << iVal << " " << Value.getSize() <<  std::endl; std::cout.flush();
      GeneratedContent << _U(ColSeparator.c_str()) << Value.getElement(iVal);
//      std::cout << "saving Vector-ab " << iVal << std::endl; std::cout.flush();
    }

//    std::cout << "saving Vector-b " << iStep << std::endl; std::cout.flush();

    GeneratedContent << wxT("\n");

  }

  return GeneratedContent;
}



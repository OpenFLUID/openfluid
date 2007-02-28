/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include <wx/filefn.h>
#include <wx/tokenzr.h>

#include <iostream>

#include "IOMan.h"
#include "ColTextParser.h"
#include "setup.h"
#include "AppTools.h"
#include "xml/tinyxml.h"


// =====================================================================
// =====================================================================


#include <wx/listimpl.cpp>
WX_DEFINE_LIST(FunctionConfigsList);

/*#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(ArrayOfAutoOutfileDefs);*/


// =====================================================================
// =====================================================================



IOManager::IOManager(mhydasdk::base::RuntimeEnvironment* RunEnv)
{
  mp_RunEnv = RunEnv;
}

// =====================================================================
// =====================================================================

IOManager::~IOManager()
{

}




// =====================================================================
// =====================================================================


bool IOManager::loadModelConfig(EngineConfig* Config)
{

  // à terminer

  TiXmlDocument LoadDoc;
  TiXmlElement* Child;
  TiXmlElement* Child2;

  wxString Str;
  double DoubleValue;
  int IntValue;

  FunctionConfig* FConf;

  if (LoadDoc.LoadFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_CONFFILE).mb_str(wxConvUTF8)))
  {

    TiXmlHandle DocHandle(&LoadDoc);


    // =========== run params ===========
    Child = DocHandle.FirstChild("mhydas").FirstChild("model").FirstChild("runparams").FirstChild("param").Element();


    for(Child;Child;Child=Child->NextSiblingElement())
    {
      if (Child->Attribute("name") != NULL && wxString(Child->Attribute("name"),wxConvUTF8) == wxT("deltat") &&
		  Child->Attribute("value",&IntValue) != NULL)
	    {
        Config->DeltaT = IntValue;
	    }

    }


    // ======= model structure ===========

    // hydromodule
    Child = DocHandle.FirstChild("mhydas").FirstChild("model").FirstChild("module").FirstChild("function").Element();


    for(Child;Child;Child=Child->NextSiblingElement())
	  {
      if (Child->Attribute("ID") != NULL  &&
          Child->Attribute("file") != NULL )
      {


        // function name and file
        FConf = new FunctionConfig();
        FConf->Name = wxString(Child->Attribute("ID"),wxConvUTF8);
        FConf->File = wxString(Child->Attribute("file"),wxConvUTF8);


        if ((FConf->Name.Length() > 0) && (FConf->File.Length() > 0))
        {

          FConf->Params.clear();

          // function params parsing
          TiXmlHandle Child2Handle(Child);
          Child2 = Child2Handle.FirstChild("param").Element();

          for(Child2;Child2;Child2=Child2->NextSiblingElement())
          {
            if (Child2->Attribute("name") != NULL && Child2->Attribute("value",&DoubleValue) != NULL)
            {
              FConf->Params[wxString(Child2->Attribute("name"),wxConvUTF8)] = DoubleValue;

            }

          }

          Config->ModuleConfig.Append(FConf);

        }
        else delete FConf;

      }
      else
      {
        mhydasdk::base::LastError::Message = wxT("Model config file (") + MHYDAS_DEFAULT_CONFFILE + wxT(") error. Incorrect function definition.");
        return false;
      }  
       

    }



  }
  else
  {
    mhydasdk::base::LastError::Message = wxT("Model config file (") + MHYDAS_DEFAULT_CONFFILE + wxT(") error.");
    return false;
  }

  return true;

}



// =====================================================================
// =====================================================================

mhydasdk::core::SUDownstreamCode IOManager::getSUDownstreamCode(wxString Code)
{
  if (Code == wxT("S")) return mhydasdk::core::SUDownstream;
  if (Code == wxT("R")) return mhydasdk::core::RLatDownstream;

  return mhydasdk::core::UnknownDownstreamCode;
}


// =====================================================================
// =====================================================================


bool IOManager::loadHydroObjects(mhydasdk::core::SpatialRepository *SpatialData)
{

  /** \internal

    the load process is quite same for each kind of objects:
      - for each file line, each column is parsed and checked.
      - if everithing's OK, it builds the new spatial object and adds it to the collection

  */


  if (SpatialData == NULL) return false;

  long ID;
  long ProcessOrder;
  long GUExch;

  int i;

  wxString SUsFilename = mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_SUDEFSFILE);
  bool SUsFileExists = wxFileExists(SUsFilename);

  wxString RSsFilename = mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_RSDEFSFILE);
  bool RSsFileExists = wxFileExists(RSsFilename);

  wxString GUsFilename = mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_GUDEFSFILE);
  bool GUsFileExists = wxFileExists(GUsFilename);


  // no available object
  if (!(SUsFileExists || RSsFileExists || GUsFileExists))
  {
    mhydasdk::base::LastError::Message = wxT("No spatial object file.");
    return false;
  }


  // ============ SUs =================


  if (SUsFileExists)
  {
    ColumnTextParser SUsFileParser(wxT("%"));

    //  checks that files has the right column number
    if (!SUsFileParser.loadFromFile(SUsFilename) ||
        SUsFileParser.getColsCount() != MHYDAS_SUDEFSFILE_COLNBR ||
        SUsFileParser.getColsCount() < 1)
    {

      std::cerr << SUsFileParser.getColsCount() << std::endl;
      mhydasdk::base::LastError::Message = MHYDAS_DEFAULT_SUDEFSFILE + wxT(" file parsing error.");
      return false;
    }
    else
    {

      double Area, Slope, FlowDist;
      wxString FlowCode;
      long FlowID;

      for (i=0;i<SUsFileParser.getLinesCount();i++)
      {
        // for each line, each column is parsed and checked

        if (SUsFileParser.getLongValue(i,0,&ID) && SUsFileParser.getDoubleValue(i,1,&Area) &&
            SUsFileParser.getDoubleValue(i,2,&Slope) && SUsFileParser.getStringValue(i,3,&FlowCode) &&
            SUsFileParser.getLongValue(i,4,&FlowID) && SUsFileParser.getDoubleValue(i,5,&FlowDist) &&
            SUsFileParser.getLongValue(i,6,&GUExch) && SUsFileParser.getLongValue(i,7,&ProcessOrder))
        {
          // if everithing's OK, builds the new spatial object and adds it to the collection

          if (!SpatialData->addSU(new mhydasdk::core::SurfaceUnit((mhydasdk::core::hoid_t)ID,
                                                                  (mhydasdk::core::hoid_t)ProcessOrder,
                                                                  Area,
                                                                  Slope,
                                                                  getSUDownstreamCode(FlowCode),
                                                                  (mhydasdk::core::hoid_t)FlowID,
                                                                  FlowDist,
                                                                  (mhydasdk::core::hoid_t)GUExch)))
          {
            mhydasdk::base::LastError::Message = wxT("Error adding SU #")+wxString::Format(wxT("%d"),ID)+wxT(". Maybe alredy in use.");
            return false;
          }

        }

        else
        {
          mhydasdk::base::LastError::Message = MHYDAS_DEFAULT_SUDEFSFILE + wxT(" file format error.");
          return false;
        }

      }
    }
  }


  // ============ RSs =================

  if (RSsFileExists)
  {
    ColumnTextParser RSsFileParser(wxT("%"));

    if (!RSsFileParser.loadFromFile(RSsFilename) ||
        RSsFileParser.getColsCount() != MHYDAS_RSDEFSFILE_COLNBR ||
        RSsFileParser.getColsCount() < 1)
    {
      mhydasdk::base::LastError::Message = MHYDAS_DEFAULT_RSDEFSFILE + wxT(" file parsing error.");
      return false;
    }
    else
    {
      long int UpNode, DownNode, LowRSID;
      double Length, Width, Height, Slope;

      for (i=0;i<RSsFileParser.getLinesCount();i++)
      {

        if (RSsFileParser.getLongValue(i,0,&ID) && RSsFileParser.getLongValue(i,1,&UpNode) &&
            RSsFileParser.getLongValue(i,2,&DownNode) && RSsFileParser.getLongValue(i,3,&LowRSID) &&
            RSsFileParser.getDoubleValue(i,4,&Length) && RSsFileParser.getDoubleValue(i,5,&Width) &&
            RSsFileParser.getDoubleValue(i,6,&Height) && RSsFileParser.getDoubleValue(i,7,&Slope) &&
            RSsFileParser.getLongValue(i,8,&GUExch) && RSsFileParser.getLongValue(i,9,&ProcessOrder))
        {


          if (!SpatialData->addRS(new mhydasdk::core::ReachSegment((mhydasdk::core::hoid_t)ID,
                                                                   (mhydasdk::core::hoid_t)ProcessOrder,
                                                                   (mhydasdk::core::nodeid_t)UpNode,
                                                                   (mhydasdk::core::nodeid_t)DownNode,
                                                                   (mhydasdk::core::hoid_t)LowRSID,
                                                                   Slope,
                                                                   Length,
                                                                   Width,
                                                                   Height,
                                                                   (mhydasdk::core::hoid_t)GUExch)))
          {
            mhydasdk::base::LastError::Message = wxT("Error adding RS #")+wxString::Format(wxT("%d"),ID)+wxT(". Maybe alredy in use.");
            return false;
          }
        }
        else
        {
          mhydasdk::base::LastError::Message = MHYDAS_DEFAULT_RSDEFSFILE + wxT(" file format error.");
          return false;
        }
      }
    }
  }


  // ============ GUs =================

  if (GUsFileExists)
  {
    ColumnTextParser GUsFileParser(wxT("%"));

    if (!GUsFileParser.loadFromFile(GUsFilename) ||
        GUsFileParser.getColsCount() != MHYDAS_GUDEFSFILE_COLNBR ||
        GUsFileParser.getColsCount() < 1)
    {
      mhydasdk::base::LastError::Message = MHYDAS_DEFAULT_GUDEFSFILE + wxT(" file parsing error.");
      return false;
    }
    else
    {

      double SubstrLevel;

      for (i=0;i<GUsFileParser.getLinesCount();i++)
      {

        if (GUsFileParser.getLongValue(i,0,&ID) && GUsFileParser.getDoubleValue(i,1,&SubstrLevel) &&
            GUsFileParser.getLongValue(i,2,&GUExch) && GUsFileParser.getLongValue(i,3,&ProcessOrder))
        {

          if (!SpatialData->addGU(new mhydasdk::core::GroundwaterUnit((mhydasdk::core::hoid_t)ID,
                                                                      (mhydasdk::core::hoid_t)ProcessOrder,
                                                                      (mhydasdk::core::hoid_t)GUExch,
                                                                      SubstrLevel)))
          {
            mhydasdk::base::LastError::Message = wxT("Error adding GU ")+wxString::Format(wxT("%d"),ID)+wxT(". Maybe alredy in use.");
            return false;
          }
        }
        else
        {
          mhydasdk::base::LastError::Message = MHYDAS_DEFAULT_GUDEFSFILE + wxT(" file format error.");
          return false;
        }
      }
    }
  }


  return true;

}



// =====================================================================
// =====================================================================


RainEventFilesMap IOManager::buildRainEventFileMap()
{
  /** \internal

    the rain event description is in an XML file.
    the XML file is parsed and for each rain source,
    an (ID,file) pair is added to the map

  */

  RainEventFilesMap RIFMap;
  TiXmlDocument LoadDoc;

  RIFMap.clear();

  int ID;
  wxString Filename;



  if (LoadDoc.LoadFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_RAINEVTFILE).mb_str(wxConvUTF8)))
  {

    TiXmlHandle DocHandle(&LoadDoc);

    TiXmlElement* Child = DocHandle.FirstChild("mhydas").FirstChild("rainevent").FirstChild("rainsource").Element();


     for(Child; Child; Child=Child->NextSiblingElement())
    {
      if (Child->Attribute("ID",&ID) != NULL && Child->Attribute("file") != NULL)
      {
        RIFMap[ID] = wxString(Child->Attribute("file"),wxConvUTF8);
      }
    }


  }
  else
  {
    mhydasdk::base::LastError::Message = wxT("Rain event file (") + MHYDAS_DEFAULT_RAINEVTFILE + wxT(") error.");
  }
  return RIFMap;

}



// =====================================================================
// =====================================================================


bool IOManager::loadRainFile(mhydasdk::core::RainEvent *RainData, mhydasdk::core::cdsid_t ID, wxString Filename)
{
  /** \internal

    rain data file is a 9 columns text file (YYYY MM DD hh mm ss Value)
    for each file line, each column is parsed and checked.
    if everithing's OK, it builds a new time serie value, added to the serie

  */

  ColumnTextParser FileParser(wxT("%"));
  bool IsOK;

  IsOK = true;

  long int Year, Month, Day, Hour, Min, Sec;
  double Value;

  mhydasdk::core::TimeSerie *Serie;

  if (FileParser.loadFromFile(mp_RunEnv->getInputFullPath(Filename)) && (FileParser.getLinesCount() > 0) && (FileParser.getColsCount() == 7))
  {

    Serie = new mhydasdk::core::TimeSerie("m/s");        

    int i = 0;

    while (i<FileParser.getLinesCount() && IsOK)
    {
      if (FileParser.getLongValue(i,0,&Year) && FileParser.getLongValue(i,1,&Month) &&
          FileParser.getLongValue(i,2,&Day) && FileParser.getLongValue(i,3,&Hour) &&
          FileParser.getLongValue(i,4,&Min) && FileParser.getLongValue(i,5,&Sec) &&
          FileParser.getDoubleValue(i,6,&Value))
      {

        #ifdef INPUT_RAIN_MMH
        Value = Value * SIFACT_MMH_TO_MS;
        #endif
        Serie->addValue(new mhydasdk::core::TimeSerieItem(Year,Month,Day,Hour,Min,Sec,Value));

      }

      else
      {
        IsOK = false;
        mhydasdk::base::LastError::Message = wxT("Rain file content error (") + Filename + wxT(").");
      }

      i++;
    }

    if (IsOK)
    {
      mhydasdk::core::ChronDataSource* Source = new mhydasdk::core::ChronDataSource(ID);
      Source->setTimeSerie(Serie);
      IsOK = RainData->addRainSource(Source);
      if (!IsOK) mhydasdk::base::LastError::Message = wxT("Error adding rain source (ID #") + wxString::Format(wxT("%d"),ID) + wxT(").");
    }

  }
  else
  {
    IsOK = false;
    mhydasdk::base::LastError::Message = wxT("Rain file format error (") + Filename + wxT(").");
  }


  return IsOK;
}


// =====================================================================
// =====================================================================



bool IOManager::loadRainEvent(mhydasdk::core::RainEvent *RainData)
{
  RainEventFilesMap RIFMap = buildRainEventFileMap();

  if (RIFMap.size() == 0) return false;
  else
  {
    RainEventFilesMap::iterator RIFit;
    for(RIFit = RIFMap.begin(); RIFit != RIFMap.end(); ++RIFit )
    {
      if (!loadRainFile(RainData,RIFit->first,RIFit->second))
      {
        mhydasdk::base::LastError::Message = wxT("Error loading ") + RIFit->second + wxT(" rain file.");
        return false;
      }
    }

  }
  return true;

}


// =====================================================================
// =====================================================================


bool IOManager::loadRainDistribution(mhydasdk::core::CoreRepository *Data)
{
  if (Data == NULL) return false;

  long SUID;
  long RainID;

  int i;

  ColumnTextParser DistriFileParser(wxT("%"));

  if (!DistriFileParser.loadFromFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_RAINDISTRIFILE)) ||
      DistriFileParser.getColsCount() != MHYDAS_RAINDISTRIFILE_COLNBR ||
      DistriFileParser.getColsCount() < 1)
  {
    mhydasdk::base::LastError::Message = MHYDAS_DEFAULT_RAINDISTRIFILE + wxT(" file parsing error.");
    return false;
  }
  else
  {
    mhydasdk::core::SurfaceUnit *CurrentSU;
    mhydasdk::core::ChronDataSource *CurrentRainGauge;

    for (i=0;i<DistriFileParser.getLinesCount();i++)
    {

      if (DistriFileParser.getLongValue(i,0,&SUID) && DistriFileParser.getLongValue(i,1,&RainID))
      {
        CurrentSU = Data->getSpatialData()->getSUByID(SUID);
        CurrentRainGauge = Data->getRainEvent()->getRainSourceByID(RainID);
        if ((CurrentSU != NULL) && (CurrentRainGauge != NULL))
        {
          CurrentSU->setRainSource(CurrentRainGauge);
        }
        else
        {
          mhydasdk::base::LastError::Message = wxT("Matching error between SU and rain source.");
          return false;
        }
      }
      else
      {
        mhydasdk::base::LastError::Message = wxT("Rain distribution contents error (") + MHYDAS_DEFAULT_RAINDISTRIFILE + wxT(").");
        return false;
      }
    }

  }
  return true;
}


// =====================================================================
// =====================================================================


bool IOManager::extractColumnOrderAndDataFromFile(wxString Filename, wxString SpecTag,
                                                  wxArrayString* ColOrder, wxString* Data)
{

  bool IsOK = true;

  if (wxFileExists(Filename))
  {
    TiXmlDocument Doc;
    TiXmlElement* Child;

    if (Doc.LoadFile(_C(Filename)))
    {

      TiXmlHandle DocHandle(&Doc);
      Child = DocHandle.FirstChild("mhydas").FirstChild(_C(SpecTag)).FirstChild("columns").Element();

      // columns
      if (Child != NULL)
      {
        if (Child->Attribute("order") != NULL)
        {
          wxStringTokenizer Tkz(_U(Child->Attribute("order")), wxT(";"));

          while (Tkz.HasMoreTokens())
          {
            ColOrder->Add(Tkz.GetNextToken());
          }

          // data
          if (ColOrder->GetCount() > 0)
          {
            Child = DocHandle.FirstChild("mhydas").FirstChild(_C(SpecTag)).FirstChild("data").Element();

            if (Child != NULL)
            {
              Data->Clear();
              Data->Append(_U(Child->GetText()));
            }
            else IsOK = false;
          }
          else IsOK = false;
        }
        else IsOK = false;
      }
      else IsOK = false;
    }
  }

  return IsOK;

}


// =====================================================================
// =====================================================================

bool IOManager::loadHydroObjectsProperties(mhydasdk::core::SpatialRepository *SpatialData)
{
  wxArrayString Columns;
  wxString Data;
  int i, j;
  bool IsOK = true;
  long ID;
  double Value;

  /*

    essayer de trouver une macro du type
    LOAD_DISTRIBUTED_PARAMS_FILE(file,HOstr,HO,getparamsfunc,columns,data)

  */

  // ============== SUs =========================

  Columns.Clear();
  Data.Clear();

  // extracts column order
  if (extractColumnOrderAndDataFromFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_SUPROPSFILE),
                                        wxT("SUprops"),&Columns,&Data))
  {

    ColumnTextParser SUProps(wxT("%"));

    if (SUProps.setFromString(Data,Columns.Count()+1))
    {
      mhydasdk::core::SurfaceUnit* SU;

      i = 0;

      // parses file and loads it in the properties hash table, ordered by columns
      while (i<SUProps.getLinesCount() && IsOK)
      {
        IsOK = SUProps.getLongValue(i,0,&ID);
        SU = SpatialData->getSUByID((int)ID);
        if (IsOK && SU != NULL)
        {
          for (j=1;j<SUProps.getColsCount();j++)
          {
            if (SUProps.getDoubleValue(i,j,&Value))
            {
              SU->getProperties()->insert(mhydasdk::core::ParamsMap::value_type(Columns[j-1],Value));
            }
          }
        }
        else return false;
        i++;
      }
    }
    else
    {
      mhydasdk::base::LastError::Message = wxT("SU distributed properties data error (") + MHYDAS_DEFAULT_SUPROPSFILE + wxT(").");
      return false;
    }
  }
  else
  {
    mhydasdk::base::LastError::Message = wxT("SU distributed properties file error (") + MHYDAS_DEFAULT_SUPROPSFILE + wxT(").");
    return false;
  }

  // ============== RSs =========================

  Columns.Clear();
  Data.Clear();


  if (extractColumnOrderAndDataFromFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_RSPROPSFILE),
                                        wxT("RSprops"),&Columns,&Data))
  {

    ColumnTextParser RSProps(wxT("%"));

    if (RSProps.setFromString(Data,Columns.Count()+1))
    {
      mhydasdk::core::ReachSegment* RS;

      i = 0;

      while (i<RSProps.getLinesCount() && IsOK)
      {
        IsOK = RSProps.getLongValue(i,0,&ID);
        RS = SpatialData->getRSByID((int)ID);
        if (IsOK && RS != NULL)
        {
          for (j=1;j<RSProps.getColsCount();j++)
          {
            if (RSProps.getDoubleValue(i,j,&Value))
            {
              RS->getProperties()->insert(mhydasdk::core::ParamsMap::value_type(Columns[j-1],Value));
            }
          }
        }
        else return false;
        i++;
      }
    }
    else
    {
      mhydasdk::base::LastError::Message = wxT("RS distributed properties data error (") + MHYDAS_DEFAULT_RSPROPSFILE + wxT(").");
      return false;
    }
  }
  else
  {
    mhydasdk::base::LastError::Message = wxT("RS distributed properties file error (") + MHYDAS_DEFAULT_RSPROPSFILE + wxT(").");
    return false;
  }


  // ============== GUs =========================

  Columns.Clear();
  Data.Clear();

  if (extractColumnOrderAndDataFromFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_GUPROPSFILE),
                                        wxT("GUprops"),&Columns,&Data))
  {

    ColumnTextParser GUProps(wxT("%"));

    if (GUProps.setFromString(Data,Columns.Count()+1))
    {
      mhydasdk::core::GroundwaterUnit* GU;

      i = 0;

      while (i<GUProps.getLinesCount() && IsOK)
      {
        IsOK = GUProps.getLongValue(i,0,&ID);
        GU = SpatialData->getGUByID((int)ID);
        if (IsOK && GU != NULL)
        {
          for (j=1;j<GUProps.getColsCount();j++)
          {
            if (GUProps.getDoubleValue(i,j,&Value))
            {
              GU->getProperties()->insert(mhydasdk::core::ParamsMap::value_type(Columns[j-1],Value));
            }
          }
        }
        else return false;
        i++;
      }
    }
    else
    {
      mhydasdk::base::LastError::Message = wxT("GU distributed properties data error (") + MHYDAS_DEFAULT_GUPROPSFILE + wxT(").");
      return false;
    }
  }
  else
  {
    mhydasdk::base::LastError::Message = wxT("GU distributed properties file error (") + MHYDAS_DEFAULT_GUPROPSFILE + wxT(").");
    return false;
  }


  return true;
}


// =====================================================================
// =====================================================================


bool IOManager::loadHydroObjectsInitialConditions(mhydasdk::core::SpatialRepository *SpatialData)
{
  wxArrayString Columns;
  wxString Data;
  int i, j;
  bool IsOK = true;
  long ID;
  double Value;


  // ============== SUs =========================

  Columns.Clear();
  Data.Clear();

  if (extractColumnOrderAndDataFromFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_SUINIFILE),
                                        wxT("SUini"),&Columns,&Data))
  {

    ColumnTextParser SUIni(wxT("%"));

    if (SUIni.setFromString(Data,Columns.Count()+1))
    {
      mhydasdk::core::SurfaceUnit* SU;

      i = 0;

      while (i<SUIni.getLinesCount() && IsOK)
      {
        IsOK = SUIni.getLongValue(i,0,&ID);
        SU = SpatialData->getSUByID((int)ID);
        if (IsOK && SU != NULL)
        {
          for (j=1;j<SUIni.getColsCount();j++)
          {
            if (SUIni.getDoubleValue(i,j,&Value))
            {
              SU->getIniConditions()->insert(mhydasdk::core::ParamsMap::value_type(Columns[j-1],Value));
            }
          }
        }
        else
        {
          mhydasdk::base::LastError::Message = wxT("SU distributed initial conditions format error (") + MHYDAS_DEFAULT_SUINIFILE + wxT(").");
          return false;
        }
        i++;
      }
    }
    else
    {
      mhydasdk::base::LastError::Message = wxT("SU distributed initial conditions data error (") + MHYDAS_DEFAULT_SUINIFILE + wxT(").");
      return false;
    }
  }
  else
  {
    mhydasdk::base::LastError::Message = wxT("SU distributed initial conditions file error (") + MHYDAS_DEFAULT_SUINIFILE + wxT(").");
    return false;
  }


  // ============== RSs =========================

  Columns.Clear();
  Data.Clear();

  if (extractColumnOrderAndDataFromFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_RSINIFILE),
                                        wxT("RSini"),&Columns,&Data))
  {

    ColumnTextParser RSIni(wxT("%"));

    if (RSIni.setFromString(Data,Columns.Count()+1))
    {
      mhydasdk::core::ReachSegment* RS;

      i = 0;

      while (i<RSIni.getLinesCount() && IsOK)
      {
        IsOK = RSIni.getLongValue(i,0,&ID);
        RS = SpatialData->getRSByID((int)ID);
        if (IsOK && RS != NULL)
        {
          for (j=1;j<RSIni.getColsCount();j++)
          {
            if (RSIni.getDoubleValue(i,j,&Value))
            {
              RS->getIniConditions()->insert(mhydasdk::core::ParamsMap::value_type(Columns[j-1],Value));
            }
          }
        }
        else
        {
          mhydasdk::base::LastError::Message = wxT("RS distributed initial conditions format error (") + MHYDAS_DEFAULT_RSINIFILE + wxT(").");
          return false;
        }
        i++;
      }
    }
    else
    {
      mhydasdk::base::LastError::Message = wxT("RS distributed initial conditions data error (") + MHYDAS_DEFAULT_RSINIFILE + wxT(").");
      return false;
    }
  }
  else
  {
    mhydasdk::base::LastError::Message = wxT("RS distributed initial conditions file error (") + MHYDAS_DEFAULT_RSINIFILE + wxT(").");
    return false;
  }


  // ============== GUs =========================

  Columns.Clear();
  Data.Clear();

  if (extractColumnOrderAndDataFromFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_GUINIFILE),
                                        wxT("GUini"),&Columns,&Data))
  {

    ColumnTextParser GUIni(wxT("%"));

    if (GUIni.setFromString(Data,Columns.Count()+1))
    {
      mhydasdk::core::GroundwaterUnit* GU;

      i = 0;

      while (i<GUIni.getLinesCount() && IsOK)
      {
        IsOK = GUIni.getLongValue(i,0,&ID);
        GU = SpatialData->getGUByID((int)ID);
        if (IsOK && GU != NULL)
        {
          for (j=1;j<GUIni.getColsCount();j++)
          {
            if (GUIni.getDoubleValue(i,j,&Value))
            {
              GU->getIniConditions()->insert(mhydasdk::core::ParamsMap::value_type(Columns[j-1],Value));
            }
          }
        }
        else
        {
          mhydasdk::base::LastError::Message = wxT("GU distributed initial conditions format error (") + MHYDAS_DEFAULT_GUINIFILE + wxT(").");
          return false;
        }
        i++;
      }
    }
    else
    {
      mhydasdk::base::LastError::Message = wxT("GU distributed initial conditions data error (") + MHYDAS_DEFAULT_GUINIFILE + wxT(").");
      return false;
    }
  }
  else
  {
    mhydasdk::base::LastError::Message = wxT("GU distributed initial conditions file error (") + MHYDAS_DEFAULT_GUINIFILE + wxT(").");
    return false;
  }



  return true;
}


// =====================================================================
// =====================================================================

bool IOManager::loadOutputConfig()
{

  TiXmlDocument LoadDoc;
  TiXmlElement* Child;
  TiXmlElement* Child2;

  wxString Str;
  int i;
  long LongValue;

  m_AutoOutFiles.ColSeparator = wxT("\t");
  m_AutoOutFiles.CommentChar = wxT("%");
  m_AutoOutFiles.DTFormat = wxT("%Y%m%dT%H%M%S");

  if (LoadDoc.LoadFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_OUTPUTCONFFILE).mb_str(wxConvUTF8)))
  {

    TiXmlHandle DocHandle(&LoadDoc);


    // auto output files
    Child = DocHandle.FirstChild("mhydas").FirstChild("output").FirstChild("autooutfiles").Element();

    if (Child != NULL)
    {
      // processing column separator attribute
      if (Child->Attribute("colsep") != NULL)
      {
        Str = _U(Child->Attribute("colsep"));
        if (Str.Length() > 0) m_AutoOutFiles.ColSeparator = Str;
      }

      // processing comment char attribute
      if (Child->Attribute("commentchar") != NULL)
      {
        Str = _U(Child->Attribute("commentchar"));
        if (Str.Length() > 0) m_AutoOutFiles.CommentChar = Str;
      }

      // processing datetime format attribute
      if (Child->Attribute("dtformat") != NULL)
      {
        Str = _U(Child->Attribute("dtformat"));
        if (Str.Length() > 0)
        {
          if (Str == wxT("6cols")) Str = wxT("%Y\t%m\t%d\t%H\t%M\t%S");
          if (Str == wxT("iso")) Str = wxT("%Y%m%dT%H%M%S");
          m_AutoOutFiles.DTFormat = Str;
        }
      }

      Child = DocHandle.FirstChild("mhydas").FirstChild("output").FirstChild("autooutfiles").FirstChild().Element();

      AutoOutfileDef* CurrentDef;

      for(Child;Child;Child=Child->NextSiblingElement())
       {
        CurrentDef = new AutoOutfileDef;

        // processing suffix file attribute
        if (Child->Attribute("filesuffix") != NULL)
        {
          CurrentDef->FileSuffix = _U(Child->Attribute("filesuffix"));
        }
        else CurrentDef->FileSuffix = wxT("");

        // processing objects selection attribute
        if (Child->Attribute("selection") != NULL)
        {
          Str = _U(Child->Attribute("selection"));
          if (Str != wxT("*") && Str.Length() > 0)
          {
            wxArrayString StrArray = SplitString(Str,wxT(";"));
            for (i=0;i<StrArray.Count();i++)
            {
              if (StrArray[i].ToLong(&LongValue)) CurrentDef->SelectedObjectIDs.push_back((int)LongValue);
              else
              {
                mhydasdk::base::LastError::Message = wxT("Output config file format error: objects selection ID (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(").");
                return false;

              }
            }

          }
        }
        else
        {
          mhydasdk::base::LastError::Message = wxT("Output config file format error: missing objects selection (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(").");
          return false;
        }


        // processing simulated vars columns attribute
        if (Child->Attribute("columns") != NULL)
        {
          Str = _U(Child->Attribute("columns"));
          if (Str != wxT("*") && Str.Length() > 0) CurrentDef->Columns = SplitString(Str,wxT(";"));
        }
        else
        {
          mhydasdk::base::LastError::Message = wxT("Output config file format error: missing column description (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(").");
          return false;
        }


        // processing object type adn adds current def to def list
        Str = _U(Child->Value());
        if ((Str == wxT("SUout")) || (Str == wxT("RSout")) || (Str == wxT("GUout")))
        {
          CurrentDef->ObjectsKind = Str;
          m_AutoOutFiles.Defs.push_back(CurrentDef);
        }
        else
        {
          mhydasdk::base::LastError::Message = wxT("Output config file format error: unknown object type (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(").");
          return false;
        }
       }
    }
    else
    {
      mhydasdk::base::LastError::Message = wxT("Output config file format error (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(").");
      return false;
    }
  }
  else
  {
    mhydasdk::base::LastError::Message = wxT("Output config file not found (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(").");
    return false;
  }

  return true;
}


// =====================================================================
// =====================================================================

bool IOManager::prepareOutputDir()
{
  bool IsOK = true;

  if (!wxDirExists(mp_RunEnv->getOutputDir()))
  #ifdef __WXMSW__
  IsOK = wxMkDir(mp_RunEnv->getOutputDir().mb_str(wxConvUTF8));
  #else
  IsOK = wxMkDir(mp_RunEnv->getOutputDir().mb_str(wxConvUTF8),0777);
  #endif

  return IsOK;
}

// =====================================================================
// =====================================================================

bool IOManager::saveResultsFromDef(mhydasdk::core::SpatialRepository *SpatialData,
                                   wxString ColSeparator, wxString CommentChar,
                                   AutoOutfileDef* Def, wxArrayString DTStrings)
{

  wxString FileContents;
  wxString Filename;
  vector<mhydasdk::core::HydroObject*> HOSet;
  vector<double>* Values;
  wxString ColsStr;
  int i,j,k;


  if (Def->SelectedObjectIDs.size() > 0)
  {

    // creating selection list for SUs
    if (Def->ObjectsKind == wxT("SUout"))
    {
      for (i=0;i<Def->SelectedObjectIDs.size();i++)
      {
        HOSet.push_back(SpatialData->getSUByID(Def->SelectedObjectIDs[i]));
      }
    }


    // creating selection list for RSs
    if (Def->ObjectsKind == wxT("RSout"))
    {
      for (i=0;i<Def->SelectedObjectIDs.size();i++)
      {
        HOSet.push_back(SpatialData->getRSByID(Def->SelectedObjectIDs[i]));
      }
    }


    // creating selection list for GUs
    if (Def->ObjectsKind == wxT("GUout"))
    {
      for (i=0;i<Def->SelectedObjectIDs.size();i++)
      {
        HOSet.push_back(SpatialData->getGUByID(Def->SelectedObjectIDs[i]));
      }
    }



  }
  else
  {

    // creating full list for SUs
    if (Def->ObjectsKind == wxT("SUout"))
    {
      mhydasdk::core::SUMap::iterator SUit;

      for(SUit = SpatialData->getSUsCollection()->begin(); SUit != SpatialData->getSUsCollection()->end(); ++SUit)
      {
        HOSet.push_back(SUit->second);
      }

    }

    // creating full list for RSs
    if (Def->ObjectsKind == wxT("RSout"))
    {
      mhydasdk::core::RSMap::iterator RSit;

      for(RSit = SpatialData->getRSsCollection()->begin(); RSit != SpatialData->getRSsCollection()->end(); ++RSit)
      {
        HOSet.push_back(RSit->second);
      }

    }

    // creating full list for GUs
    if (Def->ObjectsKind == wxT("GUout"))
    {
      mhydasdk::core::GUMap::iterator GUit;

      for(GUit = SpatialData->getGUsCollection()->begin(); GUit != SpatialData->getGUsCollection()->end(); ++GUit)
      {
        HOSet.push_back(GUit->second);
      }

    }



  }


  // creating column list if empty (*)
  if (Def->Columns.Count() == 0 && HOSet[0] != NULL)
  {
    mhydasdk::core::SimulatedVarsMap::iterator Simit;

    for(Simit = HOSet[0]->getSimulatedVars()->begin(); Simit != HOSet[0]->getSimulatedVars()->end(); ++Simit)
    {
      Def->Columns.Add(Simit->first);
    }

  }

  // building columns string for file headers
  ColsStr.Clear();
  for (i=0;i<Def->Columns.Count();i++)
  {
    ColsStr << Def->Columns[i];
    if (i != (Def->Columns.Count()-1)) ColsStr << ColSeparator;
  }


  for (i=0;i<HOSet.size();i++)
  {

    if (HOSet[i] != NULL)
    {
      Filename = wxT("SU");
      if (Def->ObjectsKind == wxT("RSout")) Filename = wxT("RS");
      if (Def->ObjectsKind == wxT("GUout")) Filename = wxT("GU");


      Filename = Filename + wxString::Format(wxT("%d"),HOSet[i]->getID());

      if (Def->FileSuffix.Length() > 0) Filename = Filename + wxT("_") + Def->FileSuffix;


      Filename = Filename + wxT(".") + MHYDAS_DEFAULT_OUPUTFILES_EXT;

      FileContents.Clear();

      // file header
      FileContents << CommentChar << wxT(" file: ") << Filename << wxT("\n");
      FileContents << CommentChar << wxT(" object ID: ") << HOSet[i]->getID() << wxT("\n");
      FileContents << CommentChar << wxT(" columns order (after date and time): ") << ColsStr << wxT("\n");

      for (j=0;j<10;j++) FileContents << CommentChar;
      FileContents << wxT("\n");

      // file contents

      for (j=0;j<DTStrings.Count();j++)
      {
        FileContents << DTStrings[j];
        for (k=0;k<Def->Columns.Count();k++)
        {
          if (HOSet[i]->getSimulatedVars()->find(Def->Columns[k]) != HOSet[i]->getSimulatedVars()->end())
            Values = HOSet[i]->getSimulatedVars()->find(Def->Columns[k])->second;
          if (Values != NULL) FileContents << ColSeparator << Values->at(j);
        }
        FileContents << wxT("\n");
      }



      wxFile RFile(mp_RunEnv->getOutputFullPath(Filename),wxFile::write);

      RFile.Write(FileContents);

      RFile.Close();
    }
  }



  return true;
}




// =====================================================================
// =====================================================================



bool IOManager::saveResults(mhydasdk::core::CoreRepository *Data)
{
  bool IsOK = true;

  if (prepareOutputDir())
  {
    int i;
    wxString Filename;
    AutoOutfileDef* CurrentDef;

    wxArrayString DTStrings;
    mhydasdk::core::TimeSerie* TSerie = Data->getRainEvent()->getRainSourceCollection().begin()->second->getTimeSerie();

    // preparing and formatting datetime column(s)
    for (i=0;i<TSerie->getItemsCollection()->size();i++)
    {
      DTStrings.Add(TSerie->getItemsCollection()->at(i)->getDateTime().asString(m_AutoOutFiles.DTFormat));
    }


    // saving data matching each definition
    for (i=0;i<m_AutoOutFiles.Defs.size();i++)
    {
      CurrentDef = m_AutoOutFiles.Defs[i];

      saveResultsFromDef(Data->getSpatialData(),
                         m_AutoOutFiles.ColSeparator,m_AutoOutFiles.CommentChar,
                         CurrentDef,DTStrings);

    }

  }

  return IsOK;
}



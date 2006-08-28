/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "IOMan.h"
#include "setup.h"
#include "xml/tinyxml.h"

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

  wxString Str;
  double DoubleValue;
  int IntValue;


  if (LoadDoc.LoadFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_CONFFILE).mb_str(wxConvUTF8)))
  {

    TiXmlHandle DocHandle(&LoadDoc);

    Child = DocHandle.FirstChild("mhydas").FirstChild("config").FirstChild("runparams").FirstChild("param").Element();


    for(Child;Child;Child=Child->NextSiblingElement())
	  {

		  if (Child->Attribute("name") != NULL && wxString(Child->Attribute("name"),wxConvUTF8) == wxT("deltat") &&
		      Child->Attribute("value",&IntValue) != NULL)
		  {
        Config->DeltaT = IntValue;
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

mhydasdk::core::SUFlowCode IOManager::getSUFlowCode(wxString Code)
{
  if (Code == wxT("S")) return mhydasdk::core::SU;
  if (Code == wxT("R")) return mhydasdk::core::RLat;

  return mhydasdk::core::UnknownFlowCode;
}


// =====================================================================
// =====================================================================


bool IOManager::loadHydroObjects(mhydasdk::core::SpatialRepository *SpatialData)
{

  if (SpatialData == NULL) return false;

  long ID;
  long ProcessOrder;
  long GUExch;

  int i;


  // ============ SUs =================

  mhydasdk::base::ColumnFileParser SUsFileParser(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_SUDEFSFILE),wxT("%"));

  if (!SUsFileParser.parseFile() ||
      SUsFileParser.getColsCount() != MHYDAS_SUDEFSFILE_COLNBR ||
      SUsFileParser.getColsCount() < 1)
  {
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

      if (SUsFileParser.getLongValue(i,0,&ID) && SUsFileParser.getDoubleValue(i,1,&Area) &&
          SUsFileParser.getDoubleValue(i,2,&Slope) && SUsFileParser.getStringValue(i,3,&FlowCode) &&
          SUsFileParser.getLongValue(i,4,&FlowID) && SUsFileParser.getDoubleValue(i,5,&FlowDist) &&
          SUsFileParser.getLongValue(i,6,&GUExch) && SUsFileParser.getLongValue(i,7,&ProcessOrder))
      {

        if (!SpatialData->addSU(new mhydasdk::core::SurfaceUnit((mhydasdk::core::hoid_t)ID,
                                                                (mhydasdk::core::hoid_t)ProcessOrder,
                                                                Area,
                                                                Slope,
                                                                getSUFlowCode(FlowCode),
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


  // ============ RSs =================

  mhydasdk::base::ColumnFileParser RSsFileParser(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_RSDEFSFILE),wxT("%"));

  if (!RSsFileParser.parseFile() ||
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


  // ============ GUs =================

  mhydasdk::base::ColumnFileParser GUsFileParser(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_GUDEFSFILE),wxT("%"));

  if (!GUsFileParser.parseFile() ||
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


  return true;

}



// =====================================================================
// =====================================================================


RainEventFilesMap IOManager::buildRainEventFileMap()
{
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
  mhydasdk::base::ColumnFileParser FileParser(mp_RunEnv->getInputFullPath(Filename),wxT("%"));
  bool IsOK;

  IsOK = true;

  long int Year, Month, Day, Hour, Min, Sec;
  double Value;

  mhydasdk::core::TimeSerie *Serie;

  if (FileParser.parseFile() && (FileParser.getLinesCount() > 0) && (FileParser.getColsCount() == 7))
  {

    Serie = new mhydasdk::core::TimeSerie("mm/h");

    int i = 0;

    while (i<FileParser.getLinesCount() && IsOK)
    {
      if (FileParser.getLongValue(i,0,&Year) && FileParser.getLongValue(i,1,&Month) &&
          FileParser.getLongValue(i,2,&Day) && FileParser.getLongValue(i,3,&Hour) &&
          FileParser.getLongValue(i,4,&Min) && FileParser.getLongValue(i,5,&Sec) &&
          FileParser.getDoubleValue(i,6,&Value))
      {
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
        //mhydasdk::base::LastError::Message = wxT("Error loading ") + RIFit->second + wxT(" rain file.");
        return false;
      }
    }

  }
  return true;

}


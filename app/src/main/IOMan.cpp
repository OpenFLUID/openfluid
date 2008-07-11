/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include <wx/filefn.h>
#include <wx/tokenzr.h>
#include <wx/dir.h>

#include <iostream>

#include "IOMan.h"
#include "openfluid-tools.h"
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



IOManager::IOManager(openfluid::base::ExecutionMessages* ExecMsgs,RuntimeEnvironment* RunEnv)
{
  mp_ExecMsgs = ExecMsgs;
  mp_RunEnv = RunEnv;

  m_ClearedOuputDir = false;
}

// =====================================================================
// =====================================================================

IOManager::~IOManager()
{

}

// =====================================================================
// =====================================================================


bool IOManager::loadRunConfig(RunConfig* Config)
{
  TiXmlDocument LoadDoc;
  TiXmlElement* Child, *Child2;

  wxString Str;
  wxDateTime ZeDate;


  long IntValue;

  FunctionConfig* FConf;

  if (LoadDoc.LoadFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_RUNFILE).mb_str(wxConvUTF8)))
  {

    TiXmlHandle DocHandle(&LoadDoc);


    // -------- DeltaT ----------------

    Child = DocHandle.FirstChild("mhydas").FirstChild("run").FirstChild("deltat").Element();

    if (Child == NULL)
    {
      mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Run config file (") + MHYDAS_DEFAULT_RUNFILE + wxT(") error. Missing deltat."));
      return false;
    }
    else
    {
      Str = _U(Child->GetText());

      if (Str.ToLong(&IntValue))
      {
        Config->DeltaT = int(IntValue);

//        std::cerr << Config->DeltaT << std::endl;
      }
      else
      {
        mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Run config file (") + MHYDAS_DEFAULT_RUNFILE + wxT(") error. Wrong deltat format."));
        return false;
      }
    }

    // -------- Simulation period ----------------

    Child = DocHandle.FirstChild("mhydas").FirstChild("run").FirstChild("period").Element();

    if (Child == NULL)
    {
      mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Run config file (") + MHYDAS_DEFAULT_RUNFILE + wxT(") error. Missing period."));
      return false;
    }
    else
    {
      if (Child->Attribute("begin") != NULL)
      {
        Str = _U(Child->Attribute("begin"));

        if (ZeDate.ParseFormat(Str,wxT("%Y-%m-%d %H:%M:%S")) != NULL)
        {
          Config->BeginDate = ZeDate;

//          std::cerr << Config->BeginDate.Format(wxT("%Y-%m-%d %H:%M:%S")).mb_str(wxConvUTF8) << std::endl;

        }
        else
        {
          mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Run config file (") + MHYDAS_DEFAULT_RUNFILE + wxT(") error. Wrong format of begin attribute for period."));
          return false;
        }
      }
      else
      {
        mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Run config file (") + MHYDAS_DEFAULT_RUNFILE + wxT(") error. Missing begin attribute for period."));
        return false;
      }


      if (Child->Attribute("end") != NULL)
      {
        Str = _U(Child->Attribute("end"));

        if (ZeDate.ParseFormat(Str,wxT("%Y-%m-%d %H:%M:%S")) != NULL)
        {
          Config->EndDate = ZeDate;

//          std::cerr << Config->EndDate.Format(wxT("%Y-%m-%d %H:%M:%S")).mb_str(wxConvUTF8) << std::endl;

        }
        else
        {
          mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Run config file (") + MHYDAS_DEFAULT_RUNFILE + wxT(") error. Wrong format of end attribute for period."));
          return false;
        }
      }
      else
      {
        mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Run config file (") + MHYDAS_DEFAULT_RUNFILE + wxT(") error. Missing end attribute for period."));
        return false;
      }

    }

    if (Config->EndDate < Config->BeginDate)
    {
      mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Run config file (") + MHYDAS_DEFAULT_RUNFILE + wxT(") error. Wrong simulation period definition."));
      return false;
    }


    // -------- Simulation ID ----------------

    Child = DocHandle.FirstChild("mhydas").FirstChild("run").FirstChild("simid").Element();

    if (Child != NULL)
    {
      Str = _U(Child->GetText());
      if (Str != wxT("") && (Str.Find(wxT(" ")) == -1)) Config->SimulationID = Str;
    }



  }
  else
  {
    mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Run configuration file (") + MHYDAS_DEFAULT_RUNFILE + wxT(") error."));
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

  if (LoadDoc.LoadFile(mp_RunEnv->getInputFullPath(MHYDAS_DEFAULT_MODELFILE).mb_str(wxConvUTF8)))
  {

    TiXmlHandle DocHandle(&LoadDoc);


    // =========== run params ===========
    Child = DocHandle.FirstChild("mhydas").FirstChild("runparams").FirstChild("param").Element();


    for(Child;Child;Child=Child->NextSiblingElement())
    {

      // process of "deltat" attribute
      if (Child->Attribute("name") != NULL && wxString(Child->Attribute("name"),wxConvUTF8) == wxT("deltat") &&
		  Child->Attribute("value",&IntValue) != NULL)
	    {
        Config->DeltaT = IntValue;
	    }

      // process of "simID" attribute
      if (Child->Attribute("name") != NULL && wxString(Child->Attribute("name"),wxConvUTF8) == wxT("simID") &&
      Child->Attribute("value") != NULL)
      {
        Config->SimulationID = wxString(Child->Attribute("value"),wxConvUTF8);
      }



    }


    // ======= model structure ===========


    Child = DocHandle.FirstChild("mhydas").FirstChild("model").FirstChild("function").Element();


    for(Child;Child;Child=Child->NextSiblingElement())
	  {
      if (Child->Attribute("fileID") != NULL)
      {

        // function fileID
        FConf = new FunctionConfig();
        FConf->FileID = wxString(Child->Attribute("fileID"),wxConvUTF8);


        if (FConf->FileID.Length() > 0)
        {

          FConf->Params.clear();

          // function params parsing
          TiXmlHandle Child2Handle(Child);
          Child2 = Child2Handle.FirstChild("param").Element();

          for(Child2;Child2;Child2=Child2->NextSiblingElement())
          {
            if (Child2->Attribute("name") != NULL && Child2->Attribute("value") != NULL)
            {
              FConf->Params[wxString(Child2->Attribute("name"),wxConvUTF8)] = wxString(Child2->Attribute("value"),wxConvUTF8);

            }

          }

          Config->FuncConfigs.Append(FConf);

        }
        else delete FConf;

      }
      else
      {
        mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Model configuration file (") + MHYDAS_DEFAULT_MODELFILE + wxT(") error. Incorrect function definition."));
        return false;
      }


    }



  }
  else
  {
    mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Model configuration file (") + MHYDAS_DEFAULT_MODELFILE + wxT(") error."));
    return false;
  }

  return true;

}



// =====================================================================
// =====================================================================

openfluid::core::SUDownstreamCode IOManager::getSUDownstreamCode(wxString Code)
{
  if (Code == wxT("S")) return openfluid::core::SUDownstream;
  if (Code == wxT("R")) return openfluid::core::RLatDownstream;

  return openfluid::core::UnknownDownstreamCode;
}


// =====================================================================
// =====================================================================


bool IOManager::loadHydroObjects(openfluid::core::SpatialRepository *SpatialData)
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
    mp_ExecMsgs->setError(wxT("IO Manager"),wxT("No spatial object file"));
    return false;
  }


  // ============ SUs =================


  if (SUsFileExists)
  {
    openfluid::tools::ColumnTextParser SUsFileParser(wxT("%"));

    //  checks that files has the right column number
    if (!SUsFileParser.loadFromFile(SUsFilename) ||
        SUsFileParser.getColsCount() != MHYDAS_SUDEFSFILE_COLNBR ||
        SUsFileParser.getColsCount() < 1)
    {
      mp_ExecMsgs->setError(wxT("IO Manager"),MHYDAS_DEFAULT_SUDEFSFILE + wxT(" file parsing error"));
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

          if (!SpatialData->addSU(new openfluid::core::SurfaceUnit((openfluid::core::HOID)ID,
                                                                  (openfluid::core::HOID)ProcessOrder,
                                                                  Area,
                                                                  Slope,
                                                                  getSUDownstreamCode(FlowCode),
                                                                  (openfluid::core::HOID)FlowID,
                                                                  FlowDist,
                                                                  (openfluid::core::HOID)GUExch)))
          {
            mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Error adding SU #")+wxString::Format(wxT("%d"),ID)+wxT(". Maybe alredy in use"));
            return false;
          }

        }

        else
        {
          mp_ExecMsgs->setError(wxT("IO Manager"),MHYDAS_DEFAULT_SUDEFSFILE + wxT(" file format error"));
          return false;
        }

      }
    }
  }


  // ============ RSs =================

  if (RSsFileExists)
  {
    openfluid::tools::ColumnTextParser RSsFileParser(wxT("%"));

    if (!RSsFileParser.loadFromFile(RSsFilename) ||
        RSsFileParser.getColsCount() != MHYDAS_RSDEFSFILE_COLNBR ||
        RSsFileParser.getColsCount() < 1)
    {
      mp_ExecMsgs->setError(wxT("IO Manager"),MHYDAS_DEFAULT_RSDEFSFILE + wxT(" file parsing error"));
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


          if (!SpatialData->addRS(new openfluid::core::ReachSegment((openfluid::core::HOID)ID,
                                                                   (openfluid::core::HOID)ProcessOrder,
                                                                   (openfluid::core::nodeid_t)UpNode,
                                                                   (openfluid::core::nodeid_t)DownNode,
                                                                   (openfluid::core::HOID)LowRSID,
                                                                   Slope,
                                                                   Length,
                                                                   Width,
                                                                   Height,
                                                                   (openfluid::core::HOID)GUExch)))
          {
            mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Error adding RS #")+wxString::Format(wxT("%d"),ID)+wxT(". Maybe alredy in use"));
            return false;
          }
        }
        else
        {
          mp_ExecMsgs->setError(wxT("IO Manager"),MHYDAS_DEFAULT_RSDEFSFILE + wxT(" file format error"));
          return false;
        }
      }
    }
  }


  // ============ GUs =================

  if (GUsFileExists)
  {
    openfluid::tools::ColumnTextParser GUsFileParser(wxT("%"));

    if (!GUsFileParser.loadFromFile(GUsFilename) ||
        GUsFileParser.getColsCount() != MHYDAS_GUDEFSFILE_COLNBR ||
        GUsFileParser.getColsCount() < 1)
    {
      mp_ExecMsgs->setError(wxT("IO Manager"),MHYDAS_DEFAULT_GUDEFSFILE + wxT(" file parsing error."));
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

          if (!SpatialData->addGU(new openfluid::core::GroundwaterUnit((openfluid::core::HOID)ID,
                                                                      (openfluid::core::HOID)ProcessOrder,
                                                                      (openfluid::core::HOID)GUExch,
                                                                      SubstrLevel)))
          {
            mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Error adding GU ")+wxString::Format(wxT("%d"),ID)+wxT(". Maybe alredy in use."));

            return false;
          }
        }
        else
        {
          mp_ExecMsgs->setError(wxT("IO Manager"),MHYDAS_DEFAULT_GUDEFSFILE + wxT(" file format error."));
          return false;
        }
      }
    }
  }


  return true;

}

// =====================================================================
// =====================================================================

bool IOManager::loadDistributedData(openfluid::core::SpatialRepository *SpatialData)
{

  wxArrayString FilesToLoad = GetFilesByExt(mp_RunEnv->getInputDir(),wxT("ddata.xml"),true);

  bool IsOK = true;
  int i=0;

  while (IsOK && i<FilesToLoad.GetCount())
  {
    IsOK =  loadDistributedDataFile(FilesToLoad[i],SpatialData);
    i++;
  }


  return IsOK;
}

// =====================================================================
// =====================================================================


bool IOManager::loadDistributedDataFile(wxString Filename, openfluid::core::SpatialRepository *SpatialData)
{

  bool IsOK = true;
  wxString Data;
  wxArrayString ColOrder;
  wxString UnitClass, DataCat;

  if (wxFileExists(Filename))
  {
    TiXmlDocument Doc;
    TiXmlElement* Child;

    if (Doc.LoadFile(_C(Filename)))
    {

      TiXmlHandle DocHandle(&Doc);

      // unitclass and category

      Child = DocHandle.FirstChild("mhydas").FirstChild("distridata").Element();

      if (Child != NULL)
      {
        if (Child->Attribute("unitclass") != NULL && Child->Attribute("datacat") != NULL)
        {

          UnitClass = _U(Child->Attribute("unitclass"));

          DataCat = _U(Child->Attribute("datacat"));

          Child = DocHandle.FirstChild("mhydas").FirstChild("distridata").FirstChild("columns").Element();

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
                Child = DocHandle.FirstChild("mhydas").FirstChild("distridata").FirstChild("data").Element();

                if (Child != NULL)
                {
                  Data.Clear();
                  Data.Append(_U(Child->GetText()));
                }
                else IsOK = false;
              }
              else IsOK = false;
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


  if ((DataCat != wxT("ini") && DataCat != wxT("param")) ||
      (UnitClass != wxT("SU") && UnitClass != wxT("RS") && UnitClass != wxT("GU"))) IsOK = false;

  int i,j;
  long ID;
  openfluid::core::PropertyValue Value;


  if (IsOK)
  {
    openfluid::tools::ColumnTextParser DataParser(wxT("%"));

    if (DataParser.setFromString(Data,ColOrder.Count()+1))
    {
      openfluid::core::HydroObject* HO;

      i = 0;

      // parses file and loads it in the properties hash table, ordered by columns
      while (i<DataParser.getLinesCount() && IsOK)
      {
        IsOK = DataParser.getLongValue(i,0,&ID);

        if (IsOK)
        {

          HO = NULL;
          if (UnitClass == wxT("SU")) HO = SpatialData->getSUByID((int)ID);
          if (UnitClass == wxT("RS")) HO = SpatialData->getRSByID((int)ID);
          if (UnitClass == wxT("GU")) HO = SpatialData->getGUByID((int)ID);

          if (HO != NULL)
          {
            for (j=1;j<DataParser.getColsCount();j++)
            {
              if (DataParser.getDoubleValue(i,j,&Value))
              {
                if (DataCat == wxT("ini"))
                  HO->getIniConditions()->insert(openfluid::core::PropertiesMap::value_type(ColOrder[j-1],Value));
                else
                  HO->getProperties()->insert(openfluid::core::PropertiesMap::value_type(ColOrder[j-1],Value));
              }
            }
          }
          else
          {
            mp_ExecMsgs->addWarning(wxT("IO Manager"),UnitClass + wxT(" ") + wxString::Format(wxT("%d"),ID) + wxT(" does not exist (") + Filename + wxT(")"));
          }
          i++;
        }
        else
        {
          mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Distributed data format error (") + Filename + wxT(")"));
          return false;
        }
      }
    }
    else
    {
      mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Distributed data error (") + Filename + wxT(")"));
      return false;
    }
  }
  else
  {
    mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Distributed data xml error (") + Filename + wxT(")"));
    return false;
  }


  return IsOK;



}

// =====================================================================
// =====================================================================

bool IOManager::loadDistributedEvents(openfluid::core::SpatialRepository *SpatialData)
{

  wxArrayString FilesToLoad = GetFilesByExt(mp_RunEnv->getInputDir(),wxT("events.xml"),true);

  bool IsOK = true;
  int i=0;


  while (IsOK && i<FilesToLoad.GetCount())
  {
    IsOK =  loadDistributedEventsFile(FilesToLoad[i],SpatialData);
    i++;
  }


  return IsOK;
}

// =====================================================================
// =====================================================================


bool IOManager::loadDistributedEventsFile(wxString Filename, openfluid::core::SpatialRepository *SpatialData)
{

  bool IsOK = true;


  wxDateTime ZeDate;
  wxString ZeDateStr;
  wxString UnitClass, UnitID;
  wxString InfoKey, InfoValue;

  if (wxFileExists(Filename))
  {
    TiXmlDocument Doc;
    TiXmlElement* Child, *Child2, *Child3;

    if (Doc.LoadFile(_C(Filename)))
    {

      TiXmlHandle DocHandle(&Doc);

      Child = DocHandle.FirstChild("mhydas").FirstChild("calendar").Element();


      if (Child != NULL)
      {
        TiXmlHandle Child2Handle(Child);
        Child2 = Child2Handle.FirstChild("event").Element();

        // loop on all events in the file

        for(Child2;Child2;Child2=Child2->NextSiblingElement())
        {
          ZeDateStr = wxT("");
          UnitClass = wxT("");
          UnitID = wxT("");

          if (Child2->Attribute("date") != NULL) ZeDateStr = wxString(Child2->Attribute("date"),wxConvUTF8);
          if (Child2->Attribute("unitclass") != NULL) UnitClass = wxString(Child2->Attribute("unitclass"),wxConvUTF8);
          if (Child2->Attribute("unitID") != NULL) UnitID = wxString(Child2->Attribute("unitID"),wxConvUTF8);

          if ((ZeDateStr != wxT("")) && (UnitClass != wxT("")) && (UnitID != wxT("")))
          {
            openfluid::core::DistributedEvent *DEvent;
            openfluid::core::HydroObject* HO;
            long ID;


            if (UnitID.ToLong(&ID))
            {
              HO = NULL;

              // get the righ unit

              if (UnitClass == wxT("SU")) HO = SpatialData->getSUByID((int)ID);
              if (UnitClass == wxT("RS")) HO = SpatialData->getRSByID((int)ID);
              if (UnitClass == wxT("GU")) HO = SpatialData->getGUByID((int)ID);

              if (HO != NULL)
              {
                ZeDate.ParseFormat(ZeDateStr,wxT("%Y-%m-%d %H:%M:%S"));

                DEvent = new openfluid::core::DistributedEvent(ZeDate);

                // read infos by event

                TiXmlHandle Child3Handle(Child2);
                Child3 = Child3Handle.FirstChild("info").Element();

                for(Child3;Child3;Child3=Child3->NextSiblingElement())
                {
                  InfoKey = wxT("");
                  InfoValue = wxT("");

                  if (Child3->Attribute("key") != NULL) InfoKey = wxString(Child3->Attribute("key"),wxConvUTF8);
                  if (Child3->Attribute("value") != NULL) InfoValue = wxString(Child3->Attribute("value"),wxConvUTF8);

                  if (InfoKey != wxT("")) DEvent->addInfo(InfoKey,InfoValue);

                }

                HO->getEvents()->addEvent(DEvent);

              }
            }
          }
        }
      }
      else
      {
        IsOK = false;
        mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Event file format error"));
      }
    }
    else
    {
      IsOK = false;
      mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Event file error"));
    }
  }
  else
  {
    IsOK = false;
    mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Event file error"));

  }

  return IsOK;



}


// =====================================================================
// =====================================================================

bool IOManager::loadOutputConfig()
{

  TiXmlDocument LoadDoc;
  TiXmlElement* Child;

  wxString Str;
  int i,j;
  long LongValue;

  wxArrayString OutVars;
  OutVars.Clear();

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
                mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Output config file format error: objects selection ID (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(")"));
                return false;

              }
            }

          }
        }
        else
        {
          mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Output config file format error: missing spatial objects selection (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(")"));
          return false;
        }


        // processing simulated vars columns attribute
        if (Child->Attribute("vars") != NULL)
        {
          Str = _U(Child->Attribute("vars"));

          CurrentDef->SaveAllVars = false;
          OutVars.Clear();


          if (Str == wxT("*")) CurrentDef->SaveAllVars = true;
          else
          {
            if (Str.Length() > 0)
            {
              OutVars = SplitString(Str,wxT(";"));
              for (j=0;j<OutVars.GetCount();j++)
              {

                if (IsVectorNamedVariable(OutVars[j])) CurrentDef->Vectors.Add(GetVectorNamedVariableName(OutVars[j]));
                else CurrentDef->Scalars.Add(OutVars[j]);
              }
            }
          }
        }
        else
        {
          mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Output config file format error: missing variables selection (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(")"));
          return false;
        }


        // processing object type and adds current def to def list
        Str = _U(Child->Value());
        if ((Str == wxT("SUout")) || (Str == wxT("RSout")) || (Str == wxT("GUout")))
        {
          CurrentDef->ObjectsKind = Str;
          m_AutoOutFiles.Defs.push_back(CurrentDef);
        }
        else
        {
          mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Output config file format error: unknown object type (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(")"));
          return false;
        }
       }
    }
    else
    {
      mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Output config file format error (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(")"));
      return false;
    }
  }
  else
  {
    mp_ExecMsgs->setError(wxT("IO Manager"),wxT("Output config file not found (") + MHYDAS_DEFAULT_OUTPUTCONFFILE + wxT(")"));
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
  {
    #ifdef __WXMSW__
    wxMkDir(mp_RunEnv->getOutputDir().mb_str(wxConvUTF8));
    #else
    wxMkDir(mp_RunEnv->getOutputDir().mb_str(wxConvUTF8),0777);
    #endif
    IsOK = wxDirExists(mp_RunEnv->getOutputDir());
  }
  else
  {
    if (mp_RunEnv->isClearOutputDir() && !m_ClearedOuputDir)
    {
      EmptyDirectoryRecursively(mp_RunEnv->getOutputDir().mb_str(wxConvUTF8));
      m_ClearedOuputDir = true;
    }
  }

  return IsOK;
}

// =====================================================================
// =====================================================================

bool IOManager::saveResultsFromDef(openfluid::core::SpatialRepository *SpatialData,
                                          wxString ColSeparator, wxString CommentChar,
                                          AutoOutfileDef* Def, wxArrayString DTStrings,
                                          ExtraSimInfos ExSI)
{

  wxString FileContents;
  wxString Filename;
  std::vector<openfluid::core::HydroObject*> HOSet;
  openfluid::core::SerieOfMHYDASScalarValue* Values;
  openfluid::core::MHYDASVectorValue VValues;
  wxString ColsStr;
  int i,j,k,l;
  wxString NaNStr = wxT("!");
  bool NoValue = false;


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
      openfluid::core::SUMap::iterator SUit;

      for(SUit = SpatialData->getSUsCollection()->begin(); SUit != SpatialData->getSUsCollection()->end(); ++SUit)
      {
        HOSet.push_back(SUit->second);
      }

    }

    // creating full list for RSs
    if (Def->ObjectsKind == wxT("RSout"))
    {
      openfluid::core::RSMap::iterator RSit;

      for(RSit = SpatialData->getRSsCollection()->begin(); RSit != SpatialData->getRSsCollection()->end(); ++RSit)
      {
        HOSet.push_back(RSit->second);
      }

    }

    // creating full list for GUs
    if (Def->ObjectsKind == wxT("GUout"))
    {
      openfluid::core::GUMap::iterator GUit;

      for(GUit = SpatialData->getGUsCollection()->begin(); GUit != SpatialData->getGUsCollection()->end(); ++GUit)
      {
        HOSet.push_back(GUit->second);
      }

    }

  }


  //  ======== Scalars ========


  // creating scalars column list if SaveAllVars
  if (Def->SaveAllVars && HOSet[0] != NULL)
  {
    openfluid::core::SimulatedVarsMap::iterator Simit;

    for(Simit = HOSet[0]->getSimulatedVars()->begin(); Simit != HOSet[0]->getSimulatedVars()->end(); ++Simit)
    {
      Def->Scalars.Add(Simit->first);
    }

  }


  if (Def->Scalars.GetCount() > 0)
  {

    // checking if requested scalar variables exist and
    // building columns string for file headers
    ColsStr.Clear();
    for (i=0;i<Def->Scalars.Count();i++)
    {
      if (HOSet[0]->getSimulatedVars()->find(Def->Scalars[i]) != HOSet[0]->getSimulatedVars()->end())
      {
        ColsStr << Def->Scalars[i];
        if (i != (Def->Scalars.Count()-1)) ColsStr << ColSeparator;
      }
      else
      {
        wxString ObjKind = wxT("SU");
        if (Def->ObjectsKind == wxT("RSout")) ObjKind = wxT("RS");
        if (Def->ObjectsKind == wxT("GUout")) ObjKind = wxT("GU");
        mp_ExecMsgs->addWarning(wxT("IOManager"),wxT("requested ")+ObjKind+wxT(" scalar variable ")+Def->Scalars[i]+wxT(" does not exists and cannot be saved"));
        Def->Scalars.RemoveAt(i);
      }
    }


    for (i=0;i<HOSet.size();i++)
    {

      if (HOSet[i] != NULL)
      {

        // set filename object kind
        Filename = wxT("SU");
        if (Def->ObjectsKind == wxT("RSout")) Filename = wxT("RS");
        if (Def->ObjectsKind == wxT("GUout")) Filename = wxT("GU");

        // set filename object ID
        Filename = Filename + wxString::Format(wxT("%d"),HOSet[i]->getID());

        // set filename suffix
        if (Def->FileSuffix.Length() > 0) Filename = Filename + wxT("_") + Def->FileSuffix;

        //Filename = Filename + wxT(".") + MHYDAS_DEFAULT_OUPUTFILES_EXT;
        Filename = Filename + wxT(".scalars.") + MHYDAS_DEFAULT_OUPUTFILES_EXT;

        FileContents.Clear();

        // file header
        FileContents << CommentChar << wxT(" simulation ID: ") << ExSI.SimID << wxT("\n");
        FileContents << CommentChar << wxT(" file: ") << Filename << wxT("\n");
        FileContents << CommentChar << wxT(" object ID: ") << HOSet[i]->getID() << wxT("\n");
        FileContents << CommentChar << wxT(" scalar variables order (after date and time): ") << ColsStr << wxT("\n");

        for (j=0;j<10;j++) FileContents << CommentChar;
        FileContents << wxT("\n");

        // file contents

        for (j=0;j<DTStrings.Count();j++)
        {
          FileContents << DTStrings[j];
          for (k=0;k<Def->Scalars.Count();k++)
          {
            Values = NULL;
            if (HOSet[i]->getSimulatedVars()->find(Def->Scalars[k]) != HOSet[i]->getSimulatedVars()->end())
              Values = HOSet[i]->getSimulatedVars()->find(Def->Scalars[k])->second;

            if (Values != NULL)
            {
              // check if value exists for this time step
              if (j<Values->size()) FileContents << ColSeparator << Values->at(j);
              else
              {
                FileContents << ColSeparator << NaNStr;
                NoValue = true;
              }
            }
          }
          FileContents << wxT("\n");
        }

        if (NoValue) mp_ExecMsgs->addWarning(wxT("IOManager"),wxT("at least one value was unavailable during ")+Filename+wxT(" saving process"));

        wxFile RFile(mp_RunEnv->getOutputFullPath(Filename),wxFile::write);

        RFile.Write(FileContents);

        RFile.Close();
      }
    }

  }


  //  ======== Vectors ========

  // creating vectors list if SaveAllVars (*)

  if (Def->SaveAllVars && HOSet[0] != NULL)
  {
    openfluid::core::SimulatedVectorVarsMap::iterator VSimit;

    for(VSimit = HOSet[0]->getSimulatedVectorVars()->begin(); VSimit != HOSet[0]->getSimulatedVectorVars()->end(); ++VSimit)
    {
      Def->Vectors.Add(VSimit->first);
    }

  }


  if (Def->Vectors.GetCount() > 0)
  {

    wxString FilenameRoot;

    for (i=0;i<HOSet.size();i++)
    {
      if (HOSet[i] != NULL)
      {
        // set filename object kind
        FilenameRoot = wxT("SU");
        if (Def->ObjectsKind == wxT("RSout")) FilenameRoot = wxT("RS");
        if (Def->ObjectsKind == wxT("GUout")) FilenameRoot = wxT("GU");

        // set filename object ID
        FilenameRoot = FilenameRoot + wxString::Format(wxT("%d"),HOSet[i]->getID());

        // set filename suffix
        if (Def->FileSuffix.Length() > 0) FilenameRoot = FilenameRoot + wxT("_") + Def->FileSuffix;

        FilenameRoot = FilenameRoot + wxT(".vector.");


        for (j=0;j<Def->Vectors.GetCount();j++)
        {
          if (HOSet[i]->getSimulatedVectorVars()->find(Def->Vectors[j]) != HOSet[i]->getSimulatedVectorVars()->end())
          {
            Filename = FilenameRoot + Def->Vectors[j] + wxT(".") + MHYDAS_DEFAULT_OUPUTFILES_EXT;

            FileContents.Clear();

            // file header
            FileContents << CommentChar << wxT(" simulation ID: ") << ExSI.SimID << wxT("\n");
            FileContents << CommentChar << wxT(" file: ") << Filename << wxT("\n");
            FileContents << CommentChar << wxT(" object ID: ") << HOSet[i]->getID() << wxT("\n");
            FileContents << CommentChar << wxT(" vector variable values (after date and time): ") << Def->Vectors[j] << wxT("\n");

            for (k=0;k<10;k++) FileContents << CommentChar;
            FileContents << wxT("\n");

            for (k=0;k<DTStrings.Count();k++)
            {
              FileContents << DTStrings[k];

              VValues.clear();

              if (HOSet[i]->getSimulatedVectorVars()->find(Def->Vectors[j]) != HOSet[i]->getSimulatedVectorVars()->end())
                VValues = (HOSet[i]->getSimulatedVectorVars()->find(Def->Vectors[j])->second)->at(k);


              if (VValues.size() > 0)
              {
                for (l=0;l<VValues.size();l++)
                {
                  FileContents << ColSeparator << VValues.at(l);
                }
              }

              FileContents << wxT("\n");
            }

            wxFile RFile(mp_RunEnv->getOutputFullPath(Filename),wxFile::write);

            RFile.Write(FileContents);

            RFile.Close();

          }
        }
      }
    }
  }


  return true;
}




// =====================================================================
// =====================================================================



bool IOManager::saveResults(openfluid::core::CoreRepository *Data, RunConfig Config, int TimeStepsCount, ExtraSimInfos ExSI)
{
  bool IsOK = true;


  if (prepareOutputDir())
  {
    int i;
    wxString Filename;
    AutoOutfileDef* CurrentDef;

    wxArrayString DTStrings;
    wxDateTime CurrentDate = Config.BeginDate;

    // preparing and formatting datetime column(s)
    for (i=0;i<TimeStepsCount;i++)
    {
      DTStrings.Add(CurrentDate.Format(m_AutoOutFiles.DTFormat));
      CurrentDate = CurrentDate + wxTimeSpan(0,0,Config.DeltaT,0);
    }


    // saving data matching each definition
    for (i=0;i<m_AutoOutFiles.Defs.size();i++)
    {
      CurrentDef = m_AutoOutFiles.Defs[i];

      saveResultsFromDef(Data->getSpatialData(),
                         m_AutoOutFiles.ColSeparator,m_AutoOutFiles.CommentChar,
                         CurrentDef,DTStrings,ExSI);

    }

  }
  else IsOK = false;



  return IsOK;
}


// =====================================================================
// =====================================================================

bool IOManager::saveSimulationInfos(openfluid::core::CoreRepository *CoreData, ExtraSimInfos ExSI, openfluid::base::SimulationInfo *SimInfo)
{


  bool IsOK = true;


  if (prepareOutputDir())
  {

    int i;


    // ********** text file **********

    wxString FileContents = wxT("");

    FileContents << wxT("************************************************************") << wxT("\n");
    FileContents << wxT("*                                                          *\n");
    FileContents << wxT("*                     Simulation report                    *") << wxT("\n");
    FileContents << wxT("*                                                          *\n");
    FileContents << wxT("************************************************************") << wxT("\n");
    FileContents << wxT("\n");

    if (mp_ExecMsgs->isErrorFlag())
    {
      FileContents << wxT("ERROR: ") << FormatExecutionMessage(mp_ExecMsgs->getErrorMsg()) << wxT("\n");
      FileContents << wxT("\n");
    }

    FileContents << wxT("Simulation ID: ") << ExSI.SimID << wxT("\n");
    FileContents << wxT("Date: ") << ExSI.StartTime.Format(wxT("%Y-%m-%d %H:%M:%S")) << wxT("\n");
    FileContents << wxT("Computer: ") << wxGetHostName() << wxT("\n");
    FileContents << wxT("User: ") << wxGetUserId() << wxT(" (") << wxGetUserName() << wxT(")") << wxT("\n");
    FileContents << wxT("\n");
    FileContents << wxT("Input data set: ") << mp_RunEnv->getInputDir() << wxT("\n");
    FileContents << wxT("Output data set: ") << mp_RunEnv->getOutputDir()  << wxT("\n");
    FileContents << wxT("\n");
    FileContents << wxT("Surface units (SU): ") << CoreData->getSpatialData()->getSUsCollection()->size() << wxT("\n");
    FileContents << wxT("Reach segments (RS): ") << CoreData->getSpatialData()->getRSsCollection()->size() << wxT("\n");
    FileContents << wxT("Groundwater units (GU): ") << CoreData->getSpatialData()->getGUsCollection()->size() << wxT("\n");

    if (SimInfo != NULL)
    {
      FileContents << wxT("Simulation period: ") << (SimInfo->getStartTime().Format(wxT("%Y-%m-%d %H:%M:%S"))) << wxT(" to ") << (SimInfo->getEndTime().Format(wxT("%Y-%m-%d %H:%M:%S"))) << wxT("\n");
      FileContents << wxT("Time steps: ") << SimInfo->getStepsCount() << wxT(" of ") << SimInfo->getTimeStep() << wxT(" seconds") << wxT("\n");
    }

    FileContents << wxT("\n");
    FileContents << wxT("------------------------------------------------------------") << wxT("\n");
    FileContents << wxT("\n");



    wxArrayString WMessages = mp_ExecMsgs->getWarningMsgs();
    int WarningCount = WMessages.Count();

    // warnings
    if ( WarningCount > 0)
    {

      for (i=0; i<WarningCount;i++)
      {
        FileContents << wxT("WARNING: ") << FormatExecutionMessage(WMessages.Item(i)) << wxT("\n");
      }
    }
    else FileContents << wxT("NO WARNING");
    FileContents << wxT("\n");
    FileContents << wxT("------------------------------------------------------------") << wxT("\n");
    FileContents << wxT("\n");


    // write file to disk
    wxFile SimInfoFile(mp_RunEnv->getOutputFullPath(MHYDAS_DEFAULT_SIMINFOFILE),wxFile::write);
    SimInfoFile.Write(FileContents);
    SimInfoFile.Close();


    // ********** xml file **********
    wxString XMLFileContents = wxT("");

    XMLFileContents << wxT("<?xml version=\"1.0\" standalone=\"yes\"?>") << wxT("\n");
    XMLFileContents << wxT("<mhydas>") << wxT("\n");
    XMLFileContents << wxT("  <simreport>") << wxT("\n");

    if (mp_ExecMsgs->isErrorFlag()) XMLFileContents << wxT("    <error message=\"") << FormatExecutionMessage(mp_ExecMsgs->getErrorMsg()) << wxT("\" />") << wxT("\n");

    if (WarningCount > 0)
    {
      XMLFileContents << wxT("    <warnings count=\"") << WarningCount << wxT("\">") << wxT("\n");
      for (i=0; i<WarningCount;i++)
      {
        XMLFileContents << wxT("      <message text=\"") << FormatExecutionMessage(WMessages.Item(i)) << wxT("\" />") << wxT("\n");
      }
      XMLFileContents << wxT("    </warnings>") << wxT("\n");
    }
    else XMLFileContents << wxT("    <warnings count=\"0\" />") << wxT("\n");

    XMLFileContents << wxT("  </simreport>") << wxT("\n");
    XMLFileContents << wxT("</mhydas>") << wxT("\n");

    wxFile XMLSimInfoFile(mp_RunEnv->getOutputFullPath(MHYDAS_DEFAULT_SIMINFOFILE + wxT(".xml")),wxFile::write);
    XMLSimInfoFile.Write(XMLFileContents);
    XMLSimInfoFile.Close();
  }
  else IsOK = false;


  return IsOK;

}

// =====================================================================
// =====================================================================


bool IOManager::prepareTraceDir(openfluid::core::CoreRepository *Data)
{

  bool IsOK = true;


  if (!wxDirExists(mp_RunEnv->getTraceDir()))
  {
#ifdef __WXMSW__
    wxMkDir(mp_RunEnv->getTraceDir().mb_str(wxConvUTF8));
#else
    wxMkDir(mp_RunEnv->getTraceDir().mb_str(wxConvUTF8),0777);
#endif
    IsOK = wxDirExists(mp_RunEnv->getTraceDir());
  }
  else
  {
    EmptyDirectoryRecursively(mp_RunEnv->getTraceDir().mb_str(wxConvUTF8));
  }



  // preparing files headers for columns order
  if (IsOK)
  {
    wxString Filename, Filecontent;
    wxString NaNStr = wxT("!");


    openfluid::core::SimulatedVarsMap::iterator Simit;
    openfluid::core::SimulatedVectorVarsMap::iterator VSimit;

    openfluid::core::SUMap::iterator SUit;
    openfluid::core::SurfaceUnit* SU;
    openfluid::core::RSMap::iterator RSit;
    openfluid::core::ReachSegment* RS;
    openfluid::core::GUMap::iterator GUit;
    openfluid::core::GroundwaterUnit* GU;

    wxFile TFile;


    // SUs headers

    for(SUit = Data->getSpatialData()->getSUsCollection()->begin(); SUit != Data->getSpatialData()->getSUsCollection()->end(); ++SUit)
    {
      SU = SUit->second;

      // scalars
      if (SU->getSimulatedVars()->size() > 0)
      {
        Filecontent = wxT("% YEAR MONTH DAY HOUR MINUTE SECOND");

        for(Simit = SU->getSimulatedVars()->begin(); Simit != SU->getSimulatedVars()->end(); ++Simit)
        {
          Filecontent << wxT(" ") << Simit->first;
        }

        Filecontent << wxT("\n");

        Filename = wxT("SU") + wxString::Format(wxT("%d"),SU->getID()) + wxT(".scalars.") + MHYDAS_DEFAULT_TRACEFILES_EXT;
        TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
        TFile.Write(Filecontent);
        TFile.Close();
      }

      // vectors
      if (SU->getSimulatedVectorVars()->size() > 0)
      {
        for(VSimit = SU->getSimulatedVectorVars()->begin(); VSimit != SU->getSimulatedVectorVars()->end(); ++VSimit)
        {
          Filecontent = wxT("% YEAR MONTH DAY HOUR MINUTE SECOND [values of vector ") + VSimit->first + wxT("]\n");
          Filename = wxT("SU") + wxString::Format(wxT("%d"),SU->getID()) + wxT(".vector.") + VSimit->first + wxT(".") + MHYDAS_DEFAULT_TRACEFILES_EXT;

          TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
          TFile.Write(Filecontent);

          TFile.Close();
        }

      }


    }


    // RSs headers

    for(RSit = Data->getSpatialData()->getRSsCollection()->begin(); RSit != Data->getSpatialData()->getRSsCollection()->end(); ++RSit)
    {
      RS = RSit->second;

      if (RS->getSimulatedVars()->size() > 0)
      {
        Filecontent = wxT("% YEAR MONTH DAY HOUR MINUTE SECOND");

        for(Simit = RS->getSimulatedVars()->begin(); Simit != RS->getSimulatedVars()->end(); ++Simit)
        {
          Filecontent << wxT(" ") << Simit->first;
        }

        Filecontent << wxT("\n");

        Filename = wxT("RS") + wxString::Format(wxT("%d"),RS->getID()) + wxT(".scalars.") + MHYDAS_DEFAULT_TRACEFILES_EXT;
        TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
        TFile.Write(Filecontent);
        TFile.Close();
      }

      // vectors
      if (RS->getSimulatedVectorVars()->size() > 0)
      {
        for(VSimit = RS->getSimulatedVectorVars()->begin(); VSimit != RS->getSimulatedVectorVars()->end(); ++VSimit)
        {
          Filecontent = wxT("% YEAR MONTH DAY HOUR MINUTE SECOND [values of vector ") + VSimit->first + wxT("]\n");
          Filename = wxT("RS") + wxString::Format(wxT("%d"),RS->getID()) + wxT(".vector.") + VSimit->first + wxT(".") + MHYDAS_DEFAULT_TRACEFILES_EXT;

          TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
          TFile.Write(Filecontent);

          TFile.Close();
        }

      }


    }


    // GUs headers

    for(GUit = Data->getSpatialData()->getGUsCollection()->begin(); GUit != Data->getSpatialData()->getGUsCollection()->end(); ++GUit)
    {
      GU = GUit->second;

      if (GU->getSimulatedVars()->size() > 0)
      {
        Filecontent = wxT("% YEAR MONTH DAY HOUR MINUTE SECOND");

        for(Simit = GU->getSimulatedVars()->begin(); Simit != GU->getSimulatedVars()->end(); ++Simit)
        {
          Filecontent << wxT(" ") << Simit->first;
        }

        Filecontent << wxT("\n");

        Filename = wxT("GU") + wxString::Format(wxT("%d"),GU->getID()) + wxT(".scalars.") + MHYDAS_DEFAULT_TRACEFILES_EXT;
        TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
        TFile.Write(Filecontent);
        TFile.Close();
      }

      // vectors
      if (GU->getSimulatedVectorVars()->size() > 0)
      {
        for(VSimit = GU->getSimulatedVectorVars()->begin(); VSimit != GU->getSimulatedVectorVars()->end(); ++VSimit)
        {
          Filecontent = wxT("% YEAR MONTH DAY HOUR MINUTE SECOND [values of vector ") + VSimit->first + wxT("]\n");
          Filename = wxT("GU") + wxString::Format(wxT("%d"),GU->getID()) + wxT(".vector.") + VSimit->first + wxT(".") + MHYDAS_DEFAULT_TRACEFILES_EXT;

          TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
          TFile.Write(Filecontent);

          TFile.Close();
        }

      }


    }


  }

   return IsOK;

}

// =====================================================================
// =====================================================================


bool IOManager::saveTrace(openfluid::core::CoreRepository *Data, int Step, wxDateTime DT)
{

  wxString Filename, Filecontent;
  wxString NaNStr = wxT("!");

  openfluid::core::SimulatedVarsMap::iterator Simit;
  openfluid::core::SimulatedVectorVarsMap::iterator VSimit;
  openfluid::core::SerieOfMHYDASScalarValue* Values;
  openfluid::core::SerieOfMHYDASVectorValue* VValues;

  openfluid::core::SUMap::iterator SUit;
  openfluid::core::SurfaceUnit* SU;
  openfluid::core::RSMap::iterator RSit;
  openfluid::core::ReachSegment* RS;
  openfluid::core::GUMap::iterator GUit;
  openfluid::core::GroundwaterUnit* GU;

  openfluid::core::MHYDASVectorValue* ZeVector;
  int i;

  wxFile TFile;

  // trace SUs

  for(SUit = Data->getSpatialData()->getSUsCollection()->begin(); SUit != Data->getSpatialData()->getSUsCollection()->end(); ++SUit)
  {
    SU = SUit->second;

    // scalars
    if (SU->getSimulatedVars()->size() > 0)
    {
      Filecontent = DT.Format(wxT("%Y %m %d %H %M %S"));

      for(Simit = SU->getSimulatedVars()->begin(); Simit != SU->getSimulatedVars()->end(); ++Simit)
      {
        Values = Simit->second;

        if (Values->size() > Step) Filecontent << wxT(" ") << Values->at(Step);
        else Filecontent << wxT(" ") << NaNStr;
      }

      Filecontent << wxT("\n");

      Filename = wxT("SU") + wxString::Format(wxT("%d"),SU->getID()) + wxT(".scalars.") + MHYDAS_DEFAULT_TRACEFILES_EXT;
      TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
      TFile.Write(Filecontent);
      TFile.Close();
    }

    // vectors
    if (SU->getSimulatedVectorVars()->size() > 0)
    {

      for(VSimit = SU->getSimulatedVectorVars()->begin(); VSimit != SU->getSimulatedVectorVars()->end(); ++VSimit)
      {
        VValues = VSimit->second;
        Filename = wxT("SU") + wxString::Format(wxT("%d"),SU->getID()) + wxT(".vector.") +VSimit->first + wxT(".") + MHYDAS_DEFAULT_TRACEFILES_EXT;

        Filecontent = DT.Format(wxT("%Y %m %d %H %M %S"));

        if (VValues->size() > Step)
        {
          ZeVector = &(VValues->at(Step));
          for (i=0;i<ZeVector->size();i++) Filecontent << wxT(" ") << ZeVector->at(i);
        }
        else Filecontent << wxT(" ") << NaNStr;

        Filecontent << wxT("\n");
        TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
        TFile.Write(Filecontent);
        TFile.Close();

      }
    }


  }


  // trace RSs

  for(RSit = Data->getSpatialData()->getRSsCollection()->begin(); RSit != Data->getSpatialData()->getRSsCollection()->end(); ++RSit)
  {
    RS = RSit->second;


    // scalars
    if (RS->getSimulatedVars()->size() > 0)
    {
      Filecontent = DT.Format(wxT("%Y %m %d %H %M %S"));

      for(Simit = RS->getSimulatedVars()->begin(); Simit != RS->getSimulatedVars()->end(); ++Simit)
      {
        Values = Simit->second;

        if (Values->size() > Step) Filecontent << wxT(" ") << Values->at(Step);
        else Filecontent << wxT(" ") << NaNStr;
      }

      Filecontent << wxT("\n");

      Filename = wxT("RS") + wxString::Format(wxT("%d"),RS->getID()) + wxT(".scalars.") + MHYDAS_DEFAULT_TRACEFILES_EXT;
      TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
      TFile.Write(Filecontent);
      TFile.Close();
    }


    // vectors
    if (RS->getSimulatedVectorVars()->size() > 0)
    {

      for(VSimit = RS->getSimulatedVectorVars()->begin(); VSimit != RS->getSimulatedVectorVars()->end(); ++VSimit)
      {
        VValues = VSimit->second;
        Filename = wxT("RS") + wxString::Format(wxT("%d"),RS->getID()) + wxT(".vector.") +VSimit->first + wxT(".") + MHYDAS_DEFAULT_TRACEFILES_EXT;

        Filecontent = DT.Format(wxT("%Y %m %d %H %M %S"));

        if (VValues->size() > Step)
        {
          ZeVector = &(VValues->at(Step));
          for (i=0;i<ZeVector->size();i++) Filecontent << wxT(" ") << ZeVector->at(i);
        }
        else Filecontent << wxT(" ") << NaNStr;

        Filecontent << wxT("\n");
        TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
        TFile.Write(Filecontent);
        TFile.Close();

      }
    }


  }


  // trace GUs

  for(GUit = Data->getSpatialData()->getGUsCollection()->begin(); GUit != Data->getSpatialData()->getGUsCollection()->end(); ++GUit)
  {
    GU = GUit->second;

    // scalars
    if (GU->getSimulatedVars()->size() > 0)
    {
      Filecontent = DT.Format(wxT("%Y %m %d %H %M %S"));

      for(Simit = GU->getSimulatedVars()->begin(); Simit != GU->getSimulatedVars()->end(); ++Simit)
      {
        Values = Simit->second;

        if (Values->size() > Step) Filecontent << wxT(" ") << Values->at(Step);
        else Filecontent << wxT(" ") << NaNStr;
      }

      Filecontent << wxT("\n");

      Filename = wxT("GU") + wxString::Format(wxT("%d"),GU->getID()) + wxT(".scalars.") + MHYDAS_DEFAULT_TRACEFILES_EXT;
      TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
      TFile.Write(Filecontent);
      TFile.Close();
    }

    // vectors
    if (GU->getSimulatedVectorVars()->size() > 0)
    {

      for(VSimit = GU->getSimulatedVectorVars()->begin(); VSimit != GU->getSimulatedVectorVars()->end(); ++VSimit)
      {
        VValues = VSimit->second;
        Filename = wxT("GU") + wxString::Format(wxT("%d"),GU->getID()) + wxT(".vector.") +VSimit->first + wxT(".") + MHYDAS_DEFAULT_TRACEFILES_EXT;

        Filecontent = DT.Format(wxT("%Y %m %d %H %M %S"));

        if (VValues->size() > Step)
        {
          ZeVector = &(VValues->at(Step));
          for (i=0;i<ZeVector->size();i++) Filecontent << wxT(" ") << ZeVector->at(i);
        }
        else Filecontent << wxT(" ") << NaNStr;

        Filecontent << wxT("\n");
        TFile.Open(mp_RunEnv->getTraceFullPath(Filename),wxFile::write_append);
        TFile.Write(Filecontent);
        TFile.Close();

      }
    }


  }


}

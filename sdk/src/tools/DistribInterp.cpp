/**
  \file InterpDTSerie.cpp
  \brief 
  
  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "DistribInterp.h"
#include "ColTextParser.h"

#include <math.h>

namespace mhydasdk { namespace tools {


DistributeInterpolate::DistributeInterpolate()
                     : Tools()
{
  m_Configured = false;
}

// =====================================================================
// =====================================================================


DistributeInterpolate::~DistributeInterpolate()
{
  // TODO clean delete of m_InterpData 
}

// =====================================================================
// =====================================================================

void DistributeInterpolate::setConfig(wxString DataDir, wxString DataSourcesFilename, wxString DistributionFilename, SeriePreprocess SPpcs, wxDateTime Begin,wxDateTime End, int TimeStep)
{
  m_DataDir = DataDir;
  m_DataSourcesFilename = DataSourcesFilename;
  m_DistributionFilename = DistributionFilename;
  m_SPpcs = SPpcs;
  m_Begin = Begin;
  m_End = End;
  m_TimeStep = TimeStep;
  
  m_Configured = true;
}



// =====================================================================
// =====================================================================


bool DistributeInterpolate::loadAndPrepareData()
{
  mhydasdk::tools::DataSourcesFile DSFile;
  mhydasdk::tools::DateTimeSerie *Serie;
  mhydasdk::tools::DateTimeSerie *InterpolatedSerie;
  mhydasdk::tools::IndexedSerie *IInterpolatedSerie;
  

  if (!m_Configured)
  {
    setErrorMessage(wxT("Distributed interpolator not configured"));
    return false; 
  }
  
  
  // loading of data sources  
  if (!DSFile.load(m_DataDir + wxFILE_SEP_PATH + m_DataSourcesFilename) && DSFile.getIDs().size() <= 1)
  {    
    setErrorMessage(wxT("Error loading ")+m_DataSourcesFilename+wxT(" file"));
    return false;
  }
  else
  {
    std::vector<int> IDs = DSFile.getIDs();
      
    
    for (int i=0;i<IDs.size();i++)
    {      
      if (!wxFileExists(m_DataDir + wxFILE_SEP_PATH + DSFile.getSource(IDs[i])))
      {
        setErrorMessage(wxT("Error loading ")+DSFile.getSource(IDs[i])+wxT(" file as data source"));
        return false;
      }
      else
      {
        // load file
   
        InterpolatedSerie = new mhydasdk::tools::DateTimeSerie();
        Serie = new mhydasdk::tools::DateTimeSerie();
        IInterpolatedSerie = new mhydasdk::tools::IndexedSerie(); 
        
        if (loadDataAsSerie(m_DataDir + wxFILE_SEP_PATH + DSFile.getSource(IDs[i]),m_SPpcs, Serie))
        {
                    
          // interpolate in time for simulation
          if (Serie->createInterpolatedSerie(m_Begin-wxTimeSpan(0,0,m_TimeStep,0),m_End+wxTimeSpan(0,0,m_TimeStep,0),m_TimeStep,InterpolatedSerie))          
          {
            // store 
            m_InterpData[IDs[i]] = InterpolatedSerie;
            InterpolatedSerie->createIndexedSerie(IInterpolatedSerie);
            m_InterpIndexedData[IDs[i]] = IInterpolatedSerie;
            delete Serie;
            
          }
          else
          {
            setErrorMessage(wxT("Error interpolating data from ")+DSFile.getSource(IDs[i])+wxT(" file"));
            delete Serie;
            delete InterpolatedSerie;
            return false;
          }                                          
        }
        else
        {
          setErrorMessage(wxT("Error loading data from")+DSFile.getSource(IDs[i])+wxT(" file"));
          delete Serie;
          delete InterpolatedSerie;
          return false;
        }
      }
    }
    
    
    if (!loadDistributionAndDistribute(m_DataDir + wxFILE_SEP_PATH + m_DistributionFilename))
    {      
      return false;
    }
      

    
  }  


  return true;
  
}

// =====================================================================
// =====================================================================


bool DistributeInterpolate::loadDataAsSerie(wxString FilePath, SeriePreprocess SPpcs, DateTimeSerie *Serie)
{
  ColumnTextParser FileParser(wxT("%"));
  bool IsOK;

  IsOK = true;

  long int Year, Month, Day, Hour, Min, Sec;
  double Value;
  double CumValue, ValueToAdd;
  wxDateTime ZeDT;

  
  
  Serie->clear();
  
  
  if (FileParser.loadFromFile(FilePath) && (FileParser.getLinesCount() > 0) && (FileParser.getColsCount() == 7))
  {
    int i = 0;
    CumValue = 0;
    
    while (i<FileParser.getLinesCount() && IsOK)
    {
      if (FileParser.getLongValue(i,0,&Year) && FileParser.getLongValue(i,1,&Month) &&
          FileParser.getLongValue(i,2,&Day) && FileParser.getLongValue(i,3,&Hour) &&
          FileParser.getLongValue(i,4,&Min) && FileParser.getLongValue(i,5,&Sec) &&
          FileParser.getDoubleValue(i,6,&Value))
      {
          
        if (isnan(Value) != 1 && isinf(Value) != 1)
        {

          ZeDT = wxDateTime(Day,wxDateTime::Month(wxDateTime::Jan + Month-1),Year,Hour,Min,Sec);

          CumValue +=  Value;

          if (SPpcs == SERIEPREPCS_CUMULATE) ValueToAdd = CumValue;
          else ValueToAdd = Value;


          if (!isnan(ValueToAdd) && !isinf(ValueToAdd))
          {
            if (!Serie->addValue(ZeDT,ValueToAdd))
            {
              IsOK = false;
            }
          }
          else
          {
            IsOK = false;
          }
        }
      }
      else
      {
        IsOK = false;
      }

      i++;
    }
  }
  else
  {
    IsOK = false;
  }


  return IsOK;

}

// =====================================================================
// =====================================================================


bool DistributeInterpolate::loadDistributionAndDistribute(wxString FilePath)
{
  long UnitID;
  long DataSrcID;

  int i;
    

  ColumnTextParser DistriFileParser(wxT("%"));

 
  if (wxFileExists(FilePath))
  {        
    
    if ((DistriFileParser.loadFromFile(FilePath)) && (DistriFileParser.getColsCount() == 2) && (DistriFileParser.getLinesCount() >0))
    {  

      for (i=0;i<DistriFileParser.getLinesCount();i++)
      {

        if (DistriFileParser.getLongValue(i,0,&UnitID) && DistriFileParser.getLongValue(i,1,&DataSrcID))
        {

          if (m_InterpData.find(DataSrcID) != m_InterpData.end())
          {
            m_UnitsData[UnitID] = m_InterpData[DataSrcID];
            m_UnitsIndexedData[UnitID] = m_InterpIndexedData[DataSrcID];
          }
          else
          {
            setErrorMessage(wxT("Error in distribution file ")+m_DistributionFilename+wxT(", data source ID not found"));
            return false;
          }
        }
        else
        {
          setErrorMessage(wxT("Error in distribution file ")+m_DistributionFilename+wxT(", format error"));
          return false;
        }
      }
    }
    else
    {
      setErrorMessage(wxT("Error in distribution file ")+m_DistributionFilename+wxT(", file not found or format error"));
      return false;      
    }
  }
  else
  {
    setErrorMessage(wxT("Distribution file ")+m_DistributionFilename+wxT(" not found"));    
    return false;
  }
  return true;
}


// =====================================================================
// =====================================================================


bool DistributeInterpolate::getValue(int ID, wxDateTime DT, mhydasdk::core::MHYDASScalarValue *Value)
{

  return (m_UnitsData[ID]->getValue(DT,Value));
}


// =====================================================================
// =====================================================================


bool DistributeInterpolate::getValue(int ID, int Index, mhydasdk::core::MHYDASScalarValue *Value)
{
    
  if (Index>m_UnitsIndexedData[ID]->Count) return false;
  
  *Value = (m_UnitsIndexedData[ID]->Values[Index]);
  return true;
}


} } 



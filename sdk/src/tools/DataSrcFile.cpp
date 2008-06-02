/**
  \file DataSrcFile.cpp
  \brief 

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "DataSrcFile.h"
#include "tinyxml.h"


namespace mhydasdk { namespace tools {


DataSourcesFile::DataSourcesFile()
{
  m_Loaded = false;
  m_IDs.clear();
  m_Sources.clear();
  
}

// =====================================================================
// =====================================================================


DataSourcesFile::~DataSourcesFile()
{

}

// =====================================================================
// =====================================================================



bool DataSourcesFile::load(wxString Filename)
{

  TiXmlDocument LoadDoc;
  int ID;
  
  
  m_Loaded = true;
  m_IDs.clear();
  m_Sources.clear();  

  
  if (LoadDoc.LoadFile(Filename.mb_str(wxConvUTF8)))
  {

    TiXmlHandle DocHandle(&LoadDoc);

    TiXmlElement* Child = DocHandle.FirstChild("mhydas").FirstChild("datasources").FirstChild("filesource").Element();


    for(Child; Child; Child=Child->NextSiblingElement())
    {
      if (Child->Attribute("ID",&ID) != NULL && Child->Attribute("file") != NULL)
      {
        m_Sources[ID] = wxString(Child->Attribute("file"),wxConvUTF8);
        m_IDs.push_back(ID);
      }
      else
      {
        m_Loaded = false;
      }
    }
  }
  else
  {
    m_Loaded = false;  
  }

  if (!m_Loaded)
  {
    m_IDs.clear();
    m_Sources.clear();      
  }
  
  return m_Loaded;
  
}

// =====================================================================
// =====================================================================


std::vector<int> DataSourcesFile::getIDs()
{
  return m_IDs;
}

// =====================================================================
// =====================================================================


wxString DataSourcesFile::getSource(int ID)
{
  IDDataSourcesMap::iterator iter;
  
  iter = m_Sources.find(ID);
  
  if (iter != m_Sources.end()) return iter->second;
  else return wxT("");
}




} } // namespace


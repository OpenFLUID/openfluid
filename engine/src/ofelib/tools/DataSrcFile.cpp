/**
  \file DataSrcFile.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "DataSrcFile.h"
#include "tinyxml.h"


namespace openfluid { namespace tools {


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



bool DataSourcesFile::load(std::string Filename)
{

  TiXmlDocument LoadDoc;
  int ID;


  m_Loaded = true;
  m_IDs.clear();
  m_Sources.clear();


  if (LoadDoc.LoadFile(Filename.c_str()))
  {

    TiXmlHandle DocHandle(&LoadDoc);

    TiXmlElement* Child = DocHandle.FirstChild("openfluid").FirstChild("datasources").FirstChild("filesource").Element();


    for(Child; Child; Child=Child->NextSiblingElement())
    {
      if (Child->Attribute("ID",&ID) != NULL && Child->Attribute("file") != NULL)
      {
        m_Sources[ID] = Child->Attribute("file");
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


std::string DataSourcesFile::getSource(int ID)
{
  IDDataSourcesMap::iterator iter;

  iter = m_Sources.find(ID);

  if (iter != m_Sources.end()) return iter->second;
  else return "";
}




} } // namespace


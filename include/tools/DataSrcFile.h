/**
  \file DataSrcFile.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DATASRCFILE_H__
#define __DATASRCFILE_H__


#include <wx/string.h>
#include <wx/arrstr.h>
#include <vector>
#include <map>
#include <wx/hashmap.h>



namespace openfluid { namespace tools {

//WX_DECLARE_HASH_MAP(int, std::string, wxIntegerHash, wxIntegerEqual, IDDataSourcesMap);
typedef std::map<int,std::string> IDDataSourcesMap;

class DataSourcesFile
{
  private:
    bool m_Loaded;

    std::vector<int> m_IDs;

    IDDataSourcesMap m_Sources;


  public:

    DataSourcesFile();

    virtual ~DataSourcesFile();

    bool load(std::string Filename);

    std::vector<int> getIDs();

    std::string getSource(int ID);

};


} } // namespace



#endif /*__DATASRCFILE_H__*/

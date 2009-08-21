/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DataSrcFile.h
  \brief Header of class for managing data sources files

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DATASRCFILE_H__
#define __DATASRCFILE_H__


#include <vector>
#include <map>
#include <string>


namespace openfluid { namespace tools {

typedef std::map<int,std::string> IDDataSourcesMap;
/**
  Class for managing data sources files
*/
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

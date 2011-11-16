/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DISTRIBINTERP_HPP__
#define __DISTRIBINTERP_HPP__

#include <map>
#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>

namespace openfluid {
namespace core {
class DoubleValue;
}
}

namespace openfluid { namespace tools {

class DateTimeSerie;
class IndexedSerie;

typedef std::map<int,DateTimeSerie*> DTSeriesMap;
typedef std::map<int,DateTimeSerie*> UnitsDTSeriesMap;
typedef std::map<int,IndexedSerie*> IndexedSeriesMap;
typedef std::map<int,IndexedSerie*> UnitsIndexedSeriesMap;



enum SeriePreprocess
{
  SERIEPREPCS_NONE,
  SERIEPREPCS_CUMULATE
};

/**
  Class for time interpolation and spatial distribution of data
*/
class DLLEXPORT DistributeInterpolate
{

  private:

    DTSeriesMap m_InterpData;
    IndexedSeriesMap m_InterpIndexedData;
    UnitsDTSeriesMap m_UnitsData;
    UnitsIndexedSeriesMap m_UnitsIndexedData;


    std::string m_DataSourcesFilename;
    std::string m_DataDir;
    std::string m_DistributionFilename;
    SeriePreprocess m_SPpcs;
    openfluid::core::DateTime m_Begin;
    openfluid::core::DateTime m_End;
    int m_TimeStep;

    bool m_Configured;

    bool loadDataAsSerie(std::string FilePath, SeriePreprocess SPpcs, DateTimeSerie *Serie);

    bool loadDistributionAndDistribute(std::string FilePath);

  public:
    DistributeInterpolate();

    void setConfig(std::string DataDir, std::string DataSourcesFilename, std::string DistributionFilename, SeriePreprocess SPpcs,
                   openfluid::core::DateTime Begin,openfluid::core::DateTime End, int TimeStep);

    bool loadAndPrepareData();

    bool getValue(int ID, openfluid::core::DateTime DT, openfluid::core::DoubleValue *Value);

    bool getValue(int ID, int Index, openfluid::core::DoubleValue *Value);

    virtual ~DistributeInterpolate();
};

} }


#endif /*__DISTRIBINTERP_H__*/


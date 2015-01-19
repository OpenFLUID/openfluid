/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/



/**
  @file CSVFilesObs.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <boost/foreach.hpp>

#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/tools/DataHelpers.hpp>



// =====================================================================
// =====================================================================


DECLARE_OBSERVER_PLUGIN


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("export.spatial-graph.files.dot")
  DECLARE_NAME("Exports spatial graph to Dot files");
  DECLARE_DESCRIPTION("This observer exports spatial domain graph to Dot files and can be post-processed with GraphViz\n"
      "Parameters can be\n"
      "  title : the main title of the graph\n"
      "  when.init : set to 1 to export the graph at initialization time\n"
      "  when.everytime : set to 1 to export the graph at every simulation time point\n"
      "  when.final : set to 1 to export the graph at finalization time\n"
      "  style.<unitclass>.<attribute> : the value of a Dot attribute for all nodes of a unit class\n");
  DECLARE_VERSION(openfluid::config::FULL_VERSION);
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);
END_OBSERVER_SIGNATURE


// =====================================================================
// =====================================================================


class DotFilesObserver : public openfluid::ware::PluggableObserver
{
  private:

    typedef std::map<std::string,std::string> StyleAttibutes_t;

    typedef std::map<std::string,StyleAttibutes_t> UnitClassStyles_t;

    std::string m_Title;

    std::string m_BaseFileName;

    UnitClassStyles_t m_UnitClassStyles;

    bool m_Init;
    bool m_EveryTime;
    bool m_Final;


    // =====================================================================
    // =====================================================================


    static std::string generateDotEdge(const std::string& SrcClass, const std::string& SrcID,
                                       const std::string& DestClass, const std::string& DestID,
                                       const std::string& Options)
    {
      return "\""+SrcClass+" #"+SrcID+"\" -> \""+DestClass+" #"+DestID+"\" " + Options;
    }


    // =====================================================================
    // =====================================================================


    static std::string generateDotNode(const std::string& UClass, const std::string& UID,
                                       const std::string& Options)
    {
      return "\""+UClass+" #"+UID+"\" " + Options;
    }


    // =====================================================================
    // =====================================================================


    void ExportUnitsGraphAsDotFile(const std::string& Filename, const std::string& SubTitle)
    {

      std::ofstream DotFile;
      std::string OutputDir;
      const openfluid::core::UnitsListByClassMap_t* UnitsByClass = mp_SpatialData->allSpatialUnitsByClass();
      const openfluid::core::UnitsList_t* UnitsList = NULL;
      std::vector<openfluid::core::UnitClass_t> ClassVector;
      openfluid::core::SpatialUnit* TheUnit;

      openfluid::core::UnitsListByClassMap_t::const_iterator itUnitsClass;
      openfluid::core::UnitsList_t::const_iterator itUnitsList;

      OPENFLUID_GetRunEnvironment("dir.output",OutputDir);

      DotFile.open(std::string(OutputDir+"/"+Filename).c_str());

      DotFile << "digraph \"spatial-graph\" {" << "\n";
      DotFile << "\n";
      DotFile << "label = \"" << m_Title << "\\n" << SubTitle << "\";" << "\n";
      DotFile << "fontsize = 24;" << "\n";
      DotFile << "\n";


      for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
      {
        ClassVector.push_back((*itUnitsClass).first);

        std::string Options = "";

        std::string ClassStr = (*itUnitsClass).first;

        UnitClassStyles_t::iterator ItStyle = m_UnitClassStyles.find(ClassStr);

        if (ItStyle != m_UnitClassStyles.end())
        {
          Options = "[";

          StyleAttibutes_t::iterator ItAttr = (*ItStyle).second.begin();

          for (;ItAttr!=(*ItStyle).second.end();++ItAttr)
          {
            if (ItAttr != (*ItStyle).second.begin())
              Options += ",";

            Options += (*ItAttr).first+"="+(*ItAttr).second;
          }

          Options += "];";
        }

        UnitsList=((*itUnitsClass).second).list();

        for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
        {
          TheUnit = const_cast<openfluid::core::SpatialUnit*>(&(*itUnitsList));
          std::string IDStr = "";
          openfluid::tools::convertValue(TheUnit->getID(),&IDStr);
          DotFile << generateDotNode(ClassStr,IDStr,Options) << "\n";
        }
      }

      DotFile << "\n";

      for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
      {

        UnitsList=((*itUnitsClass).second).list();

        for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
        {
          TheUnit = const_cast<openfluid::core::SpatialUnit*>(&(*itUnitsList));
          std::string SrcClassStr = TheUnit->getClass();
          std::string SrcIDStr = "";
          openfluid::tools::convertValue(TheUnit->getID(),&SrcIDStr);

          for (unsigned int i=0;i<ClassVector.size();i++)
          {
            const openfluid::core::UnitsPtrList_t* ToUnits = const_cast<openfluid::core::UnitsPtrList_t*>(TheUnit->toSpatialUnits(ClassVector[i]));

            if (ToUnits != NULL)
            {
              std::string DestClassStr = ClassVector[i];
              openfluid::core::UnitsPtrList_t::const_iterator itToUnits;

              for (itToUnits=ToUnits->begin();itToUnits!=ToUnits->end();++itToUnits)
              {
                std::string DestIDStr = "";
                openfluid::tools::convertValue((*itToUnits)->getID(),&DestIDStr);

                DotFile << generateDotEdge(SrcClassStr,SrcIDStr,DestClassStr,DestIDStr,"") << "\n";

              }
            }

            const openfluid::core::UnitsPtrList_t* ParentUnits = const_cast<openfluid::core::UnitsPtrList_t*>(TheUnit->parentSpatialUnits(ClassVector[i]));

            if (ParentUnits != NULL)
            {
              std::string DestClassStr = ClassVector[i];
              openfluid::core::UnitsPtrList_t::const_iterator itParentUnits;

              for (itParentUnits=ParentUnits->begin();itParentUnits!=ParentUnits->end();++itParentUnits)
              {
                std::string DestIDStr = "";
                openfluid::tools::convertValue((*itParentUnits)->getID(),&DestIDStr);

                DotFile << generateDotEdge(SrcClassStr,SrcIDStr,DestClassStr,DestIDStr,"[arrowhead=odiamond,color=gray50,style=dashed]") << "\n";

              }
            }
          }
        }
      }

      DotFile << "\n";
      DotFile << "}" << "\n";

      DotFile.close();
    }


  public:

    DotFilesObserver() : PluggableObserver(),
      m_Title("Spatial domain graph"), m_BaseFileName("spatial-graph"),
      m_Init(false),m_EveryTime(false),m_Final(false)
    {

    }


    // =====================================================================
    // =====================================================================


    ~DotFilesObserver()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      boost::property_tree::ptree ParamsPT;

      try
      {
        ParamsPT = openfluid::ware::PluggableWare::getParamsAsPropertyTree(Params);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        OPENFLUID_RaiseError(E.getMessage());
      }


      m_Title = ParamsPT.get("title",m_Title);

      m_Init = ParamsPT.get<bool>("when.init",false);
      m_EveryTime = ParamsPT.get<bool>("when.everytime",false);
      m_Final = ParamsPT.get<bool>("when.final",false);

      try
      {
        BOOST_FOREACH(const boost::property_tree::ptree::value_type &v,ParamsPT.get_child("style"))
        {
          BOOST_FOREACH(const boost::property_tree::ptree::value_type &w,ParamsPT.get_child("style."+v.first))
          {
            m_UnitClassStyles[v.first][w.first] = ParamsPT.get("style."+v.first+"."+w.first,"");
          }
        }
      }
      catch(boost::exception const& E)
      { }

      if (!(m_Init || m_EveryTime || m_Final))
        m_Final = true;

    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      if (m_Init) ExportUnitsGraphAsDotFile(m_BaseFileName+"_init.dot","at initialization");
    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {
      if (m_EveryTime)
      {
        std::ostringstream oss;
        oss << OPENFLUID_GetCurrentTimeIndex();
        ExportUnitsGraphAsDotFile(m_BaseFileName + "_t" + oss.str() + ".dot","at t="+oss.str());
      }
    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      if (m_EveryTime)
      {
        std::ostringstream oss;
        oss << OPENFLUID_GetCurrentTimeIndex();
        ExportUnitsGraphAsDotFile(m_BaseFileName + "_t" + oss.str() + ".dot","at t="+oss.str());
      }
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      if (m_Final) ExportUnitsGraphAsDotFile(m_BaseFileName+"_final.dot","at finalization");
    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(DotFilesObserver)

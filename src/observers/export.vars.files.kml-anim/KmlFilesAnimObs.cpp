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
  @file KmlFilesAnimObs.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/ware/WareParamsTree.hpp>

#include "../KmlObserverBase.hpp"


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("export.vars.files.kml-anim")

  DECLARE_NAME("Exports simulation variables to time indexed kml files for animation");
  // TODO complete description with parameters usage
  DECLARE_DESCRIPTION("");
  DECLARE_VERSION(openfluid::config::FULL_VERSION);
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

END_OBSERVER_SIGNATURE



// =====================================================================
// =====================================================================


class KmlAnimLayerInfo : public KmlLayerInfo<KmlUnitInfo>
{
  public:

    openfluid::core::VariableName_t VarName;

    std::string OutputFileName;

    std::vector<std::pair<std::string,double> > ColorScale;

    KmlAnimLayerInfo() :
      KmlLayerInfo<KmlUnitInfo>(),
      OutputFileName("")
    { };

    int getColorScaleIndexForValue(const double& Val)
    {


      for (unsigned int i=0;i<ColorScale.size()-1;i++)
      {
        if (Val < ColorScale[i].second) return i;
      }

      return ColorScale.size()-1;
    };


};


// =====================================================================
// =====================================================================


class KmlStaticLayerInfo : public KmlLayerInfo<KmlUnitInfo>
{
  public:

    std::string Color;

    KmlStaticLayerInfo() :
      KmlLayerInfo<KmlUnitInfo>(),
      Color("00000000")
    { };

};



// =====================================================================
// =====================================================================


class KmlFilesAnimObserver : public KmlObserverBase
{
  private:

    std::ofstream m_KmlFile;

    KmlAnimLayerInfo m_AnimLayerInfo;

    std::list<KmlStaticLayerInfo> m_StaticLayersInfo;

    openfluid::core::DateTime m_UpdateBeginDate;

    openfluid::core::Duration_t m_MinSamplingDelay;

    openfluid::core::TimeIndex_t m_LatestSamplingIndex;


    // =====================================================================
    // =====================================================================


    void updateKmlFile()
    {
      openfluid::core::SpatialUnit* UU;

      openfluid::core::TimeIndex_t CurrentTI = OPENFLUID_GetCurrentTimeIndex();


      std::string CurrentTIStr;

      openfluid::tools::convertValue(CurrentTI,&CurrentTIStr);

      std::ofstream CurrentKmlFile(std::string(m_TmpDir+"/"+m_KmzSubDir+"/"+
                                               m_KmzDataSubDir+"/t_"+ CurrentTIStr+".kml").c_str());

      CurrentKmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
      CurrentKmlFile << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" "
                        "xmlns:gx=\"http://www.google.com/kml/ext/2.2\" "
                        "xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n";

      CurrentKmlFile << "<Document>\n";

      CurrentKmlFile << "  <open>1</open>\n";


      std::string TmpStyleID = "anim_"+ m_AnimLayerInfo.UnitsClass+"_style";

      for (unsigned int i=0; i<m_AnimLayerInfo.ColorScale.size();i++)
      {
        if ((*(m_AnimLayerInfo.UnitsInfos.begin())).second.GeometryType == wkbPolygon)
          CurrentKmlFile << "    <Style id=\"" << TmpStyleID << "_" << i << "\"><PolyStyle><color>#"
                         << m_AnimLayerInfo.ColorScale[i].first << "</color><outline>0</outline></PolyStyle></Style>\n";
        else if ((*(m_AnimLayerInfo.UnitsInfos.begin())).second.GeometryType == wkbLineString)
          CurrentKmlFile << "    <Style id=\"" << TmpStyleID << "_" << i << "\"><LineStyle><color>#"
                         << m_AnimLayerInfo.ColorScale[i].first << "</color><width>"
                         << m_AnimLayerInfo.LineWidth
                         << "</width></LineStyle><PolyStyle><fill>0</fill></PolyStyle></Style>\n";
        else
          OPENFLUID_RaiseError("Unsupported geometry format in source geometry file");
      }


      for (std::map<openfluid::core::UnitID_t,KmlUnitInfo>::iterator it2=m_AnimLayerInfo.UnitsInfos.begin();
          it2!=m_AnimLayerInfo.UnitsInfos.end();
          ++it2)
      {

        bool ValueFound = false;
        double Val;

        UU = OPENFLUID_GetUnit(m_AnimLayerInfo.UnitsClass,(*it2).second.UnitID);

        if (UU != NULL)
        {
          if (OPENFLUID_IsVariableExist(UU,m_AnimLayerInfo.VarName,CurrentTI,openfluid::core::Value::DOUBLE))
          {
            OPENFLUID_GetVariable(UU,m_AnimLayerInfo.VarName,CurrentTI,Val);
            ValueFound = true;
          }
          else
          {
            openfluid::core::IndexedValue IVal;
            OPENFLUID_GetLatestVariable(UU,m_AnimLayerInfo.VarName,IVal);
            Val = IVal.value()->asDoubleValue().get();
            ValueFound = true;
          }
        }
        else
        {
          OPENFLUID_LogWarning("Unknown spatial unit not found");
        }


        CurrentKmlFile << "    <Placemark>\n";
        CurrentKmlFile << "      <name>" << m_AnimLayerInfo.UnitsClass << " " << (*it2).second.UnitID << "</name>\n";

        CurrentKmlFile << "      <description>\n<![CDATA[\n";
        CurrentKmlFile << "Unit class: " << m_AnimLayerInfo.UnitsClass << "<br/>\n";
        CurrentKmlFile << "Unit ID: " << (*it2).second.UnitID << "<br/>\n";

        if (ValueFound)
        {
          CurrentKmlFile << "<br/>\n";
          CurrentKmlFile << "Current time: "<< OPENFLUID_GetCurrentDate().getAsString("%Y-%m-%d %H:%M:%S")
                         << " (time index = " << CurrentTIStr<< ")<br/>\n";
          CurrentKmlFile << "Variable: " << m_AnimLayerInfo.VarName << "<br/>\n";
          CurrentKmlFile << "Value: " << Val << "<br/>\n";
        }

        CurrentKmlFile << "\n]]>\n      </description>\n";


        if (ValueFound)
          CurrentKmlFile << "      <styleUrl>#" << TmpStyleID << "_"
                         << m_AnimLayerInfo.getColorScaleIndexForValue(Val) << "</styleUrl>\n";


        if ((*it2).second.GeometryType == wkbPolygon)
        {
            CurrentKmlFile << "<Polygon><tessellate>1</tessellate><outerBoundaryIs><LinearRing><coordinates>"
                           << (*it2).second.CoordsStr << "</coordinates></LinearRing></outerBoundaryIs></Polygon>\n";
        }
        else if ((*it2).second.GeometryType == wkbLineString)
        {
          CurrentKmlFile << "<LineString><tessellate>1</tessellate><coordinates>" << (*it2).second.CoordsStr
                          << "</coordinates></LineString>\n";
        }
        else
          OPENFLUID_RaiseError("Unsupported geometry format in source geometry file");


        CurrentKmlFile << "    </Placemark>\n";
      }


      CurrentKmlFile << "</Document>\n";
      CurrentKmlFile << "</kml>\n";

      CurrentKmlFile.close();

      openfluid::core::DateTime EndDateTime = OPENFLUID_GetCurrentDate();

      m_KmlFile << "<NetworkLink>\n";
      m_KmlFile << "  <name>" << m_AnimLayerInfo.UnitsClass << " at " << EndDateTime.getAsString("%Y-%m-%d %H:%M:%S")
                << "</name>\n";
      m_KmlFile << "  <TimeSpan><begin>" << m_UpdateBeginDate.getAsString("%Y-%m-%dT%H:%M:%SZ") << "</begin><end>"
                << EndDateTime.getAsString("%Y-%m-%dT%H:%M:%SZ") << "</end></TimeSpan>\n";
      m_KmlFile << "  <Link><href>" << m_KmzDataSubDir << "/t_" << CurrentTIStr << ".kml</href></Link>\n";
      m_KmlFile << "</NetworkLink>\n";

      m_UpdateBeginDate = OPENFLUID_GetCurrentDate();

    }


  public:

    KmlFilesAnimObserver() : KmlObserverBase(),
      m_MinSamplingDelay(0),m_LatestSamplingIndex(0)
    {
      m_OutputFileName = "kmlanim.kmz";
      m_TmpSubDir = "export.vars.files.kml-anim";
    }


    // =====================================================================
    // =====================================================================


    ~KmlFilesAnimObserver()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      openfluid::ware::WareParamsTree ParamsTree;

      try
      {
        ParamsTree.setParams(Params);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        OPENFLUID_RaiseError(E.getMessage());
      }

      OGRRegisterAll();

      OPENFLUID_GetRunEnvironment("dir.input",m_InputDir);
      OPENFLUID_GetRunEnvironment("dir.output",m_OutputDir);

      // general

      try
      {
        m_Title = ParamsTree.root().getChildValue("title",m_Title);

        m_OutputFileName = ParamsTree.root().getChildValue("kmzfilename",m_OutputFileName);
        ParamsTree.root().getChildValue("tryopengearth",m_TryOpenGEarth).toBoolean(m_TryOpenGEarth);
        long MSD;
        ParamsTree.root().getChildValue("minsamplingdelay",(int)m_MinSamplingDelay).toInteger(MSD);
        m_MinSamplingDelay = MSD;


        // anim layer

        m_AnimLayerInfo.UnitsClass = ParamsTree.root().child("layers").child("anim").getChildValue("unitsclass","");
        if (m_AnimLayerInfo.UnitsClass == "")
        {
          // search for deprecated "unitclass" parameter
          m_AnimLayerInfo.UnitsClass = ParamsTree.root().child("layers").child("anim").getChildValue("unitclass","");
        }

        m_AnimLayerInfo.VarName = ParamsTree.root().child("layers").child("anim").getChildValue("varname","");
        ParamsTree.root().child("layers").child("anim").getChildValue("linewidth",1)
            .toInteger(m_AnimLayerInfo.LineWidth);


        m_AnimLayerInfo.SourceIsDatastore =
            (ParamsTree.root().child("layers").child("anim").getChildValue("source","file").get() == "datastore");
        if (m_AnimLayerInfo.SourceIsDatastore)
          return;
        else
        {
          m_AnimLayerInfo.SourceFilename =
              ParamsTree.root().child("layers").child("anim").getChildValue("sourcefile","");
          if (m_AnimLayerInfo.SourceFilename.empty())
          {
            OPENFLUID_LogWarning("wrong sourcefile format");
            return;
          }
          m_AnimLayerInfo.SourceFilename = m_InputDir + "/" + m_AnimLayerInfo.SourceFilename;
        }


        std::vector<std::string> ColorScaleVector;
        std::string ColorScaleString;

        ColorScaleString = ParamsTree.root().child("layers").child("anim").getChildValue("colorscale","");
        ColorScaleVector = openfluid::tools::splitString(ColorScaleString,";",false);

        if (ColorScaleVector.size() % 2 == 0)
        {
          OPENFLUID_LogWarning("wrong colorscale format");
          return;
        }
        else
        {
          std::pair<std::string,double> TmpColorValue("",0.0);

          for (unsigned int i=0;i<ColorScaleVector.size();i++)
          {
            double TmpVal;

            if (i == ColorScaleVector.size()-1)
            {
              if (ColorScaleVector[i].size() != 8)
              {
                OPENFLUID_LogWarning("Wrong color scale color format on last item");
                return;
              }
              TmpColorValue = std::make_pair("",0.0);
              TmpColorValue.first = ColorScaleVector[i];
              m_AnimLayerInfo.ColorScale.push_back(TmpColorValue);

            }
            else
            {
              if (i%2 == 0)
              {
                // color item
                if (ColorScaleVector[i].size() != 8)
                {
                  OPENFLUID_LogWarning("Wrong color scale color format");
                  return;
                }

                TmpColorValue.first = ColorScaleVector[i];
              }
              else
              {
                // value item
                if (openfluid::tools::convertString(ColorScaleVector[i],&TmpVal))
                {
                  TmpColorValue.second = TmpVal;
                  m_AnimLayerInfo.ColorScale.push_back(TmpColorValue);
                }
                else
                {
                  OPENFLUID_LogWarning("Wrong color scale value format");
                  return;
                }

                TmpColorValue = std::make_pair("",0.0);
              }
            }
          }
        }


        if (!transformVectorLayerToKmlGeometry(m_AnimLayerInfo))
        {
          m_OKToGo = false;
          return;
        }



        // static layers

        if (ParamsTree.root().child("layers").hasChild("static"))
        {
          for (auto& SLayer : ParamsTree.root().child("layers").child("static"))
            {
            KmlStaticLayerInfo KSLI;

            KSLI.UnitsClass = SLayer.second.getChildValue("unitsclass","");
            if (KSLI.UnitsClass == "")
            {
              // search for deprecated "unitclass" parameter
              KSLI.UnitsClass = SLayer.second.getChildValue("unitclass","");
            }
            SLayer.second.getChildValue("linewidth",1).toInteger(KSLI.LineWidth);
            KSLI.Color = SLayer.second.getChildValue("color","ffffffff");

            KSLI.SourceIsDatastore = (SLayer.second.getChildValue("source","file").get() == "datastore");
            if (KSLI.SourceIsDatastore)
              return;
            else
            {
              KSLI.SourceFilename = SLayer.second.getChildValue("sourcefile","");
              if (KSLI.SourceFilename.empty())
              {
                OPENFLUID_LogWarning("wrong sourcefile format");
              }
              KSLI.SourceFilename = m_InputDir + "/" + KSLI.SourceFilename;
            }

            if (transformVectorLayerToKmlGeometry(KSLI))
            {
              m_StaticLayersInfo.push_back(KSLI);
            }
          }
        }
      }
      catch (openfluid::base::FrameworkException& E)
      {
        OPENFLUID_RaiseError(E.getMessage());
      }

      m_OKToGo = true;

      m_KmzSubDir = m_OutputFileName + "_kmz-dir";
    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      if(!m_OKToGo) return;



      if(!m_OKToGo) return;


      prepareTempDirectory();

      if(!m_OKToGo) return;


      // open and initialize doc.kml file
      m_KmlFile.open(std::string(m_TmpDir+"/"+m_KmzSubDir+"/doc.kml").c_str());

      m_KmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
      m_KmlFile << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" "
                   "xmlns:gx=\"http://www.google.com/kml/ext/2.2\" "
                   "xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n";

      m_KmlFile << "<Document>\n";
      m_KmlFile << "  <name>" << m_Title << "</name>\n";

      for (std::list<KmlStaticLayerInfo>::iterator it=m_StaticLayersInfo.begin();it!=m_StaticLayersInfo.end();++it)
      {

        std::string TmpStyleID = "static_" + (*it).UnitsClass +"_style";

        m_KmlFile << "    <Style id=\"" << TmpStyleID << "\"><LineStyle><color>"<< (*it).Color
                  << "</color><width>" << (*it).LineWidth
                  << "</width></LineStyle><PolyStyle><fill>0</fill></PolyStyle></Style>\n";

        m_KmlFile << "    <Folder>\n";
        m_KmlFile << "      <name>" << (*it).UnitsClass << "</name>\n";

        for (std::map<openfluid::core::UnitID_t,KmlUnitInfo>::iterator it2=(*it).UnitsInfos.begin();
            it2!=(*it).UnitsInfos.end();
            ++it2)
        {
          m_KmlFile << "    <Placemark>\n";


          m_KmlFile << "      <name>" << (*it).UnitsClass << " " << (*it2).second.UnitID << "</name>\n";

          m_KmlFile << "      <description>\n<![CDATA[\n";
          m_KmlFile << "Unit class: " << (*it).UnitsClass << "<br/>\n";
          m_KmlFile << "Unit ID: " << (*it2).second.UnitID << "<br/>\n";
          m_KmlFile << "\n]]>\n      </description>\n";


          m_KmlFile << "      <styleUrl>#" << TmpStyleID << "</styleUrl>\n";


          if ((*it2).second.GeometryType == wkbPolygon)
          {
            m_KmlFile << "<Polygon><tessellate>1</tessellate><outerBoundaryIs><LinearRing><coordinates>"
                      << (*it2).second.CoordsStr << "</coordinates></LinearRing></outerBoundaryIs></Polygon>\n";
          }

          if ((*it2).second.GeometryType == wkbLineString)
          {
            m_KmlFile << "<LineString><tessellate>1</tessellate><coordinates>" << (*it2).second.CoordsStr
                      << "</coordinates></LineString>\n";
          }


          m_KmlFile << "    </Placemark>\n";
        }

        m_KmlFile << "    </Folder>\n";
      }


    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {
      if(!m_OKToGo) return;

      m_UpdateBeginDate = OPENFLUID_GetCurrentDate();
    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      if(!m_OKToGo) return;


      if (m_LatestSamplingIndex + m_MinSamplingDelay <= OPENFLUID_GetCurrentTimeIndex())
      {
        m_LatestSamplingIndex = OPENFLUID_GetCurrentTimeIndex();
        updateKmlFile();
      }
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      if(!m_OKToGo) return;

      m_KmlFile << "</Document>\n";
      m_KmlFile << "</kml>\n";

      m_KmlFile.close();

      buildKmzFile();

      tryOpenGEarth();

    }

};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(KmlFilesAnimObserver)

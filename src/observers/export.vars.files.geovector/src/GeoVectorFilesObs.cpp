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
  @file GeoVectorFilesObs.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <ogrsf_frmts.h>

#include <openfluid/utils/GDALCompatibility.hpp>
#include <openfluid/utils/GDALHelpers.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/ware/WareParamsTree.hpp>


class GeoVectorSerie
{
  public:

    enum class WhenModeCases {WHENINIT, WHENCONTINUOUS, WHENFINAL};

    typedef std::map<openfluid::core::VariableName_t,std::string> VariablesSet_t;


    std::string SerieName;

    std::string GeoSourceFilePath;

    openfluid::core::UnitsClass_t UnitsClass;

    VariablesSet_t VariablesSet;

    WhenModeCases WhenMode;

    openfluid::core::Duration_t WhenContinuousDelay;

    openfluid::core::TimeIndex_t LatestContinuousIndex;

    GDALDataset_COMPAT* GeoSource;

    OGRLayer* GeoLayer;

    std::string OutfilePattern;

    int OFLDIDFieldIndex;


    GeoVectorSerie(const std::string& SName,
                   const std::string& SrcFilePath,
                   const openfluid::core::UnitsClass_t& UClass,
                   const VariablesSet_t& VarsSet,
                   const std::string& OutfileExt,
                   const WhenModeCases& Mode = WhenModeCases::WHENCONTINUOUS,
                   const openfluid::core::Duration_t& ContModeDelay = 1):
      SerieName(SName),
      GeoSourceFilePath(SrcFilePath),
      UnitsClass(UClass),
      VariablesSet(VarsSet),
      WhenMode(Mode), WhenContinuousDelay(ContModeDelay), LatestContinuousIndex(0),
      GeoSource(nullptr), GeoLayer(nullptr),
      OutfilePattern(SName+"_%s."+OutfileExt),
      OFLDIDFieldIndex(-1)
    {

    }


    // =====================================================================
    // =====================================================================


    ~GeoVectorSerie()
    {
      GDALClose_COMPAT(GeoSource);
    }

};


// =====================================================================
// =====================================================================


class GeoVectorFilesObserver : public openfluid::ware::PluggableObserver
{
  private:

    std::string m_GDALFormat;

    std::string m_InputPath;

    std::string m_OutputPath;

    std::vector<GeoVectorSerie> m_Series;


    // =====================================================================
    // =====================================================================


    bool static isFieldExist(const std::string& FName, const GeoVectorSerie::VariablesSet_t& VarsSet)
    {
      GeoVectorSerie::VariablesSet_t::const_iterator itV;
      GeoVectorSerie::VariablesSet_t::const_iterator itVb = VarsSet.begin();
      GeoVectorSerie::VariablesSet_t::const_iterator itVe = VarsSet.end();

      for (itV = itVb; itV != itVe; ++itV)
      {
        if ((*itV).second == FName)
        {
          return true;
        }
      }

      return false;
    }


    // =====================================================================
    // =====================================================================


    void updateFieldNamesUsingFormat(GeoVectorSerie::VariablesSet_t& VarsSet)
    {
      if (m_GDALFormat == "ESRI Shapefile")
      {
        // limit field name to 10 characters,
        // and generate a new name avoiding duplicate names

        GeoVectorSerie::VariablesSet_t::iterator itV;
        GeoVectorSerie::VariablesSet_t::iterator itVb = VarsSet.begin();
        GeoVectorSerie::VariablesSet_t::iterator itVe = VarsSet.end();

        for (itV = itVb; itV != itVe; ++itV)
        {

          if ((*itV).second.size()>10)
          {

            std::string BaseName = (*itV).second.substr(0,8);
            int Nbr = 1;

            std::string NewFieldName = openfluid::tools::format("%s%d",BaseName.c_str(),Nbr);

            while (isFieldExist(NewFieldName,VarsSet) && Nbr < 100)
            {
              Nbr++;
              NewFieldName = openfluid::tools::format("%s%d",BaseName.c_str(),Nbr);
            }

            (*itV).second = NewFieldName;
          }
        }
      }
    }


    // =====================================================================
    // =====================================================================


    static GeoVectorSerie::VariablesSet_t convertParamToVariableSet(const std::string& VarsStr)
    {
      GeoVectorSerie::VariablesSet_t VarsSet;

      auto VarsList = openfluid::tools::split(VarsStr,";");

      for (auto i=0; i<VarsList.size();i++)
      {
        auto VarsNameAlias = openfluid::tools::split(VarsList[i],"=>");

        if (VarsNameAlias.size() == 1)
        {
          VarsSet[VarsNameAlias[0]] = VarsNameAlias[0];
        }
        else if (VarsNameAlias.size() == 2)
        {
          VarsSet[VarsNameAlias[0]] = VarsNameAlias[1];
        }
        else
        {
          // error in variables list format, so returning an empty variables set
          return GeoVectorSerie::VariablesSet_t();
        }
      }

      return VarsSet;
    }


    // =====================================================================
    // =====================================================================


    static bool setWhenModeFromParam(const std::string& WhenStr,
                                     GeoVectorSerie::WhenModeCases& Mode, openfluid::core::Duration_t& ContinuousDelay)
    {
      if (WhenStr == "init")
      {
        Mode = GeoVectorSerie::WhenModeCases::WHENINIT;
        ContinuousDelay = 1;
        return true;
      }

      if (WhenStr == "final")
      {
        Mode = GeoVectorSerie::WhenModeCases::WHENFINAL;
        ContinuousDelay = 1;
        return true;
      }

      if (WhenStr.empty() || WhenStr == "continuous")
      {
        Mode = GeoVectorSerie::WhenModeCases::WHENCONTINUOUS;
        ContinuousDelay = 1;
        return true;
      }

      if (openfluid::tools::startsWith(WhenStr,"continuous"))
      {
        auto WhenParts = openfluid::tools::split(WhenStr,";");

        if (WhenParts.size() == 2)
        {
          bool IsConverted;
          IsConverted = openfluid::tools::toNumeric(WhenParts[1],ContinuousDelay);

          if (IsConverted)
          {
            Mode = GeoVectorSerie::WhenModeCases::WHENCONTINUOUS;
            return true;
          }
          return false;
        }
        return false;
      }
      return false;
    }


    // =====================================================================
    // =====================================================================


    void prepareSerie(GeoVectorSerie& Serie)
    {
      // opening and checking of source files
      Serie.GeoSource = GDALOpenRO_COMPAT(Serie.GeoSourceFilePath.c_str());


      if (Serie.GeoSource)
      {
        Serie.GeoLayer = Serie.GeoSource->GetLayer(0);

        if (Serie.GeoLayer)
        {
          // checking of OFLD_ID
          Serie.OFLDIDFieldIndex = Serie.GeoLayer->GetLayerDefn()->GetFieldIndex("OFLD_ID");

        }
      }
    }


    // =====================================================================
    // =====================================================================


    void processSerie(GeoVectorSerie& Serie)
    {
      std::string IndexStr = "init";

      openfluid::base::SimulationStatus::SimulationStage CurrentStage =
          OPENFLUID_GetCurrentStage();

      bool OKToWrite = false;

      if (CurrentStage == openfluid::base::SimulationStatus::INITIALIZERUN)
      {
        OKToWrite = Serie.WhenMode == GeoVectorSerie::WhenModeCases::WHENINIT ||
                    Serie.WhenMode == GeoVectorSerie::WhenModeCases::WHENCONTINUOUS;

      }
      else if (CurrentStage == openfluid::base::SimulationStatus::RUNSTEP)
      {
        if (Serie.WhenMode == GeoVectorSerie::WhenModeCases::WHENCONTINUOUS)
        {
          openfluid::core::TimeIndex_t CurrentIndex = OPENFLUID_GetCurrentTimeIndex();
          IndexStr = std::to_string(CurrentIndex);

          if (Serie.LatestContinuousIndex + Serie.WhenContinuousDelay < CurrentIndex)
          {
            OKToWrite = true;
            Serie.LatestContinuousIndex = CurrentIndex;
          }
        }
      }
      else if (CurrentStage == openfluid::base::SimulationStatus::FINALIZERUN)
      {
        IndexStr = "final";
        OKToWrite = Serie.WhenMode == GeoVectorSerie::WhenModeCases::WHENCONTINUOUS ||
                    Serie.WhenMode == GeoVectorSerie::WhenModeCases::WHENFINAL;
      }
      else
      {
        OPENFLUID_LogWarning("Internal stage error when processing geographic series");
        return;
      }


      if (OKToWrite)
      {
        std::string FullFilePath =
            m_OutputPath + "/" + openfluid::tools::format(Serie.OutfilePattern,IndexStr.c_str());


        GDALDriver_COMPAT* Driver = GDALGetDriverByName_COMPAT(m_GDALFormat.c_str());

        if (openfluid::tools::FilesystemPath(FullFilePath).isFile())
        {
          // deletion of an existing file or files set
          GDALDelete_COMPAT(Driver,FullFilePath.c_str());
        }

        GDALDataset_COMPAT* CreatedFile = GDALCreate_COMPAT(Driver,FullFilePath.c_str());

        std::string CreatedLayerName = openfluid::tools::Path(FullFilePath).basename();

        OGRLayer* CreatedLayer = CreatedFile->CreateLayer(CreatedLayerName.c_str(),nullptr,
                                                          Serie.GeoLayer->GetLayerDefn()->GetGeomType(),
                                                          nullptr);

        OGRFieldDefn IDField("OFLD_ID",OFTInteger);
        CreatedLayer->CreateField(&IDField);


        GeoVectorSerie::VariablesSet_t::const_iterator itV;
        GeoVectorSerie::VariablesSet_t::const_iterator itVb = Serie.VariablesSet.begin();
        GeoVectorSerie::VariablesSet_t::const_iterator itVe = Serie.VariablesSet.end();

        for (itV = itVb; itV != itVe; ++itV)
        {
          std::string FieldName = (*itV).second;

          OGRFieldDefn VarField(FieldName.c_str(),OFTReal);
          VarField.SetWidth(24);
          VarField.SetPrecision(15);

          CreatedLayer->CreateField(&VarField);
        }


        OGRFeature* SourceFeature;
        openfluid::core::SpatialUnit* UU;

        Serie.GeoLayer->ResetReading();
        while ((SourceFeature = Serie.GeoLayer->GetNextFeature()) != nullptr)
        {
          int SourceID = SourceFeature->GetFieldAsInteger(Serie.OFLDIDFieldIndex);

          UU = OPENFLUID_GetUnit(Serie.UnitsClass,SourceID);

          if (UU)
          {
            CreatedLayer->GetLayerDefn();

            OGRFeature* CreatedFeature = OGRFeature::CreateFeature(CreatedLayer->GetLayerDefn());

            CreatedFeature->SetGeometry(SourceFeature->GetGeometryRef()->clone());
            CreatedFeature->SetField("OFLD_ID",SourceID);


            for (itV = itVb; itV != itVe; ++itV)
            {
              std::string VarName = (*itV).first;
              std::string FieldName = (*itV).second;
              openfluid::core::DoubleValue CreatedValue = 0.0;
              bool IsValueCreated = false;

              if (FieldName.empty())
              {
                FieldName = VarName;
              }

              openfluid::core::IndexedValue VarValue;

              if (OPENFLUID_IsVariableExist(UU,VarName))
              {
                OPENFLUID_GetLatestVariable(UU,VarName,VarValue);

                if (VarValue.value()->isDoubleValue()) // OpenFLUID value is double
                {
                  CreatedValue = VarValue.value()->asDoubleValue();
                  IsValueCreated = true;
                }
                else if (VarValue.value()->convert(CreatedValue)) // OpenFLUID value can be converted to double
                {
                  IsValueCreated = true;
                }
                else
                {
                  auto Msg = openfluid::tools::format("Variable %s on unit %s#%d is not a double or a compatible type",
                                                       VarName.c_str(),UU->getClass().c_str(),UU->getID());
                  OPENFLUID_LogWarning(Msg);
                }
              }
              else
              {
                auto Msg = openfluid::tools::format("Variable %s does not exist on unit %s#%d",
                                                    VarName.c_str(),UU->getClass().c_str(),UU->getID());
                OPENFLUID_LogWarning(Msg);
              }

              if (IsValueCreated) // OpenFLUID value is written to GIS file only if it is double or converted to double
              {
                CreatedFeature->SetField(FieldName.c_str(),CreatedValue);
              }
            }

            if (CreatedLayer->CreateFeature(CreatedFeature) != OGRERR_NONE)
            {
              auto Msg = openfluid::tools::format("Feature for unit %s#%d cannot be created",
                                                  UU->getClass().c_str(),UU->getID());
              OPENFLUID_LogWarning(Msg);
            }


            OGRFeature::DestroyFeature(CreatedFeature);
          }
        }
        GDALClose_COMPAT(CreatedFile);
      }
    }


    // =====================================================================
    // =====================================================================


    void closeSerie(GeoVectorSerie& Serie)
    {
      if (Serie.GeoSource)
      {
        GDALClose_COMPAT(Serie.GeoSource);
        Serie.GeoSource = nullptr;
      }
    }


    // =====================================================================
    // =====================================================================


  public:

    GeoVectorFilesObserver() : PluggableObserver()
    {
      GDALAllRegister_COMPAT();
    }


    // =====================================================================
    // =====================================================================


    ~GeoVectorFilesObserver()
    {
      std::vector<GeoVectorSerie>::iterator it;

      for (it = m_Series.begin();it!=m_Series.end(); ++it)
      {
        closeSerie(*it);
      }
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

      // checking of mandatory parameters
      if (!ParamsTree.root().hasChild("format"))
      {
        OPENFLUID_LogWarning("Missing GDAL format for output files");
        return;
      }

      if (!ParamsTree.root().hasChild("format"))
      {
        OPENFLUID_LogWarning("No serie defined");
        return;
      }


      // process of format parameter
      m_GDALFormat = ParamsTree.root().getChildValue("format","");

      openfluid::utils::GDALDriversFilesExts_t ValidVectorDrivers =
          openfluid::utils::getOGRFilesDriversForOpenFLUID();

      if (ValidVectorDrivers.find(m_GDALFormat) == ValidVectorDrivers.end())
      {
        OPENFLUID_LogWarning("Unsupported GDAL format for output files");
        return;
      }

      std::string OutfileExt = ValidVectorDrivers[m_GDALFormat].FilesExts.front();


      // process of parameter for optional output subdirectory
      std::string OutSeriesSubdir = ParamsTree.root().getChildValue("outsubdir","");


      // get paths for input dataset and output directory
      OPENFLUID_GetRunEnvironment("dir.input",m_InputPath);
      OPENFLUID_GetRunEnvironment("dir.output",m_OutputPath);

      if (!OutSeriesSubdir.empty())
      {
        m_OutputPath += "/"+OutSeriesSubdir;
      }


      for (auto& Serie : ParamsTree.root().child("geoserie"))
      {
        GeoVectorSerie::WhenModeCases Mode = GeoVectorSerie::WhenModeCases::WHENCONTINUOUS;
        openfluid::core::Duration_t ContinuousDelay = 1;

        std::string SerieName = Serie.first;

        std::string GeoSourceFilename = Serie.second.getChildValue("sourcefile","");
        std::string VarsString = Serie.second.getChildValue("vars","");
        openfluid::core::UnitsClass_t UnitsClass = Serie.second.getChildValue("unitsclass","");
        std::string WhenModeString = Serie.second.getChildValue("when","");


        if (GeoSourceFilename.empty())
        {
          OPENFLUID_LogWarning("Missing geographic source filename for serie "+SerieName);
        }
        else if (UnitsClass.empty())
        {
          OPENFLUID_LogWarning("Missing units class for serie "+SerieName);
        }
        else if (VarsString.empty())
        {
          OPENFLUID_LogWarning("Missing variables list for serie "+SerieName);
        }
        else
        {
          GeoVectorSerie::VariablesSet_t VarsSet = convertParamToVariableSet(VarsString);

          if (VarsSet.empty())
          {
            OPENFLUID_LogWarning("Format error in variables list for serie "+SerieName);
          }
          else
          {
            if (setWhenModeFromParam(WhenModeString,Mode,ContinuousDelay))
            {
              // everything's OK, add the serie to the active series set
              m_Series.push_back(GeoVectorSerie(SerieName,
                                                m_InputPath + "/" + GeoSourceFilename,
                                                UnitsClass,VarsSet,
                                                OutfileExt,
                                                Mode,ContinuousDelay));
            }
            else
            {
              OPENFLUID_LogWarning("Format error in whenmode for serie "+SerieName);
            }
          }
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      if (m_Series.empty())
      {
        return;
      }

      std::vector<GeoVectorSerie>::iterator it;


      // preparation of output directory for series (if any)
      openfluid::tools::Path(m_OutputPath).makeDirectory();


      // preparation of series
      for (it = m_Series.begin();it!=m_Series.end(); ++it)
      {
        prepareSerie(*it);
      }


      // removal of wrong formatted source geovector file
      for (it=m_Series.begin();it!=m_Series.end(); )
      {
        if ((*it).GeoSource == nullptr)
        {
          OPENFLUID_LogWarning("Cannot open geographic source for serie "+(*it).SerieName);
          closeSerie(*it);
          it = m_Series.erase(it);
        }
        else if ((*it).GeoLayer == nullptr)
        {
          OPENFLUID_LogWarning("Cannot open layer in geographic source for serie "+(*it).SerieName);
          closeSerie(*it);
          it = m_Series.erase(it);
        }
        else if ((*it).OFLDIDFieldIndex < 0)
        {
          OPENFLUID_LogWarning("File not found or wrong file format for geographic source for serie "+
                                 (*it).SerieName);
          closeSerie(*it);
          it = m_Series.erase(it);
        }
        else if ((*it).VariablesSet.empty())
        {
          OPENFLUID_LogWarning("No correct variable name in serie "+(*it).SerieName);
          closeSerie(*it);
          it = m_Series.erase(it);
        }
        else
        {
          ++it;
        }
      }


      // Update field names if necessary
      for (it=m_Series.begin();it!=m_Series.end(); ++it)
      {
        updateFieldNamesUsingFormat((*it).VariablesSet);
      }

    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {
      if (m_Series.empty())
      {
        return;
      }

      std::vector<GeoVectorSerie>::iterator it;

      for (it = m_Series.begin();it!=m_Series.end(); ++it)
      {
        processSerie(*it);
      }

    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      if (m_Series.empty())
      {
        return;
      }

      std::vector<GeoVectorSerie>::iterator it;

      for (it = m_Series.begin();it!=m_Series.end(); ++it)
      {
        processSerie(*it);
      }

    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      if (m_Series.empty())
      {
        return;
      }

      std::vector<GeoVectorSerie>::iterator it;

      for (it = m_Series.begin();it!=m_Series.end(); ++it)
      {
        processSerie(*it);
      }

      for (it = m_Series.begin();it!=m_Series.end(); ++it)
      {
        closeSerie(*it);
      }
    }

};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(GeoVectorFilesObserver)


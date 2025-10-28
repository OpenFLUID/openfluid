/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file DistributionBindings.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_TOOLS_DISTRIBUTIONBINDINGS_HPP__
#define __OPENFLUID_TOOLS_DISTRIBUTIONBINDINGS_HPP__


#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/tools/DistributionTables.hpp>
#include <openfluid/tools/ProgressiveChronFileReader.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/tools/VarHelpers.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace tools {


inline std::vector<openfluid::tools::ClassIDVar> 
stringArrayToClassIDVarList(const std::vector<std::string>& StringArray, bool RemoveFirst=false)
{
  std::vector<openfluid::tools::ClassIDVar> CSVTriplets;
  int Begin = RemoveFirst ? 1 : 0;
  for (std::size_t i=Begin;i<StringArray.size();i++)
  {
    const std::string& Column = StringArray[i];
    // parse and create CSVTriplet
    std::size_t HashPosition = Column.find(openfluid::config::CHAR_SEPARATOR_UNIT);
    std::size_t ColonPosition = Column.find(openfluid::config::CHAR_SEPARATOR_VAR);

    if(HashPosition == std::string::npos || ColonPosition == std::string::npos)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "wrong format for multicolumn csv file");
    }
    
    openfluid::tools::ClassIDVar CurrentCSVTriplet;
    
    CurrentCSVTriplet.UnitsClass = Column.substr(0,HashPosition);
    CurrentCSVTriplet.UnitsIDsStr = Column.substr(HashPosition+1, ColonPosition-HashPosition-1);
    CurrentCSVTriplet.VariableName = Column.substr(ColonPosition+1);
    
    CSVTriplets.push_back(CurrentCSVTriplet);
  }
  return CSVTriplets;
};


// =====================================================================
// =====================================================================


inline std::vector<openfluid::tools::ClassIDVar> stringSelectionToClassIDVarList(const std::string& SelectionStr, 
                                                                                 bool RemoveFirst=false)
{
  std::vector<std::string> Columns = openfluid::tools::split(SelectionStr, ";");
   //TODO Extract and make split char consistent between ops
  
  return stringArrayToClassIDVarList(Columns, RemoveFirst);
};


template<typename DataType=double>
class ReaderNextValue
{
  public:

    ProgressiveChronFileReader<DataType>* Reader;

    ChronItem_t<DataType> NextValue;

    bool isAvailable;

    ReaderNextValue(): Reader(nullptr), isAvailable(false)
    { }
};


// =====================================================================
// =====================================================================


template<typename DataType=double>
class GenericDistributionBindings
{
  public:

    typedef std::list<ReaderNextValue<DataType>> ReadersNextValues_t;


  protected:

    ReadersNextValues_t m_ReadersNextValues;


  public:

    GenericDistributionBindings() = default;

    ~GenericDistributionBindings()
    {

      // delete readers

      for (ReaderNextValue<DataType>& RNV : m_ReadersNextValues)
      {
        if (RNV.Reader)
        {
          delete RNV.Reader;
        }
      }
    }

    void advanceToTime(const openfluid::core::DateTime& DT)
    {
      for (ReaderNextValue<DataType>& RNV : m_ReadersNextValues)
      {
        bool DataFound = true;
        openfluid::tools::ChronItem_t<DataType> CI;

        if ((RNV.isAvailable && RNV.NextValue.first < DT) ||
            RNV.isAvailable == false)
        {
          RNV.isAvailable = false;

          while (DataFound && !RNV.isAvailable)
          {
            DataFound = RNV.Reader->getNextValue(CI);
            if (DataFound && CI.first >= DT)
            {
              RNV.isAvailable = true;
              RNV.NextValue = CI;
            }
          }
        }
      }
    }

    bool advanceToNextTimeAfter(const openfluid::core::DateTime& DT, openfluid::core::DateTime& NextDT)
    {
      openfluid::core::DateTime DTPlusOne(DT);
      DTPlusOne.addSeconds(1);
      advanceToTime(DTPlusOne);

      openfluid::core::DateTime NDT;

      bool AvailableFound = false;

      for (ReaderNextValue<DataType>& RNV : m_ReadersNextValues)
      {

        if (!AvailableFound && RNV.isAvailable)
        {
          NDT = RNV.NextValue.first;
          AvailableFound = true;
        }
      }

      if (!AvailableFound)
      {
        return false;
      }

      for (ReaderNextValue<DataType>& RNV : m_ReadersNextValues)
      {
        if (RNV.isAvailable && RNV.NextValue.first < NDT)
        {
          NDT = RNV.NextValue.first;
        }
      }

      NextDT = NDT;

      return true;
    }

};


// =====================================================================
// =====================================================================


template<typename DataType=double, typename TV=openfluid::core::DoubleValue>
class SimpleDistributionBindings : public GenericDistributionBindings<DataType> 
{
  public:

    typedef std::map<openfluid::core::UnitID_t,ReaderNextValue<DataType>*> UnitIDReader_t;


  protected:

    UnitIDReader_t m_UnitIDReaders;

  public:

    SimpleDistributionBindings(const DistributionTables& DistriTables) : GenericDistributionBindings<DataType>()
    {
      DistributionTables::SourceIDFile_t::const_iterator itb = DistriTables.SourcesTable.begin();
      DistributionTables::SourceIDFile_t::const_iterator ite = DistriTables.SourcesTable.end();

      for (DistributionTables::SourceIDFile_t::const_iterator it = itb; it != ite; ++it)
      {
        ReaderNextValue<DataType> RNV;
        RNV.Reader = new ProgressiveChronFileReader<DataType>((*it).second);
        this->m_ReadersNextValues.push_back(RNV);

        DistributionTables::UnitIDSourceID_t::const_iterator itub = DistriTables.UnitsTable.begin();
        DistributionTables::UnitIDSourceID_t::const_iterator itue = DistriTables.UnitsTable.end();

        for (DistributionTables::UnitIDSourceID_t::const_iterator itu = itub; itu != itue; ++itu)
        {
          if ((*itu).second == (*it).first)
          {
            m_UnitIDReaders[(*itu).first] = &this->m_ReadersNextValues.back();
          }
        }
      }
    }

    bool getValue(const openfluid::core::UnitID_t& UnitID,
                  const openfluid::core::DateTime& DT,
                  TV& Value)
    {
      // if a value is available for DT : passes the value to caller, and returns true
      // else returns false
      typename UnitIDReader_t::iterator it = m_UnitIDReaders.find(UnitID);
      if (it != m_UnitIDReaders.end() && (*it).second->isAvailable && (*it).second->NextValue.first == DT)
      {
        // Previous method for double, avoiding any precision issue from conversions: 
        //    Value.set((*it).second->NextValue.second);
        // Requires a dedicated function override at template level
        openfluid::core::StringValue CurrentValueSV = openfluid::core::StringValue((*it).second->NextValue.second);
        if (Value.getType() == openfluid::core::Value::STRING)
        {
          Value = CurrentValueSV;
          return true;
        }
        else
        {
          return CurrentValueSV.convert(Value);
        }
      }
      return false;
    }

    void displayBindings()
    {
      for (auto& IDReader : m_UnitIDReaders)
      {
        std::cout << IDReader.first << " -> " << IDReader.second->Reader->getFileName() << std::endl;
      }
    }
};


// =====================================================================
// =====================================================================


class OPENFLUID_API DistributionBindings : public SimpleDistributionBindings<double, openfluid::core::DoubleValue>
{
  public:
    DistributionBindings(const DistributionTables& DistriTables) : SimpleDistributionBindings(DistriTables) {
    }
};


// =====================================================================
// =====================================================================


class OPENFLUID_API MulticolDistributionBindings : public GenericDistributionBindings<std::vector<std::string>>
{
  public:
    
    typedef std::map<std::vector<std::string>, unsigned int> TripletLocation_t;

  private:
    
    TripletLocation_t m_ColBySelectionTriplets;

    inline static const std::string s_MissingValueString = "NA";  // TODO set as parameter transmitted by calling object


  public:

    MulticolDistributionBindings(const std::string& DataFile,
                         const std::string& DateFormat = "%Y%m%dT%H%M%S",
                         const std::string& ColSeparators = ";");


    /**
      Apply to Value the wanted value from distribution table for given variable at given time
      @return the status as int: 1 for success, 0 for expected NA, -1 for failed conversion from string to double
      @throw openfluid::base::FrameworkException when wrong number of columns
    */
    int getValue(const openfluid::core::UnitsClass_t& UnitsClass, 
                  const openfluid::core::UnitID_t& UnitID, 
                  const openfluid::core::VariableName_t& VariableName, 
                  const openfluid::core::DateTime& DT,
                  openfluid::core::DoubleValue& Value);

    TripletLocation_t getColBySelectionTriplet()
    {
      return m_ColBySelectionTriplets;
    }
};


} }


#endif /* __OPENFLUID_TOOLS_DISTRIBUTIONBINDINGS_HPP__ */

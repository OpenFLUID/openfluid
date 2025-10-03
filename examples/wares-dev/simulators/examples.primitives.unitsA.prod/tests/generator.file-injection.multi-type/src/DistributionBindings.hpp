/**
  @file DistributionBindings.hpp
*/

#ifndef DISTRIBUTIONBINDINGS_HPP
#define DISTRIBUTIONBINDINGS_HPP

#include <openfluid/tools/DistributionTables.hpp>

#include "ProgressiveChronFileReader.hpp"



class ReaderNextValue
{
  public:

    ProgressiveChronFileReader* Reader;

    std::pair<openfluid::core::DateTime, std::string> NextValue;

    bool isAvailable;

    ReaderNextValue(): Reader(nullptr), isAvailable(false)
    { }
};


// =====================================================================
// =====================================================================


template<typename DataType=openfluid::core::DoubleValue>
class DistributionBindings
{
  public:

    typedef std::map<openfluid::core::UnitID_t,ReaderNextValue*> UnitIDReader_t;

    typedef std::list<ReaderNextValue> ReadersNextValues_t;


  private:

    UnitIDReader_t m_UnitIDReaders;

    ReadersNextValues_t m_ReadersNextValues;
    
    std::string m_MissingValue;

  public:

    DistributionBindings(const openfluid::tools::DistributionTables& DistriTables, std::string MissingValue)
    {
      m_MissingValue = MissingValue;
      
      openfluid::tools::DistributionTables::SourceIDFile_t::const_iterator itb = DistriTables.SourcesTable.begin();
      openfluid::tools::DistributionTables::SourceIDFile_t::const_iterator ite = DistriTables.SourcesTable.end();

      for (openfluid::tools::DistributionTables::SourceIDFile_t::const_iterator it = itb; it != ite; ++it)
      {
        ReaderNextValue RNV;
        RNV.Reader = new ProgressiveChronFileReader((*it).second);
        m_ReadersNextValues.push_back(RNV);

        openfluid::tools::DistributionTables::UnitIDSourceID_t::const_iterator itub = DistriTables.UnitsTable.begin();
        openfluid::tools::DistributionTables::UnitIDSourceID_t::const_iterator itue = DistriTables.UnitsTable.end();

        for (openfluid::tools::DistributionTables::UnitIDSourceID_t::const_iterator itu = itub; itu != itue; ++itu)
        {
          if ((*itu).second == (*it).first)
          {
            m_UnitIDReaders[(*itu).first] = &m_ReadersNextValues.back();
          }
        }
      }
    }
  
      
// =====================================================================
// =====================================================================


    ~DistributionBindings()
    {
      // delete readers
      ReadersNextValues_t::iterator it;
      ReadersNextValues_t::iterator bit = m_ReadersNextValues.begin();
      ReadersNextValues_t::iterator eit = m_ReadersNextValues.end();

      for (it=bit;it!=eit;++it)
      {
        if ((*it).Reader)
        {
          delete (*it).Reader;
        }
      }
    }
    

// =====================================================================
// =====================================================================


    void advanceToTime(const openfluid::core::DateTime& DT)
    {
      // set readers position to the first value equal or greater for each reader
      ReadersNextValues_t::iterator itb = m_ReadersNextValues.begin();
      ReadersNextValues_t::iterator ite = m_ReadersNextValues.end();

      for (ReadersNextValues_t::iterator it = itb; it != ite; ++it)
      {
        bool DataFound = true;
        std::pair<openfluid::core::DateTime, std::string> CI;

        if (((*it).isAvailable && (*it).NextValue.first < DT) ||
            (*it).isAvailable == false)
        {
          (*it).isAvailable = false;

          while (DataFound && !(*it).isAvailable)
          {
            DataFound = (*it).Reader->getNextValue(CI);
            if (DataFound && CI.first >= DT)
            {
              (*it).isAvailable = true;
              (*it).NextValue = CI;
            }
          }
        }
      }
    }


// =====================================================================
// =====================================================================


    bool advanceToNextTimeAfter(const openfluid::core::DateTime& DT, openfluid::core::DateTime& NextDT)
    {
      openfluid::core::DateTime DTPlusOne(DT);
      DTPlusOne.addSeconds(1);
      advanceToTime(DTPlusOne);


      ReadersNextValues_t::iterator itb = m_ReadersNextValues.begin();
      ReadersNextValues_t::iterator ite = m_ReadersNextValues.end();

      openfluid::core::DateTime NDT;

      bool AvailableFound = false;

      for (ReadersNextValues_t::iterator it = itb; it != ite; ++it)
      {

        if (!AvailableFound && (*it).isAvailable)
        {
          NDT = (*it).NextValue.first;
          AvailableFound = true;
        }
      }

      if (!AvailableFound)
      {
        return false;
      }

      for (ReadersNextValues_t::iterator it = itb; it != ite; ++it)
      {
        if ((*it).isAvailable && (*it).NextValue.first < NDT)
        {
          NDT = (*it).NextValue.first;
        }
      }

      NextDT = NDT;
      return true;
    }


// =====================================================================
// =====================================================================


    int getValue(const openfluid::core::UnitID_t& UnitID,
                 const openfluid::core::DateTime& DT,
                 DataType& Value)
    {
      UnitIDReader_t::iterator it = m_UnitIDReaders.find(UnitID);

      if (it != m_UnitIDReaders.end() && (*it).second->isAvailable && (*it).second->NextValue.first == DT)
      {
        std::string CurrentValue = (*it).second->NextValue.second;
        if (CurrentValue == m_MissingValue)
        {
          return 0; // Doing nothing when NA detected
        }
        openfluid::core::StringValue CurrentValueSV = openfluid::core::StringValue(CurrentValue);
        if (Value.getType() == openfluid::core::Value::STRING)
        {
          Value = CurrentValueSV;
          return 1;
        }
        else if (CurrentValueSV.convert(Value))
        {
          return 1;
        }
        else
        {
          return -1; // sending error code instead of throw to let calling function handle it
        }
        return true;
      }

      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "No value available at time " + DT.getAsISOString() + " for variable ${varname} on ${unitsclass}#" + std::to_string(UnitID));
    }   


// =====================================================================
// =====================================================================       
    

    void displayBindings()
    {
      for (auto& IDReader : m_UnitIDReaders)
      {
        std::cout << IDReader.first << " -> " << IDReader.second->Reader->getFileName() << std::endl;
      }
    }

};


#endif // DISTRIBUTIONBINDINGS_HPP


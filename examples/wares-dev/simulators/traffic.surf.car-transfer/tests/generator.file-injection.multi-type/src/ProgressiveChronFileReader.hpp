/**
  @file ProgressiveChronFileReader.hpp
*/

#include <openfluid/tools/ProgressiveColumnFileReader.hpp>
#include <openfluid/tools/ChronologicalSerie.hpp>


class ProgressiveChronFileReader : public openfluid::tools::ProgressiveColumnFileReader
{
  private:

    std::string m_DateFormat;


  public:

    ProgressiveChronFileReader(const std::string& FileName,
                               const std::string& DateFormat = "%Y-%m-%dT%H:%M:%S",
                               const std::string& ColSeparators = " \t\r\n"):
    ProgressiveColumnFileReader(FileName,ColSeparators), m_DateFormat(DateFormat)
    { }

    virtual ~ProgressiveChronFileReader()
    { }


    bool getNextValue(std::pair<openfluid::core::DateTime, std::string>& Value)
    {
      std::vector<std::string> Values;
      openfluid::core::DateTime DT;

      while (getNextLine(Values))
      {
        if (Values.size() == 1 || Values.size() == 2)
        {
          if (DT.setFromString(Values.front(), m_DateFormat))
          {
            Value.first = DT;
            Value.second = (Values.size() == 2) ? Values.back() : "";
            return true;
          }
        } 
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "wrong data in " + m_FileName);
        }

      }

    return false;
    }

};

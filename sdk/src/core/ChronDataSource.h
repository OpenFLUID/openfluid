/**
  \file ChronDataSource.h
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#ifndef __CHRONDATASOURCE_H__
#define __CHRONDATASOURCE_H__

#include <string>
#include "TimeSerie.h"


namespace mhydasdk { namespace core {


typedef int cdsid_t;
typedef double RainValue;


/**
  \brief Generic definition class for chronological data producer source

  This is a base class allowing, by inheritance, definition of a data source,
  georeferenced or not.
  It associates a source identifier with a temporal serie (TimeSerie objects)

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class ChronDataSource
{
  private:

  protected:
    cdsid_t m_ID;
    std::string m_Name;
    TimeSerie* m_TimeSerie;

    RainValue *mp_ProcessedData;

  public:
		/**
		  Constructor
      \param[in] ID identifier of the source
		*/
    ChronDataSource(cdsid_t ID);

		/**
		  Virtual destructor
		*/
    virtual ~ChronDataSource();

    /**
		  Sets the temporal serie of the source
      \param[in] Serie Associated serie
		*/
    void setTimeSerie(TimeSerie *Serie);

    /**
		  Returns the source ID
      \return un ID
		*/
    cdsid_t getID();


    /**
		  Returns source name
      \return an ID
		*/
    std::string getName() const {return m_Name;};

    /**
		  Returns source ID
      \return an ID
		*/
    std::string getIDName() const;

    /**
		  Sets the source name
      \param[in] ID The source name
		*/
    void setName(std::string Name) {m_Name = Name;};


    /**
		  Returns the associated serie
      \return a temporal Serie
		*/
    TimeSerie *getTimeSerie();

    void Process();

    RainValue* getProcessedData() { return mp_ProcessedData; };

};



} } // namespace mhydasdk::core



#endif


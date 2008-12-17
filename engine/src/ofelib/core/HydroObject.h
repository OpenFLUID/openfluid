/**
  \file HydroObject.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
 */

#ifndef __HYDROOBJECT_H__
#define __HYDROOBJECT_H__

#include <string>
#include <vector>
#include <wx/hashmap.h>

#include "EventCollection.h"
#include "TypeDefs.h"

namespace openfluid { namespace core {


/**
  Hash table for calculated scalar variables during simulation.
  Each variable is stored in the hash table as an entry, located by its name.
  (exemple "effrain" for efficient rain calculated by the production function)
  Each variable is stored as a vector of double (one vector item = one step, vector[25] is calculated at the 25th step)
*/
WX_DECLARE_STRING_HASH_MAP(SerieOfScalarValue*, SimulatedVarsMap);

/**
  Hash table for calculated vector variables during simulation.
  Each variable is stored in the hash table as an entry, located by its name.
  (exemple "effrain" for efficient rain calculated by the production function)
  Each variable is stored as a vector of double (one vector item = one step, vector[25] is calculated at the 25th step)
 */
WX_DECLARE_STRING_HASH_MAP(SerieOfVectorValue*, SimulatedVectorVarsMap);

/**
  Hash table for parameters (distributed properties, distributed initial conditions, ...)
 */
WX_DECLARE_STRING_HASH_MAP(ScalarValue,PropertiesMap);

/**
  Hash table for parameters (distributed properties, distributed initial conditions, ...)
 */
WX_DECLARE_STRING_HASH_MAP(wxString,ParamsMap);


/**
  Generic definition class for homogeneous units

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */
class HydroObject
{
  private:

  protected:
    UnitID m_ID;
    int m_ProcessOrder;

    SimulatedVarsMap* mp_SimVars;
    SimulatedVectorVarsMap* mp_SimVectorVars;

    PropertiesMap* mp_Properties;

    PropertiesMap* mp_IniConditions;

    EventCollection* mp_Events;


  public:
    /**
		  Default constructor
     */
    HydroObject();

    /**
		  Constructor
		  \param[in] ID The unit identifier
		  \param[in] ProcessOrder the process order of the unit during the model execution
     */
    HydroObject(UnitID ID, int ProcessOrder);

    /**
		  Virtual destructor
     */
    virtual ~HydroObject();

    /**
      Returns the ID of the unit
    */
    UnitID getID() const;

    /**
      Returns the process order of the unit
    */
    int getProcessOrder() const;

    /**
      Sets the process order of the unit
    */
    void setProcessOrder(const int ProcessOrder);

    /**
      Returns the set of simulated scalar variables of the unit
    */
    SimulatedVarsMap* getSimulatedVars() { return mp_SimVars; };

    /**
      Returns the set of simulated vector variables of the unit
    */
    SimulatedVectorVarsMap* getSimulatedVectorVars() { return mp_SimVectorVars; };

    /**
      Returns the set of properties of the unit
    */
    PropertiesMap* getProperties() { return mp_Properties; };

    /**
      Returns the initial conditions of the unit
    */
    PropertiesMap* getIniConditions() { return mp_IniConditions; };

    /**
      Returns the set of events associated with the unit
    */
    EventCollection* getEvents() { return mp_Events; };
};



} } // namespace openfluid::core



#endif

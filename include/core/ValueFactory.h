/**
  \if DOCFORDEV
  \file ValueFactory.h
  \brief header of ValueFactory class
  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
  \endif
*/




#ifndef __VALUEFACTORY_H__
#define __VALUEFACTORY_H__


namespace openfluid { namespace core {

/**
  \if DOCFORDEV
  \brief Class for numerical values conversions and manipulations

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
  \endif
*/
template <class T>
class ValueFactory
{

  private:

    T m_Multiplier;

  public:

    /**
      Constructor
    */
    ValueFactory(T Multiplier) { m_Multiplier = Multiplier;};

    /**
      Default constructor
    */
    ValueFactory() { m_Multiplier = 1;};


    T create(T Input) {return (Input * m_Multiplier); };

    T getMin(T Val1, T Val2) { if (Val1 < Val2) return Val1; else return Val2; };

    T getMax(T Val1, T Val2) { if (Val1 > Val2) return Val1; else return Val2; };

};

} } // namespace openfluid::core


#endif


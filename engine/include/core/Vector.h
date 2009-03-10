
#ifndef __ARRAYVALUE_H__
#define __ARRAYVALUE_H__


#include <iostream>
#include "base/OFException.h"

namespace openfluid { namespace core {

//class OFException;

/**
  Template class for vector data
*/
template <class T>
class Vector
{
  private :
    T* m_Data;
    long m_Size;

    bool allocate(long Size);

    void init();

    static void copy(const Vector& Source, Vector& Dest);

  public :

	/**
	  Default constructor, creates an empty vector
	*/
    Vector();

	/**
	  Copy constructor
	*/
    Vector(const Vector &Vector);

	/**
	  Constructor, creates a vector containing Size elements
	*/
    Vector(long Size);

	/**
	  Constructor, creates a vector containing Size elements, initialized with value InitValue
	*/
    Vector(long Size, T InitValue);

    /**
      Constructor, creates a vector of size Size, containing Data
    */
    Vector(T* Data, long Size);

    /**
      Destructor
    */
    ~Vector();

    /**
      Returns the size of the vector
    */
    long getSize() const { return m_Size; };

    /**
      Returns the size of the vector
    */
    long size() const { return getSize(); };

    /**
      Returns a pointer to the content of the vector (like C arrays)
    */
    T* getData() const { return m_Data; };

    /**
      Sets data for the vector from Data and with size Size
    */
    void setData(T* Data, long Size);

    /**
      Returns the element of the vector for index Index
    */
    T getElement(long Index) const;

    /**
      Returns the element of the vector for index Index
    */
    T at(long Index) const { return getElement(Index); };

    /**
      Sets a new value for element at the given index
    */
    void setElement(long Index, T Element);

    /**
      Operator to set a new value for element given between []
    */
    T& operator[](long Index);

    /**
      Allocation operator
    */
    Vector<T>& operator = (const Vector &A);

    /**
      Clears the vector (empty and size is 0)
    */
    void clear();

};

// =====================================================================
// =====================================================================

template <class T>
Vector<T>::Vector()
{
  init();
}


// =====================================================================
// =====================================================================

template <class T>
Vector<T>::Vector(const Vector &A)
{
  init();

  if (!allocate(A.m_Size)) throw openfluid::base::OFException("ofelib","Vector::Vector(const Vector)","Cannot allocate memory");

  std::copy(A.m_Data, A.m_Data + A.m_Size, m_Data);

}


// =====================================================================
// =====================================================================

template <class T>
Vector<T>::Vector(long Size)
{
  init();

  if (!allocate(Size)) throw openfluid::base::OFException("ofelib","Vector::Vector(Size)","Cannot allocate memory");
}



// =====================================================================
// =====================================================================
template <class T>
Vector<T>::Vector(long Size, T InitValue)
{
  init();


  if (!allocate(Size)) throw openfluid::base::OFException("ofelib","Vector::Vector(Size,T)","Cannot allocate memory");


  if (m_Data != NULL)
  {
    int i;
    for (i=0;i<m_Size;i++) m_Data[i] = InitValue;
  }

}


// =====================================================================
// =====================================================================


template <class T>
Vector<T>::Vector(T* Data, long Size)
{
  init();

  if (!allocate(Size)) throw openfluid::base::OFException("ofelib","Vector::Vector(T*,Size)","Cannot allocate memory");

  std::copy(Data, Data + Size, m_Data);

}


// =====================================================================
// =====================================================================

template <class T>
Vector<T>::~Vector()
{
  if (m_Data != NULL) clear();
}


// =====================================================================
// =====================================================================

template <class T>
bool Vector<T>::allocate(long Size)
{

  if (Size > 0)
  {
    m_Data = new T[Size];
    if (m_Data != NULL) m_Size = Size;
    else
    {
      return false;
    }
  }

  return true;


}

// =====================================================================
// =====================================================================

template <class T>
void Vector<T>::setData(T* Data, long Size)
{
  clear();

  if (!allocate(Size)) throw openfluid::base::OFException("ofelib","Vector::setData","Cannot allocate memory");

  if (m_Data != NULL) std::copy(Data, Data + Size, m_Data);

}


// =====================================================================
// =====================================================================

template <class T>
T Vector<T>::getElement(long Index) const
{
  if (Index < 0 || Index >= m_Size) throw openfluid::base::OFException("ofelib","Vector::getElement","element access range error");
  return m_Data[Index];
}


// =====================================================================
// =====================================================================

template <class T>
void Vector<T>::setElement(long Index, T Element)
{
  if (Index < 0 || Index >= m_Size) throw openfluid::base::OFException("ofelib","Vector::setElement","element access range error");
  m_Data[Index] = Element;
}


// =====================================================================
// =====================================================================


template <class T>
T& Vector<T>::operator[](long Index)
{
  if (Index < 0 || Index >= m_Size) throw openfluid::base::OFException("ofelib","Vector::operator[]","element access range error");
  return m_Data[Index];
}

// =====================================================================
// =====================================================================

template <class T>
Vector<T>& Vector<T>::operator=(const Vector &A)
{

  if (this == &A) return *this; // in case somebody tries assign array to itself

  clear();

  allocate(A.m_Size);
  std::copy(A.m_Data, A.m_Data + A.m_Size, m_Data);

  return *this;
}

// =====================================================================
// =====================================================================


template <class T>
void Vector<T>::init()
{
  m_Data = NULL;
  m_Size = 0;
}

// =====================================================================
// =====================================================================

template <class T>
void Vector<T>::clear()
{
//  if (m_Data != NULL) free(m_Data);
  delete [] m_Data;
  init();
}

// =====================================================================
// =====================================================================

template <class T>
void Vector<T>::copy(const Vector& Source, Vector& Dest)
{
  Dest.clear;
  Dest.allocate(Source.m_Size);
  for (unsigned int i = 0; i < Source.m_Size;i++)
  {
    Dest.m_Data[i] = Source.m_Data[i];
  }
}



} }


#endif

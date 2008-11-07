
#ifndef __ARRAYVALUE_H__
#define __ARRAYVALUE_H__


#include <iostream>


namespace openfluid { namespace core {


/**
  Template class for vector data
*/
template <class T>
class Vector
{
  private :
    T* m_Data;
    long m_Size;

    void allocate(long Size);

    void init();

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
//  std::cout << "copy constructor in" << std::endl;std::cout.flush();

//  clear();
  init();

  allocate(A.m_Size);

  memcpy((T*)m_Data,(T*)A.m_Data,A.m_Size*sizeof(T));


//  std::cout << "copy constructor out" << std::endl;std::cout.flush();


}


// =====================================================================
// =====================================================================

template <class T>
Vector<T>::Vector(long Size)
{
//  std::cout << "size constructor in" << std::endl;std::cout.flush();

  init();

  allocate(Size);

//  std::cout << "size constructor out" << std::endl;std::cout.flush();
}


// =====================================================================
// =====================================================================
template <class T>
Vector<T>::Vector(long Size, T InitValue)
{
  init();


  allocate(Size);


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

  allocate(Size);

  memcpy((T*)m_Data,(T*)Data,Size*sizeof(T));
}


// =====================================================================
// =====================================================================

template <class T>
Vector<T>::~Vector()
{
//  std::cout << "destructor in" << std::endl;std::cout.flush();
  if (m_Data != NULL)
  {
    //std::cout << "m_Data not NULL, size is " << m_Size << std::endl;std::cout.flush();
    clear();
  }
//  std::cout << "destructor out" << std::endl;std::cout.flush();
}


// =====================================================================
// =====================================================================

template <class T>
void Vector<T>::allocate(long Size)
{

  if (Size > 0)
  {
    m_Data = (T*)malloc(Size*sizeof(T));
    if (m_Data != NULL) m_Size = Size;
  }



}

// =====================================================================
// =====================================================================

template <class T>
void Vector<T>::setData(T* Data, long Size)
{
  clear();

  allocate(Size);

  if (m_Data != NULL) memcpy((T*)m_Data,(T*)Data,Size*sizeof(T));
}


// =====================================================================
// =====================================================================

template <class T>
T Vector<T>::getElement(long Index) const
{
  return m_Data[Index];
}


// =====================================================================
// =====================================================================

template <class T>
void Vector<T>::setElement(long Index, T Element)
{
  m_Data[Index] = Element;
}


// =====================================================================
// =====================================================================


template <class T>
T& Vector<T>::operator[](long Index)
{
  return m_Data[Index];
}

// =====================================================================
// =====================================================================

template <class T>
Vector<T>& Vector<T>::operator=(const Vector &A)
{
  /*std::cout << "operator = in" << std::endl;std::cout.flush();*/

  if (this == &A) return *this; // in case somebody tries assign array to itself


  if (A.m_Size == 0) clear(); // is other array is empty -- clear this array

  allocate(A.m_Size);
  memcpy(m_Data, A.m_Data, sizeof(T)*A.m_Size);



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
//  std::cout << "clear in" << std::endl;std::cout.flush();
  if (m_Data != NULL) free(m_Data);
  init();
}





} }


#endif

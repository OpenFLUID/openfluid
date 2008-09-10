
#ifndef __ARRAYVALUE_H__
#define __ARRAYVALUE_H__


#include <iostream>


namespace openfluid { namespace core {



template <class T>
class ArrayValue
{
  private :
    T* m_Data;
    long m_Size;

    void allocate(long Size);

    void init();

  public :

    ArrayValue();

    ArrayValue(const ArrayValue &Vector);

    ArrayValue(long Size);

    ArrayValue(long Size, T InitValue);

    ArrayValue(T* Data, long Size);

    ~ArrayValue();

    long getSize() const { return m_Size; };

    long size() const { return getSize(); };

    T* getData() const { return m_Data; };

    void setData(T* Data, long Size);

    T getElement(long Index) const;

    T at(long Index) const { return getElement(Index); };

    void setElement(long Index, T Element);

    T& operator[](long Index);

    ArrayValue<T>& operator = (const ArrayValue &A);

    void clear();

};

// =====================================================================
// =====================================================================

template <class T>
ArrayValue<T>::ArrayValue()
{
  init();
}


// =====================================================================
// =====================================================================

template <class T>
ArrayValue<T>::ArrayValue(const ArrayValue &A)
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
ArrayValue<T>::ArrayValue(long Size)
{
//  std::cout << "size constructor in" << std::endl;std::cout.flush();

  init();

  allocate(Size);

//  std::cout << "size constructor out" << std::endl;std::cout.flush();
}


// =====================================================================
// =====================================================================
template <class T>
ArrayValue<T>::ArrayValue(long Size, T InitValue)
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
ArrayValue<T>::ArrayValue(T* Data, long Size)
{
  init();

  allocate(Size);

  memcpy((T*)m_Data,(T*)Data,Size*sizeof(T));
}


// =====================================================================
// =====================================================================

template <class T>
ArrayValue<T>::~ArrayValue()
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
void ArrayValue<T>::allocate(long Size)
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
void ArrayValue<T>::setData(T* Data, long Size)
{
  clear();

  allocate(Size);

  if (m_Data != NULL) memcpy((T*)m_Data,(T*)Data,Size*sizeof(T));
}


// =====================================================================
// =====================================================================

template <class T>
T ArrayValue<T>::getElement(long Index) const
{
  return m_Data[Index];
}


// =====================================================================
// =====================================================================

template <class T>
void ArrayValue<T>::setElement(long Index, T Element)
{
  m_Data[Index] = Element;
}


// =====================================================================
// =====================================================================


template <class T>
T& ArrayValue<T>::operator[](long Index)
{
  return m_Data[Index];
}

// =====================================================================
// =====================================================================

template <class T>
ArrayValue<T>& ArrayValue<T>::operator=(const ArrayValue &A)
{
  std::cout << "operator = in" << std::endl;std::cout.flush();

  if (this == &A) return *this; // in case somebody tries assign array to itself


  if (A.m_Size == 0) clear(); // is other array is empty -- clear this array

  allocate(A.m_Size);
  memcpy(m_Data, A.m_Data, sizeof(T)*A.m_Size);



  return *this;
}

// =====================================================================
// =====================================================================


template <class T>
void ArrayValue<T>::init()
{
  m_Data = NULL;
  m_Size = 0;
}

// =====================================================================
// =====================================================================

template <class T>
void ArrayValue<T>::clear()
{
//  std::cout << "clear in" << std::endl;std::cout.flush();
  if (m_Data != NULL) free(m_Data);
  init();
}





} }


#endif

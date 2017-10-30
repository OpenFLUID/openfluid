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
  @file Tree.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_TREE_HPP__
#define __OPENFLUID_CORE_TREE_HPP__

#include <sstream>
#include <iostream>
#include <iomanip>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {


template<typename K, typename V>
class OPENFLUID_API Tree
{
  public:

    typedef std::map<K,Tree<K,V>> Children_t;

    typedef typename Children_t::iterator iterator;

    typedef typename Children_t::const_iterator const_iterator;


  protected:

    V m_Value;

    Children_t m_Children;

    bool m_HasValue;


    std::string keyToStr(const K& Key) const
    {
      std::ostringstream oss;

      if (!(oss << Key).fail())
        return oss.str();
      else
        return "<unknown>";
    }


  public:

    /*
      Default constructor
    */
    Tree<K,V>() : m_HasValue(false)
    {

    }


    // =====================================================================
    // =====================================================================

    /**
      Returns the value of the current tree node
      @return the value associated to the node
      @throw openfluid::base::FrameworkException if the node does not have a value
    */
    V getValue() const
    {
      if (m_HasValue)
        return m_Value;
      else
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Node has no value");

    }


    // =====================================================================
    // =====================================================================

    /**
      Returns the value of the current tree node, or a default value if the node does not have a value
      @param[in] DefaultVal the default value
      @return the value associated to the node or the default value
    */
    V getValue(const V& DefaultVal) const
    {
      if (m_HasValue)
        return m_Value;
      else
        return DefaultVal;
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the value of the current tree node
      @param[in] Val the value of the node
    */
    void setValue(const V& Val)
    {
      m_Value = Val;
      m_HasValue = true;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns true if the node has a value
      @return true if the node has a value, false otherwise
    */
    bool hasValue() const
    {
      return m_HasValue;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a reference to the map of children of the current node
      @return a map of children
    */
    Children_t& children()
    {
      return m_Children;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a const reference to the map of children of the current node
      @return a map of children
    */
    const Children_t& children() const
    {
      return m_Children;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a reference to the child of the current node, identified by its key
      @param[in] Key the key of the child node
      @return the reference to the child node
      @throw openfluid::base::FrameworkException if the child does not exist
    */
    Tree<K,V>& child(const K& Key)
    {
      auto it = m_Children.find(Key);

      if (it == m_Children.end())
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Key " + keyToStr(Key) + " does not exist");

      return (*it).second;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a const reference to the child of the current node, identified by its key
      @param[in] Key the key of the child node
      @return the reference to the child node
      @throw openfluid::base::FrameworkException if the child does not exist
    */
    const Tree<K,V>& child(const K& Key) const
    {
      auto it = m_Children.find(Key);

      if (it == m_Children.end())
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Key " + keyToStr(Key) + " does not exist");

      return (*it).second;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the value of the child of the current node, identified by its key.
      If the child node does not have a value, the given default value is returned
      @param[in] Key the key of the child node
      @param[in] DefaultVal the default value
      @return the value of the child node or the default value
    */
    V getChildValue(const K& Key, const V& DefaultVal) const
    {
      auto it = m_Children.find(Key);

      if (it == m_Children.end())
        return DefaultVal;

      return (*it).second.getValue(DefaultVal);
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns true if the current node has a child identified by the given key
      @return true if the child node has been found, false otherwise
    */
    bool hasChild(const K& Key) const
    {
      return (m_Children.find(Key) != m_Children.end());
    }


    // =====================================================================
    // =====================================================================

    /**
      Adds a child to the current node, identified by its key and with the given value
      @param[in] Key the key of the child node
      @param[in] Val the value of the child node
      @return the reference to the added child node
      @throw openfluid::base::FrameworkException if the child already exists
    */
    Tree<K,V>& addChild(const K& Key, const V& Val)
    {
      if (hasChild(Key))
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Key " + keyToStr(Key) + " already exists");

      m_Children[Key] = Tree<K,V>();
      m_Children[Key].setValue(Val);

      return m_Children[Key];
    }


    // =====================================================================
    // =====================================================================


    /**
      Adds a child to the current node, identified by its key and with no value
      @param[in] Key the key of the child node
      @return the reference to the added child node
      @throw openfluid::base::FrameworkException if the child already exists
    */
    Tree<K,V>& addChild(const K& Key)
    {
      if (hasChild(Key))
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Key " + keyToStr(Key) + " already exists");

      m_Children[Key] = Tree<K,V>();

      return m_Children[Key];
    }


    // =====================================================================
    // =====================================================================


    /**
      Deletes a child of the current node, identified by its key and with no value
      @param[in] Key the key of the child node to delete
    */
    void deleteChild(const K& Key)
    {
      auto it = m_Children.find(Key);

      if (it != m_Children.end())
        m_Children.erase(it);
    }


    // =====================================================================
    // =====================================================================

    /**
      Returns an iterator referring to the first element of the children map
      @return an iterator to the first element in the children map
    */
    inline iterator begin()
    {
      return m_Children.begin();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a const iterator referring to the first element of the children map
      @return a const iterator to the first element in the children map
    */
    inline const_iterator begin() const
    {
      return m_Children.begin();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns an iterator referring to the past-the-end element of the children map
      @return an iterator to the past-the-end element of the children map
    */
    inline iterator end()
    {
      return m_Children.end();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a const iterator referring to the past-the-end element of the children map
      @return a const iterator to the past-the-end element of the children map
    */
    inline const_iterator end() const
    {
      return m_Children.end();
    }


    // =====================================================================
    // =====================================================================


    /**
      Clears the tree determined by the current node
    */
    void clear()
    {
      m_Value = V();

      for (auto& ChildNode : m_Children)
        ChildNode.second.clear();

      m_Children.clear();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the size of the tree determined by the current node
      @return the size of the tree
    */
    unsigned int size() const
    {
      int Size = 1;

      for (auto& ChildNode : m_Children)
        Size += ChildNode.second.size();

      return Size;
    }


    // =====================================================================
    // =====================================================================


    /**
      Writes hierarchically the tree determined by the current node into the given stream, using the given indentation
      @param[in] OutStm the output stream to write into
      @param[in] Indent the indent value for children nodes
    */
    void writeToStreamHierarchically(std::ostream& OutStm, unsigned int Indent = 0) const
    {
      for (auto& ChildNode : m_Children)
      {
        if (Indent)
          OutStm  << std::setw(Indent);

        OutStm << "[" << ChildNode.first;
        if (ChildNode.second.hasValue())
          OutStm << " -> " << ChildNode.second.getValue();
        OutStm << "]\n";

        ChildNode.second.writeToStreamHierarchically(OutStm,Indent+4);
      }
    }


    // =====================================================================
    // =====================================================================


    void writeToStreamFlat(std::ostream& OutStm) const
    {
      for (auto& ChildNode : m_Children)
      {
        OutStm << "{" << ChildNode.first;
        if (ChildNode.second.hasValue())
          OutStm << "(" << ChildNode.second.getValue() << ")";
        ChildNode.second.writeToStreamFlat(OutStm);
        OutStm << "}";
      }
    }

};


} }  // namespaces

#endif /* __OPENFLUID_TREE_HPP__ */

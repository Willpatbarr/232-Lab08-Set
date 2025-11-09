/***********************************************************************
* Header:
*    Set
* Summary:
*      __       ____       ____         __
*     /  |    .'    '.   .'    '.   _  / /
*     `| |   |  .--.  | |  .--.  | (_)/ /
*      | |   | |    | | | |    | |   / / _
*     _| |_  |  `--'  | |  `--'  |  / / (_)
*    |_____|  '.____.'   '.____.'  /_/
*
*    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    Sara Nuss, William Patrick Barr
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
   friend class ::TestSet; // give unit tests access to the privates
public:
   
   // 
   // Construct
   //
   set() 
   { 
   }
   set(const set &  rhs)
   {
      bst = rhs.bst;
   }
   set(set && rhs) 
   {
      bst = std::move(rhs.bst);
   }
   set(const std::initializer_list <T> & il) 
   {
      bst = il;
   }
   template <class Iterator>
   set(Iterator first, Iterator last) 
   {
      for (auto it = first; it != last; ++it)
            bst.insert(*it, true); // insert unique elements
   }
   ~set() { bst.clear(); }

   //
   // Assign
   //

   set & operator = (const set & rhs)
   {
      bst = rhs.bst;
      return *this;
   }
   set & operator = (set && rhs)
   {
      bst = std::move(rhs.bst);
      return *this;
   }
   set & operator = (const std::initializer_list <T> & il)
   {
      bst = il;
      return *this;
   }
   void swap(set& rhs) noexcept
   {
      bst.swap(rhs.bst);
   }

   //
   // Iterator
   //

   class iterator;
   iterator begin() const noexcept 
   { 
      return iterator(bst.begin());
   }
   iterator end() const noexcept 
   { 
      return iterator(bst.end());
   }

   //
   // Access
   //
   iterator find(const T& t) 
   { 
      return iterator(bst.find(t));
   }

   //
   // Status
   //
   bool   empty() const noexcept 
   { 
      return bst.empty();
   }
   size_t size() const noexcept 
   { 
      return bst.size();     
   }

   //
   // Insert
   //
   // copy insert
   std::pair<iterator, bool> insert(const T& t)
   {
      std::pair<iterator, bool> p = bst.insert(t, true);
      return p;
   }
   // move insert
   std::pair<iterator, bool> insert(T&& t)
   {
       std::pair<iterator, bool> p = bst.insert(std::move(t), true);
      return p;
   }
   // insert all the elements in a given initializer list
   void insert(const std::initializer_list <T>& il)
   {
       for(T i : il)
           insert(i);
   }
   // insert a range of elements
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
       // use iterator to insert each element
        for(iterator it = first; first != last; ++first)
            insert(*first);
   }


   //
   // Remove
   //
   // remove every element using BST clear.
   void clear() noexcept 
   {
       bst.clear();
   }
   // erase a single element
   iterator erase(iterator &it)
   {
      return iterator(bst.erase(it.it));
   }
   // erase a given element
   size_t erase(const T & t) 
   {
      iterator it = find(t);
      // check if it's in there
      if (it == end())
          return 0; // no elemenets removed
      // erase it
      erase(it);
      return 1;
   }
   // erase elements in a given range
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
       // go through each element and erase it
       while (itBegin != itEnd)
           itBegin = erase(itBegin);
      return itEnd;
   }

private:
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
   friend class ::TestSet; // give unit tests access to the privates
   friend class custom::set<T>;

public:
   // constructors, destructors, and assignment operator
   iterator() 
   {
       it.pNode = nullptr;
   }
   iterator(const typename custom::BST<T>::iterator& itRHS) 
   {
       it = itRHS;
   }
   iterator(const iterator & rhs) 
   {
       it = rhs.it;
   }
   iterator & operator = (const iterator & rhs)
   {
       it = rhs.it;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      return  it != rhs.it;
   }
   bool operator == (const iterator & rhs) const 
   { 
      return it == rhs.it;
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const 
   { 
      return *it;
   }

   // prefix increment
   iterator & operator ++ ()
   {
       // increment and return
       ++it;
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
       // iterator to be returned
       iterator old(*this);
       // increment it
       ++it;
       // return old since it's postfix
       return old;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
       // decrement and return
       --it;
      return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
        // create iterator to returned
        iterator old(*this);
        // decrement this
        --it;
        // return old since it's postfix
        return *old;
   }
   
private:

   typename custom::BST<T>::iterator it;
};


}; // namespace custom




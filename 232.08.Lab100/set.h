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
*    <Your name here>
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
   {}
   set(const set &  rhs) : bst(rhs.bst)
   { 

   }
   set(set && rhs) : bst(std::move(rhs.bst))
   { 
   }
   set(const std::initializer_list <T> & il) 
   {
      for (const auto& node : il)
      {
         bst.insert(node);
      }
   }
   template <class Iterator>
   set(Iterator first, Iterator last) 
   {
      for (auto iter = first; iter != last; ++iter)
      {
         bst.insert(*iter);
      }
   }
  ~set() { }

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
   std::pair<iterator, bool> insert(const T& t)
   {
      std::pair<iterator, bool> p = bst.insert(t, true);
      return p;
   }
   std::pair<iterator, bool> insert(T&& t)
   {
      std::pair<iterator, bool> p = bst.insert(std::move(t), true);
      return p;
   }
   void insert(const std::initializer_list <T>& il)
   {
      for (const auto& node : il)
      {
         bst.insert(node);
      }
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
      for (auto iter = first; iter != last; ++iter)
      {
         bst.insert(*iter);
      }
   }


   //
   // Remove
   //
   void clear() noexcept 
   { 
   }
   iterator erase(iterator &it)
   { 
      typename custom::BST<T>::iterator er = bst.erase(it.it);
      return er; 
   }
   size_t erase(const T & t) 
   {
      // auto it = iterator(t);
      // typename custom::BST<T>::iterator er = bst.erase(it.it);
      return bst.numElements;
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
      //for (auto iter = itBegin; iter != itEnd; ++iter)
      //{
      //   //bst.erase(iter);
      //}
      return iterator();
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
      this->it = rhs.it;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   {
      return it != rhs.it; 
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
      ++it;
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
      iterator& temp = *this;
      ++it;
      return *temp;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
      --it;
      return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
      iterator& temp = *this;
      --it;
      return *temp;
   }
   
private:

   typename custom::BST<T>::iterator it;
};


}; // namespace custom




#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>
#include <functional>

class TestSet;

namespace custom
{

   template <typename T>
   class set
   {
      friend class ::TestSet;

   public:

      // Constructors
      set() : bst() {}

      set(const set& rhs) : bst(rhs.bst) {}

      set(set&& rhs) : bst(std::move(rhs.bst)) {}

      set(const std::initializer_list <T>& il) : bst(il) {}

      template <class Iterator>
      set(Iterator first, Iterator last)
      {
         for (; first != last; ++first)
            bst.insert(*first, true);
      }

      ~set() {}

      // Assignment Operators
      set& operator = (const set& rhs)
      {
         bst = rhs.bst;
         return *this;
      }

      set& operator = (set&& rhs)
      {
         bst = std::move(rhs.bst);
         return *this;
      }

      set& operator = (const std::initializer_list <T>& il)
      {
         bst = il;
         return *this;
      }

      void swap(set& rhs) noexcept
      {
         bst.swap(rhs.bst);
      }

      // Iterator
      class iterator;
      iterator begin() const noexcept { return iterator(bst.begin()); }
      iterator end() const noexcept { return iterator(bst.end()); }

      // Access
      iterator find(const T& t) { return iterator(bst.find(t)); }

      // Status
      bool empty() const noexcept { return bst.empty(); }
      size_t size() const noexcept { return bst.size(); }

      // Insert
      std::pair<iterator, bool> insert(const T& t)
      {
         auto result = bst.insert(t, true);
         return { iterator(result.first), result.second };
      }

      std::pair<iterator, bool> insert(T&& t)
      {
         auto result = bst.insert(std::move(t), true);
         return { iterator(result.first), result.second };
      }

      void insert(const std::initializer_list <T>& il)
      {
         for (const T& t : il)
            bst.insert(t, true);
      }

      template <class Iterator>
      void insert(Iterator first, Iterator last)
      {
         for (; first != last; ++first)
            bst.insert(*first, true);
      }

      // Remove
      void clear() noexcept { bst.clear(); }

      iterator erase(iterator& it)
      {
         return iterator(bst.erase(it.it));
      }

      size_t erase(const T& t)
      {
         iterator it = find(t);
         if (it == end()) return 0;
         erase(it);
         return 1;
      }

      iterator erase(iterator& itBegin, iterator& itEnd)
      {
         while (itBegin != itEnd)
            erase(itBegin++);
         return itEnd;
      }

   private:
      custom::BST <T> bst;
   };

   // SET ITERATOR
   template <typename T>
   class set <T> ::iterator
   {
      friend class ::TestSet;
      friend class custom::set<T>;

   public:
      iterator() : it() {}

      iterator(const typename custom::BST<T>::iterator& itRHS) : it(itRHS) {}

      iterator(const iterator& rhs) : it(rhs.it) {}

      iterator& operator = (const iterator& rhs)
      {
         it = rhs.it;
         return *this;
      }

      bool operator != (const iterator& rhs) const { return it != rhs.it; }
      bool operator == (const iterator& rhs) const { return it == rhs.it; }

      const T& operator * () const { return *it; }

      iterator& operator ++ () { ++it; return *this; }
      iterator operator++ (int) { iterator temp(*this); ++it; return temp; }

      iterator& operator -- () { --it; return *this; }
      iterator operator-- (int) { iterator temp(*this); --it; return temp; }

   private:
      typename custom::BST<T>::iterator it;
   };

}; // namespace custom

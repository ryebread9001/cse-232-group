/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    Ryan Whitehead, Cesar 
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
//#include "vector.h"
#include <vector>

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   // 
   // Construct
   // 

   stack()                            { container.resize(0); }
   stack(const stack <T>& rhs) { container = rhs.container; }
   stack(      stack <T> && rhs)      { container = std::move(rhs.container); }
   stack(const std::vector<T> &  rhs) 
   { 
      container.resize(0);
      for (int i = 0; i < rhs.size(); i++)
      {
         container.push_back(rhs[i]);
      }
   }
   stack(      std::vector<T> && rhs) 
   {
      container.resize(0);
      for (int i = 0; i < rhs.size(); i++)
      {
         container.push_back(rhs[i]);
      }
   }
   ~stack()                           {                      }

   //
   // Assign
   //

   stack <T> & operator = (const stack <T> & rhs)
   {
      container = rhs.container;
      return *this;
   }
   stack <T>& operator = (stack <T> && rhs)
   {
      container = std::move(rhs.container);
      return *this;
   }
   void swap(stack <T>& rhs)
   {
      std::swap(container, rhs.container);
   }

   // 
   // Access
   //

         T& top()       { return container[container.size() - 1]; }
   const T& top() const { return container[container.size() - 1]; }

   // 
   // Insert
   // 

   void push(const T& t) { container.push_back(t); }
   void push(      T&& t) { container.push_back(t); }

   //
   // Remove
   //

   void pop() 
   {
      container.pop_back();
   }

   //
   // Status
   //
   size_t  size () const { return container.size();  }
   bool empty   () const { return container.size() == 0; }
   
private:
   
  std::vector<T> container;  // underlying container
};



} // custom namespace



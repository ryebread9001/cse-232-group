/***********************************************************************
 * Header:
 *    Array
 * Summary:
 *    Our custom implementation of std::array
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *    
 *    This will contain the class definition of:
 *       array             : similar to std::array
 *       array :: iterator : an iterator through the array
 * Author
 *    Ryan Whitehead
 ************************************************************************/

#pragma once

#include <stdexcept>
#include <iostream>
class TestArray;

namespace custom
{

/************************************************
 * ARRAY
 * A class that holds stuff
 ***********************************************/
template <int N>
struct array
{
public:
   //
   // Construct
   //

   // No constructors or destructors needed here

   //
   // Assign
   //

   // No assignment operators needed here

   //
   // Iterator
   //

   // Forward declaration for the iterator class
   class iterator;

   // Begin and end methods
   iterator begin() 
   { 
      /* your code goes here; it should return an iterator referring to the first element in the array */
      return iterator(__elems_); 
   }
   iterator end()   
   { 
      /* your code goes here; it should return an iterator referring to the first element off the end of the array */
      return iterator(__elems_+N);
   }

   // 
   // Access
   //

   // Subscript
   int& operator [] (size_t index)       
   { 
      /*your code goes here; it should return the i-th element in the array */
      if (index < 0 || index > N - 1) throw std::out_of_range("index is out of range");
      return __elems_[index]; 
   }
   const int& operator [] (size_t index) const 
   { 
      /*your code goes here; it should return the i-th element in the array */
      if (index < 0 || index > N - 1) throw std::out_of_range("index is out of range");
      return __elems_[index];
   }

   friend std::ostream& operator<<(std::ostream & os, array& arr)
   {
       return os << arr[0] << std::endl;
   }

   // At
   int& at(size_t index)
   {
      /*your code goes here; it should return the i-th element in the array */
      if (index < 0 || index > N - 1) throw std::out_of_range("index is out of range");
      return __elems_[index];
   }
   const int& at(size_t index) const
   {
      /*your code goes here; it should return the i-th element in the array */
      if (index < 0 || index > N - 1) throw std::out_of_range("index is out of range");
      return __elems_[index];
   }

   // Front and Back
   int& front()       
   { 
      /* your code goes here; it should return the first element in the array */
      return __elems_[0]; 
   }
   int& back()        
   { 
      /* your code goes here; it should return the last element in the array */
      return __elems_[N-1];
   }
   const int& front() const 
   { 
      return __elems_[0];
   }
   const int& back()  const 
   { 
      /* your code goes here; it should return the last element in the array */
      if (N < 1) return 0;
      return __elems_[N-1];
   }

   //
   // Remove

   // No clear or erase here

   //
   // Status
   //

   // number of elements in the array
   size_t size()  const 
   { 
      /* your code goes here, this should not always return 999 */
      return N;           
   }
   bool empty() const { 
      /* your code goes here; this should not always return TRUE */
      if (N < 1) return true;
      return false; 
   }
   
   //
   // Member Variable
   //

   // statically allocated array of T
   int __elems_[N];          /* your code goes here */
};


/**************************************************
 * ARRAY ITERATOR
 * An iterator through array
 *************************************************/
template <int N>
class array <N> :: iterator
{
   friend class ::TestArray;
public:
   // constructors, destructors, and assignment operator
   iterator()     
   { 
      /* your code goes here; it should initialize an iterator */
      this->p = nullptr;
   }
   iterator(int * p)   
   { 
      /* your code goes here; it should point to an element in the array */
      this->p = p;
   }
   iterator(const iterator & rhs) 
   {  
      /* your code goes here; it should copy an iterator */
      this->p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
      /* your code goes here; it should copy an iterator */
      this->p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      /* your code goes here; it should not always return TRUE */
      return p != rhs.p; 
   }
   bool operator == (const iterator & rhs) const 
   { 
      /* your code goes here; it should not always return TRUE */
      return p == rhs.p;
   }
   
   // dereference operator
   int & operator * ()       
   { 
      /* your code goes here; it should fetch the element referred to by p */
      return *p; 
   }
   const int & operator * () const 
   { 
      /* your code goes here; it should fetch the element referred to by p */
      return *p; 
   }
   
   // prefix increment
   iterator & operator ++ ()
   {
      /* your code goes here; it should advance the iterator by one */
      this->p++;
      return *this;
   }
   
   // postfix increment
   iterator operator ++ (int postfix)
   {
      /* your code goes here; it should advance the iterator by one */
      iterator result = *this;
      this->p++;
      return result;
   }
   
private:
   int * p;
};



}; // namespace custom

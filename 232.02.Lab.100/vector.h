/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
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
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An interator through Vector
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

/*****************************************
 * VECTOR
 * Just like the std :: vector <T> class
 ****************************************/
template <typename T>
class vector
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   
   // 
   // Construct
   //

   vector();
   vector(size_t numElements);
   vector(size_t numElements, const T& t);
   vector(const std::initializer_list<T>& l );
   vector(const vector &  rhs);
   vector(      vector && rhs);
   ~vector();

   //
   // Assign
   //

   void swap(vector& rhs)
   {
      std::swap(data, rhs.data);
      std::swap(numElements, rhs.numElements);
      std::swap(numCapacity, rhs.numCapacity);
   }
   vector & operator = (const vector & rhs);
   vector& operator = (vector&& rhs);

   //
   // Iterator
   //

   class iterator;
   iterator       begin() { return iterator(data); }
   iterator       end() { return iterator(data+numElements); }

   //
   // Access
   //

   T& operator [] (size_t index);
   const T& operator [] (size_t index) const;
   T& front();
   const T& front() const;
         T& back();
   const T& back() const;

   //
   // Insert
   //

   void push_back(const T& t);
   void push_back(T&& t);
   void reserve(size_t newCapacity);
   void resize(size_t newElements);
   void resize(size_t newElements, const T& t);

   //
   // Remove
   //

   void clear()
   {
      /*data = nullptr;
      numCapacity = 0;*/
      numElements = 0;
   }
   void pop_back()
   {
      if (numElements > 0) numElements--;
   }
   void shrink_to_fit();

   //
   // Status
   //

   size_t  size()          const { return numElements; }
   size_t  capacity()      const { return numCapacity; }
   bool empty()            const { return (numElements == 0); }
   
   // adjust the size of the buffer
   
   // vector-specific interfaces
   
private:
   
   T *  data;                 // user data, a dynamically-allocated array
   size_t  numCapacity;       // the capacity of the array
   size_t  numElements;       // the number of items currently used
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T>
class vector <T> ::iterator
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   // constructors, destructors, and assignment operator
   iterator()                           { p = nullptr; }
   iterator(T* p)                       { this->p = p; }
   iterator(const iterator& rhs)        { p = rhs.p; }
   iterator(size_t index, vector<T>& v) { p = v.data+index; }
   iterator& operator = (const iterator& rhs)
   {
      p = rhs.p;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return p != rhs.p; }
   bool operator == (const iterator& rhs) const { return p == rhs.p; }

   // dereference operator
   T& operator * ()
   {
      return *p;
   }

   // prefix increment
   iterator& operator ++ ()
   {
      ++p;
      return *this;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator t = *this;
      p++;
      return *this;
   }

   // prefix decrement
   iterator& operator -- ()
   {
      p--;
      return *this;
   }

   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator t = *this;
      p--;
      return *this;
   }

private:
   T* p;
};

/*****************************************
 * VECTOR :: DEFAULT constructors
 * Default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector()
{
   data = nullptr;
   numCapacity = 0;
   numElements = 0;
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num, const T & t) 
{
   if (num > 0) {
      numElements = num;
      numCapacity = num;
      data = new T[num];
      for (int i = 0; i < num; i++) {
         data[i] = t;
      }
   }
   else
   {
      data = nullptr;
      numCapacity = num;
      numElements = num;
   }
}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
template <typename T>
vector <T> :: vector(const std::initializer_list<T> & l) 
{
   numElements = l.size();
   numCapacity = numElements;
   if (numElements > 0)
   {
      data = new T[numCapacity];
      int i = 0;
      for (const T& index : l)
      {
         data[i++] = index;
      }
   }
   else
   {
      data = nullptr;
   }
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num) 
{
   if (num > 0) {
      data = new T[num]();
      numCapacity = num;
      numElements = num;
   }
   else
   {
      data = nullptr;
      numCapacity = num;
      numElements = num;
   }
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T>
vector <T> :: vector (const vector & rhs) 
{
    
   numElements = rhs.size();
   numCapacity = rhs.size();
   if (numCapacity > 0)
   {
      data = new T[numCapacity];
      for (int i = 0; i < numElements; i++) {
         data[i] = rhs[i];
      }
   }
   else
   {
      data = nullptr;
   }
}

/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T>
vector <T> :: vector (vector && rhs)
{
   numElements = std::move(rhs.numElements);
   numCapacity = std::move(rhs.numCapacity);
   data = std::move(rhs.data);

   rhs.numElements = 0;
   rhs.numCapacity = 0;
   rhs.data = nullptr;
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T>
vector <T> :: ~vector()
{
   
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: resize(size_t newElements)
{
   if (newElements == 0) {
      data = nullptr;
      numElements = 0;
      return;
   }

   if (newElements > numCapacity)
   {
      if (newElements <= numCapacity)
      {
         return;
      }
      T* newData = new T[newElements];

      for (size_t i = 0; i < numElements; i++)
      {
         newData[i] = std::move(data[i]);
      }
      delete[] data;

      data = newData;
      numCapacity = newElements;
   }

   for (size_t i = numElements; i < newElements; i++)
   {
      data[i] = T();
   }
   numElements = newElements;
}

template <typename T>
void vector <T> :: resize(size_t newElements, const T & t)
{
   if (newElements == 0) {
      data = nullptr;
      numElements = 0;
      return;
   }

   if (newElements > numElements)
   {
      if (newElements > numCapacity) {
         if (newElements <= numCapacity)
         {
            return;
         }
         T* newData = new T[newElements];

         for (size_t i = 0; i < numElements; i++)
         {
            newData[i] = std::move(data[i]);
         }
         delete[] data;

         data = newData;
         numCapacity = newElements;
      }
      for (size_t i = numElements; i < newElements; i++)
      {
         data[i] = t;
      }
   }

   
   numElements = newElements;
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: reserve(size_t newCapacity)
{
   if (newCapacity <= numCapacity)
   {
      return;
   }
   T* newData = new T[newCapacity];

   for (size_t i = 0; i < numElements; i++)
   {
      newData[i] = std::move(data[i]);
   }
   delete[] data;

   data = newData;
   numCapacity = newCapacity;
}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: shrink_to_fit()
{
   numCapacity = numElements;
   if (numCapacity == 0) data = nullptr;
}



/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: operator [] (size_t index)
{
    assert(index >= 0 && index < numElements);
    return data[index];
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: operator [] (size_t index) const
{
    assert(index >= 0 && index < numElements);
    return data[index];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: front ()
{
   if (numElements > 0)
   {
      return data[0]; // Return a reference to the first element
   }
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: front () const
{
   if (numElements > 0)
   {
      return data[0]; // Return a reference to the first element
   }
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: back()
{
   if (numElements > 0)
   {
      return data[0+numElements-1]; // Return a reference to the first element
   }
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: back() const
{
   if (numElements > 0)
   {
      return data[0+numElements-1]; // Return a reference to the first element
   }
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T>
void vector <T> :: push_back (const T & t)
{
   if (numElements == numCapacity)
   {
      if (numCapacity == 0)
      {
         reserve(1);
         data[0] = t;
         numElements++;
         return;
      }
      else
      {
         reserve(numCapacity * 2);
      }
   }
   data[numElements] = t;
   numElements++;
}

template <typename T>
void vector <T> ::push_back(T && t)
{
   if (numElements == numCapacity)
   {
      if (numCapacity == 0)
      {
         reserve(1);
         data[0] = t;
         numElements = 1;
         return;
      }
      else
      {
         reserve(numCapacity * 2);
      }
   }
   data[numElements] = std::move(t);
   ++numElements;
}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T>
vector <T> & vector <T> :: operator = (const vector & rhs)
{
   if (this != &rhs)
   {
      delete[] data;

      numElements = rhs.numElements;
      numCapacity = rhs.numCapacity > numCapacity ? rhs.numCapacity : numCapacity;

      if (numCapacity > 0)
      {
         data = new T[numCapacity];
         for (int i = 0; i < numElements; ++i)
         {
            data[i] = rhs.data[i];
         }
      }
      else
      {
         data = nullptr;
      }
   }
   return *this;
}
template <typename T>
vector <T>& vector <T> :: operator = (vector&& rhs)
{

   if (this != &rhs) 
   {
      delete[] data;

      data = rhs.data;
      numElements = rhs.numElements;
      numCapacity = rhs.numCapacity; // > numCapacity ? rhs.numCapacity : numCapacity;

      rhs.data = nullptr;
      rhs.numElements = 0;
      rhs.numCapacity = 0;
   }
   return *this;
}


} // namespace custom


/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Julio Cesar Tavarez
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
 
class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:  
   // 
   // Construct
   //

   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list() 
   {
   }

   // 
   // Assign
   //

   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);
   void swap(list <T>& rhs);

   //
   // Iterator
   //

   class  iterator;
   iterator begin()  { return iterator(pHead); }
   iterator rbegin() { return iterator(); }
   iterator end()    { return nullptr; }

   //
   // Access
   //

   T& front();
   T& back();

   //
   // Insert
   //

   void push_front(const T&  data);
   void push_front(      T&& data);
   void push_back (const T&  data);
   void push_back (      T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //

   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   // 
   // Status
   //

   bool empty()  const { return numElements < 1; }
   size_t size() const { return numElements;   }


private:
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;    // pointer to the beginning of the list
   Node * pTail;    // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
   //
   // Construct
   //
   Node()  
   {
      pNext = pPrev = nullptr;
   }
   Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr) {}
   Node(T&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}

   //
   // Data
   //

   T data;                 // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
   // constructors, destructors, and assignment operator
   iterator() 
   {
      p = nullptr;
   }
   iterator(Node * p) 
   {
      this->p = p;
   }
   iterator(const iterator& rhs) : p(rhs.p) {}
   iterator & operator = (const iterator & rhs)
   {
      p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return p == rhs.p; }
   bool operator != (const iterator & rhs) const { return p != rhs.p; }

   // dereference operator, fetch a node
   T & operator * ()
   {
      return p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;   // save current state
      ++(*this);               // advance this using prefix increment
      return temp;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      if (p) p = p->pNext; return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator temp = *this;
      --(*this);  // call prefix decrement
      return temp;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      p = p->pPrev;
      return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T> :: insert(iterator it, const T &  data);
   friend iterator list <T> :: insert(iterator it,       T && data);
   friend iterator list <T> :: erase(const iterator & it);

private:

   typename list <T> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) 
{
   numElements = 0;
   pHead = pTail = nullptr;

   for (size_t i = 0; i < num; ++i)
   {
      push_back(t);  // Use your existing push_back() method
   }
}

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list <T> ::list(Iterator first, Iterator last)
{
   numElements = 0;
   pHead = pTail = nullptr;

   for (Iterator it = first; it != last; ++it)
   {
      push_back(*it);
   }
}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il)
{
   numElements = 0;
   pHead = pTail = nullptr;

   for (const auto& item : il)
   {
      push_back(item);
   }
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num)
{
   numElements = 0;
   pHead = pTail = nullptr;

   for (size_t i = 0; i < num; ++i)
   {
      push_back(T{});
   }

}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list() 
{
   numElements = 0;
   pHead = pTail = nullptr;
}

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs) 
{
   //numElements = rhs.numElements;
   pHead = pTail = nullptr;

   for (Node* p = rhs.pHead; p != nullptr; p = p->pNext)
   {
      push_back(p->data);
   }
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs)
{
   numElements = rhs.numElements;
   pHead = rhs.pHead;
   pTail = rhs.pTail;

   rhs.numElements = 0;
   rhs.pHead = rhs.pTail = nullptr;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{

   clear(); 
   numElements = rhs.numElements;
   pHead = std::move(rhs.pHead);
   pTail = std::move(rhs.pTail);

      
   rhs.pHead = nullptr;
   rhs.pTail = nullptr;
   rhs.numElements = 0;


   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{
   clear();
   //numElements = rhs.numElements;

   Node* pSource = rhs.pHead;

   while (pSource)
   {
      push_back(pSource->data);
      pSource = pSource->pNext;
   }

   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{
   clear();
   //numElements = rhs.numElements;

   for (const T& item : rhs)
   {
      push_back(item);  // assumes you have push_back() implemented
   }

   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
void list <T> :: clear()
{
   Node* temp = nullptr;
   Node* next = nullptr;

   while (temp != nullptr) {
      next = temp->pNext;
      delete temp;
      temp = next;
   }

   pHead = nullptr;
   pTail = nullptr;
   numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_back(const T & data)
{
   // allocate a new node with the data
   Node* newNode = new Node(data);

   if (pTail == nullptr) // if the list is empty
   {
      pHead = pTail = newNode;
   }
   else
   {
      pTail->pNext = newNode;   // link the current tail to the new node
      newNode->pPrev = pTail;   // link the new node back to the current tail
      pTail = newNode;          // update tail to the new node
   }

   numElements++;
}

template <typename T>
void list <T> ::push_back(T && data)
{
   Node* newNode = new Node(std::move(data));

   if (pTail == nullptr) // if the list is empty
   {
      pHead = pTail = newNode;
   }
   else
   {
      pTail->pNext = newNode;   // link the current tail to the new node
      newNode->pPrev = pTail;   // link the new node back to the current tail
      pTail = newNode;          // update tail to the new node
   }

   numElements++;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_front(const T & data)
{
   Node* newNode = new Node(data);

   if (pHead == nullptr)
   {
      pHead = pTail = newNode;
   }
   else
   {
      newNode->pNext = pHead;
      pHead->pPrev = newNode;
      pHead = newNode;
   }
   numElements++;
}

template <typename T>
void list <T> ::push_front(T && data)
{
   Node* newNode = new Node(std::move(data));

   if (pHead == nullptr)
   {
      pHead = pTail = newNode;
   }
   else
   {
      newNode->pNext = pHead;
      pHead->pPrev = newNode;
      pHead = newNode;
   }
   numElements++;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{
   if (pTail == nullptr) return;

   Node* tempNode = new Node();
   pTail = pTail->pPrev;

   if (pTail)
   {
      pTail->pNext = nullptr;
   }
   else
   {
      pHead = nullptr;
   }

   delete tempNode;
   numElements--;

}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{
   if (pHead == nullptr) return;

   Node* tempNode = pHead;
   pHead = pHead->pNext;

   if (pHead)
   {
      pHead->pPrev = nullptr;
   }
   else
   {
      pTail = nullptr;
   }

   delete tempNode;
   numElements--;
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T& list <T> ::front()
{
   if (pHead == nullptr)
   {
      throw "ERROR: unable to access data from an empty list";
   }
   return pHead->data;
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: back()
{
   if (pTail == nullptr)
   {
      throw "ERROR: unable to access data from an empty list";
   }
   return pTail->data;
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator  list <T> :: erase(const list <T> :: iterator & it)
{
   if (it.p == nullptr) return end();

   Node* pDelete = it.p;

   if (pDelete->pPrev)
      pDelete->pPrev->pNext = pDelete->pNext;
   else
      pHead = pDelete->pNext;

   if (pDelete->pNext)
      pDelete->pNext->pPrev = pDelete->pPrev;
   else
      pTail = pDelete->pPrev;

   iterator itReturn(pDelete->pNext);
   delete pDelete;
   numElements--;

   return itReturn;
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
                                                 const T & data) 
{
   if (it.p == nullptr) {
      push_back(data);
      return iterator(pTail);
   }

   Node* newNode = new Node(data);
   newNode->pPrev = it.p->pPrev;
   newNode->pNext = it.p;

   if (it.p->pPrev)
      it.p->pPrev->pNext = newNode;
   else
      pHead = newNode;

   it.p->pPrev = newNode;
   numElements++;

   return iterator(newNode);
}

template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
   T && data)
{
   if (it.p == nullptr) {
      push_back(std::move(data));
      return iterator(pTail);
   }

   Node* newNode = new Node(std::move(data));
   newNode->pPrev = it.p->pPrev;
   newNode->pNext = it.p;

   if (it.p->pPrev)
      it.p->pPrev->pNext = newNode;
   else
      pHead = newNode;

   it.p->pPrev = newNode;
   numElements++;

   return iterator(newNode);
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
   std::swap(lhs.pHead, rhs.pHead);
   std::swap(lhs.pTail, rhs.pTail);
   std::swap(lhs.numElements, rhs.numElements);
}

template <typename T>
void list<T>::swap(list <T>& rhs)
{
   std::swap(*this, rhs);
}

//#endif
}; // namespace custom

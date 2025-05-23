/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Cesar Tavarez, Ryan Whiteheads, Roy G
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:

   //
   // Construct
   //

   Node()
   {
      pPrev = pNext = nullptr;
   }
   Node(const T &  data) : data(data)
   {
      pPrev = pNext = nullptr;
   }
   Node(      T && data) : data(data)
   {
      pPrev = pNext = nullptr;
   }

   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource) 
{
   if (!pSource)
      return nullptr;

   Node<T>* pNewHead = new Node<T>(pSource->data);
   Node<T>* pNewPrev = pNewHead;

   const Node<T>* temp = pSource->pNext;

   while(temp)
   {
      Node<T>* pNew = new Node<T>(temp->data);
      pNewPrev->pNext = pNew;
      pNew->pPrev = pNewPrev;

      pNewPrev = pNew;
      temp = temp->pNext;
   }

   return pNewHead;
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   /*if (!pSource)
      */

   Node<T>* pDestTemp = pDestination;
   Node<T>* pDestPrev = nullptr;
   const Node<T>* pSourceTemp = pSource;

   // copy until we hit the end of source or dest lists
   while (pSourceTemp && pDestTemp)
   {
      pDestTemp->data = pSourceTemp->data;

      // ierate
      pDestPrev = pDestTemp;
      pDestTemp = pDestTemp->pNext;
      pSourceTemp = pSourceTemp->pNext;
   }

   // copy the rest of source
   while (pSourceTemp)
   {
      Node<T>* pNewNode = new Node<T>(pSourceTemp->data);
      if (pDestPrev)
      {
         pDestPrev->pNext = pNewNode;
         pNewNode->pPrev = pDestPrev;
      }
      else
      {
         pDestination = pNewNode;
      }

      pDestPrev = pNewNode;
      pSourceTemp = pSourceTemp->pNext;
   }

   // delete extra destination nodes
   if (pSource)
   {
      while (pDestTemp)
      {
         Node<T>* pTemp = pDestTemp;
         pDestTemp = pDestTemp->pNext;
         delete pTemp;
      }
   }
   else {
      // if source is null delete nodes and set head to nullptr
      while (pDestTemp)
      {
         Node<T>* pTemp = pDestTemp;
         pDestTemp = pDestTemp->pNext;
         delete pTemp;
      }
      pDestination = nullptr;
   }

   // end the list
   if (pDestPrev)
      pDestPrev->pNext = nullptr;

}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
    std::swap(pLHS, pRHS);
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
   if (!pRemove) return nullptr;

   // head
   if (!pRemove->pPrev)
   {
      Node<T>* temp = pRemove->pNext;
      pRemove->pNext->pPrev = nullptr;
      delete pRemove;
      return temp;
   }

   // tail
   if (!pRemove->pNext)
   {
      Node<T>* temp = pRemove->pPrev;
      pRemove->pPrev->pNext = nullptr;
      delete pRemove;
      return temp;
   }
   Node<T>* temp = pRemove->pPrev;
   pRemove->pPrev->pNext = pRemove->pNext;
   pRemove->pNext->pPrev = pRemove->pPrev;
   delete pRemove;
   return temp;
}

/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
   if (!pCurrent) return new Node<T>(t);
   Node<T>* pNew = new Node<T>(t);
   if (after)
   {
      if (pCurrent->pNext) {
         // set pNew next and prev
         pNew->pNext = pCurrent->pNext;
         pNew->pPrev = pCurrent;

         // set pNew->pNext->pPrev
         pCurrent->pNext->pPrev = pNew;
         pCurrent->pNext = pNew;

      }
      else
      {
         // just add pNew onto the end
         pNew->pNext = nullptr;
         pCurrent->pNext = pNew;
         pNew->pPrev = pCurrent;
      }
   }
   else
   {
      if (pCurrent->pPrev) {
         // set pNew next and prev
         pNew->pPrev = pCurrent->pPrev;
         pNew->pNext = pCurrent;

         // set pNew->pNext->pPrev
         pCurrent->pPrev->pNext = pNew;
         pCurrent->pPrev = pNew;

      }
      else
      {
         // just add pNew onto the end
         pNew->pPrev = nullptr;
         pNew->pNext = pCurrent;
         pCurrent->pPrev = pNew;
      }
   }


   return pNew;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
    if (pHead == NULL) {
        return 0;
    }
    size_t size = 1;
    const Node<T>* temp = pHead->pNext;
    while (temp)
    {
       size++;
       temp = temp->pNext;
    }
    return size;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   Node <T>* temp = pHead->pNext;
   while (temp)
   {
      if (temp->pNext)
      {   
         out << temp->data << "->";
      }
      else
      {
         out << temp->data << std::endl;
      }
      temp = temp->pNext;
   }
   return out;
}

/*****************************************************
 * CLEAR
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
    while (pHead)
    {
       const Node<T>* temp = pHead;
       pHead = pHead->pNext;
       delete temp;
    }
    pHead = nullptr;
}



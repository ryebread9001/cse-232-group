/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Cesar Tavarez, Ryan Whitehead, Roy Garcia
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair
#include <iostream>
#include <string>

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

   template <class TT>
   class set;
   template <class KK, class VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return root == nullptr; }
   size_t size()  const noexcept {
      if (root == nullptr)
      {
         return 0;
      }
      else
      {
         return root->size();
      }
   }
   

private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
   void clear(BNode* node) noexcept;
   BNode* copyRecursive(BNode* src, BNode* parent);
   void assign(BNode*& pDest, BNode* pSrc);
   void clearRecursive(BNode* pNode);
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr)
   {
      //pLeft = pRight = this;
   }
   BNode(const T &  t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr)
   {
      //pLeft = pRight = this; 
   }
   BNode(T && t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr)
   {  
      //pLeft = pRight = this;
   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return true; }
   bool isLeftChild( BNode * pNode) const { return true; }
   size_t size() {
      if (pLeft == nullptr && pRight == nullptr)
      {
         return 1;
      }
      else if (pLeft == nullptr || pRight == nullptr)
      {
         if (pLeft) return 1 + pLeft->size();
         if (pRight) return 1 + pRight->size();
      }
      else if (pLeft && pRight)
      {
         return pLeft->size() + 1 + pRight->size();
      }
   }

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff


};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set; 
public:
   // constructors and assignment
   iterator(BNode* p = nullptr) : pNode(p) {
      //if (this->pNode == nullptr) {
      //   std::cout << "Begin iterator is null!" << std::endl;
      //}
      //else {
      //   std::cout << "Begin iterator points to node"  << std::endl;
      //}
   }
   iterator(const iterator& rhs) 
   {
      this->pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
      this->pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return true;
   }
   bool operator != (const iterator & rhs) const
   {
      return true;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      return this->pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      return *this;;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST()
{
    root = nullptr;
    numElements = 0;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) 
{
    root = nullptr;
    numElements = 0;
    *this = rhs;
}

/*********************************************
 * BST :: COPY RECURSIVE CONSTRUCTOR
 * Copy one tree to another
 ********************************************/

template <typename T>
typename BST<T>::BNode* BST<T>::copyRecursive(BNode* src, BNode* parent)
{
    if (!src || src == src->pLeft) return nullptr;  // skip dummy

    BNode* newNode = new BNode(src->data);
    newNode->pParent = parent;
    newNode->pLeft = copyRecursive(src->pLeft, newNode);
    newNode->pRight = copyRecursive(src->pRight, newNode);
    return newNode;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) 
{
    root = rhs.root;
    numElements = rhs.numElements;

    rhs.root = nullptr;
    rhs.numElements = 0;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il)
{
    root = nullptr;
    numElements = 0;
    for (const T& t : il)
        insert(t);
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
    clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
    // Handle self-assignment
    if (this == &rhs)
        return *this;

    // Call the helper function to handle assignment
    assign(root, rhs.root);
    numElements = rhs.numElements;

    return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
    clear();
    for (const T& t : il)
        insert(t);
    return *this;
}

/*********************************************
    * BST :: ASSIGN
    * Assign nodes from source tree to destination tree
    ********************************************/
template <typename T>
void BST<T>::assign(BNode*& pDest, BNode* pSrc)
{
    // If both are null, nothing to do
    if (pSrc == nullptr && pDest == nullptr)
        return;

    // If source is null but destination exists, delete destination
    if (pSrc == nullptr)
    {
        clear(pDest);
        pDest = nullptr;
        return;
    }

    // If destination is null but source exists, create new node
    if (pDest == nullptr)
    {
        pDest = new BNode(pSrc->data);
        pDest->isRed = pSrc->isRed;
        pDest->pLeft = nullptr;
        pDest->pRight = nullptr;
    }
    // Both nodes exist, copy the data
    else
        pDest->data = pSrc->data;

    // Recursively handle left and right subtrees
    assign(pDest->pLeft, pSrc->pLeft);
    assign(pDest->pRight, pSrc->pRight);

    // Fix parent pointers
    if (pDest->pLeft)
        pDest->pLeft->pParent = pDest;
    if (pDest->pRight)
        pDest->pRight->pParent = pDest;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
    // Handle self-assignment
    if (this != &rhs)
    {
        // Clear current contents (O(n) operation)
        clear();

        // Swap with rhs (O(1) operation)
        swap(rhs);
    }
    return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
    // Swap root pointers
    BNode* tempRoot = rhs.root;
    rhs.root = this->root;
    this->root = tempRoot;

    // Swap number of elements
    size_t tempElements = rhs.numElements;
    rhs.numElements = this->numElements;
    this->numElements = tempElements;
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   // If the tree is empty
  if (!root)
   {
      root = new BNode(t);
      numElements++;
      return { iterator(root), true };
   }
   BNode* pCurrent = root;
   BNode* pParent = nullptr;
   bool isleft = false;

   while (pCurrent)
   {
      pParent = pCurrent;
      if (keepUnique && t == pCurrent->data)
         return { iterator(pCurrent), false };
      if (t < pCurrent->data)
      {
         pCurrent = pCurrent->pLeft;
         isleft = true;
      }
      else
      {
         pCurrent = pCurrent->pRight;
         isleft = false;
      }
   }


   BNode* pNew = new BNode(t);

   pNew->pParent = pParent;

   if (isleft)
      pParent->pLeft = pNew;
   else
      pParent->pRight = pNew;

   numElements++;


   return { iterator(pNew), true };
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   // if the tree is empty*
   if (!root)
   {
      root = new BNode(std::move(t));
      numElements++;
      return { iterator(root), true };
   }
   BNode* pCurrent = root;
   BNode* pParent = nullptr;
   bool isleft;

   while (pCurrent)
   {
      pParent = pCurrent;
      if (keepUnique && t == pCurrent->data)
         return { iterator(pCurrent), false };
      if (t < pCurrent->data)
      {
         pCurrent = pCurrent->pLeft;
         isleft = true;
      }
      else
      {
         pCurrent = pCurrent->pRight;
         isleft = false;
      }
   }


   BNode* pNew = new BNode(std::move(t));

   pNew->pParent = pParent;

   if (isleft)
      pParent->pLeft = pNew;
   else
      pParent->pRight = pNew;

   numElements++;


   return { iterator(pNew), true };
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator& it)
{
   BNode* eraseNode = it.pNode;
   if (eraseNode == nullptr)
      return end();

   // Compute the successor before deletion
   iterator next(eraseNode);
   ++next;

   // Case 1: No children
   if (eraseNode->pLeft == nullptr && eraseNode->pRight == nullptr)
   {
      if (eraseNode->pParent)
      {
         if (eraseNode->pParent->pLeft == eraseNode)
            eraseNode->pParent->pLeft = nullptr;
         else
            eraseNode->pParent->pRight = nullptr;
      }
      else
      {
         root = nullptr; // deleting the root
      }

      delete eraseNode;
      numElements--;
      return next;
   }

   // Case 2: One child
   if (eraseNode->pLeft == nullptr || eraseNode->pRight == nullptr)
   {
      BNode* child = (eraseNode->pLeft) ? eraseNode->pLeft : eraseNode->pRight;

      if (eraseNode->pParent)
      {
         if (eraseNode->pParent->pLeft == eraseNode)
            eraseNode->pParent->pLeft = child;
         else
            eraseNode->pParent->pRight = child;
      }
      else
      {
         root = child;
      }

      child->pParent = eraseNode->pParent;

      delete eraseNode;
      numElements--;
      return next;
   }

   // Case 3: Two children
   BNode* successor = eraseNode->pRight;
   BNode* successorParent = eraseNode;

   while (successor->pLeft)
   {
      successorParent = successor;
      successor = successor->pLeft;
   }

   if (successorParent != eraseNode)
   {
      successorParent->pLeft = successor->pRight;
      if (successor->pRight)
         successor->pRight->pParent = successorParent;

      successor->pRight = eraseNode->pRight;
      if (successor->pRight)
         successor->pRight->pParent = successor;
   }

   successor->pLeft = eraseNode->pLeft;
   if (successor->pLeft)
      successor->pLeft->pParent = successor;

   successor->pParent = eraseNode->pParent;

   if (eraseNode->pParent)
   {
      if (eraseNode->pParent->pLeft == eraseNode)
         eraseNode->pParent->pLeft = successor;
      else
         eraseNode->pParent->pRight = successor;
   }
   else
   {
      root = successor;
   }

   delete eraseNode;
   numElements--;

   return next;
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
    clearRecursive(root);
    root = nullptr;
    numElements = 0;
}

template <typename T>
void BST <T> ::clear(BNode* node) noexcept
{
    if (node == nullptr)
        return;

    // Post-order traversal: first clear left and right subtrees
    clear(node->pLeft);
    clear(node->pRight);

    // Then delete the current node
    delete node;
}

/*********************************************
 * BST :: CLEAR RECURSIVE HELPER
 ********************************************/
template <typename T>
void BST <T> ::clearRecursive(BNode* pNode)
{
    if (pNode == nullptr)
        return;

    clearRecursive(pNode->pLeft);
    clearRecursive(pNode->pRight);
    delete pNode;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   BNode* current = root;
   if (current == nullptr) return iterator(nullptr);

   while (current->pLeft != nullptr) {
      current = current->pLeft;
   }
   return iterator(current);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   BNode* current = root;
   while (current) {
      if (t == current->data) 
      {
         return iterator(current);
      }
      if (t < current->data)
      {
         current = current->pLeft;
      }
      else
      {
         current = current->pRight;
      }
   }
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
    pLeft = pNode;
    if (pNode)
        pNode->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
    pRight = pNode;
    if (pNode)
        pNode->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
    BNode* newNode = new BNode(t);
    addLeft(newNode);
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
    BNode* newNode = new BNode(std::move(t));
    addLeft(newNode);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
    BNode* newNode = new BNode(t);
    addRight(newNode);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
    BNode* newNode = new BNode(std::move(t));
    addRight(newNode);
}



/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
    if (!pNode)
        return *this;

    // Have a right child, go right then left
    if (pNode->pRight)
    {
        pNode = pNode->pRight;
        while (pNode->pLeft)
            pNode = pNode->pLeft;
        return *this;
    }

    // Get the parent for later 
    BNode* parent = pNode->pParent;

    // No right child and are our parent's left child, move to parent
    if (parent && parent->pLeft == pNode)
    {
        pNode = parent;
        return *this;
    }

    // No right child and we are our parent's right child. go up until we're not right
    while (parent && parent->pRight == pNode)
    {
        pNode = parent;
        parent = pNode->pParent;
    }
    pNode = parent;
    return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
    if (!pNode)
        return *this;

    // Have a right child, go right then left
    if (pNode->pLeft)
    {
        pNode = pNode->pLeft;
        while (pNode->pRight)
            pNode = pNode->pRight;
        return *this;
    }

    // Get the parent for later
    BNode* parent = pNode->pParent;

    // No right child and are our parent's left child, move to parent
    if (parent && parent->pRight == pNode)
    {
        pNode = parent;
        return *this;
    }

    // No right child and we are our parent's right child. go up until we're not right
    while (parent && parent->pLeft == pNode)
    {
        pNode = parent;
        parent = pNode->pParent;
    }
    pNode = parent;
    return *this;

}


} // namespace custom



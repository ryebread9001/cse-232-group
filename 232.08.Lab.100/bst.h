#pragma once

#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif

#include <cassert>
#include <utility>
#include <memory>
#include <functional>
#include <initializer_list>
#include <iostream>

class TestBST;
class TestMap;
class TestSet;

namespace custom
{

   template <class TT> class set;
   template <class KK, class VV> class map;

   template <typename T>
   class BST
   {
      friend class ::TestBST;
      friend class ::TestMap;
      friend class ::TestSet;
      template <class KK, class VV> friend class map;
      template <class TT> friend class set;
      template <class KK, class VV> friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);

   public:
      BST();
      BST(const BST& rhs);
      BST(BST&& rhs);
      BST(const std::initializer_list<T>& il);
      ~BST();

      BST& operator=(const BST& rhs);
      BST& operator=(BST&& rhs);
      BST& operator=(const std::initializer_list<T>& il);

      void swap(BST& rhs);

      class iterator;
      iterator begin() const noexcept;
      iterator end() const noexcept { return iterator(nullptr); }

      iterator find(const T& t);
      std::pair<iterator, bool> insert(const T& t, bool keepUnique = false);
      std::pair<iterator, bool> insert(T&& t, bool keepUnique = false);
      iterator erase(iterator& it);
      void clear() noexcept;

      bool empty() const noexcept { return root == nullptr; }
      size_t size() const noexcept { return root ? root->size() : 0; }

   private:
      class BNode;
      BNode* root;
      size_t numElements;

      void clear(BNode* node) noexcept;
      void clearRecursive(BNode* pNode);
      BNode* copyRecursive(BNode* src, BNode* parent);
      void assign(BNode*& pDest, BNode* pSrc);
   };

   /******************************************************
    * BST :: BNode
    ******************************************************/
   template <typename T>
   class BST<T>::BNode
   {
   public:
      BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}
      BNode(const T& t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}
      BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}

      void addLeft(BNode* pNode) { pLeft = pNode; if (pNode) pNode->pParent = this; }
      void addRight(BNode* pNode) { pRight = pNode; if (pNode) pNode->pParent = this; }
      void addLeft(const T& t) { addLeft(new BNode(t)); }
      void addLeft(T&& t) { addLeft(new BNode(std::move(t))); }
      void addRight(const T& t) { addRight(new BNode(t)); }
      void addRight(T&& t) { addRight(new BNode(std::move(t))); }

      size_t size()
      {
         if (!pLeft && !pRight) return 1;
         if (!pLeft) return 1 + pRight->size();
         if (!pRight) return 1 + pLeft->size();
         return 1 + pLeft->size() + pRight->size();
      }

      T data;
      BNode* pLeft;
      BNode* pRight;
      BNode* pParent;
      bool isRed;
   };

   /******************************************************
    * BST :: iterator
    ******************************************************/
   template <typename T>
   class BST<T>::iterator
   {
      friend class ::TestBST;
      friend class ::TestMap;
      friend class ::TestSet;
      template <class KK, class VV> friend class map;
      template <class TT> friend class set;
      friend BST<T>::iterator BST<T>::erase(iterator& it);

   public:
      iterator(BNode* p = nullptr) : pNode(p) {}
      iterator(const iterator& rhs) : pNode(rhs.pNode) {}
      iterator& operator=(const iterator& rhs) { pNode = rhs.pNode; return *this; }

      bool operator==(const iterator& rhs) const { return pNode == rhs.pNode; }
      bool operator!=(const iterator& rhs) const { return pNode != rhs.pNode; }

      const T& operator*() const { assert(pNode); return pNode->data; }

      iterator& operator++();
      iterator operator++(int) { iterator tmp(*this); ++(*this); return tmp; }
      iterator& operator--();
      iterator operator--(int) { iterator tmp(*this); --(*this); return tmp; }

   private:
      BNode* pNode;
   };

   /******************************************************
    * BST :: CONSTRUCTORS / DESTRUCTOR
    ******************************************************/
   template <typename T>
   BST<T>::BST() : root(nullptr), numElements(0) {}

   template <typename T>
   BST<T>::BST(const BST& rhs) : root(nullptr), numElements(0) { *this = rhs; }

   template <typename T>
   BST<T>::BST(BST&& rhs) : root(rhs.root), numElements(rhs.numElements)
   {
      rhs.root = nullptr;
      rhs.numElements = 0;
   }

   template <typename T>
   BST<T>::BST(const std::initializer_list<T>& il) : root(nullptr), numElements(0)
   {
      for (const T& t : il) insert(t);
   }

   template <typename T>
   BST<T>::~BST() { clear(); }

   /******************************************************
    * BST :: ASSIGNMENT OPERATORS
    ******************************************************/
   template <typename T>
   BST<T>& BST<T>::operator=(const BST& rhs)
   {
      if (this != &rhs)
      {
         clear();
         assign(root, rhs.root);
         numElements = rhs.numElements;
      }
      return *this;
   }

   template <typename T>
   BST<T>& BST<T>::operator=(BST&& rhs)
   {
      if (this != &rhs)
      {
         clear();
         swap(rhs);
      }
      return *this;
   }

   template <typename T>
   BST<T>& BST<T>::operator=(const std::initializer_list<T>& il)
   {
      clear();
      for (const T& t : il) insert(t);
      return *this;
   }

   /******************************************************
    * BST :: UTILITY FUNCTIONS
    ******************************************************/
   template <typename T>
   void BST<T>::swap(BST& rhs)
   {
      std::swap(root, rhs.root);
      std::swap(numElements, rhs.numElements);
   }

   template <typename T>
   void BST<T>::clear() noexcept
   {
      clearRecursive(root);
      root = nullptr;
      numElements = 0;
   }

   template <typename T>
   void BST<T>::clear(BNode* node) noexcept
   {
      if (!node) return;
      clear(node->pLeft);
      clear(node->pRight);
      delete node;
   }

   template <typename T>
   void BST<T>::clearRecursive(BNode* pNode)
   {
      if (!pNode) return;
      clearRecursive(pNode->pLeft);
      clearRecursive(pNode->pRight);
      delete pNode;
   }

   template <typename T>
   typename BST<T>::BNode* BST<T>::copyRecursive(BNode* src, BNode* parent)
   {
      if (!src) return nullptr;
      BNode* newNode = new BNode(src->data);
      newNode->pParent = parent;
      newNode->pLeft = copyRecursive(src->pLeft, newNode);
      newNode->pRight = copyRecursive(src->pRight, newNode);
      return newNode;
   }

   template <typename T>
   void BST<T>::assign(BNode*& pDest, BNode* pSrc)
   {
      if (!pSrc && !pDest) return;

      if (!pSrc)
      {
         clear(pDest);
         pDest = nullptr;
         return;
      }

      if (!pDest)
      {
         pDest = new BNode(pSrc->data);
         pDest->isRed = pSrc->isRed;
      }
      else
      {
         pDest->data = pSrc->data;
      }

      assign(pDest->pLeft, pSrc->pLeft);
      assign(pDest->pRight, pSrc->pRight);

      if (pDest->pLeft) pDest->pLeft->pParent = pDest;
      if (pDest->pRight) pDest->pRight->pParent = pDest;
   }

   /******************************************************
    * BST :: INSERT / FIND / ERASE
    ******************************************************/
   template <typename T>
   std::pair<typename BST<T>::iterator, bool> BST<T>::insert(const T& t, bool keepUnique)
   {
      if (!root)
      {
         root = new BNode(t);
         numElements++;
         return { iterator(root), true };
      }

      BNode* pCurrent = root;
      BNode* pParent = nullptr;
      bool isLeft = false;

      while (pCurrent)
      {
         pParent = pCurrent;
         if (keepUnique && t == pCurrent->data)
            return { iterator(pCurrent), false };
         if (t < pCurrent->data)
         {
            pCurrent = pCurrent->pLeft;
            isLeft = true;
         }
         else
         {
            pCurrent = pCurrent->pRight;
            isLeft = false;
         }
      }

      BNode* pNew = new BNode(t);
      pNew->pParent = pParent;
      if (isLeft)
         pParent->pLeft = pNew;
      else
         pParent->pRight = pNew;

      numElements++;
      return { iterator(pNew), true };
   }

   template <typename T>
   std::pair<typename BST<T>::iterator, bool> BST<T>::insert(T&& t, bool keepUnique)
   {
      if (!root)
      {
         root = new BNode(std::move(t));
         numElements++;
         return { iterator(root), true };
      }

      BNode* pCurrent = root;
      BNode* pParent = nullptr;
      bool isLeft;

      while (pCurrent)
      {
         pParent = pCurrent;
         if (keepUnique && t == pCurrent->data)
            return { iterator(pCurrent), false };
         if (t < pCurrent->data)
         {
            pCurrent = pCurrent->pLeft;
            isLeft = true;
         }
         else
         {
            pCurrent = pCurrent->pRight;
            isLeft = false;
         }
      }

      BNode* pNew = new BNode(std::move(t));
      pNew->pParent = pParent;
      if (isLeft)
         pParent->pLeft = pNew;
      else
         pParent->pRight = pNew;

      numElements++;
      return { iterator(pNew), true };
   }

   template <typename T>
   typename BST<T>::iterator BST<T>::find(const T& t)
   {
      BNode* current = root;
      while (current)
      {
         if (t == current->data)
            return iterator(current);
         current = (t < current->data) ? current->pLeft : current->pRight;
      }
      return end();
   }

   template <typename T>
   typename BST<T>::iterator BST<T>::erase(iterator& it)
   {
      BNode* eraseNode = it.pNode;
      if (!eraseNode) return end();

      // Case 1: No children
      if (!eraseNode->pLeft && !eraseNode->pRight)
      {
         if (eraseNode->pParent)
         {
            if (eraseNode->pParent->pLeft == eraseNode)
               eraseNode->pParent->pLeft = nullptr;
            else
               eraseNode->pParent->pRight = nullptr;
         }
         else root = nullptr;

         delete eraseNode;
         numElements--;
         return end();
      }

      // Case 2: One child
      if (!eraseNode->pLeft || !eraseNode->pRight)
      {
         BNode* child = eraseNode->pLeft ? eraseNode->pLeft : eraseNode->pRight;
         if (eraseNode->pParent)
         {
            if (eraseNode->pParent->pLeft == eraseNode)
               eraseNode->pParent->pLeft = child;
            else
               eraseNode->pParent->pRight = child;
         }
         else root = child;

         child->pParent = eraseNode->pParent;
         delete eraseNode;
         numElements--;
         return end();
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
         if (successor->pRight) successor->pRight->pParent = successorParent;
         successor->pRight = eraseNode->pRight;
         if (successor->pRight) successor->pRight->pParent = successor;
      }

      successor->pLeft = eraseNode->pLeft;
      if (successor->pLeft) successor->pLeft->pParent = successor;

      successor->pParent = eraseNode->pParent;
      if (eraseNode->pParent)
      {
         if (eraseNode->pParent->pLeft == eraseNode)
            eraseNode->pParent->pLeft = successor;
         else
            eraseNode->pParent->pRight = successor;
      }
      else
         root = successor;

      delete eraseNode;
      numElements--;
      return iterator(successor);
   }

   /******************************************************
    * BST :: BEGIN
    ******************************************************/
   template <typename T>
   typename BST<T>::iterator BST<T>::begin() const noexcept
   {
      BNode* current = root;
      while (current && current->pLeft)
         current = current->pLeft;
      return iterator(current);
   }

   /******************************************************
    * BST :: ITERATOR OPERATORS
    ******************************************************/
   template <typename T>
   typename BST<T>::iterator& BST<T>::iterator::operator++()
   {
      if (!pNode) return *this;
      if (pNode->pRight)
      {
         pNode = pNode->pRight;
         while (pNode->pLeft)
            pNode = pNode->pLeft;
         return *this;
      }

      BNode* parent = pNode->pParent;
      while (parent && parent->pRight == pNode)
      {
         pNode = parent;
         parent = parent->pParent;
      }

      pNode = parent;
      return *this;
   }

   template <typename T>
   typename BST<T>::iterator& BST<T>::iterator::operator--()
   {
      if (!pNode) return *this;
      if (pNode->pLeft)
      {
         pNode = pNode->pLeft;
         while (pNode->pRight)
            pNode = pNode->pRight;
         return *this;
      }

      BNode* parent = pNode->pParent;
      while (parent && parent->pLeft == pNode)
      {
         pNode = parent;
         parent = parent->pParent;
      }

      pNode = parent;
      return *this;
   }

} // namespace custom

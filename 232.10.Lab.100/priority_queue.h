/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    Cesar Tavarez
 ************************************************************************/


#pragma once

#include "vector.h" // custom vector implementation

namespace custom
{
   template <class T>
   class priority_queue
   {
   public:
      // Underlying container
      custom::vector<T> container;

      //
      // Constructors
      //
      priority_queue() = default;

      // Copy constructor
      priority_queue(const priority_queue& rhs) : container(rhs.container) {}

      // Move constructor
      priority_queue(priority_queue&& rhs) noexcept
         : container(std::move(rhs.container)) {}

      // Range constructor
      template <class Iterator>
      priority_queue(Iterator first, Iterator last)
      {
         while (first != last)
            container.push_back(*first++);
         heapify();
      }

      // Move-initialize constructor
      priority_queue(custom::vector<T>&& v)
         : container(std::move(v))
      {
         heapify();
      }

      //
      // Assignment / Swap
      //
      void swap(priority_queue& rhs)
      {
         container.swap(rhs.container);
      }

      //
      // Top
      //
      T top() const
      {
         if (container.empty())
            throw std::out_of_range("std:out_of_range");
         return container[0];
      }

      //
      // Size / Empty
      //
      size_t size() const { return container.size(); }
      bool empty() const { return container.empty(); }

      //
      // Push (copy)
      //
      void push(const T& t)
      {
         container.push_back(t);
         percolateUp(container.size() - 1);
      }

      // Push (move)
      void push(T&& t)
      {
         container.push_back(std::move(t));
         percolateUp(container.size() - 1);
      }

      //
      // Pop
      //
      void pop()
      {
         if (container.empty())
            return;

         container[0] = container.back();
         container.pop_back();
         if (!container.empty())
            percolateDown(0);
      }

      //
      // Heapify
      //
      void heapify()
      {
         for (int i = (container.size() - 2) / 2; i >= 0; --i)
            percolateDown(i);
      }

      //
      // Percolate Down
      //
      bool percolateDown(size_t index)
      {
         size_t parent = index;
         size_t left = 2 * index + 1;
         size_t right = 2 * index + 2;
         size_t largest = parent;

         if (left < container.size() && container[left] > container[largest])
            largest = left;

         if (right < container.size() && container[right] > container[largest])
            largest = right;

         if (largest != parent)
         {
            std::swap(container[parent], container[largest]);
            percolateDown(largest);
            return true;
         }

         return false;
      }

   private:
      //
      // Percolate Up (for push)
      //
      void percolateUp(size_t index)
      {
         while (index > 0)
         {
            size_t parent = (index - 1) / 2;
            if (container[parent] >= container[index])
               break;
            std::swap(container[parent], container[index]);
            index = parent;
         }
      }
   };

   //
   // Swap (non-member)
   //
   template <class T>
   void swap(priority_queue<T>& lhs, priority_queue<T>& rhs)
   {
      lhs.swap(rhs);
   }

} // namespace custom

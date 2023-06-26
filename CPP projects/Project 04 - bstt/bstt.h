/*bstt.h*/
/*
  * Program: Binary search threaded tree (project 04)
  * Course: UIC CS 251
  * Programmer: Tony Lau
  * Professor: Reckinger
  * 
  *  Threaded binary search tree
  *  A binary search tree where if the node is threaded the Right will point to the next largest nodes
  *  The algorithm to determine whether a node will be threaded is done during insertion where when a
  *  node is inserted left it is threaded and its Right will point to its parent
  *  when is inserted right it is only threaded if its parent is and will inherit the thread of its parent
  *  This allows the tree to be traversed in key order without recursion
  * 
  *  The Root node is threaded to nullptr
  * 
  *  EXAMPLE DECLARTION: bstt<int, int>  nameOfTree;
  */

#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded; // true => Right is a thread, false => non-threaded
  };
  
  //Recursive function that copies from another tree to this one using inorder traversal
  void _copyTo(NODE* cur)
  {
      if(cur == nullptr)
          return;
      else
      {
          insert(cur->Key, cur->Value);
          if(!cur->isThreaded)
              _copyTo(cur->Right);
          _copyTo(cur->Left);
      }
  }
  
  //Recursive function that deletes all nodes using a postorder traversal.
  void _destructor(NODE* cur)
  {  
      if(cur == nullptr)
          return;
      else
      {
          _destructor(cur->Left);
          if(!cur->isThreaded)//isThreaded is treated the same as a nullptr
              _destructor(cur->Right);
          delete(cur);
      }
  }
  
  void _printInorder(NODE* cur, ostream& output)const
  {
      if(cur == nullptr)
          return;
      else
      {
          _printInorder(cur->Left, output);
          if(cur->isThreaded &&cur->Right!=nullptr)//the second condition to keep the max node from printing as threaded
              output << '(' << cur->Key<< ',' << cur->Value << ',' << (cur->Right)->Key << ')' <<endl;
          else
          {
              output << '(' << cur->Key<< ',' << cur->Value << ')' <<endl;
              _printInorder(cur->Right, output);
          }
          
      }
  }
  
  

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* state;  // internal state that denotes where in key order traversal the class is


public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
    Size = 0;
  }

  //
  // copy constructor
  //
  bstt(const bstt& other)
  {
      Size = 0;
      Root = nullptr;
      _copyTo(other.Root);
      
  }

	//
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt()
  {
      _destructor(Root);
      Root = nullptr;
  }

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
      clear();
      _copyTo(other.Root);
      return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
      _destructor(Root);
      Root = nullptr;
      Size = 0;
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
      NODE* cur = Root;

        //iterate depthwise through the tree
        while (cur != nullptr)
        {
            if (key == cur->Key) //found
            {
                value = cur->Value;
                return true;
            }
            if (key < cur->Key)  // search left:
            {
                cur = cur->Left;
            }
            else//search right
            {
                if(cur->isThreaded)//threaded right is counted as nullptr
                    cur = nullptr;
                else
                    cur = cur->Right;
            }

        }//end of while
        //not found
        return false;
  }


  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  // Threads are also assigned in this function
  // Time complexity:  O(lgN) on average
  //
  void insert(KeyT key, ValueT value)
  {
      NODE* prev = nullptr;
      NODE* cur = Root;
      //depthwise traversal of the tree to locate the parent of the 
      //insert node and store the parent into prev
      while(cur != nullptr)
      {
          if(cur->Key == key)
              return;
          else if(key < cur->Key)
          {
              prev = cur;
              cur = cur ->Left;
          }
          else
          {
              if(cur->isThreaded)
              {
                  prev = cur;
                  cur = nullptr;
              }
              else
              {
                  prev = cur;
                  cur = cur->Right;
              }
          }
      }//end of while
      
      //create a new node to insert
      NODE* temp = new NODE;
      temp->Key = key;
      temp->Value = value;
      temp->Right = nullptr;
      temp->Left = nullptr;
      
      //determine if the new node will be inserted to the left or right of prev
      //then determine if it will inherit the parents thread if inserted to the right
      //or if it will have its own thread pointing to its parent when inserted left
      if(prev==nullptr)
      {
          Root = temp;
          temp->isThreaded = true;//the root is threaded to nullptr
      }
      else if (key < prev->Key)
      {
          temp->Right = prev;
          temp->isThreaded = true;
          prev->Left = temp;   
      }
      else
      {
          if(prev->isThreaded)
          {
              temp->Right = prev->Right;
              temp->isThreaded = true;
              prev->Right = temp;
              prev->isThreaded = false;
          }
          else
          {
              prev->Right = temp;
              temp->isThreaded = false;
          }
      }
      Size++;
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const
  {
      ValueT output;
      if(search(key, output))
          return output;
      else
          return ValueT{ };
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) const
  {
   NODE* cur = Root;
    //depthwise search for the key
    while (cur != nullptr)
    {
      if (key == cur->Key) //found
      {
          if(cur->Right != nullptr)
              return ((cur->Right)->Key);
          break;
      }
      if (key < cur->Key)  // search left:
      {
        cur = cur->Left;
      }
      else//search right
      {
          if(cur->isThreaded)//threaded is not counted
              cur = nullptr;
          else
              cur = cur->Right;
      }
    }//while  
  
    // if get here, not found
    return KeyT{ };
  }
  

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
      NODE* cur = Root;
      //traverse cur to the min
      if(cur!=nullptr)
      {
          while(cur->Left != nullptr)
          {
              cur = cur->Left;
          }
      }
      state = cur;
  }

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
    if(state == nullptr)//check if tree has been traversed
        return false;
    else
        key = state->Key;
    
    //presets the next state if the state is not nullptr
    if(state->isThreaded)
        state = state->Right;
    else//not threaded
    {
        if(state->Right != nullptr)//not the max key
        {
            NODE* cur = state->Right;
            //go to the farthest left leaf
            while(cur->Left != nullptr)
            {
                cur = cur->Left;
            }
            state = cur;
        }
        else
        {
            state = nullptr;
        }
    }
    return true;
  }

  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //
    _printInorder(Root, output);
    //
    // TODO
    //

    output << "**************************************************" << endl;
  }
	
};


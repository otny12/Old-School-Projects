/*avlt.h*/

//
// Threaded AVL tree
/*
  * Program: AVL threaded tree (project 05)
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
  * AVL Tree
  * each node stores its height and the tree will auto balance itself when it detects that the balance factor
  * of a node is greater than 2 or less than -2 by performing a rotation
  * 
  *  EXAMPLE DECLARTION: bstt<int, int>  nameOfTree;
  */


#pragma once

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

template<typename KeyT, typename ValueT>
class avlt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded; // true => Right is a thread, false => non-threaded
    int    Height;     // height of tree rooted at this node
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* state;  // internal state that denotes where in key order traversal the class is
  
  void _insertNoRotate(KeyT key, ValueT value)
  {
      NODE* prev = nullptr;
      NODE* cur = Root;
      //depthwise traversal of the tree to locate the parent of the 
      //insert node and store the parent into prev
      stack<NODE*> nodes;
      
      while(cur != nullptr)
      {
          if(cur->Key == key)
              return;
              
          nodes.push(cur);
          if(key < cur->Key)
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
      temp->Height = 0;
      
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
      
      //update the heigts
      while(!nodes.empty())
      {
          int HL, HR, HCur;
          cur = nodes.top();
          nodes.pop();
          if(cur->Left == nullptr)
              HL = -1;
          else
              HL = (cur->Left)->Height;

          if(cur->Right == nullptr || cur->isThreaded)
              HR = -1;
          else
              HR = (cur->Right)->Height;

          HCur = 1+max(HR, HL);

          if(HCur == cur->Height)
              break;
          else
          {
              cur->Height = HCur;
          }
      }
  }
  
  void _copyTo(NODE* cur)
  {
      if(cur == nullptr)
          return;
      else
      {
          _insertNoRotate(cur->Key, cur->Value);
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
  
  
  void _RightRotate(NODE* Parent, NODE* N)
  {
     NODE* L = N->Left;
     NODE* A = L->Left;
     NODE* B = nullptr;
     if(!L->isThreaded)
        B = L->Right;
     NODE* C = nullptr;
     if(!N->isThreaded)
         C = N->Right;
        
         
     L->Right = N;
     N->Left = B;
     
     if(Parent == nullptr)
      Root = L;
     else if(Parent->Left == N)
      Parent->Left= L;
     else
      Parent->Right =L;
      
      int HA, HB,HC;
      if(A == nullptr)
         HA = -1;
      else
         HA = A->Height;
       
       if(B == nullptr)
         HB = -1;
      else
         HB = B->Height;
          
      if(C == nullptr)
         HC = -1;
      else
         HC = C->Height;
         
      N->Height = 1+ max(HB, HC);
      L->Height = 1+ max(N->Height, HA);
      
      //update threading
      L->isThreaded = false;
      
  }
  
  void _LeftRotate(NODE* Parent, NODE* N)
  {
     
     NODE* R = N->Right;
     NODE* A = N->Left;
     NODE* B = R->Left;
     
     NODE* C = nullptr;
     if(!R->isThreaded)
         C = R->Right;
     
     R->Left = N;
     N->Right = B;
     
     if(Parent == nullptr)
      Root = R;
     else if(Parent->Left == N)
      Parent->Left= R;
     else
      Parent->Right =R;
      
      int HA, HB,HC;
      if(A == nullptr)
         HA = -1;
      else
         HA = A->Height;
       
       if(B == nullptr)
         HB = -1;
      else
         HB = B->Height;
          
      if(C == nullptr)
         HC = -1;
      else
         HC = C->Height;
         
      N->Height = 1+ max(HB, HA);
      R->Height = 1+ max(N->Height, HC);
      
      //update threads
      if(B==nullptr)
      {
      N->isThreaded = true;
      N->Right = R;
      }
  }
  
  int _balanceFactor(NODE* cur)
  {
      int HR, HL;
      if(cur->Right == nullptr || cur->isThreaded)
          HR = -1;
      else
          HR = (cur->Right)->Height;
          
      if(cur->Left == nullptr)
          HL = -1;
      else
          HL = (cur->Left)->Height;
      
      return (HL-HR);
  }
  // (key,value,height) if the node is not threaded OR thread==nullptr
    // (key,value,height,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //
void _printInorder(NODE* cur, ostream& output)const
{
  if(cur == nullptr)
      return;
  else
  {
      _printInorder(cur->Left, output);
      if(cur->isThreaded &&cur->Right!=nullptr)//the second condition to keep the max node from printing as threaded
          output << '(' << cur->Key<< ',' << cur->Value << ',' << (cur->Height) << "," << (cur->Right)->Key << ')' <<endl;
      else
      {
          output << '(' << cur->Key<< ',' << cur->Value <<","<< (cur->Height) << ')' <<endl;
          _printInorder(cur->Right, output);
      }

  }
}

NODE* _next(NODE* cur)
{
    if(cur->isThreaded)
        cur = cur->Right;
    else//not threaded
    {
        if(cur->Right != nullptr)//not the max key
        {
            NODE* next = cur->Right;
            //go to the farthest left leaf
            while(next->Left != nullptr)
            {
                next = next->Left;
            }
            cur = next;
        }
        else
        {
            cur = nullptr;
        }
    }
    return cur;
}
  

public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  avlt()
  {
    Root = nullptr;
    Size = 0;
  }

  //
  // copy constructor
  //
  // NOTE: makes an exact copy of the "other" tree, such that making the
  // copy requires no rotations.
  //
  avlt (const avlt& other)
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
  virtual ~avlt()
  {
    _destructor(Root);
    Root = nullptr;
  }

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  // NOTE: makes an exact copy of the "other" tree, such that making the
  // copy requires no rotations.
  //
  avlt& operator=(const avlt& other)
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
  // height:
  //
  // Returns the height of the tree, -1 if empty.
  //
  // Time complexity:  O(1) 
  //
  int height() const
  {
    if (Root == nullptr)
      return -1;
    else
      return Root->Height;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) worst-case
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
  // range_search
  //
  // Searches the tree for all keys in the range [lower..upper], inclusive.
  // It is assumed that lower <= upper.  The keys are returned in a vector;
  // if no keys are found, then the returned vector is empty.
  //
  // Time complexity: O(lgN + M), where M is the # of keys in the range
  // [lower..upper], inclusive.
  //
  // NOTE: do not simply traverse the entire tree and select the keys
  // that fall within the range.  That would be O(N), and thus invalid.
  // Be smarter, you have the technology.
  //
  vector<KeyT> range_search(KeyT lower, KeyT upper)
  {
    vector<KeyT>  keys;

    NODE* cur = Root;
    NODE* prev = nullptr;
    
    while(cur != nullptr)
    {
        if(cur->Key == lower)
        {
            prev = cur;
            cur = nullptr;
        }
        else if(cur->Key > lower)
        {
            prev = cur;
            cur = cur->Left;
        }
        else//(cur->key < lower)
        {
            if(cur->isThreaded)
            {
                prev = cur->Right;
                cur = nullptr;
            }
            else
            {
                prev = cur;
                cur = cur->Right;
            }
        }
    }
    
    //inorder traverse from lower to upper
    while(prev != nullptr)
    {
    if(prev->Key <= upper)
        keys.push_back(prev->Key);
    else
        break;
    prev = _next(prev);
    }
    return keys;
  }

  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.  Rotations are performed
  // as necessary to keep the tree balanced according to AVL definition.
  //
  // Time complexity:  O(lgN) worst-case
  //
  void insert(KeyT key, ValueT value)
  {
      NODE* prev = nullptr;
      NODE* cur = Root;
      //depthwise traversal of the tree to locate the parent of the 
      //insert node and store the parent into prev
      stack<NODE*> nodes;
      
      while(cur != nullptr)
      {
          if(cur->Key == key)
              return;
              
          nodes.push(cur);
          if(key < cur->Key)
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
      temp->Height = 0;
      
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
      
      //update the heigts
      while(!nodes.empty())
      {
          int HL, HR, HCur;
          int balanceFactor;
          
          cur = nodes.top();
          nodes.pop();
          if(cur->Left == nullptr)
              HL = -1;
          else
              HL = (cur->Left)->Height;

          if(cur->Right == nullptr || cur->isThreaded)
              HR = -1;
          else
              HR = (cur->Right)->Height;

          HCur = 1 + max(HR, HL);

          if(HCur == cur->Height)
              return;
          else
          {
              cur->Height = HCur;
              //determine the balanceFactor and rotate depending
              NODE* parent;
              balanceFactor = HL-HR;
              if(balanceFactor == 2)
              {
                  if(nodes.empty())
                      parent = nullptr;
                  else
                      parent = nodes.top();
                      
                  balanceFactor = _balanceFactor(cur->Left);
                  if(balanceFactor == 1)//case1
                      _RightRotate(parent, cur);
                  else//case2
                  {
                      _LeftRotate(cur,cur->Left);
                      _RightRotate(parent, cur);
                  } 
              }
              else if(balanceFactor == -2)
              {
                  if(nodes.empty())
                      parent = nullptr;
                  else
                      parent = nodes.top();
                      
                  balanceFactor = _balanceFactor(cur->Right);
                  if(balanceFactor == 1)//case3
                  {
                      _RightRotate(cur, cur->Right);
                      _LeftRotate(parent, cur);
                  }
                  else//case4
                      _LeftRotate(parent, cur);
              }
          }
      }//end while
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) worst-case
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
  // Time complexity:  O(lgN) worst-case
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
          return KeyT{ };
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
  // %
  //
  // Returns the height stored in the node that contains key; if key is
  // not found, -1 is returned.
  //
  // Example:  cout << tree%12345 << endl;
  //
  // Time complexity:  O(lgN) worst-case
  //
  int operator%(KeyT key) const
  {
    NODE* cur = Root;
    //depthwise search for the key
    while (cur != nullptr)
    {
      if (key == cur->Key) //found
      {
          return (cur->Height);
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
    }

    return -1;
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
  // Time complexity:  O(lgN) worst-case
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
  // Time complexity:  O(lgN) worst-case
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
    state = _next(state);
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
    output << "********************* AVLT ***********************" << endl;

    output << "** size: " << this->size() << endl;
    output << "** height: " << this->height() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value,height) or (key,value,height,THREAD)
    //
    // (key,value,height) if the node is not threaded OR thread==nullptr
    // (key,value,height,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //
    _printInorder(Root, output);

    output << "**************************************************" << endl;
  }	
};
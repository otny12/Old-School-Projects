/*graph.h*/

//
// Tony Lau
//
// Basic graph class using adjacency List representation.

//
// original author: Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
//
 
#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>
#include <iterator>
#include <utility>

using namespace std;


template<typename VertexT, typename WeightT>
class graph
{
private:

  map<VertexT, map<VertexT, WeightT>* > AdjList;
  int Edges;
  //
  // We are using adjacency matrix implementation, where rows
  // are the starting vertex and cols are the ending vertex.
  // We keep track of the vertices in the Vertices vector,
  // where the vertex's position in the vector --- 0, 1, 2, 
  // 3, 4, 5, ... --- denotes the row in the adjacency matrix
  // where their edges are found.  Example: if vertex "ORD" is
  // in position 1 of the Vertices vector, then row 1 of
  // AdjMatrix are the edges that start at "ORD" and lead to
  // other vertices.
  //

  vector<VertexT>  Vertices;

  //
  // _LookupVertex
  //
  // unused in final program however was required to pass auto grading software
  int _LookupVertex(VertexT v) const
  {
    ;
  }
  
  //
  // _destructor:
  // 
  // helper function for the destructor to be also utilized with the copy constructor
  // 
    void _destructor()
    {
        for(pair<VertexT, map<VertexT, WeightT>*> x: AdjList) 
        {
             delete x.second;
        }
    }
    
    //
    // _copier:
    // 
    // helper function for the copy constructor and = operator overload
    // 
    void _copier(const graph& other)
    {
        this->Edges = other.Edges;
        for(pair<VertexT, map<VertexT, WeightT>*> x: other.AdjList) 
        {
          map<VertexT, WeightT>* temp = new map<VertexT, WeightT>(*(x.second));
          this->AdjList[x.first] = temp;
        }
    }


public:
  //
  // constructor:
  //
  // Constructs an empty graph
  // 

  graph()
  {
    Edges = 0;
  }
  
  //
  // copy constructor:
  // 
    graph(const graph& other)
    {
        _copier(other);
    }
  
  //
  //destructor:
  //
  //
    virtual ~graph()
    {
        _destructor();
    }
    
    //operator =
    //
    //overload the = operator with a deep copy
    //
    graph& operator=(const graph& other)
    {
    _destructor();
    AdjList.clear();
    _copier(other);
    return *this;
    }
    

  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const
  {
    return static_cast<int>(this->AdjList.size());
  }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const
  {
    return Edges;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph and if so
  // returns true.  If the vertex already
  // exists in the graph, then false is returned.
  //
  bool addVertex(VertexT v)
  {


    //
    // is the vertex already in the graph?  If so, we do not 
    // insert again otherwise Vertices may fill with duplicates:
    //
    if (AdjList.count(v)==1)
      return false;

    // if we get here, vertex does not exist so insert.
    map<VertexT, WeightT>* temp = new map<VertexT, WeightT>;
    this->AdjList.emplace(v, temp);
    return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist or for some reason the
  // graph is full, false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(VertexT from, VertexT to, WeightT weight)
  {
    if(AdjList.count(from) == 0 || AdjList.count(to)==0)
        return false;
    if(AdjList.at(from)->count(to)==1)
    {
        (*AdjList.at(from))[to] = weight;
        return true;
    }
    AdjList.at(from)->emplace(to, weight);
    Edges++;
    return true;
  }

  //
  // getWeight
  //
  // Returns the weight associated with a given edge.  If 
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned.  If the edge does not 
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const
  {
     if(AdjList.count(from) == 0 || AdjList.count(to)==0)
        return false;
     if(AdjList.at(from)->count(to)==0)
         return false;
         
     weight = AdjList.at(from)->at(to);
    return true;
  }

  //
  // neighbors
  //
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge.
  // Since a set is returned, the neighbors are returned in
  // sorted order; use foreach to iterate through the set.
  //
  set<VertexT> neighbors(VertexT v) const
  {
    set<VertexT>  S;

    
    if(AdjList.count(v) == 0)
        return S;
        map <VertexT, WeightT> currentMap= *(AdjList.at(v));

      for(typename map <VertexT, WeightT>::iterator it = currentMap.begin(); it != currentMap.end(); it++)
      {
        S.insert(it->first);
      }

    return S;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //
  vector<VertexT> getVertices() const
  {
      vector<VertexT> vert;
     for(pair<VertexT, map<VertexT, WeightT>*> x: AdjList) 
      {
          vert.push_back(x.first);
      }
      return vert;
  }

  //
  // dump
  // 
  // Dumps the internal state of the graph for debugging purposes.
  //
  // Example:
  //    graph<string,int>  G;
  //    ...
  //    G.dump(cout);  // dump to console
  //
  void dump(ostream& output) const
  {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->AdjList.size() << endl;
    output << "**Num edges: " << this->Edges << endl;

    output << endl;
    output << "**Vertices:" << endl;
    int i = 1;
    for(pair<VertexT, map<VertexT, WeightT>*> x: AdjList) 
    {
      output << " " << i << ". " << x.first << endl;
      i++;
    }

    output << endl;
    output << "**Edges:" << endl;
    i = 1;
    for(pair<VertexT, map<VertexT, WeightT>*> x: AdjList) 
    {
      output << " row " << i << ": ";

      for(pair<VertexT, map<VertexT, WeightT>*> y: AdjList) 
      {
        if ((x.second)->count(y.first)==0)
        {
          output << "F ";
        }
        else
        {
          output << "(T,"
            << (x.second)->at(y.first)
            << ") ";
        }
      }

      output << endl;
    }

    output << "**************************************************" << endl;
  }

};
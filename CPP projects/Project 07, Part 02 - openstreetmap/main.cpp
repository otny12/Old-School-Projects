/*main.cpp*/

//
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #07: open street maps, graphs, and Dijkstra's alg
// 
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:  
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <limits>
#include <set>
#include <queue>
#include <stack>


#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;


class prioritize
{
 public:
 bool operator()(const pair<long long, double>& p1, const pair<long long,double>& p2) const
 {
    if(p1.second==p2.second)
      return p1.first > p2.first;
    return p1.second > p2.second;
 }
};

bool search(vector<long long> V, long long input)
{
 for(long long x:V)
 {
    if(input == x)
      return true;
 }
 return false;
}

vector<long long> Dijkstra(graph<long long,double>& G, 
                        long long startV, 
                        map<long long, double>& distances,
                        map<long long, long long>& predV)
    {
    const double INF = numeric_limits<double>::max();//Infinity
      vector<long long>  visited;
      priority_queue<pair<long long,double>,vector<pair<long long,double>>, prioritize> Q;
      pair<long long,double> current;

      vector<long long> V =G.getVertices();
      for(long long x:V)
      {
         pair<long long, double> temp;
         temp.first = x;
         temp.second = INF;
         distances.emplace(temp);
         Q.push(temp);
         predV[x] =0;
      }

      distances[startV] = 0;
      Q.push(make_pair(startV, 0));

      while(!Q.empty())
      {
         current = Q.top();
         Q.pop();
         set<long long> neighbors = G.neighbors(current.first);

         if(current.second == INF)
          break;
         else if(search(visited, current.first))
          continue;

        for(long long x : neighbors)
         {
          double edgeWeight;
          G.getWeight(current.first, x, edgeWeight);
          double altPathWeight = current.second + edgeWeight;

          if(altPathWeight < distances[x])
          {
             distances[x] = altPathWeight;
             Q.push(make_pair(x, altPathWeight));
              predV[x] = current.first;
          }
         }
         
         visited.push_back(current.first);
      }

      return visited;
}


//////////////////////////////////////////////////////////////////
//
// main
//
int main()
{
  map<long long, Coordinates>  Nodes;     // maps a Node ID to it's coordinates (lat, lon)
  vector<FootwayInfo>          Footways;  // info about each footway, in no particular order
  vector<BuildingInfo>         Buildings; // info about each building, in no particular order
  XMLDocument                  xmldoc;
  graph<long long, double> G; // vertices are nodes, weights are distances.
  const double INF = numeric_limits<double>::max();//Infinity

  cout << "** Navigating UIC open street map **" << endl;
  cout << endl;
  cout << std::setprecision(8);

  string def_filename = "map.osm";
  string filename;

  cout << "Enter map filename> ";
  getline(cin, filename);

  if (filename == "")
  {
    filename = def_filename;
  }

  //
  // Load XML-based map file 
  //
  if (!LoadOpenStreetMap(filename, xmldoc))
  {
    cout << "**Error: unable to load open street map." << endl;
    cout << endl;
    return 0;
  }
  
  //
  // Read the nodes, which are the various known positions on the map:
  //
  int nodeCount = ReadMapNodes(xmldoc, Nodes);

  //
  // Read the footways, which are the walking paths:
  //
  int footwayCount = ReadFootways(xmldoc, Footways);

  //
  // Read the university buildings:
  //
  int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

  //
  // Stats
  //
  assert(nodeCount == Nodes.size());
  assert(footwayCount == Footways.size());
  assert(buildingCount == Buildings.size());

  cout << endl;
  cout << "# of nodes: " << Nodes.size() << endl;
  cout << "# of footways: " << Footways.size() << endl;
  cout << "# of buildings: " << Buildings.size() << endl;

    for(pair<long long, Coordinates> x : Nodes)
    {
        G.addVertex(x.first);
    }
    for(FootwayInfo x: Footways)
    {
        for(int i = 0; i< x.Nodes.size()-1; i++)
        {
            G.addEdge(x.Nodes[i], x.Nodes[i+1], distBetween2Points(Nodes[x.Nodes[i]].Lat, Nodes[x.Nodes[i]].Lon,
                                                                   Nodes[x.Nodes[i+1]].Lat, Nodes[x.Nodes[i+1]].Lon));
            G.addEdge(x.Nodes[i+1], x.Nodes[i], distBetween2Points(Nodes[x.Nodes[i]].Lat, Nodes[x.Nodes[i]].Lon,
                                                                   Nodes[x.Nodes[i+1]].Lat, Nodes[x.Nodes[i+1]].Lon));
        }
    }
  //
  // TODO: build the graph, output stats:
  //


  cout << "# of vertices: " << G.NumVertices() << endl;
  cout << "# of edges: " << G.NumEdges() << endl;
  cout << endl;

  //
  // Navigation from building to building
  //
  string startBuilding, destBuilding;

  cout << "Enter start (partial name or abbreviation), or #> ";
  getline(cin, startBuilding);

  while (startBuilding != "#")
  {
    cout << "Enter destination (partial name or abbreviation)> ";
    getline(cin, destBuilding);
    
    BuildingInfo start, destination;
      for(BuildingInfo x: Buildings)
      {
          if(x.Abbrev == startBuilding)
          {
              start = x;
              break;
          }
          else if((x.Fullname).find(startBuilding)!=string::npos)
          {
              start = x;
              break;
          }
      }
      for(BuildingInfo x: Buildings)
      {
          if(x.Abbrev == destBuilding)
          {
              destination = x;
              break;
          }
          else if((x.Fullname).find(destBuilding)!=string::npos)
          {
              destination = x;
              break;
          }
      }
      //buildings not found
      if(start.Fullname == "")
      {
          cout << "Start building not found" << endl;
      }
      else if(destination.Fullname == "")
      {
        cout << "Destination building not found" << endl;
      }
      else//both buildingns found
      {
          cout << "Starting point:" << endl;
          cout << " " << start.Fullname << endl;
          cout << " (" << (start.Coords).Lat << ", " << (start.Coords).Lon << ")" << endl;
          cout << "Destination point:" << endl;
          cout << " " << destination.Fullname << endl;
          cout << " (" << (destination.Coords).Lat << ", " << (destination.Coords).Lon << ")" << endl;
          cout << endl;
          
          //find closest node to building's nodes
          long long startNodeID;
          long long destNodeID;
          double StartCurDist = -1;
          double DestCurDist = -1;
          double StartMinDist = INF;
          double DestMinDist = INF;
          
         for(FootwayInfo x: Footways)
            {
            for(int i = 0; i< x.Nodes.size(); i++)
                {
                  StartCurDist = distBetween2Points((start.Coords).Lat, (start.Coords).Lon, Nodes[x.Nodes[i]].Lat, Nodes[x.Nodes[i]].Lon);
                  if(StartMinDist > StartCurDist)
                     {
                        StartMinDist = StartCurDist;
                        startNodeID = x.Nodes[i];
                     }

                 DestCurDist = distBetween2Points((destination.Coords).Lat, (destination.Coords).Lon, Nodes[x.Nodes[i]].Lat, Nodes[x.Nodes[i]].Lon);
                  if(DestMinDist > DestCurDist)
                     {
                        DestMinDist = DestCurDist;
                        destNodeID = x.Nodes[i];;
                     }
                }
            }
          
          cout << "Nearest start node:" << endl;
          cout << " " << startNodeID << endl;
          cout << " (" << Nodes[startNodeID].Lat << ", " << Nodes[startNodeID].Lon <<")" << endl;
          cout << "Nearest destination node:" << endl;
          cout << " " << destNodeID << endl;
          cout << " (" << Nodes[destNodeID].Lat << ", " << Nodes[destNodeID].Lon <<")" << endl;
          cout << endl;
          
          cout << "Navigating with Dijkstra..." << endl;
          map<long long, double> distances;
          map<long long, long long> pred;
          vector<long long> pathID = Dijkstra(G, startNodeID, distances, pred);
          if(distances[destNodeID] == INF)
          {
              cout << "Sorry, destination unreachable" << endl;
          }
          else
          {
              cout << "Distance to dest: " << distances[destNodeID] << " miles" << endl;
              cout << "Path: ";
               stack<long long> path;
          long long prev = destNodeID;
          
          while(prev != 0)
          {
              path.push(prev);
              prev = pred[prev];
          }
          
          while(!path.empty())
          {
              cout << path.top();
              path.pop();
              if(!path.empty())
                  cout << "->";
          }
          cout << endl;
          }
          
      }

    //
    // TODO: lookup buildings, find nearest start and dest nodes,
    // run Dijkstra's alg, output distance and path to destination:
    //




    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter start (partial name or abbreviation), or #> ";
    getline(cin, startBuilding);
  }

  //
  // done:
  //
  cout << "** Done **" << endl;

  return 0;
}

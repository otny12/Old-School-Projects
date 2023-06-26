/*main.cpp*/

//
// Tony Lau
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// 
// 
// 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm> // for sort()
#include "util.h"
#include "hash.h"
#include "hashmap.h"
#include <set> // for set data structure
using namespace std;

//
//Struct to hold station data taken from stations.csv
//it stores the ID, Abbreviated name, full name, lattitude, longittude, capacity and online date
//of each station
//
struct StationData
{
    int ID;
    string Abbrev;
    string Name;
    double Lat;//lattitude
    double Lon;//longittude
    int Capacity;
    string Date;
    
    StationData()
    {
        ID = -1;
        Abbrev = "";
        Name = "";
        Lat = 0.0;
        Lon = 0.0;
        Capacity = -1;
        Date = "";
    }
};

//
//Struct that holds the data for trips
//it stores the trip ID, start time, stop time, Bike ID, Trip duration, from station ID, to station ID
//Identifiers, and birth year
//
struct TripData
{
    string TripID;
    string Start;
    string Stop;
    string BikeID;
    int Duration;
    int ToStationID;
    int FromStationID;
    string Identifiers;
    int BirthYear;
    
    TripData()
    {
        TripID = "";
        Start = "";
        Stop = "";
        BikeID = "";
        Duration = 0;
        ToStationID = 0;
        FromStationID = 0;
        Identifiers = "";
        BirthYear = 0;
    }
};

//
// inputStationData
// 
// Given a filename denoting StationData,
// inputs that data into the given hash table.
// 
bool inputStationData(string filename, hashmap<int, StationData*>& byID, hashmap<string, StationData*>& byAbbrev, int& count)
{
	ifstream  infile(filename);
	
	cout << endl << "Reading " << filename << endl;
	
	if (!infile.good())
	{
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	
	// file is open, start processing:
	string line;
	
	getline(infile, line);  // input and discard first row --- header row
	
    count = 0;
	
	while (getline(infile, line))
	{
		stringstream s(line);
		
    string iD, lat, lon, capacity;
		StationData* temp = new StationData;
		
		
		getline(s, iD, ',');  
		getline(s, temp->Abbrev, ',');   
		getline(s, temp->Name, ',');
		getline(s, lat, ',');
    getline(s, lon, ',');
    getline(s, capacity, ',');
    getline(s, temp->Date, ',');

    temp->ID = stoi(iD);
    temp->Lat = stod(lat);
    temp->Lon = stod(lon);
    temp->Capacity= stoi(capacity);

        
		byID.insert(temp->ID, temp, DirectHash);
		byAbbrev.insert(temp->Abbrev, temp, AbbrevHash);
		count++;
	}
	
	
	return true;  // we have data to be processed:
}

bool inputTripsData(string filename, hashmap<int, TripData*>& tripsID, hashmap<int, int>& bikesID, int& count, int& numBikes)
{
  ifstream  infile(filename);
	
	cout << "Reading " << filename << endl;
	
	if (!infile.good())
	{
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	
	// file is open, start processing:
	string line;
	
	getline(infile, line);  // input and discard first row --- header row
	
    count = 0;
	
	while (getline(infile, line))
	{
		stringstream s(line);
		
    string duration, from, to, birthyear;
		TripData* temp = new TripData;
		int usage = 0;
		
		getline(s, temp->TripID, ',');  
		getline(s, temp->Start, ',');   
		getline(s, temp->Stop, ',');
		getline(s, temp->BikeID, ',');
    getline(s, duration, ',');
    getline(s, from, ',');
    getline(s, to, ',');
    getline(s, temp->Identifiers, ',');
    getline(s, birthyear, ',');


    temp->Duration = stoi(duration);
    temp->FromStationID = stoi(from);
    temp->ToStationID = stoi(to);
    if(birthyear != "")
      temp->BirthYear= stoi(birthyear);
        
		//
		// store into hash table
		// 
    string holder = temp->TripID;
    holder = holder.substr(2,holder.length()-2);

		tripsID.insert(stoi(holder), temp, DirectHash);
    
    int bikeIDAsInt = stoi(temp->BikeID.substr(1,temp->BikeID.length()-1));
    if(!bikesID.search(bikeIDAsInt, usage,  DirectHash))
    {
      usage = 0;
      numBikes++;
    }
    bikesID.insert(bikeIDAsInt, usage+1, DirectHash);
		count++;
	}
	
	
	return true;  // we have data to be processed:
}



//
//helper function to print station data
//
void printStationData(StationData* input)
{
  cout << "Station:" << endl;
        cout <<" ID: " << input->ID << endl;
        cout <<" Abbrev: " << input->Abbrev << endl;
        cout <<" Fullname: " << input->Name << endl;
        cout <<" Location: (" << input->Lat << ", " << input->Lon << ")" << endl;
        cout <<" Capacity: " << input->Capacity << endl;
        cout <<" Online date: " << input->Date << endl;
}


//
//command #1 Look up station by ID
//takes a string of a station ID verifies it and searches it
//
void stationLookUp(string input, hashmap<int, StationData*>& hmap)
{
    StationData* temp;
    
    if(!hmap.search(stoi(input), temp, DirectHash))
        cout << "Station not found" << endl;
    else
      printStationData(temp);
}


//
//command #2 Look up station by Abbreviated name
//takes a string of a station Abbreviated name, verifies it and searches it
//
void stationLookUpByAbbrev(string input, hashmap<string, StationData*>& hmap)
{
    StationData* temp;
    
    if(!hmap.search(input, temp, AbbrevHash))
        cout << "Station not found" << endl;
    else
      printStationData(temp);
}


//
//command #3 lookup trip by tripid
//takes a trip id in the format ,TrX, where Tr is "Tr" and X is some non negetive integer
//
void tripLookUpByID(string input, hashmap<int, TripData*> hmap, hashmap<int, StationData*>& hmapById)
{
  TripData* temp;
  StationData* Station;
  string from, to;
  
  input = input.substr(2,input.length()-2);
  
  if(!hmap.search(stoi(input), temp, DirectHash))
     cout << "Trip not found" << endl;
   else
   {
     //find the names of the to and from stations as they are stored as IDs
     if(hmapById.search(temp->FromStationID, Station, DirectHash))
     from = Station->Abbrev;
     if(hmapById.search(temp->ToStationID, Station, DirectHash))
     to = Station->Abbrev;
     
     //format the time to be in hours minutes and seconds since duration is formatted in seconds
     int seconds = temp->Duration;
     int hours = seconds / 3600;
     int minutes = (seconds % 3600) / 60;
     seconds = seconds % 60;
     
     cout << "Trip: " << endl;
     cout << " ID: " << temp->TripID << endl;
     cout << " Starttime: " << temp->Start << endl;
     cout << " Bikeid: " << temp->BikeID << endl;
     cout << " Duration: ";
     if(hours > 0)
       cout << hours << " hours, ";
     if(minutes > 0)
       cout << minutes << " minutes, ";
       cout << seconds << " seconds";
     cout << endl;
     cout << " From station: " << from << " (" << temp->FromStationID<< ")" << endl;
     cout << " To station: " << to << " (" << temp->ToStationID<< ")" << endl;
     cout << " Rider identifies as: " << temp->Identifiers << endl;
     cout << " Birthyear: ";
     if(temp->BirthYear!=0)
       cout<< temp->BirthYear << endl;
   }
}


//
//command #4 lookup bike usage by bikeid
//
void bikeUsage(string bikeId, hashmap<int, int> hmap)
{
  int usage;
  int bikeIDAsInt = stoi(bikeId.substr(1,bikeId.length()-1));//parse the bike id into an int
  if(!hmap.search(bikeIDAsInt, usage, DirectHash))
  {
    cout <<"Bike not found" << endl;
    return;
  }
  else
  {
    cout <<"Bike:" << endl;
    cout <<" ID: " << bikeId << endl;
    cout <<" Usage: " << usage << endl;
  }
}


//
//command #5 find nearby stations based on lattitude and longittude
//function outputs the nearest stations within the specified range into standard outputs
//
void findNearby(string input, hashmap<int, StationData*> stationsById)
{
  string lats, lons, rans;
  double lat, lon, range;
  int tracker = 0; //used to track if any stations were found within range
  vector<int> keys = stationsById.activeKeys();//get all active keys in the hashmap
  vector<pair<double, int>> output;//int is the stationID and double is the distance to the station
  
  stringstream s(input.substr(7, input.length()-7));//removes the "nearby "
  
  getline(s, lats, ' ');
  getline(s, lons, ' ');
  getline(s, rans, ' ');
  
  lat = stod(lats);
  lon = stod(lons);
  range = stod(rans);
  
  cout<< "Stations within " << rans << " miles of (" << lats << ", " << lons <<")" << endl;
  
  for(int x : keys)
  {
    StationData* temp;
    
    if(stationsById.search(x, temp, DirectHash))
    {
      double dist = distBetween2Points(lat, lon, temp->Lat, temp->Lon);
      if( dist < range)
      {
        output.push_back(make_pair(dist, x));
        tracker++;
      }
    }
  }//end of for(Int
  
  sort(output.begin(), output.end());//sort the outputs to display the closests first
  if(tracker==0)
    cout <<" none found" <<endl;
  else
  {
    for(pair<double, int> i: output)
    {
    cout << " station " << i.second << ": " << i.first << " miles" << endl;
    }
  }
}


//
//command #6 find similiar trips
//
void findSimiliar(string userInput, hashmap<int, StationData*> stationsById, hashmap<int, TripData*> tripsById)
{
  set<int> start;//set of stations near start
  set<int> destination;//set of stations near destination
  vector<int> stations = stationsById.activeKeys();//get all active keys in the hashmap
  vector<int> trips= tripsById.activeKeys();//get all active keys in the 
  
  TripData* requestedTrip;
  StationData* StartStation;
  StationData* DestinationStation;

  int numTrips = 0;
  string tripIDs, rans;
  int tripId;
  double range;
  
  stringstream s(userInput.substr(8, userInput.length()-8));//removes the "similiar "
  getline(s, tripIDs, ' ');
  getline(s, rans, ' ');

  tripId = stoi(tripIDs.substr(2, tripIDs.length()-2));
  range = stod(rans);
  
  cout << "Trips that follow a similar path (+/-" << rans <<" miles) as " << tripIDs << endl;

   if(!tripsById.search(tripId, requestedTrip, DirectHash))//get requestedTrip
   {
     cout << " no such trip" << endl;
     return;
   }
  stationsById.search(requestedTrip->FromStationID, StartStation, DirectHash);
     start.insert(requestedTrip->FromStationID);
  if(stationsById.search(requestedTrip->ToStationID, DestinationStation, DirectHash))
     destination.insert(requestedTrip->ToStationID);
  
 
  for(int x : stations)
  {
    StationData* temp;
    if(stationsById.search(x, temp, DirectHash))
    {
      double distFromS = distBetween2Points(StartStation->Lat, StartStation->Lon, temp->Lat, temp->Lon);
      double distFromD = distBetween2Points(DestinationStation->Lat, DestinationStation->Lon, temp->Lat, temp->Lon);
      if(distFromS < range)
        start.insert(x);
      if(distFromD < range)
        destination.insert(x);
    }
  }//end for(int x
  
  for(int i : trips)
  {
    TripData* temp;
    tripsById.search(i, temp, DirectHash);
    if(start.count(temp->FromStationID) > 0 && destination.count(temp->ToStationID) > 0)
      numTrips++;  
  }
  
  
  
  if(numTrips < 2)
  {
    cout << " no such trip" << endl;
  }
  else
  {
    cout << " nearby starting points: ";
      for(int i : start)
        cout << i <<" ";
    cout << endl << " nearby ending points: ";
      for(int i : destination)
        cout << i <<" ";
    cout << endl << " trip count: " << numTrips << endl;
  }
  
 }


int main()
{
    hashmap<int, StationData*> stationsById(10000);
    hashmap<string, StationData*> stationsByAbbrev(10000);
    hashmap<int, TripData*> tripsById(2500000);
    hashmap<int, int> bikesById(50000);
    
    string stationFile;
    string tripFile;
    int numStations, numTrips, numBikes = 0;
    bool showMenu = true;
  
    
    cout << "** DIVVY analysis program **" << endl << endl;
    
    cout << "Enter stations file> ";
    cin >> stationFile;
    
    cout << "Enter trips file> ";
    cin >> tripFile;
    
    bool success = inputStationData(stationFile, stationsById, stationsByAbbrev, numStations);
    bool success2 = inputTripsData(tripFile, tripsById, bikesById, numTrips, numBikes);
    
    if(!success || !success2)
    {
        cout << "failed read" << endl;
        return -1;
    }
    cout << endl;
    cout << "# of stations: " << numStations << endl;
    cout << "# of trips: " << numTrips << endl;
    cout << "# of bikes: " << numBikes << endl;
  
  string dump;
  getline(cin, dump);
    while(showMenu)
    {
        string userInput;
        cout <<endl << "Please enter a command, help, or #> ";
        getline(cin, userInput);
        
        if( userInput == "#")//exit command
            showMenu = false;
        else if(isNumeric(userInput))//command 1
            stationLookUp(userInput,stationsById);
        else if(userInput.substr(0,2) == "Tr" && isNumeric(userInput.substr(2,userInput.length()-2)))//command 3
            tripLookUpByID(userInput, tripsById, stationsById);
        else if(userInput[0] == 'B' && isNumeric(userInput.substr(1,userInput.length()-1)))//command 4
            bikeUsage(userInput, bikesById);
        else if(userInput.substr(0,7) == "nearby ")//command #5
            findNearby(userInput, stationsById);
        else if(userInput.substr(0,8) == "similar ")//command #6
            findSimiliar(userInput, stationsById, tripsById);
        else//command 2
            stationLookUpByAbbrev(userInput, stationsByAbbrev);
        
    }
  
  //deallocate memory
  vector<int> stations = stationsById.activeKeys();//get all active keys in the hashmap
  vector<int> trips= tripsById.activeKeys();//get all active keys in the 
  for(int x: stations)
  {
    StationData* temp;
    stationsById.search(x, temp,DirectHash);
    delete(temp);
  }
  for(int x: trips)
  {
    TripData* temp;
    tripsById.search(x, temp,DirectHash);
    delete(temp);
  }
        
    
    return 0;    
}


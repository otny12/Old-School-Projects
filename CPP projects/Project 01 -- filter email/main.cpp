/*
  * Program: Spam filter (project 01)
  * Course: UIC CS 251
  * Programmer: Tony Lau
  * Professor: Reckinger
  * 
  * This Program takes input from a txt of spam emails with the expected format:
  * Domain:username ie.(aaa.com:user)
  * if a username is "*" any username will be accepted
  * 
  * This program will also take the input of a emails txt in the format:
  * Inbox# emailAddress subjectLine ie.(9 jhummel2@uic.edu Question about grading...)
  * 
  * commands taken by this program include:
  * load "exampleSpamList.txt"
  * display
  * check "exampleEmailAddress"
  * filter "exampleEmailInbox.txt" "exampleOutput.txt"
  * 
*/
#include <iostream>
#include <string>
#include <fstream>
#include "ourvector.h"
using namespace std;


//uses binary search to look through a vector for a specific string using the
//.compare() function to continously divide the search space by 2 until the word is found
int binarySearch(ourvector<string>& list, string address, int domainSize){
    
    long low, mid, high; // longs that define the indices used for the binary search
    long direction = -1; // direction shows the direction the search word is compared to the index of mid
    string currWord;     // is the current word being searched for from the spamlist
    
    low = 0;
    high = list.size()-1;
   
    while(low <= high){
        
        mid = (low + high)/2;
        currWord = list.at(mid);
        direction = (address.substr(0, domainSize)+":*").compare(currWord);
        
        if(direction == 0){//if dirrection is 0 then the domain has been found
                return mid;    
            }
        else if(direction < 0){//if direction is negetive then the word comes earlier in the vector
            high = mid - 1;
        }
        else{//if the direction is positive the word comes later in the vector
            low = mid + 1;
        }
    }//end of while(low...
    
    low = mid;
    high= list.size()-1;
    
    while(low <= high){

        mid = (low + high)/2;
        currWord= list.at(mid);
        direction = address.compare(currWord);

        if(direction == 0){//if dirrection is 0 then the domain has been found
                return mid;    
            }
        else if(direction < 0){//if direction is negetive then the word comes earlier in the vector
            high = mid - 1;
        }
        else{//if the direction is positive the word comes later in the vector
            low = mid + 1;
        }
    }
    return -1;//if the word is not found the function returns -1
}//end of int binarySearch(...


//takes input of spamlist file name then places them into a cleared vector
void load(ourvector<string>& spamlist, string targetFile){
    
    int counter = 0;//counts the number of lines inserted
    string lineData = "";
    ifstream inFS;
    
    //opens an input file stream
    inFS.open(targetFile);
    if(!inFS.is_open()){
        cout << "**Error, unable to open '" << targetFile<< "'" << endl;
        return;
    }
    cout << "Loading '" << targetFile << "'" << endl;
    spamlist.clear();
    
    //reads in the data from file stream and inputs to vector
    inFS >> lineData;
    while(!inFS.eof()){
        if(inFS.fail())
            return;
        counter ++;
        spamlist.push_back(lineData);
        inFS >> lineData;
    }
    inFS.close();
    cout << "# of spam entries: " << counter << endl;
}//end of void load


//displays the current data in the spamlist vector by iterating through the vector 
void display(ourvector<string>& spamlist){
    for(int i = 0; i<spamlist.size(); i++){
        cout << spamlist.at(i) << endl;
    }
}


//takes in one email address and outputs to terminal whether or not the email is spam
//by looking through the vector
void check(ourvector<string>& spamlist, string emailAdress){
    string username;
    string domain;
    size_t i;
    
    //parses the email address into a username and domain
    for(i = 0; i < emailAdress.size(); i ++){
        if(emailAdress[i] == '@')
            break;
    }
    if(i==emailAdress.size()){
        cout << emailAdress << " is not spam" << endl;
        return;
    }
    username = emailAdress.substr(0,i);
    domain = emailAdress.substr(i+1, emailAdress.size()-1);

    //format the username and domain into domain:username then pass it into a binarysearch
    if((binarySearch(spamlist, domain + ':' + username,domain.size()) > -1)){
        cout << emailAdress << " is spam" << endl;
    }
    else{
        cout << emailAdress << " is not spam" << endl;
    }
}

//Takes in emails txt file and an output txt file 
//and copies all non-spam emails from the emails txt to the output txt file
void filter(ourvector<string>& spamlist, string emailfile, string outputfile ){
    
    int inboxNum;            //holder variable to remove number from email file
    string inboxSubject;     //holder variable to remove subject line from email file
    string inboxAdress = ""; //holder variable for the email address
    int numProcessed = 0;    //counts number of emails processed
    int numSpam = 0;         //counts number of emails copied to output file
    string username;         //holds username of email Adress
    string domain;           //holds domain name of email address
    
    size_t i;
    ifstream inFS;
    ofstream outFS;
    
    //opens email file as input stream
    inFS.open(emailfile);
    if(!inFS.is_open()){
        cout << "**Error, unable to open '" << emailfile << "'" << endl;
        return;
    }
    
    //opens outputfile as output stream
    outFS.open(outputfile);
    if(!outFS.is_open()){
        cout << "**Error, unable to open '" << outputfile << "'" << endl;
        return;
    }
    
    //takes input data from the email file and extracts the inbox address
    inFS >> inboxNum >> inboxAdress;
    getline(inFS,inboxSubject);
    while(!inFS.eof()){
        if(inFS.fail())
            return;
        numProcessed ++;
        //parses the email address to a format containing 
        for(i = 0; i < inboxAdress.size(); i ++){
            if(inboxAdress[i] == '@')
                    break;
        }
        username = inboxAdress.substr(0,i);
        domain = inboxAdress.substr(i+1, inboxAdress.size()-1);

        //check if the email address is in spamlist through a binarySearch
        if((binarySearch(spamlist, domain + ':' + username, domain.size()) == -1)){
           outFS << inboxNum << " " << inboxAdress << " " << inboxSubject << endl;
           numSpam ++;
        }
                 
        inFS >> inboxNum >> inboxAdress;
        getline(inFS,inboxSubject);
    }//end of while(!inFS.eof()){
    
    inFS.close();
    outFS.close();
    cout << "# emails processed: " << numProcessed << endl;
    cout << "# non-spam emails: " << numSpam << endl;
}
 
/*
    The main function handles the menu inputs and exting the program.
    load a spam list,
    display the contents of a spam list,
    check a single email address to see if it’s spam,
    filter an email list and output the resulting emails to a file
*/
int main(){
    
    string userCommand = "";//command the user inputs
    string userTarget1 = "";//target file
    string userTarget2 = "";//2nd target file
    bool showMenu = true;
    ourvector<string> spamlist;//vector containing the contents of the spamlist

    cout << "** Welcome to spam filtering app **" << endl;    
    while(showMenu){//loops to replay the menu
        cout << endl;
        cout << "Enter command or # to exit> ";
        cin >> userCommand;

        if(userCommand=="load")
        {
            cin >> userTarget1;// takes in target file
            load(spamlist, userTarget1);
        }
        else if(userCommand=="display")
        {
            display(spamlist);
        }
        else if(userCommand=="check")
        {
            cin >> userTarget1;//takes in target file
            check(spamlist, userTarget1);
        }
        else if(userCommand=="filter")
        {
            cin >> userTarget1 >> userTarget2;//takes in target file 1 and 2
            filter(spamlist, userTarget1, userTarget2);
        }
        else if(userCommand=="#")
        {
            showMenu = false;
            cout << endl;
        }
        else
        {
            cout << "**invalid command" << endl;
        }
        
    }//end of while(showMenu)
    return 0;
}
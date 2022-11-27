////////////////////////////////////////////////////
//                                                //
// Authors    : Ethan Coyle, Jonathan Hogan and   //
//              Dymon Browne                       //
//                                                //
// Teacher    : Dr. Passos                        //
// Class      : Intro to Operating Systems        //
// Assignment : Mini Project 4                    //
//                                                //
//  Description                                   //
//         The prupose of this project is to      //
//    implement a page replacement algorithm      //
//    that will be used to simulate a             //
//    virtual memory system comparing a FIFO      //
//    page replacement algorithm to a Second      //
//    Chance page replacement algorithm and see   //
//    the difference in a system running 10,000   //
//    pages and using 10 frames and a random      //
//    number generator to generate the values     //
//                                                //
////////////////////////////////////////////////////

#include<iostream>
#include<vector>
#include<iomanip>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include<bits/stdc++.h>

using namespace std;
//create prototype for functions
bool SecondChanceNeedUpdating(int page, int Frames,vector<pair<int,bool>> &frames);
int SecondChanceReplace(int page, int Frames,vector<pair<int,bool>> &frames,int page_pointer);
int SecondChanceAlgorithm(vector<int> reference_string, int Frames,vector<pair<int,bool>> frames);
int FIFOpageFaults(vector<int> reference_string_fifo, int n, int Frames);




//main driver for program will call functions to read in file and perform page replacement algorithms
int main()
{
    int i,Second_Chance_Page_Faults=0;//variable to store the number of page faults and counter variable
    //no of frames
    int Frames=10;// we got 10 frames
    int TotPages=10000;// total pages we are running on is 10000
    bool flag=false;//flag initially set to false
    srand((int)time(0));// for random number
    map<unsigned int, int> Pages;// map to store the pages
    //create reference string array
    vector<int> reference_string(TotPages); 
    vector<int> reference_string_fifo(TotPages); 

    int FIFOPageFaults= 0;
    int FIFOOldest;
    vector<int> FIFO;

    //generate a random number and generate it from 0 %4096
    srand(time(0));
    for(int i=0;i<TotPages;i++)
    {
        int randNum = rand() % 4096;//generate a random number
        unsigned int pageNum =  (randNum >> 6);//shift the random number by 6 bits
        reference_string[i] = pageNum;//store the page number in the reference string
        reference_string_fifo[i] = pageNum;//store the page number in the reference string
        
        Pages[pageNum] += 1;
    }
    
    //create vector fram that will store the pages and reference bit
    vector<pair<int,bool>> frames(Frames);
    //frame set from strt to page no -1 and then the reference bit for this is 0 for second chance
    for(int i=0; i<Frames; i++)
    {
      frames[i].first = -1;  
      frames[i].second = false;
    }
    //print out the size of the reference string
    cout<<"The size of the reference string is: "<<reference_string.size()<<endl;
    Second_Chance_Page_Faults = SecondChanceAlgorithm(reference_string, Frames,frames);// the total ammount of page faults
    int n = sizeof(reference_string_fifo);
    //print out the size of the reference string fifo
    cout<<"The size of the reference string fifo is: "<<reference_string_fifo.size()<<endl;
    int pageFaults = FIFOpageFaults(reference_string_fifo, n, Frames);
    //are passed back to the variable Second_Chance_Page_Faults and then we display to the scrren and user
    cout<<"------------------------------------------------------------"<<endl;
    cout<<" Authors    : Ethan Coyle, Jonathan Hogan and Dymon Browne"<<endl;
    cout<<" Teacher    : Dr. Passos"<<endl;
    cout<<" Class      : Intro to Operating Systems"<<endl;
    cout<<" Assignment : Mini Project 4"<<endl;
    cout<<"------------------------------------------------------------"<<endl;
    cout<<"\nTotal Number of Page Faults for the SCR  is : "<<Second_Chance_Page_Faults;
    cout << "Total Number of Page Faults for the FIFO is : "<<pageFaults;
    return 0;//end the file
}
//write function for FIFO page replacement algorithm



// Function to check if we need to update 
bool SecondChanceNeedUpdating(int page, int Frames,vector<pair<int,bool>> &frames)
{//Check if the page is already present in the frames vector
    for(int i = 0 ; i < Frames; i++)
    {
        if(frames[i].first == page)        //the page already is present
        {
            frames[i].second = true;        //set the reference bit to 1
            return false; //we dont need to update the page
    }
    return true ; // Need for Page replacement due to not found in frames
}}
// function to find the page that need be replaced  and then replacing it with new pagee, the updated loacation
// if then passed back to be used as the new starting point for the next search
int SecondChanceReplace(int page, int Frames,vector<pair<int,bool>> &frames,int page_pointer)
{
    while(true)
    {
       if(frames[page_pointer].second == 0)
       {
           frames[page_pointer].first = page;//put your page at this pointed location in frames
           page_pointer = (page_pointer + 1)% Frames;//Update the page_pointer to point to next location
           return page_pointer;//return the updated page_pointer
       }
       //if reference bit is -1 then make it 0 and move to next location this gives a second chance
       frames[page_pointer].second = 0;//second chance
       page_pointer = (page_pointer + 1)% Frames;//increment the page_pointer
    }
    return page_pointer;//return the updated page_pointer of location from where new search is staarting
}
//function that performs the Second Chance Algorithm and returns the number of page faults
//parameters are  reference string, number of frames and vector bool frames
int SecondChanceAlgorithm(vector<int> reference_string, int Frames,vector<pair<int,bool>> frames)
{
    int Second_Chance_Page_Faults = 0,page_pointer=0;
    for(auto page : reference_string)
    {
       //Check if there is need for updating the page
       if(SecondChanceNeedUpdating(page,Frames,frames))
       {
          //Replace the page using Second Chance Algorithm 
          page_pointer = SecondChanceReplace(page,Frames,frames,page_pointer);
          //increment the page faults
          Second_Chance_Page_Faults++;//increment the page faults
       } 
    }
   return Second_Chance_Page_Faults;//return the number of page faults
}
int FIFOpageFaults(vector<int> reference_string_fifo, int n, int Frames)
{
    //find all of the page faults
    int pageFaults = 0;
    //create a set to store the pages
    set<int> s;
    //create a queue to store the pages
    queue<int> indexes;
    //loop through the reference string

    for (int i=0; i<n; i++)
    {
        //if the set size is less than the number of frames
        if (s.size() < Frames)
        {
            //if the page is not in the set
            if (s.find(reference_string_fifo[i])==s.end())
            {
                //insert the page into the set
                s.insert(reference_string_fifo[i]);
                //insert the page into the queue
                indexes.push(reference_string_fifo[i]);
                //increment the page faults
                pageFaults++;
            }
        }
        //if the set size is equal to the number of frames
        else
        {
            //if the page is not in the set
            if (s.find(reference_string_fifo[i]) == s.end())
            {
                //get the first page in the queue
                int val = indexes.front();
                //remove the first page from the queue
                indexes.pop();
                //remove the first page from the set
                s.erase(val);
                //insert the page into the set
                s.insert(reference_string_fifo[i]);
                //insert the page into the queue
                indexes.push(reference_string_fifo[i]);
                //increment the page faults
                pageFaults++;
            }
        }
    }
    //return the number of page faults
    return pageFaults;
}

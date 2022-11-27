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
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <map>
#include <set>
#include<bits/stdc++.h>

using namespace std;
//create prototype for functions
bool SecondChanceNeedUpdating(int page, int Frames,vector<pair<int,bool>> &frames);
int SecondChanceReplace(int page, int Frames,vector<pair<int,bool>> &frames,int page_pointer);
int SecondChanceAlgorithm(vector<int> reference_string, int Frames,vector<pair<int,bool>> frames);




//main driver for program will call functions to read in file and perform page replacement algorithms
int main()
{
    int i,Second_Chance_Page_Faults=0;//variable to store the number of page faults and counter variable
    int q, r, s, PagePresent = 0; // iterint over and finding page faults with fifo checking for present pages all set to 0
    //no of frames
    int Frames=10;// we got 10 frames
    int TotPages=10000;// total pages we are running on is 10000
    bool flag=false;//flag initially set to false
    srand((int)time(0));// for random number
    map<unsigned int, int> Pages;// map to store the pages
    //create reference string array
    vector<int> reference_string(TotPages); 

    vector <int> FifoPages;// vector to store the pages for fifo
    vector <int> hit(TotPages);//keeping track of how many page hits for fifo calculating page faults
    vector<vector<int>> FifoPageFrame(Frames);//vector to store the frames
    for(q = 0; q < Frames; q++){    //iterating through the frames
        FifoPageFrame[q] = vector <int>(TotPages,-1);   //filling the frames with -1 for referencing bit
    }

    //generate a random number and generate it from 0 %4096
    srand(time(0));
    for(int i=0;i<TotPages;i++)
    {
        int randNum = rand() % 4096;//generate a random number
        unsigned int pageNum =  (randNum >> 6);//shift the random number by 6 bits
        reference_string[i] = pageNum;//store the page number in the reference string
        FifoPages.push_back(pageNum);//store the page number in the reference string
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
    cout << "The size of the reference string is: " << reference_string.size() << endl;
    Second_Chance_Page_Faults = SecondChanceAlgorithm(reference_string, Frames,frames);// the total amount of page faults
      //are passed back to the variable Second_Chance_Page_Faults and then we display to the screen and user
    cout << "------------------------------------------------------------" << endl;
    cout << " Authors    : Ethan Coyle, Jonathan Hogan and Dymon Browne" << endl;
    cout << " Teacher    : Dr. Passos" << endl;
    cout << " Class      : Intro to Operating Systems" << endl;
    cout << " Assignment : Mini Project 4" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "\nThe Total Number of SCR  Page Faults were: " << Second_Chance_Page_Faults << endl;
  //now for the fifo page replacement
    map <int, int> FifoPageMap; //map to store the pages   
    
    for(q = 0; q < TotPages ; q++){
        vector<pair<int,int>> c;//vector to store the page number and the index of the page
        
        for(auto x: FifoPageMap)//iterating through the map and page index
        {
            c.push_back({x.second, x.first});//pushing the page number and the index of the page
        }

        sort(c.begin(),c.end());//sorting the vector
        bool hasCompleted = false;//flag to check if the page has been completed or not
        for(r = 0;r < Frames; r++){//iterating through the frames
            if(FifoPageFrame[r][q] == FifoPages[q])//check to see if page is already present in pages
            {
                PagePresent++;
                hasCompleted = true;
                break;
            }
            if(FifoPageFrame[r][q] == -1)//if the page is empty
            {
                for(s = q ; s < TotPages; s++)//iterate through the pages
                    FifoPageFrame[r][s] = FifoPages[q];//fill the page with the page number
                    
                FifoPageMap[FifoPages[q]]++;//increment the page number
                hasCompleted = true;//set the flag to true
                break;//break out of the loop
            }
        }
        if(r == Frames || hasCompleted == false){//if the page is not present in the frames
            for(r = 0;r < Frames; r++){//iterate through the frames
                if(FifoPageFrame[r][q] == c[c.size() - 1].second){
                    FifoPageMap.erase(FifoPageFrame[r][q]);//erase the page from the map
                    
                    for(s = q; s < TotPages ; s++)
                        FifoPageFrame[r][s]= FifoPages[q];//fill the page with the page number
                        
                    FifoPageMap[FifoPages[q]]++;//increment the page number
                    break;
                }
            }
        }
        for(auto x:FifoPageMap){//iterate through the map
            if(x.first != FifoPages[q]){//if the page is not present in the map
                FifoPageMap[x.first]++;//increment the page number
            }
        }
    }
        cout << "The Total Number of Fifo Page Faults were: " << TotPages - PagePresent << endl;//print the page fault
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
    }
    }
// function to find the page that need be replaced  and then replacing it with new page, the updated location
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
    return page_pointer;//return the updated page_pointer of location from where new search is starting
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
//hello! what you think? i practically have comment on each line lol

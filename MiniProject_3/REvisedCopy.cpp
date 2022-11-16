#include <fstream>
#include <iostream>
#include<vector>
#include <algorithm>

using namespace std;
int main() 
{
  // open file
  ifstream ifile("spice.txt");
  unsigned int Address;// the address used for conversion to hex value
  int Code; //first value read in
  int Segment, Page,TotalDifferentSegments,TotalDifferentPages,TotalPages,TotalSegments= 0;
  //the size specified for the segment and page 2^?
  int SizeOfSegment = 10;
  int SizeOfPage = 11;
  //vector for storing unique segment and pages
  vector<int> pages;
  vector<int> segments;


  // pair<string, int>(trees, 1)
  // while not end of file
  while (!ifile.eof()) 
  {
    while(ifile >> Code>> hex >> Address){
    Segment = (Address >> SizeOfSegment);
    //cout<< " segment is :"<< Segment <<'\n';
    Page = (Address >> SizeOfPage); 
    //store each segment read in into a vector
    //if the value for segment has not been seen before, add it to the vector
    if (find(segments.begin(), segments.end(), Segment) == segments.end())
    {
      segments.push_back(Segment);
      //cout<< " segment is without duplicates :"<< Segment <<'\n';
      TotalDifferentSegments++;
    }
    //if the value for page has not been seen before, add it to the vector
    //cout<< " segment is :"<< Segment <<'\n';
    if (find(pages.begin(), pages.end(), Page) == pages.end())
    {
      pages.push_back(Page);
      //cout<< " page is :"<< Page <<'\n';
      TotalDifferentPages++;
    }
    TotalPages++;
    TotalSegments++;
  }}
    // Print out analyzis for pages and segments accumulated above
  cout << "Total number of different segments: " << TotalDifferentSegments << endl;
  cout << "Total number of different pages: " << TotalDifferentPages << endl;
  cout << "Total number of ALL segments: " << TotalSegments << endl;
  cout << "Total number of ALL pages: " << TotalPages << endl;
  // close file
  ifile.close();
  return 0;
}

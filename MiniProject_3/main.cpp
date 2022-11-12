//-------------------------------------------------
// Authors   : Ethan Coyle, Jon Hogan, Dymon Brown
// Teacher   : Dr. Passos
// Class     : Intro to Operating Systems
// Assignment: Mini Project 3 – Virtual memory
//------------------------------------------------

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() 
{
  // create our variables and booleans for checking addresses and data from the
  // infile
  string Address_From_Infile = "";
  string Infile_Data = "";
  bool New_Address;
  int Number_Of_Addresses;
  double Access_Average;
  int j = 0;
  int Number_Of_Zeroes;
  vector<string> Page_Address;
  ifstream infile;
  
  // open up the infile spce.txt and read the data file of 1 million entries
  // with Address_From_Infile
  infile.open("spice.txt");
  // while the infile read the data
  while (infile >> Infile_Data) 
  {
    infile >> Address_From_Infile;
    New_Address = true; // set the Boolean to True for a new address access
    if (Address_From_Infile.length() != 8) 
    {
      if (Address_From_Infile.length() < 8) 
      {
        Number_Of_Zeroes = 8 - Address_From_Infile.length();
        Address_From_Infile =
            string(Number_Of_Zeroes, '0').append(Address_From_Infile);
      }
    }
    // taking off the offset
    Address_From_Infile =
        Address_From_Infile.substr(0, Address_From_Infile.size() - 3);

    for (int i = 0; i < Page_Address.size(); i++) 
    {
      if (Page_Address[i] == Address_From_Infile) 
      {
        New_Address = false;
        i = Page_Address.size();
      }
    }
    // if the boolean is true for the new address, then push new address
    if (New_Address == true) 
    {
      Page_Address.push_back(Address_From_Infile);
    }
  }
  // calculating the Access_Average for the accesses based off
  // 1 million and then divided by the size of the size of the vector
  Access_Average = 1000000.0 / (double)Page_Address.size();
  cout << "------------------------------------------------------------------\n";
  cout << "Authors   : Ethan Coyle, Jon Hogan, Dymon Brown                   \n";
  cout << "Teacher   : Dr. Passos                                            \n";
  cout << "Class     : Intro to Operating Systems                            \n";
  cout << "Assignment: Mini Project 3 – Virtual memory                       \n";
  cout << "------------------------------------------------------------------\n\n";
  cout << "After Analysing the infile spice.txt below are the gathered statistics :\n\n";
  cout << "The Total Amount Of Pages That Were Used Was : "<< Page_Address.size() << " Pages \n";
  cout << "The Average Number Of Accesses               : " << Access_Average << " Accesses\n";
  // end the file
  return 0;
}

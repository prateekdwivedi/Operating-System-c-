
/* 
 * Program Description: (Disk Manager)

   Multiprogramming with a Fixed number of Tasks (MFT)
   I have used MFT for the disk management where, each job has fixed size block. 
    It will have constant maximum degree of multiprogramming. 
    It has a ready queue as an array, here, freeMemory[] is the array of queue.
    It has internal fragmentation as each job has some left over pages in the particular blocks where it goes.
    Largest job <= largest block
*/


#include "disk.h"
#include "PCB.h"
#include "exception_handler.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <sstream> 
#include <string>
using namespace std;

errorHandler e;
PCBManager p;

/* The mechanism use for the management of the disk is MFT */ 


/* Functional Description:  This function will convert Hex number to Decimal */ 

int diskManager::GetDecimal(string str)                 // Function to change HEX to decimal (Internal Purpose)
{
     string HEX="0123456789ABCDEF";
     transform(str.begin(), str.end(), str.begin(), ::toupper);    // To change the string in Uppercase
     int value=0,pos;

     for(int i=0;i<str.size();i++)
        {
         char c= str[i];
         pos = HEX.find(c,0);
         value = 16*value + pos;
        }
     return value;
}




/* Functional Description: This function will keep track of free memory blocks in the disk */

int diskManager::freeMemoryCheck()
{
 int index = 999;
 for(int i=0;i<8;i++)
   {
   if(freeMemory[i]== 0)
     { 
     freeMemory[i] = 1;                       // 1 : Occupied
     index = i;
     break;
     }
   }
 return index;
}





/* Functional Description: This function will read all the data provided by user */

int diskManager::readFileData(string fileName)
 {

 streampos position;
 int pageSize,location,length,loaderSwitch=0,dataSwitch =0,programCheck=0,nullJobCheck=0,programTooLongCheck=0;
 string input,storeArr[3];
 int pageSet,dataSize,loaderSize;
 double outputSize;
 fstream fs;

// It will read the input file 
 if(openFile == 0)
    {
    diskRead.open(fileName.c_str(),ios::in);
    diskRead.seekg(0,diskRead.end);
    readLength = diskRead.tellg();
    openFile = 1;
    }
 
// This will relocate the reader to specific location where, he/she left before.
 diskRead.seekg(readPosition);

// Reading the file
 while(!diskRead.eof())
       {
        getline(diskRead, input);
        int len = input.length();
        char characters[len];
        for(int k=0;k<=len;k++)
           characters[k] = input[k];

// Splitting the string on the basis of space character
        char *token = strtok(characters," ");
        int i=0;
        while (token != NULL) {
             storeArr[i] = token;
             token = strtok(NULL, " "); 
             i++;
         }
// If the **JOB found
        if(storeArr[0]=="**JOB")
          {
// To count the label with **JOB
           jobCount++;
           p.setNature("Normal",jobID);
           dataSize = GetDecimal(storeArr[1]) ;
           
// Provide the diskIndex = freeMemory()
           outputSize = GetDecimal(storeArr[2])/4 ;  
           if(GetDecimal(storeArr[2])%4 != 0)
              outputSize++; 
           p.setWdAddress(dataSize,jobID); 
           p.setWdEndAddress(dataSize,jobID);
           p.setErrMessage("",jobID);
           p.setCumulativeTime(0,jobID);
           p.setTimeCompletion(0,jobID);
           dataSwitch = 0;
 
// Condition if no **FIN found
           if(finSwitch == 1 || jobCount == 2)
              {
              e.EHANDLER(6);
              p.setErrMessage(e.descriptive,jobID);
            if (finCount == 0)
               {
                e.EHANDLER(6);
                p.setErrMessage(e.descriptive,jobID);
                e.descriptive="";
               }
            if(jobCount == 1)                  // To check **JOB
               {
                e.EHANDLER(7);
               p.setErrMessage(e.descriptive,jobID);
               e.descriptive="";
               int initial = p.getPageTable(0,jobID);
               diskMemory[initial] = "00000000";
                }
            if(dataCount == 0)                 // To check **DATA
               {
                e.EHANDLER(8);
                p.setErrMessage(e.descriptive,jobID);
               e.descriptive="";
               }
            if(dataCount == 2)
               {
               e.EHANDLER(9);
               p.setErrMessage(e.descriptive,jobID);
               e.descriptive="";
               }

       // Resetting the counts 
             jobID++;
             dataCount = 0;
             jobCount = 1;
              }
           finSwitch = 1;
           
          }



       else if(storeArr[0]=="**DATA")
          {
           dataCount++;
           dataSwitch = 1;
          }


// Condition if **FIN found
       else if(storeArr[0]=="**FIN")
          {
           finCount++;
           dataSwitch = 0;

            if (finCount == 0)
               {
                e.EHANDLER(6);
                p.setErrMessage(e.descriptive,jobID);
               }
            if(jobCount == 0)
               {
                e.EHANDLER(7);
               p.setErrMessage(e.descriptive,jobID);
               p.setNature("Abnormal",jobID);
               int initial = p.getPageTable(0,jobID);
               }
            if(dataCount == 0)
               {
                e.EHANDLER(8);
                p.setErrMessage(e.descriptive,jobID);
               }
            if(dataCount == 2)
               {
               e.EHANDLER(9);
               p.setErrMessage(e.descriptive,jobID);
               }

       // Resetting the counts 
             jobID++;
             dataCount = 0;
             jobCount = 0;
             finSwitch = 0;
             programCheck = 0;
             nullJobCheck = 0;
             position = diskRead.tellg();
             readPosition = position;
          }
       if(len == 5 && i==2)
          {
             lookIndex = freeMemoryCheck();
             if(lookIndex ==999)
               {
               finSwitch = 0;
               break;
               }
             else
                diskIndex = lookIndex  *256;
           
           availableJobs.push_back(jobID);
           p.setPageTable(diskIndex,0,jobID);
           location = GetDecimal(storeArr[0]);
           length = GetDecimal(storeArr[1]);
           loaderSize = (length/16); 
           if (loaderSize > p.getWdAddress(jobID))
            {
               e.EHANDLER(3);
               p.setErrMessage(e.descriptive,jobID);
               e.descriptive="";
               p.setNature("Abnormal",jobID);
               p.setPageFaultCount(0,jobID);
            }
           int availablePages = p.getWdAddress(jobID) - 2;
             if(p.getWdAddress(jobID) > 3) {
                availablePages = p.getWdAddress(jobID) - 3;
               }
           
           int pF;
           if(length != 0)
               pF = availablePages*(length/2);
           else
               pF =0;
           pF = pF *5;            
           p.setPageFaultCount(pF,jobID);
           if(length%16 != 0)
              loaderSize++;  
           p.setPageTable(loaderSize,1,jobID);

// Defining the output address
           wd = lookIndex *256 + p.getWdAddress(jobID)*16;
           p.setWdAddress(wd,jobID);
           p.setWdEndAddress(wd,jobID); 
           p.setNature("Normal",jobID);
           pageSize = dataSize + outputSize;
           rd = lookIndex *256 + loaderSize*16;
           p.setRdAddress(rd,jobID);
           p.setJobID(jobID,jobID);                                 // Setting the value of Job ID
           loaderSwitch = 1;
          }
       if(len == 4 && i==2)
          {
           loaderSwitch = 0;
           diskIndex = rd;
                  
           p.setPC(GetDecimal(storeArr[0]),jobID);
// Condition to check the trace bit
           if(storeArr[1] == "0" || storeArr[1] == "1")
              { 
               if(jobID == 98)
                 p.setTraceFlag(0,jobID);
               else
               p.setTraceFlag(GetDecimal(storeArr[1]),jobID);
              }
           else
              {
               e.descriptive="";
               e.EHANDLER(2);
               p.setErrMessage(e.descriptive,jobID);
               p.setNature("Abnormal",jobID);
               p.setPageFaultCount(0,jobID);
               diskMemory[lookIndex  *256]= "00000000";
              }
           if(p.getTraceFlag(jobID) == 1)
              {
             string fName = "trace#:";
             stringstream ss;
             ss<<jobID;
             string s = ss.str();
             fName.append(s);
             fName.append(".txt");
             fs.open(fName.c_str(),ios::out);
             fs<<"\n PC(DEC) \t\t Instruction(HEX) \t Contents of A before execution(HEX) \t Contents of A after execution(HEX)  \t EA after execution(HEX)";
             fs.close();
              }
          }
       if(len == 2)
          {
               e.descriptive="";
               e.EHANDLER(2);
               p.setErrMessage(e.descriptive,jobID);
               e.descriptive="";
               p.setNature("Abnormal",jobID);
               p.setPageFaultCount(0,jobID);
               diskMemory[lookIndex  *256]= "00000000";
               p.setTraceFlag(0,jobID);                              // Passing zero if traceflag is missing  
          }
       if(loaderSwitch == 1 && i==1)                                 // storing data in the disk memory
          {
          int loop = len/8;
          programCheck = 1;
          for(int i=0;i<loop;i++)
             {
             diskMemory[diskIndex] = storeArr[0].substr(i*8,8);
             diskIndex++;
             }
          }
       if(loaderSwitch == 0 && i==1)                                 // storing data in the disk memory
          {
          int loop = len/8;
          for(int i=0;i<loop;i++)
             {
             diskMemory[diskIndex] = storeArr[0].substr(i*8,8);
             diskIndex++;
             }
          }
               position = diskRead.tellg();
               readPosition = position;
    } 

if (finSwitch == 1)
   {
    jobID++;
    position = diskRead.tellg();
    readPosition = position;
    finSwitch == 0;
   }

 return jobID;
 }
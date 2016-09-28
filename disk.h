          /* To declare all the functions and class related to error_handler */

/* 
 * Program Description: (Disk Manager)

 * Multiprogramming with a Fixed number of Tasks (MFT)
        I have used MFT for the disk management where, each job has fixed size block. 
        It will have constant maximum degree of multiprogramming. 
        It has a ready queue as an array, here, freeMemory[] is the array of queue.
        It has internal fragmentation as each job has some left over pages in the particular blocks where it goes.
        Largest job <= largest block
*/


#include<string>
#ifndef DISK_H
#define DISK_H
#include <vector>
#include <fstream>
using namespace std;

/* 
 * Comment: This class will perform the property of Disk Management
*/

class diskManager
{ 
public:
   string diskMemory[4096];                            // As per specification, Disk memory is 16 times of the main memory

// I am using MFT, therefore, Queue will be an array
   int freeMemory[8];
   int readPosition,diskIndex,jobID,initiate,dataCount,finCount,jobCount,rd,wd,finSwitch,programCheck,nullJobCheck,programTooLongCheck,lookIndex;
   int openFile,readLength;
   fstream diskRead;
   int readFileData(string str);
   int GetDecimal(string str); 
   int freeMemoryCheck();

// readyQueue used by Memory and Disk
   vector<int> readyQueue; 
   vector<int> blockedQueue;
   vector<int> availableJobs;
};
#endif
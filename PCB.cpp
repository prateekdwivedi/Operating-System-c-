/* 

Program Description:
 According to the specification PCB will store all the needful data required by the job to resume its task in the CPU .
 It will store Program Counter, registers, page table, time job entered the system, cumulative time used by job, job ID, disk address of current RD data record, disk address of current WR record
 and the time completion of current I/O operation.

 I have created functions to store value modified from Disk and System.
 All get functions are used to retrieve the data and all set functions are used to set the data.
 
It will give the defination to PCB.h

*/ 


#include "PCB.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
PCBManager pm[200];



/* Function Description : This will find the value of Page */
int PCBManager::findPage(int content,int index)
{
 vector <int>::iterator i = pm[index].memoryTrack.begin ();
 int nPosition;
   while (i != pm[index].memoryTrack.end ()){
       ++ i;
   }

   i = find (pm[index].memoryTrack.begin(),pm[index].memoryTrack.end(), content);

   if (i != pm[index].memoryTrack.end())
   {
       nPosition = distance (pm[index].memoryTrack.begin(), i);
   }
 return nPosition;
}




/* Function Description : This will set the value of Program Counter */
void PCBManager::setPC(int content,int index)
{
 pm[index].PC = content;
}


/* Function Description : This will get the value of Program Counter */
int PCBManager::getPC(int index)
{
return pm[index].PC;
}



/* Function Description : This will set the value of Page Count */
void PCBManager::setPageFaultCount(int content,int index)
{
 pm[index].pageFaultCount = content;
}


/* Function Description : This will get the value of Page Count */
int PCBManager::getPageFaultCount(int index)
{
return pm[index].pageFaultCount;
}



/* Function Description : This will set the value of No. of turns */
void PCBManager::setNoOfTurns(int content,int index)
{
 pm[index].noOfTurns = content;
}


/* Function Description : This will get the value of No. of turns */
int PCBManager::getNoOfTurns(int index)
{
return pm[index].noOfTurns;
}



/* Function Description : This will set the value of No. of Queue turns */
void PCBManager::setQueueTurns(int content,int index)
{
 pm[index].queueTurns = content;
}


/* Function Description : This will get the value of No. of Queue turns */
int PCBManager::getQueueTurns(int index)
{
return pm[index].queueTurns;
}



/* Function Description : This will set the value of Register */
void PCBManager::setReg(int content,int index,int jobNumber)
{
pm[jobNumber].REG[index] = content;
}



/* Function Description : This will get the value of Register */
int PCBManager::getReg(int index,int jobNumber)
{
return pm[jobNumber].REG[index];
}

/* Function Description : This will set the value of page table reference */
void PCBManager::setPageTable(int content,int index,int jobNumber)
{
 pm[jobNumber].pageTable[index] = content;
}


/* Function Description : This will get the value of page table reference */
int PCBManager::getPageTable(int index,int jobNumber)
{
return pm[jobNumber].pageTable[index];
}


/* Function Description : This will set the value of Time job entered the system */
void PCBManager::setTimeJob(int content,int index)
{
 pm[index].timeJob = content;
}


/* Function Description : This will get the value of Time job entered the system */
int PCBManager::getTimeJob(int index)
{
return pm[index].timeJob;
}



/* Function Description : This will set the value of Cumulative time used by job */
void PCBManager::setCumulativeTime(int content,int index)
{
 pm[index].cumulativeTime = content;
}


/* Function Description : This will get the value of Cumulative time used by job */
int PCBManager::getCumulativeTime(int index)
{
return pm[index].cumulativeTime;
}



/* Function Description : This will set the value of Job ID */
void PCBManager::setJobID(int content,int index)
{
 pm[index].jobID = content;
}


/* Function Description : This will get the value of Job ID */
int PCBManager::getJobID(int index)
{
return pm[index].jobID;
}



/* Function Description : This will set the value of Disk Address of current RD data record */
void PCBManager::setRdAddress(int content,int index)
{
 pm[index].rdAddress = content;
}


/* Function Description : This will get the value of Disk Address of current RD data record */
int PCBManager::getRdAddress(int index)
{
return pm[index].rdAddress;
}



/* Function Description : This will set the value of Disk Address of current WR record */
void PCBManager::setWdAddress(int content,int index)
{
 pm[index].wrAddress = content;
}


/* Function Description : This will get the value of Disk Address of current WR record */
int PCBManager::getWdAddress(int index)
{
return pm[index].wrAddress;
}


/* Function Description : This will set the value of Disk Address of current WR record */
void PCBManager::setWdEndAddress(int content,int index)
{
 pm[index].wrEndAddress = content;
}


/* Function Description : This will get the value of Disk Address of current WR record */
int PCBManager::getWdEndAddress(int index)
{
return pm[index].wrEndAddress;
}



/* Function Description : This will set the value of Time of completion of current I/O operation */
void PCBManager::setTimeCompletion(int content,int index)
{
 pm[index].timeCompletion = content;
}


/* Function Description : This will get the value of Time of completion of current I/O operation */
int PCBManager::getTimeCompletion(int index)
{
return pm[index].timeCompletion;
}


/* Functional Description : This will set the value of pages allocated in the memory */
void PCBManager::setMemoryTrack(int content,int index)
{
 pm[index].memoryTrack.push_back(content);
}


/* Functional Description : This will get the value of pages allocated in the memory */
int PCBManager::getMemoryTrack(int index,int i)
{
return pm[index].memoryTrack.at(i);
}


/* Functional Description : This will set the value of trace flag in the memory */
void PCBManager::setTraceFlag(int content,int index)
{
pm[index].traceFlag = content;
}


/* Functional Description : This will get the value of trace flag in the memory */
int PCBManager::getTraceFlag(int index)
{
return pm[index].traceFlag;
}


/* Functional Description : This will set the value of MemoryEntry in the memory */
void PCBManager::setMemoryEntry(int content,int index)
{
pm[index].memoryEntry= content;
}


/* Functional Description : This will get the value of MemoryEntry in the memory */
int PCBManager::getMemoryEntry(int index)
{
return pm[index].memoryEntry;
}


/* Function Description : This will set the value of Time of completion of current I/O operation */
void PCBManager::setErrMessage(string content,int index)
{
 pm[index].errMessage = content;
}


/* Function Description : This will get the value of Time of completion of current I/O operation */
string PCBManager::getErrMessage(int index)
{
return pm[index].errMessage;
}


/* Functional Description : This will set the value of Nature in the memory */
void PCBManager::setNature(string content,int index)
{
pm[index].nature = content;
}


/* Functional Description : This will get the value of Nature in the memory */
string PCBManager::getNature(int index)
{
return pm[index].nature;
}
